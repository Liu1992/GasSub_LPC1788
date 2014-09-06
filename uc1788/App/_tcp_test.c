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
#include "FreeRTOS.h"
#include "task.h"

/**
 * ClientTest
 *
 * @param   none
 * @return  none
 *
 * @brief  socket client test thread
 */
void ClientTest(void *arg)
{
    struct sockaddr_in addr;
    int fd;
    int i;
    int ret;
    char buf[32];
    
    for (i = 0; i < 1000; ++ i)
    {
        /* first time delay is to wait server start */
        msDelay(5000);
            
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8888);
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

        fd = socket(AF_INET,  SOCK_STREAM, 0);
        
        printf("connecting fd %d...\n", fd);
        ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
        if (ret != -1)
        {
            printf("connected\n");
            sprintf(buf, "hi ~ %d.", i);
            ret = send(fd, buf, strlen(buf) + 1, 0);
            if (ret == -1)
            {
                printf("send errno = %d\n", errno);
            }
        }
        else
        {
            printf("connect errno = %d\n", errno);
        }
        close(fd);
    }
}

/**
 * ServerTest
 *
 * @param   none
 * @return  none
 *
 * @brief  socket server test thread
 */
void ServerTest(void *arg)
{
    struct sockaddr_in addr;
    int fd;
    int ret;
    
    fd = socket(AF_INET,  SOCK_STREAM, 0);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
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
                char buf[32];
                socklen_t sin_size = sizeof(struct sockaddr_in);
                
                printf("waiting connection...\n");
                if((client_fd = accept(fd, (struct sockaddr *)&client_addr, &sin_size)) != -1)
                {
                    printf("a client connected.\n");
                    ret = recv(client_fd, buf, sizeof(buf), 0);
                    if (ret != -1)
                    {
                        printf("received %s\n", buf);
                    }
                    else
                    {
                        printf("recv errno = %d\n", errno);
                    }
                    close(client_fd);
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
    }
    
    close(fd);
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
	xTaskCreate(ServerTest, (signed char *) "ServerTest",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);
	xTaskCreate(ClientTest, (signed char *) "ClientTest",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 2UL),
				(xTaskHandle *) NULL);
}
