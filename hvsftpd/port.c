/**
 * port.c
 *
 * hvsftpd portable implement
 *
 * Guo Jian
 * 2014.06.24
 */

#include "hvsftpd.h"

/**
 * port_socket_geterr
 *
 * @param   none
 * @return  status
 *
 * @brief   get socket error code
 */
int port_socket_geterr()
{
#ifdef  _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

/**
 * port_socket_init
 *
 * @param   none
 * @return  status
 *
 * @brief   init socket
 */
int port_socket_init()
{
#ifdef  _WIN32
    WSADATA wsaData;
    WORD wVersion = MAKEWORD(2,2);
    int ret = WSAStartup(wVersion, &wsaData);
    if (ret == SOCKET_ERROR)
    {
        hflog("WSAStartup() error : %d\n", port_socket_geterr());
        return HF_ERR;
    }
#endif
    return HF_OK;
}

/**
 * port_set_sockaddr
 *
 * @param   sa      - sockaddr_in struct
 * @return  addr    - address
 *
 * @brief   set address in sockaddr_in
 */
void port_set_sockaddr(struct sockaddr_in *sa, int addr)
{
#ifdef _WIN32
    sa->sin_addr.S_un.S_addr = addr;
#else
    sa->sin_addr.s_addr = addr;
#endif
}
