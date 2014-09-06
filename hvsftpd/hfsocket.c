/**
 * hfsocket.c
 *
 * asynchronous socket
 *
 * Guo Jian
 * 2014.06.24
 */

#include "hfsocket.h"

int server_fd;
struct hfsocket sock_list[MAX_SOCKET];

/**
 * socket_init
 *
 * @param   none
 * @return  status
 *
 * @brief   inti socket
 */
int socket_init()
{
    struct hfsocket *s;
    for (s = sock_list; s < sock_list + MAX_SOCKET; ++ s)
        s->fd = INVALID_SOCKET;
    return port_socket_init();
}

/**
 * socket_geterr
 *
 * @param   none
 * @return  error code
 *
 * @brief   get socket error code
 */
int socket_geterr()
{
    return port_socket_geterr();
}

/**
 * socket_alloc
 *
 * @param   none
 * @return  allocated socket pointer
 *
 * @brief   alloc a new socket room, note that socket is not created yet
 */
struct hfsocket *socket_alloc()
{
    struct hfsocket *s;
    for (s = sock_list; s < sock_list + MAX_SOCKET; ++ s)
    {
        if (s->fd == INVALID_SOCKET)
        {
            memset(s, 0, sizeof(struct hfsocket));
            s->fd = INVALID_SOCKET;
            return s;
        }
    }

    hflog("no more socket avalibale.\n");
    return NULL;
}

/**
 * socket_set_nonblock
 *
 * @param   fd  - socket descriptor
 *          on  - 0 for block, 1 for nonblock
 * @return  status
 *
 * @brief   set socket to nonblock/block mode
 */
int socket_set_nonblock(int fd, int on)
{
    int flag = on;
    int ret;

    ret = ioctlsocket(fd, FIONBIO, &flag);
    if (ret == SOCKET_ERROR)
    {
        hflog("ioctlsocket() error : %d\n", socket_geterr());
        return HF_ERR;
    }
    return HF_OK;
}

/**
 * socket_set_nodelay
 *
 * @param   fd  - socket descriptor
 *          on  - 0 for delay, 1 for nodelay
 * @return  status
 *
 * @brief   enable/disable socket no delay
 */
int socket_set_nodelay(int fd, int on)
{
    int flag = on ? 1 : 0;
    int ret;

    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*) &flag, sizeof(int));
    if (ret == SOCKET_ERROR)
    {
        hflog("ioctlsocket() error : %d\n", socket_geterr());
        return HF_ERR;
    }
    return HF_OK;
}

/**
 * socket_server
 *
 * @param   port    - local bind port
 * @return  new socket pointer, or NULL when failed
 *
 * @brief   create a socket listening at a given port
 */
struct hfsocket *socket_server(unsigned short port)
{
    int ret;
    struct hfsocket *s = NULL;
    int server_fd;
    struct sockaddr_in sa;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd != INVALID_SOCKET)
    {
        /* bind to all netif */
        port_set_sockaddr(&sa, htonl(INADDR_ANY));
        sa.sin_family = AF_INET;
        sa.sin_port = htons(port);
        memset(sa.sin_zero, 0, sizeof(sa.sin_zero));

        ret = bind(server_fd, (struct sockaddr *)&sa, sizeof(sa));
        if (ret != SOCKET_ERROR)
        {
            ret = listen(server_fd, 4);
            if (ret != SOCKET_ERROR)
            {
//                ret = socket_set_nodelay(server_fd, 1);
                ret = socket_set_nonblock(server_fd, 1);
                s = socket_alloc();
                if (s != NULL)
                {
                    s->fd = server_fd;
                    s->state = SOCKET_LISTENING;
                    memcpy(&s->addr, &sa, sizeof(sa));
                }
            }
            else
            {
                hflog("listen() error : %d\n", socket_geterr());
            }
        }
        else
        {
            hflog("bind() error : %d\n", socket_geterr());
        }
        if (s == NULL)
            closesocket(server_fd);
    }
    else
    {
        hflog("socket() error : %d\n", socket_geterr());
    }

    return s;
}

/**
 * socket_client
 *
 * @param   addr    - remote address
 *          port    - remote port
 * @return  new socket pointer, or NULL when failed
 *
 * @brief   create a socket connecting to a given address
 */
struct hfsocket *socket_client(char *addr, unsigned short port)
{
	int fd;
	struct sockaddr_in sa;
	struct hfsocket *sock;
	int ret;

	sock = socket_alloc();
	if (sock != NULL)
	{
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd != INVALID_SOCKET)
		{
            // ret = socket_set_nodelay(fd, 1);
			ret = socket_set_nonblock(fd, 1);
			if (ret == HF_OK)
			{
				sa.sin_family = AF_INET;
				sa.sin_port = htons(port);
                port_set_sockaddr(&sa, inet_addr(addr));
				memset(&sa.sin_zero, 0, sizeof(sa.sin_zero));

				ret = connect(fd, (struct sockaddr *)&sa, sizeof(sa));
				if (ret != SOCKET_ERROR || socket_geterr() == EWOULDBLOCK || socket_geterr() == EINPROGRESS)
				{
                    memcpy(&sock->addr, &sa, sizeof(sa));
					sock->fd = fd;
					sock->state = SOCKET_CONNECTING;
					ret = HF_OK;
				}
				else
				{
					hflog("connect() error : %d\n", socket_geterr());
					ret = HF_ERR;
				}
			}
			if (ret != HF_OK)
			{
				closesocket(fd);
				fd = INVALID_SOCKET;
			}
		}
		else
		{
			hflog("sockte() error : %d\n", socket_geterr());
		}

		if (sock->fd == INVALID_SOCKET)
			sock = NULL;
	}

	return sock;
}

/**
 * socket_accept
 *
 * @param   sock    - socket pointer
 * @return  new socket pointer, or NULL when failed
 *
 * @brief   accept a new connection
 */
struct hfsocket *socket_accept(struct hfsocket *sock)
{
    struct sockaddr_in sa;
    int client_fd;
    socklen_t len;
    struct hfsocket *s = NULL;
    int ret;

    len = sizeof(sa);
    client_fd = accept(sock->fd, (struct sockaddr *)&sa, &len);
    if (client_fd != INVALID_SOCKET)
    {
        /* client is also none block */
        ret = socket_set_nonblock(client_fd, 1);
        ret = socket_set_nodelay(client_fd, 1);
        if (ret == HF_OK)
        {
            s = socket_alloc();
            if (s != NULL)
            {
                s->fd = client_fd;
                s->state = SOCKET_CONNECTED;
                /* save peer addr */
                memcpy(&s->addr, &sa, sizeof(sa));
            }
        }
        if (s == NULL)
            closesocket(client_fd);
    }
    else
    {
        hflog("accept() error : %d\n", socket_geterr());
    }

    return s;
}

/**
 * socket_block
 *
 * @param   sock    - socket to block, or NULL for all sockets
 *          process - auto call callback or not
 * @return  status
 *
 * @brief   block until a socket event occur
 */
int socket_block(struct hfsocket *sock, int process)
{
    struct hfsocket *s;
    int max_fd = 0;
    int ret;
    struct fd_set read_fds, write_fds, except_fds;
    struct timeval tv;

    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);

    /* watch all sockets */
    for (s = sock_list; s < sock_list + MAX_SOCKET; ++ s)
    {
        if (s->fd != INVALID_SOCKET && (s == sock || sock == NULL))
        {
            if (s->interest & SOCKET_READABLE)
                FD_SET(s->fd, &read_fds);
            if (s->interest & SOCKET_WRITEABLE)
                FD_SET(s->fd, &write_fds);
            if (s->interest & SOCKET_EXCEPTION)
                FD_SET(s->fd, &except_fds);
            if (s->fd > max_fd)
                max_fd = s->fd;
        }
    }

    tv.tv_sec = 30;
    tv.tv_usec = 0;

    ret = select(max_fd + 1, &read_fds, &write_fds, &except_fds, &tv);

    /* no event returned */
    if (ret <= 0)
        return HF_ERR;

    if (!process)
        return HF_OK;
    
    /* check result set */
    for (s = sock_list; s < sock_list + MAX_SOCKET; ++ s)
    {
        if (s->fd != INVALID_SOCKET)
        {
            if (FD_ISSET(s->fd, &read_fds))
            {
				if (s->state == SOCKET_CONNECTING)
					s->state = SOCKET_CONNECTED;
                if(s->state == SOCKET_LISTENING)
                    s->sockAccept(s);
                else
                    s->sockRead(s);
            }
            if (FD_ISSET(s->fd, &write_fds))
            {
                s->sockWrite(s);
            }
            if (FD_ISSET(s->fd, &except_fds))
            {
                s->sockExcept(s);
            }
        }
    }

    return HF_OK;
}

/**
 * socket_read
 *
 * @param   s       - socket pointer
 *          buf     - reveice buffer
 *          maxlen  - buffer size
 * @return  length received
 *
 * @brief   read data from a socket
 */
int socket_read(struct hfsocket *s, unsigned char *buf, int maxlen)
{
    int ret;

    ret = recv(s->fd, buf, maxlen, 0);

    /* remote normaly shutdown */
    if (ret == 0)
    {
        s->state = SOCKET_CLOSED;
        return 0;
    }

    /* data received */
    if (ret != SOCKET_ERROR)
        return ret;

    /* should try again */
    if (socket_geterr() == EWOULDBLOCK)
        return 0;

    /* other error */
    s->state = SOCKET_CLOSED;
    return ret;
}

/**
 * socket_write
 *
 * @param   s   - socket pointer
 *          buf - data to send
 *          len - data size
 * @return  length sent
 *
 * @brief   send data to a socket
 *
 * @BUG!!!  when system buffer is full, extra data will be lost
 */
int socket_write(struct hfsocket *s, unsigned char *buf, int len)
{
    return send(s->fd, buf, len, 0);
}

/**
 * socket_free
 *
 * @param   s   - socket pointer
 * @return  length sent
 *
 * @brief   dealloc a socket
 */
void socket_free(struct hfsocket *s)
{
    closesocket(s->fd);
    s->fd = INVALID_SOCKET;
}
