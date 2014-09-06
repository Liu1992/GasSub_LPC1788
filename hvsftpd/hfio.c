/**
 * hfio.c
 *
 * file io utilities
 *
 * Guo Jian
 * 2014.06.25
 */

#include "hfio.h"

/**
 * make_path
 *
 * @param   root    - root path
            path    - relative path
            buf     - path buffer, may be the same with root
            size    - buffer size
 * @return  path length
 *
 * @brief   retr command handler, triger file content transfer
 *
 * @fixme   concat with a relative path
 */
int make_path(char *root, char *path, char *buf, int size)
{
    int l;
    if (buf != root)
       strncpy(buf, root, size);
    l = strlen(buf);
    if (buf[l - 1] == '\\' || buf[l - 1] == '/')
        buf[-- l] = '\0';
    strncat(buf, "/", size);
    if (*path == '/' || *path == '\\')
        ++ path;
    strncat(buf, path, size);
    l = strlen(buf);
    if (buf[l - 1] == '\\' || buf[l - 1] == '/')
        buf[-- l] = '\0';
    return l;
}
