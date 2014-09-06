/**
 * hvsftpd.c
 *
 * hvsftpd main logic
 *
 * Guo Jian
 * 2014.06.23
 */

#include "hvsftpd.h"
#include "hfsocket.h"
#include "session.h"
#include "cmd.h"

struct hfsocket *sock_server;

/**
 * sockRead
 *
 * @param   sock    - client socket
 * @return  none
 *
 * @brief   callback when command socket readable
 */
static void sockRead(struct hfsocket *sock)
{
    char buf[NET_BUF_SIZE + 1];
    int len;
    struct session *session = (struct session *)sock->user_data;

    len = socket_read(sock, (unsigned char *)buf, NET_BUF_SIZE);
    if (len > 0)
    {
        /* dispatch command if \r\n recieved */
        if (cmd_append(session, buf, len) == 1)
        {
            if (cmd_disptach(session->cmd_buf, session) != HF_OK)
            {
                /* mark that session should be closed */
                sock->state = SOCKET_CLOSED;
            }
            *session->cmd_buf = '\0';
        }
    }
    if (sock->state == SOCKET_CLOSED)
    {
        session_free(session);
    }
}

/**
 * sockAccept
 *
 * @param   sock    - server socket
 * @return  none
 *
 * @brief   callback when server socket connected
 */
static void sockAccept(struct hfsocket *sock)
{
    struct hfsocket *client_sock;
    struct session *session;

    client_sock = socket_accept(sock);
    if (client_sock != NULL)
    {
        hflog("connected from %s:%d\n", inet_ntoa(client_sock->addr.sin_addr), ntohs(client_sock->addr.sin_port));

        /* create new session */
        session = session_alloc();
        client_sock->user_data = (void *)session;
        client_sock->sockRead = sockRead;
        client_sock->interest = SOCKET_READABLE;
        
        if (session != NULL)
        {
            *session->cmd_buf = '\0';
            session->cmd_sock = client_sock;
            strcpy(session->current_dir, "/");
            strcpy(session->root_dir, "/nand");
            session_welcome(session);
        }
        else
            socket_free(client_sock);
    }
}

/**
 * hvsftpd_main
 *
 * @param   none
 * @return  status
 *
 * @brief   init ftpd, enter main loop
 */
int hvsftpd_main()
{
    int ret;

    ret = socket_init();
    if (ret != HF_OK)
        return ret;

    /* start server listen */
    sock_server = socket_server(21);
    if (sock_server == NULL)
        return HF_ERR;
    sock_server->interest = SOCKET_READABLE;
    sock_server->sockAccept = sockAccept;

    hflog("server started.\n");

    /* loop forever */
    for (;;)
        socket_block(NULL, 1);

    // return ret;
}
