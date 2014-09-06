/**
 * hfsocket.h
 *
 * asynchronous socket
 *
 * Guo Jian
 * 2014.06.24
 */

#ifndef _HF_SOCKET_H_
#define _HF_SOCKET_H_

#include "hvsftpd.h"

#define SOCKET_LISTENING	0
#define SOCKET_CONNECTING	1
#define SOCKET_CONNECTED	2
#define SOCKET_CLOSED		3

#define SOCKET_READABLE     1
#define SOCKET_WRITEABLE    2
#define SOCKET_EXCEPTION    4

struct hfsocket
{
    int fd;
    struct sockaddr_in addr;
    int state;
    int interest;
    void (*sockAccept)(struct hfsocket *sock);
    void (*sockRead)(struct hfsocket *sock);
    void (*sockWrite)(struct hfsocket *sock);
    void (*sockExcept)(struct hfsocket *sock);
    void *user_data;
};

int socket_init(void);
int socket_geterr(void);
int socket_block(struct hfsocket *sock, int process);
struct hfsocket *socket_server(unsigned short port);
struct hfsocket *socket_client(char *addr, unsigned short port);
struct hfsocket *socket_accept(struct hfsocket *s);
void socket_free(struct hfsocket *s);
int socket_read(struct hfsocket *s, unsigned char *buf, int maxlen);
int socket_write(struct hfsocket *s, unsigned char *buf, int len);
int socket_set_nonblock(int fd, int on);
int socket_set_nodelay(int fd, int on);

#endif /* _HF_SOCKET_H_ */
