#ifndef _HVSFTPD_H_
#define _HVSFTPD_H_

#include "port.h"

#define HF_OK   0
#define HF_ERR  1

#define hflog printf

#define MAX_SESSION		2
#define MAX_SOCKET		8

#define NET_BUF_SIZE	64
#define FILE_BUF_SIZE   1024

#define MAX_REQUEST_LEN     128
#define MAX_RESPONSE_LEN    128
#define MAX_PATH_LEN        128

#endif /* _HVSFTPD_H_ */
