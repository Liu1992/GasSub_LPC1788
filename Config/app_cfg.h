/*
*********************************************************************************************************
*                                              uC/OS-II
*                                        The Real-Time Kernel
*
*                          (c) Copyright 2004-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/OS-II is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/OS-II in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to
*               help you experience uC/OS-II.  The fact that the source code is provided
*               does NOT mean that you can use it without paying a licensing fee.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       APPLICATION CONFIGURATION
*
*                                        IAR Development Kits
*                                               on the
*
*                                            NXP LPC1768
*                                      LPC1766-SK KICKSTART KIT
*
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : FT
*                 DC
*********************************************************************************************************
*/

#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT


/*
*********************************************************************************************************
*                                       ADDITIONAL uC/MODULE ENABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_PRIO                           17u
#define  APP_CFG_TASK_TCP_PRIO                             23u

#define  APP_CFG_TASK_LCD_PRIO                             19u
#define  APP_CFG_TASK_TCPSERVER_PRIO                       15u
#define  APP_CFG_TASK_TCPCLIENT_PRIO                       14u
#define  APP_CFG_TASK_ETHERNET_INPUT_PRIO                  12u
#define  APP_CFG_TASK_HTTP_PRIO                            18u
#define  APP_CFG_TASK_FTP_PRIO                             16u

#define  OS_TASK_TMR_PRIO                                  (OS_LOWEST_PRIO - 2)


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_STK_SIZE                     32768u
#define  APP_CFG_TASK_TCPTEST_STK_SIZE                   4096u

#define  APP_CFG_TASK_LCD_STK_SIZE                       512u
#define  APP_CFG_TASK_ETHERNET_INPUT_STK_SIZE            512u
#define  APP_CFG_TASK_HTTP_STK_SIZE                      4096u
#define  APP_CFG_TASK_FTP_STK_SIZE                       4096u

/*
*********************************************************************************************************
*                                             uC/LIB CONFIGURATION
*********************************************************************************************************
*/

#define  LIB_MEM_CFG_OPTIMIZE_ASM_EN                    DEF_ENABLED
#define  LIB_MEM_CFG_ARG_CHK_EXT_EN                     DEF_DISABLED
#define  LIB_MEM_CFG_ALLOC_EN                           DEF_ENABLED
#define  LIB_MEM_CFG_HEAP_SIZE                          20*1024u


/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#define  TRACE_LEVEL_OFF                                0u
#define  TRACE_LEVEL_INFO                               1u
#define  TRACE_LEVEL_DBG                                2u


#define  APP_CFG_TRACE_LEVEL                            TRACE_LEVEL_DBG
#define  APP_CFG_TRACE                                  printf

#define  APP_TRACE_INFO(x)                     ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_CFG_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                      ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_CFG_TRACE x) : (void)0)

#endif
