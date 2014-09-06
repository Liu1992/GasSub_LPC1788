/**
 * port.h
 *
 * hvsftpd portable declaration
 *
 * Guo Jian
 * 2014.06.24
 */

#ifndef _HF_PORT_H_
#define _HF_PORT_H_

#ifdef _WIN32

#include "winsock.h"
#pragma comment(lib, "ws2_32.lib")
#include "windows.h"
#include "stdio.h"

#include "direct.h"
#include "io.h"
#include "sys/stat.h"
#include "fcntl.h"

#define EWOULDBLOCK WSAEWOULDBLOCK

#define socklen_t int
#define strcasecmp stricmp
#define mkdir(a, b) mkdir(a)

#else /* lpc1788 ucos-ii lwip yaffs2 */

#include "lwip/sockets.h"
#include "yaffsfs.h"

#define open yaffs_open
#define read yaffs_read
#define write yaffs_write
#define close yaffs_close
#define flush yaffs_flush
#define mkdir yaffs_mkdir
#define rmdir yaffs_rmdir
#define unlink yaffs_unlink
#define rename yaffs_rename
#define stat yaffs_stat
#define opendir yaffs_opendir
#define readdir yaffs_readdir
#define closedir yaffs_closedir
#define DIR yaffs_DIR
#define dirent yaffs_dirent

#define INVALID_SOCKET  -1
#define SOCKET_ERROR    -1

#ifndef O_BINARY
    #define O_BINARY    0
#endif
#ifndef O_TEXT
    #define O_TEXT      0
#endif

#endif

int port_socket_init(void);
int port_socket_geterr(void);
void port_set_sockaddr(struct sockaddr_in *sa, int addr);

#endif /* _HF_PORT_H_ */
