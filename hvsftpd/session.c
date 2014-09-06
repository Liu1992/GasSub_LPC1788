/**
 * session.c
 *
 * ftp session layer
 *
 * Guo Jian
 * 2014.06.24
 */

#include <stdarg.h>
#include "session.h"
#include "hfsocket.h"

static int session_count = 0;

/**
 * sockExcept
 *
 * @param   sock    - data socket
 * @return  none
 *
 * @brief   callback when data socket has problem of connection
 */
static void sockExcept(struct hfsocket *sock)
{
    struct session *session = sock->user_data;
    hflog("except fd %d\n", sock->fd);
    session_response(session, "501 Cannot RETR. Client data socket connection Error.");
    session_closedata(session);
}

/**
 * sockRead
 *
 * @param   sock    - data socket
 * @return  none
 *
 * @brief   callback when data socket is readable
 */
static void sockRead(struct hfsocket *sock)
{
    struct session *session = sock->user_data;
    hflog("readable fd %d\n", sock->fd);

    session->callback(session, session->param);
}

/**
 * sockWrite
 *
 * @param   sock    - data socket
 * @return  none
 *
 * @brief   callback when data socket is writeable
 */
static void sockWrite(struct hfsocket *sock)
{
    struct session *session = sock->user_data;
    hflog("writeable fd %d\n", sock->fd);

    session->callback(session, session->param);
}

/**
 * session_alloc
 *
 * @param   none
 * @return  session pointer
 *
 * @brief   alloc a new session
 */
struct session *session_alloc()
{
    struct session *s = NULL;
    if (session_count < MAX_SESSION)
    {
        s = (struct session *)malloc(sizeof(struct session));
        if (s)
        {
            memset(s, 0, sizeof(struct session));
            ++ session_count;
        }
        else
        {
            hflog("memory alloc failed.\n");
        }
    }
    else
    {
        hflog("no more session avaliable.\n");
    }
    return s;
}

/**
 * session_free
 *
 * @param   s   - session pointer
 * @return  none
 *
 * @brief   free session
 */
void session_free(struct session *s)
{
    if (s->cmd_sock)
        socket_free(s->cmd_sock);
    if (s->data_sock)
        socket_free(s->data_sock);
    free(s);
    -- session_count;
}

/**
 * cmd_append
 *
 * @param   s   - session pointer
 *          buf - new characters received
 *          len - length of characters received
 * @return  0   - command is not commpleted
 *          1   - command is completed with \r\n and should be run
 *
 * @brief   append received characters to command buffer
 */
int cmd_append(struct session *s, char *buf, int len)
{
    int i, l;
    l = strlen(s->cmd_buf);

    for (i = 0; i < len; ++ i)
    {
        if (i < len - 1 && buf[i] == '\r' && buf[i + 1] == '\n')
        {
            /* trim \r\n and report command is completed */
            s->cmd_buf[l] = '\0';
            return 1;
        }
        if (l < MAX_REQUEST_LEN)
            s->cmd_buf[l ++] = buf[i];
    }
    s->cmd_buf[l] = '\0';
    return 0;
}

/**
 * session_response
 *
 * @param   s   - session pointer
 *          str - response content
 * @return  length sent
 *
 * @brief   send response of command
 */
int session_response(struct session *s, char *str)
{
    int ret = 0;
    ret += socket_write(s->cmd_sock, (unsigned char *)str, strlen(str));
    ret += socket_write(s->cmd_sock, (unsigned char *)"\r\n", 2);
    return ret;
}

/**
 * session_format_response
 *
 * @param   s   - session pointer
 *          fmt - format string
 * @return  length sent
 *
 * @brief   send response of command, in format
 */
int session_format_response(struct session *s, const char *fmt, ...) 
{
	va_list ap;
	char buf[MAX_RESPONSE_LEN];
    int ret;

	va_start(ap, fmt);
	ret = _vsnprintf(buf, sizeof(buf), fmt, ap);
    ret = socket_write(s->cmd_sock, (unsigned char *)buf, ret);
    ret += socket_write(s->cmd_sock, (unsigned char *)"\r\n", 2);
    return ret;
}

/**
 * session_welcome
 *
 * @param   s   - session pointer
 * @return  length sent
 *
 * @brief   send welcome string
 */
int session_welcome(struct session *s)
{
    return session_response(s, "220 welcome to hvsftpd");
}

/**
 * session_open_data
 *
 * @param   s           - session pointer
 *          type        - data transfer type, "r" or "w"
 *          callback    - callback when data connection avaliable for r/w
 *          param       - parameter for callback
 * @return  length sent
 *
 * @brief   open an initiative ftp data transfer
 */
int session_opendata(struct session *s, char *type, session_callback callback, char *param)
{
    struct hfsocket *sock;

    if (*type != 'r' && *type != 'w')
        return HF_ERR;

    s->callback = callback;
    strncpy(s->param, param, MAX_REQUEST_LEN);

    sock = socket_client(s->addr, s->port);
    if (sock != NULL)
    {
        s->data_sock = sock;
        sock->user_data = s;
        sock->sockExcept = sockExcept;
        sock->sockRead = sockRead;
        sock->sockWrite = sockWrite;
        sock->interest = SOCKET_EXCEPTION | (*type == 'r' ? SOCKET_READABLE : SOCKET_WRITEABLE);
        session_response(s, "150 Opening data connection.");
        return HF_OK;
    }

    /* return something ? */

    return HF_ERR;
}

/**
 * session_closedata
 *
 * @param   s           - session pointer
 * @return  none
 *
 * @brief   close data socket
 */
void session_closedata(struct session *s)
{
    socket_free(s->data_sock);
    s->data_sock = NULL;
}
