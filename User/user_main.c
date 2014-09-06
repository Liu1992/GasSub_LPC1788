/**********************************************************************
* $Id$		Rtc_Alarm.c			2011-06-02
*//**
* @file		Rtc_Alarm.c
* @brief	This example describes how to use RTC to generate interrupt
* 			in Second Counter Increment Interrupt (1s) and generate
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/
#include "lpc_types.h"
#include "sdram_mt48lc8m32lfb5.h"
#include "Gpioconfig.h"
#include "LPC177x_8x_ssp.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_lcd.h"
#include "lpc177x_8x_timer.h"

#include "GUI.H"
#include "BUTTON.H"
#include "stdlib.h"
#include "math.h"
#include "nandflash_k9f4g08u0b.h"
#include "phylan_dm9161a.h"

#include "norflash_sst39vf1601.h"
#include "lpc177x_8x_rtc.h"
#include "lpc177x_8x_eeprom.h"

#include "DIALOG.h"
#include "DROPDOWN.h"
#include "main.h"

#include <includes.h>
//#include <cortexm3.h>
#include "DataStruct.h"

#include "SSP_GT21L16S2W.h"
#include "SSP_M25P128.h"

#include "com_uart.h"
#include "eeprom_st24c02.h"
#include "user_SaveOption.h"
#include "Sensor.h"

/* 队列大小 */
#define KEYQSIZE 8						//按键消息队列大小
#define CONFIGQSIZE 2					//配置信息队列大小
#define UPLOADQSIZE 128				//上传信息队列大小
#define UARTSENDQSIZE 8				//8路开出口控制消息队列大小

OS_EVENT    *KeyQueueHead = NULL;           //按键消息队列指针
void        *KeyMsg[KEYQSIZE]; 		   				//按键消息指针数组

OS_EVENT    *ConfigQueueHead = NULL;				//配置信息消息队列
void        *ConfigMsg[CONFIGQSIZE];				//配置信息指针数组

OS_EVENT    *UploadQueueHead = NULL;        //上传信息消息队列
void        *UploadMsg[UPLOADQSIZE];        //上传信息指针数组 

extern void  App_KeyTaskCreate (void);
extern void  Equipment_Task (void);
extern void  Menu_Task (void);
extern void  TCPTest(void);
extern void  FileReceive_Task (void);
extern void  ErrorRate_RS485_Task(void);

extern void File_Create_TpConfig(Config_Struct* rev_config);

extern void  FsTest(void);
extern void  File_Init(void);

extern int hvsftpd_main();

extern void UartRcv_Task(void); 

static OS_STK  GstkStart[TASK_START_STK_SIZE];                     /*  The stack of start task   */
static OS_STK  Gstk2[MENU_TASK_STK_SIZE];                          /*  Led?????             */
static OS_STK  TaskEquipmentStk[TASK_EQUIPMENT_STK_SIZE]; 		   	//任务堆栈
static OS_STK  TaskUartRcvStk[TASK_UARTRCV_STK_SIZE];
static OS_STK  TaskFileRcvStk[TASK_FILERCV_STK_SIZE];
static OS_STK  TaskFtpStk[ TASK_FTP_STK_SIZE ];
static OS_STK  TaskErrorRateRS485Stk[ TASK_ERROR_RATE_RS485_STK_SIZE ];
//static OS_STK  TestStk[512];          

INT8U	err;
OS_EVENT *EquipmentTaskSem = NULL;


INT8U      uart0RcvOK;              
INT8U      uart1RcvOK;                   //串口接收成功
INT8U      uart2RcvOK;
INT8U      uart3RcvOK;
INT8U      uart4RcvOK;
IPConfigStruct ipConfig;								//IP地址配置参数全局结构体
EEPROMDataStruct optionSaveStruct;			//配置储存结构体

RcvStruct  uart1RcvInfo;                 //串口接收信息
OS_EVENT  *UartRcvTaskSem = NULL;           //串口信息结构体的信号量

extern SendStruct uart1SendInfo;
OS_EVENT  *UartSendTaskSem = NULL;

/** TCP IP **/
OS_EVENT *sem_tcp_init_done;
static struct netif eth_netif;
extern err_t ethernetif_init(struct netif *netif);
/**  **/

extern GUI_BITMAP bmG631Y2;                //Logo

extern void FsTest(void);
extern void KeyInit(void);
void Display_Logo(void);									//Display logo
void eth_netif_init(IPConfigStruct ipConfig);	//配置网络参数
void Data_To_Array(INT8U array[],INT8U data1,INT8U data2,INT8U data3,INT8U data4);
/**
 * eth_netif_init
 *
 * @param   none
 * @return  none
 *
 * @brief   init eth netif
 */
void eth_netif_init(IPConfigStruct ipConfig)
{
  ip_addr_t eth_ipaddr, eth_netmask, eth_gw;
  
  IP4_ADDR(&eth_gw,ipConfig.gw[0],ipConfig.gw[1],ipConfig.gw[2],ipConfig.gw[3]);
  IP4_ADDR(&eth_ipaddr, ipConfig.ip[0],ipConfig.ip[1],ipConfig.ip[2],ipConfig.ip[3]);
  IP4_ADDR(&eth_netmask,ipConfig.sub[0],ipConfig.sub[1],ipConfig.sub[2],ipConfig.sub[3]);

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

/*
Use for debug

void Test_Key(void)
{
		int i = 0;
		while (1)
		{
				OSTimeDly(500);
				if ( i < 3 )
				{
						GUI_StoreKeyMsg(GUI_KEY_TAB, 1);
						OSTimeDly(10);
						GUI_StoreKeyMsg(GUI_KEY_TAB, 0);
				}
				else 
				{
						GUI_StoreKeyMsg(GUI_KEY_DOWN, 1);
						OSTimeDly(10);
						GUI_StoreKeyMsg(GUI_KEY_DOWN, 0);
						OSTimeDly(10);
						GUI_StoreKeyMsg(GUI_KEY_ENTER, 1);
						OSTimeDly(10);
						GUI_StoreKeyMsg(GUI_KEY_ENTER, 0);
				}
				++i;
				if ( i > 4 ) i = 0;
		}
		
		return;
}
*/

/*********************************************************************************************************
** Function name:           taskStart	   
** Descriptions:            Start task	
** input parameters:        *parg
** output parameters:       ?      
** Returned value:          ? 
*********************************************************************************************************/
static void TaskStart (void  *parg)
{
		INT8U err;
		
    (void)parg;

    #if OS_TASK_STAT_EN > 0
    OSStatInit();                                                       /*  Enable statistics           */
    #endif
	
		/* Init the file system */
    yaffs_start_up();	
		File_Init();
		
		KeyInit();
   	KeyQueueHead=OSQCreate(KeyMsg,KEYQSIZE);		//创建按键消息队列 在优先级较高的任务中执行
		
		ConfigQueueHead = OSQCreate( ConfigMsg, CONFIGQSIZE );
		UploadQueueHead = OSQCreate( UploadMsg, UPLOADQSIZE );
	
		EquipmentTaskSem = OSSemCreate(0);
		UartRcvTaskSem   = OSSemCreate(1);
		sem_tcp_init_done = OSSemCreate(0);
		UartSendTaskSem = OSSemCreate(1);
		
		tcpip_init(tcpip_init_done, NULL);
    OSSemPend(sem_tcp_init_done, 0, &err);

    eth_netif_init(ipConfig);
		
		TCPTest();
 
    err = OSTaskCreate((void (*)(void *)) Equipment_Task,				
                          (void          * ) 0,							
                          (OS_STK        * )&TaskEquipmentStk[TASK_KEY_STK_SIZE - 1],		
                          (INT8U           ) TASK_EQUIPMENT_PRIO  );  				/*	Create Equipment Task */
		
		//printf("Equipment_Task: err = %d\n", err );
	
		err = OSTaskCreate((void (*)(void *)) UartRcv_Task,				
                          (void          * ) 0,							
                          (OS_STK        * )&TaskUartRcvStk[TASK_KEY_STK_SIZE - 1],		
                          (INT8U           ) TASK_UART_RCV_PRIO  );  				  /*	Create Equipment Task */
													
		//printf("UartRcv_Task: err = %d\n", err );
													
		err = OSTaskCreate((void (*)(void *)) FileReceive_Task,				
                          (void          * ) 0,							
                          (OS_STK        * )&TaskFileRcvStk[TASK_FILERCV_STK_SIZE - 1],		
                          (INT8U           ) TASK_FILE_RCV_PRIO  );  			  	/*	Create FileReceive Task */
		
		//printf("FileReceive_Task: err = %d\n", err );
				
		err = OSTaskCreate((void (*)(void *)) hvsftpd_main,				
                          (void          * ) 0,							
                          (OS_STK        * )&TaskFtpStk[TASK_FTP_STK_SIZE - 1],		
                          (INT8U           ) TASK_FTP_PRIO  );  			  	/*	Create FileReceive Task */
		
		printf(" hvsftpd_Task: err = %d\n", err );
													
	
		err = OSTaskCreate ((void (*)(void	*))Menu_Task, 
						   (void 	*)0,   
						   (OS_STK	*)&Gstk2[MENU_TASK_STK_SIZE-1],
						   (INT8U	 )MENU_TASK_PRIO  	);                           /*  Create Menu Task     */ 	
							 
/*		err = OSTaskCreate ((void (*)(void	*))Test_Key, 
						   (void 	*)0,   
						   (OS_STK	*)&TestStk[511],
						   (INT8U	 )44  	);                           /  Create Menu Task     */ 	
		
	/*	
		err = OSTaskCreate ((void (*)(void	*))ErrorRate_RS485_Task, 
						   (void 	*)0,   
						   (OS_STK	*)&TaskErrorRateRS485Stk[ TASK_ERROR_RATE_RS485_STK_SIZE-1 ],
						   (INT8U	 )TASK_ERROR_RATE_RS485  	);                          
		printf("ErrorRateRS485_Task: err = %d\n", err );*/
			
		App_KeyTaskCreate();						  /* Create Task Key */

		while (1) 
		{                             
        OSTaskSuspend(OS_PRIO_SELF);                                    /*  The start task can be pended*/
                                                                        /*  here. ??????????  */
		}
}

/*********************************************************************************************************
** Function name:           System_Time_Init	   
** Descriptions:            Initialize Real Time Clock	
** input parameters:        void
** output parameters:       none      
** Returned value:          void
** Changed by:              GuoBingrong 2014-3-30  
*********************************************************************************************************/

static void System_Time_Init(void)
{
	/* RTC Block section ------------------------------------------------------ */
	// Init RTC module
	RTC_Init(LPC_RTC);

    /* Disable RTC interrupt */
    NVIC_DisableIRQ(RTC_IRQn);
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(RTC_IRQn, ((0x01<<3)|0x01));

	/* Enable rtc (starts increase the tick counter and second counter register) */
	RTC_ResetClockTickCounter(LPC_RTC);
	RTC_Cmd(LPC_RTC, ENABLE);
	RTC_CalibCounterCmd(LPC_RTC, DISABLE);

	/* Set current time for RTC */
	// Current time is 06:45:00PM, 2011-03-25
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, 0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, 45);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, 15);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, 3);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, 2014);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, 30);

	return;
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
	
		/*	init eeprom	*/
		EEPROM_24c02_Init();
	
		/*	init option	*/
		optionSaveStruct=EEPROM_ReadStruct();
	
		ipConfig=optionSaveStruct.ipConfig;
	
    /* init lcd */
		//TFT_Init();
}

/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */

int main(void)
{	
		OS_CPU_SysTickInit(CLKPWR_GetCLK(CLKPWR_CLKTYPE_CPU) / 1000 - 1);

		Board_Init();
	
		//KeyInit();
	
		GUI_Init();
	
		Display_Logo();
		
	/* init phy */
    PHY_Init(optionSaveStruct.ipConfig.mac);
		
		M25P128_SSP_Init();
	
		GT21L16S2W_SSP_Init();
	
		LPC_Uart_Init(1200*(0x01<<optionSaveStruct.uartConfig[2]),1200*(0x01<<optionSaveStruct.uartConfig[3]),1200*(0x01<<optionSaveStruct.uartConfig[4]),1200*(0x01<<optionSaveStruct.uartConfig[5]));
	
		System_Time_Init();		 									   /*   Init RTC    */

		OSInit();

    OSTaskCreate ( TaskStart,(void *)0,&GstkStart[TASK_START_STK_SIZE-1],TASK_START_PRIO );     /*  Initialize the start task   */
                                                                   /*  Start OS Schedule         */  
    OSStart();                                                     /*  Start uC/OS-II ??uC/OS-II */
    return(0) ;	
	
}

/*********************************************************************************************************
** Function name:           Display_Logo	   
** Descriptions:            Display logo	
** input parameters:        none
** output parameters:       none      
** Returned value:          void
** Changed by:              Lius 2014-8-18  
*********************************************************************************************************/
void Display_Logo()
{
	GUI_SetColor(GUI_WHITE);
	GUI_FillRect(0,0,C_GLCD_H_SIZE,C_GLCD_V_SIZE);
	GUI_DrawBitmap(&bmG631Y2,87,54);		
	GPIO_SetValue(2,LCD_BACK_LIGHT);		
}

