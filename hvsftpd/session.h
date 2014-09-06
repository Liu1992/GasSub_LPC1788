/**
 * session.h
 *
 * ftp session layer
 *
 * Guo Jian
 * 2014.06.24
 */

#ifndef _HF_SESSION_H_
#define _HF_SESSION_H_

#include "hvsftpd.h"
#include "hfsocket.h"

struct session;

typedef void (*session_callback)(struct session *sock, char *param);

struct session
{
    struct hfsocket *cmd_sock;
    struct hfsocket *data_sock;

    session_callback callback;
    char param[MAX_REQUEST_LEN + 1];

    char root_dir[MAX_PATH_LEN + 1];
    char current_dir[MAX_PATH_LEN + 1];
    char cmd_buf[MAX_REQUEST_LEN + 1];

    char renaming[MAX_PATH_LEN + 1];

    char addr[16];
    unsigned short port;
};

struct session *session_alloc(void);
void session_free(struct session *s);
int cmd_append(struct session *s, char *buf, int len);
int session_welcome(struct session *s);
int session_response(struct session *s, char *str);
int session_format_response(struct session *s, const char *format, ...);
int session_opendata(struct session *s, char *type, session_callback callback, char *param);
void session_closedata(struct session *s);

#endif /* _HF_SESSION_H_ */
