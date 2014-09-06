/**
 * cmd.c
 *
 * ftp command handler
 *
 * Guo Jian
 * 2014.06.24
 */

#include <string.h>
#include "cmd.h"
#include "hfio.h"

extern struct command_map commands[];

/**
 * cmd_disptach
 *
 * @param   cmd - command to 
            s   - session
 * @return  status
 *
 * @brief   dispatch command via function mapping table
 */
int cmd_disptach(char *cmd, struct session *s)
{
    struct command_map *cmds = commands;
    char *p;

    hflog("cmd : %s\n", cmd);

    p = strchr(cmd, ' ');
    if (p == NULL)
        p = "";
    else
        *(p ++) = '\0';
    while (cmds->cmd)
    {
        if (!strcasecmp(cmds->cmd, cmd))
            return cmds->handler(s, p);
        ++ cmds;
    }
    
    hflog("unknown cmd : %s %s\n", cmd, p);

    session_response(s, "500 command not understood.");
    return HF_OK;
}

/**
 * noop_handler
 *
 * @param   s       - session
            param   - parameter, not used
 * @return  status
 *
 * @brief   noop command handler, do nothing, simply reply "200 Ok."
 */
static int noop_handler(struct session *s, char *param)
{
    session_response(s, "200 Ok.");
    return HF_OK;
}

/**
 * syst_handler
 *
 * @param   s       - session
            param   - not used
 * @return  status
 *
 * @brief   syst command handler, reply system type, we use unix compatible
 */
static int syst_handler(struct session *s, char *param)
{
    session_response(s, "215 UNIX Type: L8 Internet Component Suite.");
    return HF_OK;
}

/**
 * user_handler
 *
 * @param   s       - session
            param   - user name
 * @return  status
 *
 * @brief   user command handler, set login name
 */
static int user_handler(struct session *s, char *param)
{
    session_response(s, "331 Password required.");
    return HF_OK;
}

/**
 * pass_handler
 *
 * @param   s       - session
            param   - password
 * @return  status
 *
 * @brief   pass command handler, set password and triger login check
 */
static int pass_handler(struct session *s, char *param)
{
    session_response(s, "230 User logged in.");
    return HF_OK;
}

/**
 * pwd_handler
 *
 * @param   s       - session
            param   - not used
 * @return  status
 *
 * @brief   pwd command handler, get pressent working directory
 */
static int pwd_handler(struct session *s, char *param)
{
    session_format_response(s, "257 \"%s\" is current directory.", s->current_dir);
    return HF_OK;
}

/**
 * type_handler
 *
 * @param   s       - session
            param   - transfer mode, 'A' or 'I'
 * @return  status
 *
 * @brief   type command handler, set transfer mode, only BINARY mode is supported currently
 */
static int type_handler(struct session *s, char *param)
{
    if (!strcasecmp(param, "A"))
        session_response(s, "200 Type set to A.");
    else if (!strcasecmp(param, "I"))
        session_response(s, "200 Type set to I.");
    else
    {
        hflog("unknown mode : %s\n", param); 
        session_response(s, "500 command not understood.");
    }
    return HF_OK;
}

/**
 * port_handler
 *
 * @param   s       - session
            param   - address and port, in format "ip0,ip1,ip2,ip3,porth,portl"
 * @return  status
 *
 * @brief   port command handler, set initiative mode transfer address/port
 */
static int port_handler(struct session *s, char *param)
{
    int k = 0;
    char *p = param, *s_port = NULL;
    unsigned char p1, p2;

    /* format param as "ip.ip.ip.ip\0port port" */
    while (*p)
    {
        if (*p == ',')
        {
            if (++ k == 4)
            {
                *p = '\0';
                s_port = p + 1;
            }
            else if (k < 4)
                *p = '.';
            else
                *p = ' ';
        }
        ++ p;
    }
    /* there was five commas */
    if (k == 5)
    {
        if (sscanf(s_port, "%hhu%hhu", &p1, &p2) == 2)
        {
            strncpy(s->addr, param, sizeof(s->addr));
            s->port = ((int)p1 << 8) | p2;
            hflog("port requested : %s:%hu\n", s->addr, s->port);
            
            session_response(s, "200 Port command successful.");
            return HF_OK;
        }
    }
    session_response(s, "501 Invalid PORT command.");
    return HF_OK;
}

/**
 * cwd_handler
 *
 * @param   s       - session
            param   - directory path, relative or absolute
 * @return  status
 *
 * @brief   cwd command handler, set pressend working directory
 *
 * @fixme   should check if escaped from root dir
 *          relative dir not fully supported
 */
static int cwd_handler(struct session *s, char *param)
{
    char *p = param;
    while (*p)
    {
        if (*p == '\\')
            *p = '/';
        ++ p;
    }
    if (*param == '/')
    {
        /* absolute path will be new pwd */
        strncpy(s->current_dir, param, MAX_PATH_LEN);
    }
    else
    {
        /* just append new path to pwd */
        make_path(s->current_dir, param, s->current_dir, MAX_PATH_LEN);
    }
    session_format_response(s, "250 CWD command successful, \"%s\" is current directory.", s->current_dir);
    return HF_OK;
}

/**
 * list_write
 *
 * @param   s       - session
            param   - not used
 * @return  none
 *
 * @brief   data transfer routine of directory content
 *
 * @fixme   file time is not implemented
 *          should use asynchronous mode
 */
static void list_write(struct session *s, char *param)
{
    char *path, *buf;
#ifdef _WIN32
    long hfile;
    struct _finddata_t file; 
#else
    DIR *dir;
    struct dirent *ent;
    struct stat st;
#endif
    
    path = malloc(MAX_PATH_LEN * 2);
    if (path)
    {
        buf = path + MAX_PATH_LEN;
        make_path(s->root_dir, s->current_dir, path, MAX_PATH_LEN);

        hflog("listing %s\n", path);
#ifdef _WIN32
        strncat(path, "/*", MAX_PATH_LEN);
        if ((hfile = _findfirst(path, &file)) > 0)
        {
            /* in synchronous mode */
            socket_set_nonblock(s->data_sock->fd, 0);
            do
            {
                _snprintf(buf, MAX_PATH_LEN, "%crwxrwxrwx   1 %8s %8s %7d Dec 07  2013 %s\r\n", file.attrib & 0x10 ? 'd' : '-', "ftp", "ftp", file.size, file.name);
                socket_write(s->data_sock, (unsigned char *)buf, strlen(buf));
            } while(_findnext(hfile, &file) == 0);
            _findclose(hfile);
        }
#else
        if ((dir = opendir(path))!= NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                make_path(path, ent->d_name, buf, MAX_PATH_LEN);
                if (stat(buf, &st) == 0)
                {
                    _snprintf(buf, MAX_PATH_LEN, "%crwxrwxrwx   1 %8s %8s %7d Dec 07  2013 %s\r\n", st.st_mode & S_IFDIR ? 'd' : '-', "ftp", "ftp", (unsigned int)st.st_size, ent->d_name);
                    socket_write(s->data_sock, (unsigned char *)buf, strlen(buf));
                }
            }
            closedir(dir);
        }
#endif
        else
        {
            /* no such dir, return something ? */
            hflog("no such dir : %s\n", path);
        }
        free(path);
    }
    else
    {
        hflog("buf alloc failed.\n");
    }
    session_closedata(s);
    session_response(s, "226 File sent ok");
}

/**
 * list_handler
 *
 * @param   s       - session
            param   - not used
 * @return  status
 *
 * @brief   list command handler, triger directory content transfer
 *
 * @fixme   should list files that match filename provided in param, including wildcard
 */
static int list_handler(struct session *s, char *param)
{
    int ret;
    
    ret = session_opendata(s, "w", list_write, param);
    (void)ret;
    return HF_OK;
}

/**
 * retr_write
 *
 * @param   s       - session
            param   - file path
 * @return  none
 *
 * @brief   file content transfer routine
 *
 * @fixme   should use asynchronous mode
 */
static void retr_write(struct session *s, char *param)
{
    int ret;
    char *path, *buf;
    struct stat st;
    int fd;
    int len;

    path = malloc(MAX_PATH_LEN + FILE_BUF_SIZE);
    if (path)
    {
        buf = path + MAX_PATH_LEN;
        make_path(s->root_dir, s->current_dir, path, MAX_PATH_LEN);
        make_path(path, param, path, MAX_PATH_LEN);

        hflog("reading %s\n", path);

        if (stat(path, &st) == 0)
        {
            fd = open(path, O_RDONLY | O_BINARY, 0777);
            if (fd >= 0)
            {
                /* in synchronous mode */
                socket_set_nonblock(s->data_sock->fd, 0);
                while ((len = read(fd, buf, FILE_BUF_SIZE)) > 0)
                {
                    ret = socket_write(s->data_sock, (unsigned char *)buf, len);
                    if (ret <= 0)
                    {
                        hflog("send() error : %d\n", socket_geterr());
                    }
                }
                close(fd);
                session_response(s, "226 File sent ok");
                ret = HF_OK;
            }
            else
            {
                hflog("open() error : %d\n", fd);
                session_response(s, "501 Permission Denied");
                ret = HF_ERR;
            }
        }
        else
        {
            ret = HF_ERR;
            session_format_response(s, "550 File '%s': no such file or directory.", param);
        }

        free(path);
    }
    else
    {
        hflog("buf alloc failed.\n");
        session_response(s, "550 Cannot RETR");
    }
    session_closedata(s);
}

/**
 * retr_handler
 *
 * @param   s       - session
            param   - file path
 * @return  status
 *
 * @brief   retr command handler, triger file download
 */
static int retr_handler(struct session *s, char *param)
{
    int ret;

    ret = session_opendata(s, "w", retr_write, param);
    (void)ret;
    return HF_OK;
}

/**
 * dele_handler
 *
 * @param   s       - session
            param   - file path
 * @return  status
 *
 * @brief   dele command handler, delete file
 */
static int dele_handler(struct session *s, char *param)
{
    int ret;
    char *path;
    struct stat st;

    path = malloc(MAX_PATH_LEN);
    if (path)
    {
        ret = make_path(s->root_dir, s->current_dir, path, MAX_PATH_LEN);
        ret = make_path(path, param, path, MAX_PATH_LEN);

        hflog("deleting file : %s\n", path);
        
        if ((ret = stat(path, &st)) == 0 && !(st.st_mode & S_IFDIR))
        {
            if ((ret = unlink(path)) == 0)
            {
                session_format_response(s, "250 File '%s' deleted.", param);
                ret = HF_OK;
            }
            else
            {
                hflog("unlink() error : %d\n", ret);
                session_format_response(s, "450 File '%s' can't be deleted.", param);
                ret = HF_ERR;
            }
        }
        else
        {
            session_format_response(s, "550 File '%s': no such file or directory.", param);
            ret = HF_ERR;
        }
        free(path);
    }
    else
    {
        hflog("buf alloc failed.\n");
        session_response(s, "550 Cannot DELE");
        ret = HF_ERR;
    }

    return HF_OK;
}

/**
 * stor_read
 *
 * @param   s       - session
            param   - file path
 * @return  none
 *
 * @brief   file content upload routine
 *
 * @fixme   should use asynchronous mode
 */
static void stor_read(struct session *s, char *param)
{
    int ret;
    char *path, *buf;
    int fd;

    path = malloc(MAX_PATH_LEN + FILE_BUF_SIZE);
    if (path)
    {
        buf = path + MAX_PATH_LEN;

        ret = make_path(s->root_dir, s->current_dir, path, MAX_PATH_LEN);
        ret = make_path(path, param, path, MAX_PATH_LEN);

        hflog("writing file : %s\n", path);

        fd = open(path, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC, 0777);
        if (fd >= 0)
        {
            /* in synchronous mode */
            // socket_set_nonblock(s->data_sock->fd, 0);
            while ((ret = socket_read(s->data_sock, (unsigned char *)buf, FILE_BUF_SIZE)) > 0 || socket_geterr() == EWOULDBLOCK)
            {
                if (socket_geterr() == EWOULDBLOCK)
                {
                    socket_block(s->data_sock, 0);
                    ret = 0;
                }
                printf("recved %d\n", ret);
                if (ret)
                {
                    ret = write(fd, buf, ret);
                    if (ret <= 0)
                    {
                        hflog("write() error : %d\n", ret);
                    }
                }
            }
            close(fd);
#if 0
            yaffs_unmount("/nand");
            NandFlash_Reset();
            yaffs_mount("/nand");
#endif
            session_response(s, "226 File received ok");
            ret = HF_OK;
        }
        else
        {
            hflog("open() error : %d\n", fd);
            session_response(s, "501 Permission Denied");
            ret = HF_ERR;
        }
        free(path);
    }
    else
    {
        hflog("buf alloc failed.\n");
        session_response(s, "550 Cannot STOR");
        ret = HF_ERR;
    }
    session_closedata(s);
}

/**
 * stor_handler
 *
 * @param   s       - session
            param   - file path
 * @return  status
 *
 * @brief   stor command handler, triger file upload
 */
static int stor_handler(struct session *s, char *param)
{
    int ret;
    ret = session_opendata(s, "r", stor_read, param);

    (void)ret;
    return HF_OK;
}

/**
 * rmd_handler
 *
 * @param   s       - session
            param   - dir path
 * @return  status
 *
 * @brief   remove empty directory
 *
 * @fixme   should check for security problem, don't escape root dir
 */
static int rmd_handler(struct session *s, char *param)
{
    int ret;
    char *path;
    struct stat st;

    path = malloc(MAX_PATH_LEN);
    if (path)
    {
        ret = make_path(s->root_dir, s->current_dir, path, MAX_PATH_LEN);
        ret = make_path(path, param, path, MAX_PATH_LEN);

        hflog("removing dir : %s\n", path);
        
        if ((ret = stat(path, &st)) == 0 && (st.st_mode & S_IFDIR))
        {
            if ((ret = rmdir(path)) == 0)
            {
                session_format_response(s, "250 '%s': directory removed.", param);
                ret = HF_OK;
            }
            else
            {
                hflog("rmdir() error : %d\n", ret);
                session_format_response(s, "550 '%s': can't remove directory.", param);
                ret = HF_ERR;
            }
        }
        else
        {
            session_format_response(s, "550 File '%s': no such directory.", param);
        }
        free(path);
    }
    else
    {
        hflog("buf alloc failed.\n");
        session_response(s, "550 Cannot RMD");
        ret = HF_ERR;
    }

    return HF_OK;
}

/**
 * rnfr_handler
 *
 * @param   s       - session
            param   - path
 * @return  status
 *
 * @brief   set file path to rename, note that file is not renamed yet
 *
 * @fixme   should check for security problem, don't escape root dir
 */
static int rnfr_handler(struct session *s, char *param)
{
    int ret;
    char *path;
    struct stat st;

    path = malloc(MAX_PATH_LEN);
    if (path)
    {
        ret = make_path(s->root_dir, s->current_dir, path, MAX_PATH_LEN);
        ret = make_path(path, param, path, MAX_PATH_LEN);

        hflog("prepare rename : %s\n", path);
        
        if ((ret = stat(path, &st)) == 0)
        {
            strncpy(s->renaming, path + strlen(s->root_dir), MAX_PATH_LEN);
            session_response(s, "350 File exists, ready for destination name.");
        }
        else
        {
            session_format_response(s, "550 File '%s': no such file or directory.", param);
        }
        free(path);
    }
    else
    {
        hflog("buf alloc failed.\n");
        session_response(s, "550 Cannot RNFR");
        ret = HF_ERR;
    }

    (void)ret;
    return HF_OK;
}

/**
 * rnto_handler
 *
 * @param   s       - session
            param   - new path
 * @return  status
 *
 * @brief   set dest path and triger file rename operation
 *
 * @fixme   should check for security problem, don't escape root dir
 */
static int rnto_handler(struct session *s, char *param)
{
    int ret;
    char *path, *old_path;
    struct stat st;

    path = malloc(MAX_PATH_LEN + MAX_PATH_LEN);
    if (path)
    {
        old_path = path + MAX_PATH_LEN;

        ret = make_path(s->root_dir, s->renaming, old_path, MAX_PATH_LEN);

        ret = make_path(s->root_dir, s->current_dir, path, MAX_PATH_LEN);
        ret = make_path(path, param, path, MAX_PATH_LEN);

        hflog("renaming to : %s\n", path);
        
        if ((ret = stat(old_path, &st)) == 0)
        {
            if ((ret = rename(old_path, path)) == 0)
            {
                ret = HF_OK;
                session_format_response(s, "250 File '%s' renamed to '%s'.", s->renaming, param);
            }
            else
            {
                ret = HF_ERR;
                session_format_response(s, "450 File '%s' can't be renamed.", s->renaming);
            }
        }
        else
        {
            ret = HF_ERR;
            session_format_response(s, "550 File '%s': no such file or directory.", s->renaming);
        }
        free(path);
    }
    else
    {
        hflog("buf alloc failed.\n");
        ret = HF_ERR;
        session_response(s, "550 Cannot RNTO");
    }

    (void)ret;
    return HF_OK;
}

/**
 * mkd_handler
 *
 * @param   s       - session
            param   - dir path
 * @return  status
 *
 * @brief   make directory
 *
 * @fixme   should check for security problem, don't escape root dir
 */
static int mkd_handler(struct session *s, char *param)
{
    int ret;
    char *path;
    struct stat st;

    path = malloc(MAX_PATH_LEN);
    if (path)
    {
        if (*param == '/')
            make_path(s->root_dir, param, path, MAX_PATH_LEN);
        else
        {
            make_path(s->root_dir, s->current_dir, path, MAX_PATH_LEN);
            make_path(path, param, path, MAX_PATH_LEN);
        }

        hflog("mkdir : %s\n", path);
        
        if ((ret = stat(path, &st)) != 0)
        {
            if ((ret = mkdir(path, 0777)) == 0)
            {
                ret = HF_OK;
                session_format_response(s, "250 '%s': directory created.", param);
            }
            else
            {
                ret = HF_ERR;
                session_format_response(s, "550 '%s': can't create directory.", param);
            }
        }
        else
        {
            session_format_response(s, "550 '%s': file or directory already exists.", param);
        }
        free(path);

    }
    else
    {
        hflog("buf alloc failed.\n");
        ret = HF_ERR;
        session_response(s, "550 Cannot MKD");
    }

    (void)ret;
    return HF_OK;
}

/**
 * quit_handler
 *
 * @param   s       - session
            param   - not used
 * @return  status
 *
 * @brief   close session
 */
static int quit_handler(struct session *s, char *param)
{
    session_response(s, "221 Goodbye.");
    return HF_ERR;
}

/**
 * fmt_handler
 *
 * @param   s       - session
            param   - not used
 * @return  status
 *
 * @brief   close session
 */
static int fmt_handler(struct session *s, char *param)
{
    
    yaffs_unmount("/nand");
    //FsTest();
    yaffs_mount("/nand");
    
    session_response(s, "200 ok.");
    
    return HF_ERR;
}

struct command_map commands[] = {
    {"noop",    noop_handler},
    {"syst",    syst_handler},
    {"user",    user_handler},
    {"pass",    pass_handler},
    {"pwd",     pwd_handler},
    {"xpwd",    pwd_handler},
    {"type",    type_handler},
    {"port",    port_handler},
    {"cwd",     cwd_handler},
    {"nlst",    list_handler},
    {"list",    list_handler},
    {"retr",    retr_handler},
    {"dele",    dele_handler},
    {"stor",    stor_handler},
    {"rmd",     rmd_handler},
    {"xrmd",    rmd_handler},
    {"rnfr",    rnfr_handler},
    {"rnto",    rnto_handler},
    {"mkd",     mkd_handler},
    {"xmkd",    mkd_handler},
    {"quit",    quit_handler},
    {"bye",     quit_handler},
    {"fmt",     fmt_handler},
    {NULL, NULL}
};
