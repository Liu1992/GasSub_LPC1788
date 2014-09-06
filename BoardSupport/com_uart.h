/*********************************************************************************************
* File name	: com_uart.h
* HardWare	: LPC1788 
* Author		: Lius
* History		:	none
*	Data			:	2014-07-17
* Descript	: LPC1788串口驱动
*	Version		: V2.2.0
*********************************************************************************************/

#ifndef __COM_UART_H_
#define __COM_UART_H_

/*****************************************************************************
 * Include Files
 ****************************************************************************/
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_clkpwr.h"
#include "os_cpu.h"
#include "ucos_ii.h"

/*****************************************************************************
 * Defines and typedefs
 ****************************************************************************/

typedef struct
{
	uint8_t head;
	uint8_t address;	
	uint8_t ctrl;
	uint8_t len;
	uint8_t	*data;
	uint8_t cheak;
	uint8_t tail;
}RS485SendStruct;

typedef struct
{
	uint8_t head;
	uint8_t ctrl;
	uint8_t len;
	uint8_t *data;
	uint8_t check;
	uint8_t tail;
}RS486RcvStruct;

typedef struct
{
	uint8_t Send[10];
	uint8_t	Rcv[15];
	uint8_t	sendIndex;
	uint8_t rcvIndex;
	uint8_t sendOK;
	uint8_t readyToRcv;
	uint8_t sendLen;
}RS485TMP;

/*****************************************************************************
 * 用户使用结构体定义
 ****************************************************************************/
typedef struct											/*	RS485发送结构体	*/
{
	uint8_t address;
	uint8_t ctrl;
	uint8_t len;
	uint8_t data[3];
}UserRS485Send;				

typedef struct											/*	RS485接收结构体	*/
{
	uint8_t ctrl;
	uint8_t len;
	uint8_t data[10];
}UserRS485Rcv;

 typedef struct											/*	Uart1接收结构体	*/
{
	uint16_t Fre[16];//存储十六路输入口的fre统计数据
	uint16_t AD[16];//存储十六路输入口的AD
	uint32_t count[16];//存储十六路输入口频率统计计数和
	uint16_t sel;//存储十六路输入口的高低电平情况
}RcvStruct;

typedef struct											/*	Uart1发送结构体	*/
{
	uint16_t clearFreCount;//是否清除端口计数
	uint16_t SEL;//SEL模块
	uint8_t outPut;//output端口电位控制
}SendStruct;

typedef enum {											/*	US485串口号			*/	
	RS485_UART0 	= 0,								/*	RS485_UART0		*/
	RS485_UART2		=	1,	 							/*	RS485_UART2 	*/
	RS485_UART3		=	2, 								/*	RS485_UART3 	*/
	RS485_UART4		=	3, 								/*	RS485_UART4		*/
} RS485_UART_NUM;

/*****************************************************************************
 * 函数声明区
 ****************************************************************************/

void LPC_Uart_Init(uint16_t RS485BAND_1,uint16_t RS485BAND_2,uint16_t RS485BAND_3,uint16_t RS485BAND_4);
void UART1_IRQHandler(void);
RcvStruct Uart1_Get_Struct(void);
void Uart1_Send_Struct(SendStruct* sstruct);
void RS485_Baudrate_Config(RS485_UART_NUM UART_NUM,uint16_t Baudrate);
void RS485Send_Struct(RS485_UART_NUM UART_NUM,UserRS485Send* rs485Send_S);
UserRS485Rcv RS485_Get_Struct(RS485_UART_NUM UART_NUM);


/*****************************************************************************
 * Global variable
 ****************************************************************************/



#endif
