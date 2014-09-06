/****************************************Copyright (c)****************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               Main.c
** Last modified Date:      2007.01.18
** Last Version:            1.0
** Description:             The main function example template  主函数例子模版
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              Steven Zhou 周绍刚
** Created date:            2007.01.18
** Version:                 1.0
** Descriptions:            The original version 初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao
** Modified date:           2007.10.29
** Version:                 1.1
** Description:             The second version 第二版
**
*********************************************************************************************************/

#ifndef __MAIN_H 
#define __MAIN_H 

#ifdef __cplusplus
extern "C" {
#endif


/*********************************************************************************************************
  TASK PRIORITIES 任务优先级
*********************************************************************************************************/		
#define  TASK_START_PRIO                0
#define  TASK1_PRIO			   	  					1
#define  MENU_TASK_PRIO			   	 				2
#define  TASK_KEY_PRIO                  3
#define  TASK_EQUIPMENT_PRIO            4
#define  TASK_UART_RCV_PRIO             5
#define  TASK_FILE_RCV_PRIO							6
	
#define  TASK_FTP_PRIO									7
	
#define  TASK_ERROR_RATE_RS485					55
							          
/*********************************************************************************************************
  TASK STACK SIZES  任务堆栈大小
*********************************************************************************************************/
#define  TASK_START_STK_SIZE            1024
#define  MENU_TASK_STK_SIZE		  	      1024
#define  TASK_KEY_STK_SIZE		          1024
#define  TASK_EQUIPMENT_STK_SIZE		    1024
#define	 TASK_UARTRCV_STK_SIZE          1024
#define  TASK_FILERCV_STK_SIZE					1024

#define  TASK_FTP_STK_SIZE							1024

#define  TASK_ERROR_RATE_RS485_STK_SIZE 1024

#ifdef __cplusplus
    }
#endif


#endif
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/


