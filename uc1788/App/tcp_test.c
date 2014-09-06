/**
 * tcp_test.c
 *
 * LWIP TCP Test
 *
 * Guo Jian
 * 2014.05.11
 */

#include "stdio.h"
#include "lwip/sockets.h"
#include "lwip/tcpip.h"
#include "string.h"
#include "stdlib.h"
#include "DataStruct.h"
#include "Sensor.h"

static OS_STK App_TaskServerStk[APP_CFG_TASK_TCPTEST_STK_SIZE];
static OS_STK App_TaskClientStk[APP_CFG_TASK_TCPTEST_STK_SIZE];

extern EEPROMDataStruct optionSaveStruct;			//配置储存结构体
extern OS_EVENT    *ConfigQueueHead;				//配置信息消息队列
extern OS_EVENT    *UploadQueueHead;        //上传信息消息队列

Config_Struct rev_config;

/**
 * ClientTest
 *
 * @param   none
 * @return  none
 *
 * @brief  socket client test thread
 */
void ClientTest(void)
{
    struct sockaddr_in addr;
    int fd;
    int i;
    int ret;
    char buf[64];
		INT8U err;
		Upload_Info* upload, *test;
    
		addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
		addr.sin_addr.s_addr = inet_addr("192.168.1.100");
		memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));
	
		fd = socket(AF_INET,  SOCK_STREAM, IPPROTO_TCP );
	
		printf("connecting fd %d...\n", fd);
    ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
		printf( "ret = %d\n", ret );
    
		while (1)
    {
        upload = OSQPend( UploadQueueHead, 0, &err );
                          
        if (ret != -1)
        {
            printf("connected\n");
            //sprintf(buf, "hi ~ %d.\n", i);
						memcpy( buf, upload, sizeof( Upload_Info ) );
            ret = send(fd, buf, sizeof( Upload_Info ), 0);
						printf("sizeof=%d\n", sizeof( Upload_Info ));
						test = (Upload_Info*)buf;
						printf("Test: %u %u %u %s\n", test->testpointNo, test->sensorState, test->collectData, test->collectTime );
						if (ret == -1)
            {
                printf("send errno = %d\n", errno);
            }
						for ( i = 0; i < 64; i++ )
						{
								printf( "%d ", buf[i] );
						}
            
/*
            ret = recv(fd, buf, 8, 0);
            if (ret == -1)
            {
                printf("recv errno = %d\n", errno);
            }
            else
            {
                buf[ret] = '\0';
                printf("recived: %sn", buf);
            }
*/
        }
        else
        {
            printf("connect errno = %d\n", errno);
						break;
        }
        
    }
		shutdown(fd, SHUT_RDWR);
    closesocket(fd);
}

char buf1[512];

/**
 * ServerTest
 *
 * @param   none
 * @return  none
 *
 * @brief  socket server test thread
 */
void ServerTest(void)
{
		int i;
    struct sockaddr_in addr;
    int fd;
    int ret;
    char IPstr[16];
		
    fd = socket(AF_INET,  SOCK_STREAM, IPPROTO_TCP );
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    sprintf( IPstr, "%d.%d.%d.%d", optionSaveStruct.ipConfig.ip[0], optionSaveStruct.ipConfig.ip[1], optionSaveStruct.ipConfig.ip[2], optionSaveStruct.ipConfig.ip[3] );
		addr.sin_addr.s_addr = inet_addr(IPstr);
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

    ret = bind(fd,(struct sockaddr *)&addr, sizeof(struct sockaddr));
    if (ret != -1)
    {
        ret = listen(fd, 10);
        if (ret != -1)
        {
            for (;;)
            {
                int client_fd;
                struct sockaddr_in client_addr;
                socklen_t sin_size = sizeof(struct sockaddr_in);
                
                printf("waiting connection...\n");
                if((client_fd = accept(fd, (struct sockaddr *)&client_addr, &sin_size)) != -1)
                {
                    fd_set readFds, writeFds, exceptFds;
                    unsigned int flag = 1;
                    struct timeval  tv;

                    printf("a client connected.\n");
/*                  
                    ioctlsocket(client_fd, FIONBIO, &flag);

                    FD_ZERO(&readFds);
                    FD_ZERO(&writeFds);
                    FD_ZERO(&exceptFds);
                    FD_SET(client_fd, &readFds);
                    FD_SET(client_fd, &exceptFds);

                    tv.tv_sec = (long) (10000000 / 1000);
                    tv.tv_usec = (unsigned int) (10000000 % 1000) * 1000;

                    select(client_fd + 1, &readFds, &writeFds, &exceptFds, &tv);
                    ret = recv(client_fd, buf, sizeof(buf), 0);
                    if (ret != -1)
                    {
                        printf("received %s\n", buf);
                    }
                    else
                    {
                        printf("recv errno = %d\n", errno);
                    }
                    
                    FD_ZERO(&readFds);
                    FD_ZERO(&writeFds);
                    FD_ZERO(&exceptFds);
                    FD_SET(client_fd, &readFds);
                    FD_SET(client_fd, &exceptFds);

                    select(client_fd + 1, &readFds, &writeFds, &exceptFds, &tv);
*/
                    for (;;)
                    {
                        ret = recv(client_fd, buf1, sizeof(buf1), 0);
                        if (ret != -1)
                        {
														rev_config = (*( Config_Struct* )buf1);
														printf("%u %u %s\n",rev_config.analogueConfig.testpointNo, rev_config.analogueConfig.sensorName, rev_config.analogueConfig.location );
                            printf("received %d\n", ret);
														if ( ret )
														{
																printf( "receive buf=[\n" );
																for ( i = 0; i < ret; i++ )
																	printf( "%d ", buf1[i] );
																printf("]\n");
																OSQPost( ConfigQueueHead, &rev_config );
														}
                        }
                        else
                        {
                            printf("recv errno = %d\n", errno);
                            break;
                        }
                    }
                    closesocket(client_fd);
                        
                }
                else
                {
                    printf("accept errno = %d\n", errno);
                }
            }
        }
        else
        {
            printf("listen errno = %d\n", errno);
        }
    }
    else
    {
        printf("bind errno = %d\n", errno);
				//perror("bind: ");
    }
    
    closesocket(fd);
}

/**
 * TCPTest
 *
 * @param   none
 * @return  none
 *
 * @brief   start tcp server and client
 */
void TCPTest(void)
{
  OSTaskCreateExt((void (*)(void *))ServerTest,            /* Create task                                          */
                  (void          *)0,
                  (OS_STK        *)&App_TaskServerStk[APP_CFG_TASK_TCPTEST_STK_SIZE - 1],
                  (INT8U          )APP_CFG_TASK_TCPSERVER_PRIO,
                  (INT16U         )APP_CFG_TASK_TCPSERVER_PRIO,
                  (OS_STK        *)&App_TaskServerStk[0],
                  (INT32U         )APP_CFG_TASK_TCPTEST_STK_SIZE,
                  (void          *)0,
                  (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
#if 1
  OSTaskCreateExt((void (*)(void *))ClientTest,            /* Create task                                          */
                  (void          *)0,
                  (OS_STK        *)&App_TaskClientStk[APP_CFG_TASK_TCPTEST_STK_SIZE - 1],
                  (INT8U          )APP_CFG_TASK_TCPCLIENT_PRIO,
                  (INT16U         )APP_CFG_TASK_TCPCLIENT_PRIO,
                  (OS_STK        *)&App_TaskClientStk[0],
                  (INT32U         )APP_CFG_TASK_TCPTEST_STK_SIZE,
                  (void          *)0,
                  (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
#endif
}
