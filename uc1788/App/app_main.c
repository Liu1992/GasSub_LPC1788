/**
 * app_main.c
 *
 * Main App
 *
 * Guo Jian
 * 2014.04.22
 */
 
#include <includes.h>

void FsTest(void);
void NandTest(void);
void SctTest(void);
void TCPTest(void);
void EMACTest(void);
int goahead_main(int argc, char **argv);
int hvsftpd_main(void);

static OS_STK App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE]
#ifndef __RAM_MODE__
__attribute__ ((section("OS_STACK")))
#endif
;
static OS_STK App_TaskHttpStk[APP_CFG_TASK_HTTP_STK_SIZE]
#ifndef __RAM_MODE__
__attribute__ ((section("OS_STACK")))
#endif
;
static OS_STK App_TaskFtpStk[APP_CFG_TASK_FTP_STK_SIZE]
#ifndef __RAM_MODE__
__attribute__ ((section("OS_STACK")))
#endif
;

OS_EVENT *sem_tcp_init_done;
static struct netif eth_netif;

err_t ethernetif_init(struct netif *netif);

/**
 * eth_netif_init
 *
 * @param   none
 * @return  none
 *
 * @brief   init eth netif
 */
void eth_netif_init(void)
{
  ip_addr_t eth_ipaddr, eth_netmask, eth_gw;
  
  IP4_ADDR(&eth_gw, 10,0,0,1);
  IP4_ADDR(&eth_ipaddr, 10,0,0,2);
  IP4_ADDR(&eth_netmask, 255,0,0,0);

  netif_add(&eth_netif, &eth_ipaddr, &eth_netmask, &eth_gw, NULL, ethernetif_init, tcpip_input);
  netif_set_up(&eth_netif);
}


/**
 * tcpip_init_done
 *
 * @param   none
 * @return  none
 *
 * @brief   callback when tcp/ip init
 */
void tcpip_init_done(void *arg)
{
  OSSemPost(sem_tcp_init_done);
}

/**
 * Http thread
 *
 * @param  none
 * @return none
 *
 * @brief  run http server
 */
static void App_TaskHttp(void *arg)
{
    goahead_main(0, NULL);
}

/**
 * FTP thread
 *
 * @param  none
 * @return none
 *
 * @brief  run http server
 */
static void App_TaskFtp(void *arg)
{
    hvsftpd_main();
}

/**
 * Main thread
 *
 * @param  none
 * @return none
 *
 * @brief  Main loop
 *         Create App objects and tasks
 */
static void App_TaskStart (void *p_arg)
{
    int off = 0;
    INT8U err;
    (void)p_arg;

    printf("in app_taskstart\n");
#if (OS_TASK_STAT_EN > 0)
    /* Determine CPU capacity */
    OSStatInit();
#endif

#if 0
    NandTest();
#endif
    
    /* Init the file system */
    yaffs_start_up();
    
#if 0
    FsTest();
#endif
    yaffs_mount("/nand");

    /* Init the tcp/ip stack */
    sem_tcp_init_done = OSSemCreate(0);
    tcpip_init(tcpip_init_done, NULL);
    OSSemPend(sem_tcp_init_done, 0, &err);

    eth_netif_init();

    /*
    TCPTest();
    SctTest();
    EMACTest();
    */
    
    /* create the ftp task */
    OSTaskCreateExt((void (*)(void *))App_TaskFtp,
                    (void          *)0,
                    (OS_STK        *)&App_TaskFtpStk[APP_CFG_TASK_FTP_STK_SIZE - 1],
                    (INT8U          )APP_CFG_TASK_FTP_PRIO,
                    (INT16U         )APP_CFG_TASK_FTP_PRIO,
                    (OS_STK        *)&App_TaskFtpStk[0],
                    (INT32U         )APP_CFG_TASK_FTP_STK_SIZE,
                    (void          *)0,
                    (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    /* create the http task */
    OSTaskCreateExt((void (*)(void *))App_TaskHttp,
                    (void          *)0,
                    (OS_STK        *)&App_TaskHttpStk[APP_CFG_TASK_HTTP_STK_SIZE - 1],
                    (INT8U          )APP_CFG_TASK_HTTP_PRIO,
                    (INT16U         )APP_CFG_TASK_HTTP_PRIO,
                    (OS_STK        *)&App_TaskHttpStk[0],
                    (INT32U         )APP_CFG_TASK_HTTP_STK_SIZE,
                    (void          *)0,
                    (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
}

/**
 * Initialize Board
 *
 * @param  none
 * @return none
 *
 * @brief  Initialize Board
 */
static void Board_Init()
{
    /* init nand */
    NandFlash_Init();
    
    /* init nor */
    NORFLASHInit();
    
    /* init lcd */
    // TFT_Init();
}

/**
 * C Entry
 *
 * @param  none
 * @return none
 *
 * @brief  Initialize OS, start App.
 */
int main(void)
{
    /* Initialize BSP functions */
    Board_Init();

    GUI_Init();
    
    /* Initialize "uC/OS-II, The Real-Time Kernel" */
    OSInit();

    /* Create the start task */
    OSTaskCreateExt((void (*)(void *))App_TaskStart,
                    (void          *)0,
                    (OS_STK        *)&App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                    (INT8U          )APP_CFG_TASK_START_PRIO,
                    (INT16U         )APP_CFG_TASK_START_PRIO,
                    (OS_STK        *)&App_TaskStartStk[0],
                    (INT32U         )APP_CFG_TASK_START_STK_SIZE,
                    (void          *)0,
                    (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    /* init sys clock */
    OS_CPU_SysTickInit(CLKPWR_GetCLK(CLKPWR_CLKTYPE_CPU) / 1000 - 1);

    /* Start multitasking (i.e. give control to uC/OS-II). */
    OSStart();

    return 0;
}
