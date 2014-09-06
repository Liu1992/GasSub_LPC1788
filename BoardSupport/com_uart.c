/*********************************************************************************************
* File name	: com_uart1.c
* HardWare	: LPC1788 
* Author		: Lius
* History		:	none
*	Data			:	2014-07-17
* Descript	: LPC1788串口驱动
*	Version		: V2.2.0
*********************************************************************************************/
 
 /*****************************************************************************
 * Include Files
 ****************************************************************************/
#include "com_uart.h"
#include <stdlib.h>


/*****************************************************************************
 * Defines and typedefs
 ****************************************************************************/

#define RS485BAUDRATE 		(2400)								/*	RS485波特率默认设置			*/
#define UART1SEND_INT  		1											/*	UART1发送中断方式设置		*/
#define UART1RCV_INT	 		1											/*	UART1接收中断方式设置		*/
#define RS485SEND_INT			1											/*	RS485发送中断						*/
#define RS485RCV_INT			1											/*	RS485接收中断						*/
#define ENABLEUARTIRQ0_4 	1											/*	RS485中断开启设置				*/
#define RS485PARITY				UART_PARITY_ODD				/*	RS485校验位设置					*/
#define RS485FIFOTRGLEV		UART_FIFO_TRGLEV0			/*	RS485FIFO中断字节设置		*/
#define ENABLERS485RCVER	1											/*	开启rs485接收						*/


#ifdef FUNCTIONREWRITE
/* buffer size definition */
#define UART_RING_BUFSIZE 256

/* Buf mask */
#define __BUF_MASK (UART_RING_BUFSIZE-1)
/* Check buf is full or not */
#define __BUF_IS_FULL(head, tail) ((tail&__BUF_MASK)==((head+1)&__BUF_MASK))
/* Check buf will be full in next receiving or not */
#define __BUF_WILL_FULL(head, tail) ((tail&__BUF_MASK)==((head+2)&__BUF_MASK))
/* Check buf is empty */
#define __BUF_IS_EMPTY(head, tail) ((head&__BUF_MASK)==(tail&__BUF_MASK))
/* Reset buf */
#define __BUF_RESET(bufidx)	(bufidx=0)
#define __BUF_INCR(bufidx)	(bufidx=(bufidx+1)&__BUF_MASK)

/************************** PRIVATE TYPES *************************/
/** @brief UART Ring buffer structure */
typedef struct
{
    __IO uint32_t tx_head;                /*!< UART Tx ring buffer head index */
    __IO uint32_t tx_tail;                /*!< UART Tx ring buffer tail index */
    __IO uint32_t rx_head;                /*!< UART Rx ring buffer head index */
    __IO uint32_t rx_tail;                /*!< UART Rx ring buffer tail index */
    __IO uint8_t  tx[UART_RING_BUFSIZE];  /*!< UART Tx data ring buffer */
    __IO uint8_t  rx[UART_RING_BUFSIZE];  /*!< UART Rx data ring buffer */
} UART_RING_BUFFER_T;

// UART Ring buffer
UART_RING_BUFFER_T rb;

#endif

/************************** PRIVATE DEFINITIONS *************************/
/*
 * These following defines can be modified:
 * - RECEIVER_ALWAYS_EN (0/1)
 * - AUTO_SLVADDR_DETECT (0/1) in case RECEIVER_ALWAYS_EN is set to 0
 */

/* Receiver always be enabled to receive any data frame on RS485 bus,
 * regardless that frame is data frame or slave address frame (9-bit).
 * - When receiving a data frame, slave will display that data frame content
 * via UART0.
 * - When receiving a slave address frame (9bit mode), line error interrupt
 * - 0: Receiver is not always enabled, only slave address frame can trigger
 * an interrupt event to allow slave handle.
 * - 1: Receiver always be enabled */
 
#define RECEIVER_ALWAYS_EN	1

#if (RECEIVER_ALWAYS_EN == 0)

/* Enable/Disable Auto Slave Address Detection
 * - In case of '0': any received data bytes will be ignored and will not
 * be stored in the RXFIFO. When an address byte is detected (parity bit
 * = '1') it will be placed into the RXFIFO and an Rx Data Ready Interrupt
 * will be generated. The interrupt handler can then read the address byte
 * and decide whether or not to enable the receiver to accept the following data.
 * - In case of '1': any received byte will be discarded if it is either a
 * data byte OR an address byte which fails to match the slave address configured
 * when initializing RS485. When a matching address character is detected it will
 * be pushed onto the RXFIFO along with the parity bit, and the receiver will
 * be automatically enabled (RS485CTRL bit 1 will be cleared by hardware).
 * The receiver will also generate an Rx Data Ready Interrupt */
 
#define AUTO_SLVADDR_DETECT 1										/*		RS485地址相关设置			*/
#define UART0_ADDR 'A'
#define UART2_ADDR 'B'
#define UART3_ADDR 'C'
#define UART4_ADDR 'D'
#endif


/**********************************全局变量*************************************************/

uint8_t dataToSend[8];
uint8_t dataToRcv[134];
extern uint8_t uart1RcvOK;
uint8_t uart1RcvStart;
uint8_t readyToRcv;
uint16_t uart1SendIndex;
uint16_t uart1RcvIndex;
uint8_t uart1SendOK;

RS485TMP RS485Uart0;
RS485TMP RS485Uart2;
RS485TMP RS485Uart3;
RS485TMP RS485Uart4;

extern uint8_t uart0RcvOK;
extern uint8_t uart2RcvOK;
extern uint8_t uart3RcvOK;
extern uint8_t uart4RcvOK;

/*********************************************************************************************
* name			:		LPC_Uart_Init
* func			:		Uart初始化
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/
void LPC_Uart_Init(uint16_t RS485BAND_1,uint16_t RS485BAND_2,uint16_t RS485BAND_3,uint16_t RS485BAND_4)
{
	
	/*********************struct define*************************/
	UART_CFG_Type UartConfig;
	UART_FIFO_CFG_Type UartFIFOConfig;
	UART1_RS485_CTRLCFG_Type rs485cfg;
	
	uart1RcvOK=0;																		/**接收成功标记清除*/
	uart1RcvStart=0;
	readyToRcv=0;
	uart1RcvIndex=0;
	uart1SendIndex=0;
	uart1SendOK=1;
	
	RS485Uart0.rcvIndex=0;
	RS485Uart0.sendIndex=0;
	RS485Uart0.sendOK=1;
	RS485Uart0.readyToRcv=0;
	
	RS485Uart2.rcvIndex=0;
	RS485Uart2.sendIndex=0;
	RS485Uart2.sendOK=1;
	RS485Uart2.readyToRcv=0;
	
	RS485Uart3.rcvIndex=0;
	RS485Uart3.sendIndex=0;
	RS485Uart3.sendOK=1;
	RS485Uart3.readyToRcv=0;
	
	RS485Uart4.rcvIndex=0;
	RS485Uart4.sendIndex=0;
	RS485Uart4.sendOK=1;
	RS485Uart4.readyToRcv=0;
	
	/********************Set the pin configuration******************/
	
	
	PINSEL_ConfigPin(0,2,1);/*TX*/										/***uart 0***/
	PINSEL_ConfigPin(0,3,1);/*RX*/
	PINSEL_ConfigPin(5,4,1);/*OE*/
	
	PINSEL_ConfigPin(0,15,1);/*TX*/										/***uart 1***/
	PINSEL_ConfigPin(0,16,1);/*RX*/
	
	PINSEL_ConfigPin(4,22,2);/*TX*/										/***uart 2***/
	PINSEL_ConfigPin(4,23,2);/*RX*/
	PINSEL_ConfigPin(1,19,6);/*OE*/
	
	PINSEL_ConfigPin(0,25,3);/*TX*/										/***uart 3***/
	PINSEL_ConfigPin(0,26,3);/*RX*/
	PINSEL_ConfigPin(1,30,5);/*OE*/
	
	PINSEL_ConfigPin(0,22,3);/*TX*/										/***uart 4***/
	PINSEL_ConfigPin(5,3,4); /*RX*/
	PINSEL_ConfigPin(0,21,3);/*OE*/
	
	/********************Set the uart configuration*****************/
	UartConfig.Baud_rate=115200;											/***uart 1***/
	UartConfig.Databits=UART_DATABIT_8;
	UartConfig.Stopbits=UART_STOPBIT_1;
	UartConfig.Parity=UART_PARITY_NONE;
	UART_Init((LPC_UART_TypeDef *)LPC_UART1,&UartConfig);
	
	/**************************RS485********************************/
	
	UartConfig.Parity=RS485PARITY;
	
	UartConfig.Baud_rate=RS485BAND_1;	
	UART_Init(LPC_UART0,&UartConfig);									/***uart 0***/
	
	UartConfig.Baud_rate=RS485BAND_2;	
	UART_Init(LPC_UART2,&UartConfig);									/***uart 2***/
	
	UartConfig.Baud_rate=RS485BAND_3;	
	UART_Init(LPC_UART3,&UartConfig);									/***uart 3***/
	
	UartConfig.Baud_rate=RS485BAND_4;	
	UART_Init((LPC_UART_TypeDef *)LPC_UART4,&UartConfig);									/***uart 4***/
	
	/********************Set the uart FIFO configure************/
	UartFIFOConfig.FIFO_DMAMode=DISABLE;
	UartFIFOConfig.FIFO_ResetRxBuf=ENABLE;
	UartFIFOConfig.FIFO_ResetTxBuf=ENABLE;
	UartFIFOConfig.FIFO_Level=UART_FIFO_TRGLEV0; /* 8 character  */
	UART_FIFOConfig(UART_1,&UartFIFOConfig);
	
	
	UartFIFOConfig.FIFO_Level=RS485FIFOTRGLEV; 	/* 8 character  */
	UART_FIFOConfig(UART_0,&UartFIFOConfig);
	UART_FIFOConfig(UART_2,&UartFIFOConfig);
	UART_FIFOConfig(UART_3,&UartFIFOConfig);
	UART_FIFOConfig(UART_4,&UartFIFOConfig);
	
	/********************Set the RS485 Configuration************/
	// Configure RS485
	/*
	* - Auto Direction in Tx/Rx driving is enabled
	* - Direction control pin is set to DTR1
	* - Direction control pole is set to "1" that means direction pin
	* will drive to high state before transmit data.
	* - Multidrop mode is enable
	* - Auto detect address is disabled
	* - Receive state is enable
	*/
	rs485cfg.AutoDirCtrl_State = ENABLE;
	rs485cfg.DirCtrlPin = UART_RS485_DIRCTRL_DTR;
	rs485cfg.DirCtrlPol_Level = RESET;
	rs485cfg.DelayValue = 50;
	rs485cfg.NormalMultiDropMode_State = ENABLE;


#if RECEIVER_ALWAYS_EN
	rs485cfg.Rx_State = ENABLE;
	rs485cfg.AutoAddrDetect_State = DISABLE;
#else
	rs485cfg.Rx_State = DISABLE;
#if AUTO_SLVADDR_DETECT
	rs485cfg.AutoAddrDetect_State = ENABLE;
#else
	rs485cfg.MatchAddrValue = UART0_ADDR;
#endif
#endif
	UART_RS485Config(UART_0, &rs485cfg);				/***Uart_0***/

#if RECEIVER_ALWAYS_EN
	rs485cfg.Rx_State = ENABLE;
	rs485cfg.AutoAddrDetect_State = DISABLE;
#else
	rs485cfg.Rx_State = DISABLE;
#if AUTO_SLVADDR_DETECT
	rs485cfg.AutoAddrDetect_State = ENABLE;
#else
	rs485cfg.AutoAddrDetect_State = DISABLE;
	rs485cfg.MatchAddrValue = UART2_ADDR;
#endif
#endif
	UART_RS485Config(UART_2, &rs485cfg);				/***Uart_2***/

#if RECEIVER_ALWAYS_EN
	rs485cfg.Rx_State = ENABLE;
	rs485cfg.AutoAddrDetect_State = DISABLE;
#else
	rs485cfg.Rx_State = DISABLE;
#if AUTO_SLVADDR_DETECT
	rs485cfg.AutoAddrDetect_State = ENABLE;
#else
	rs485cfg.AutoAddrDetect_State = DISABLE;
	rs485cfg.MatchAddrValue = UART3_ADDR;
#endif
#endif
	UART_RS485Config(UART_3, &rs485cfg);				/***Uart_3***/

#if RECEIVER_ALWAYS_EN
	rs485cfg.Rx_State = ENABLE;
	rs485cfg.AutoAddrDetect_State = DISABLE;
#else
	rs485cfg.Rx_State = DISABLE;
#if AUTO_SLVADDR_DETECT
	rs485cfg.AutoAddrDetect_State = ENABLE;
#else
	rs485cfg.AutoAddrDetect_State = DISABLE;
	rs485cfg.MatchAddrValue = UART4_ADDR;
#endif
#endif
	UART_RS485Config(UART_4, &rs485cfg);				/***Uart_4***/

	/*****************Enable the uartrx exit********************/
	UART_IntConfig(UART_1, UART_INTCFG_RBR, ENABLE);
	UART_IntConfig(UART_0, UART_INTCFG_RBR, ENABLE);
	UART_IntConfig(UART_2, UART_INTCFG_RBR, ENABLE);
	UART_IntConfig(UART_3, UART_INTCFG_RBR, ENABLE);
	UART_IntConfig(UART_4, UART_INTCFG_RBR, ENABLE);
	
	/*******************Enable the uart line exit***************/
	UART_IntConfig(UART_1, UART_INTCFG_RLS, ENABLE);
	UART_IntConfig(UART_0, UART_INTCFG_RLS, ENABLE);
	UART_IntConfig(UART_2, UART_INTCFG_RLS, ENABLE);
	UART_IntConfig(UART_3, UART_INTCFG_RLS, ENABLE);
	UART_IntConfig(UART_4, UART_INTCFG_RLS, ENABLE);
	
	/*******************Set the NVIC priority*******************/
	NVIC_SetPriority(UART1_IRQn, ((0x01<<3)|0x01));
	NVIC_SetPriority(UART0_IRQn, ((0x01<<3)|0x02));
	NVIC_SetPriority(UART2_IRQn, ((0x01<<3)|0x02));
	NVIC_SetPriority(UART3_IRQn, ((0x01<<3)|0x02));
	NVIC_SetPriority(UART4_IRQn, ((0x01<<3)|0x02));
	
	/***********************Enable the NVIC********************/
	NVIC_EnableIRQ(UART1_IRQn);
	
#if ENABLEUARTIRQ0_4
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART3_IRQn);
	NVIC_EnableIRQ(UART4_IRQn);
#endif

#if ENABLERS485RCVER
	UART_RS485ReceiverCmd(UART_0,ENABLE);
	UART_RS485ReceiverCmd(UART_2,ENABLE);
	UART_RS485ReceiverCmd(UART_3,ENABLE);
	UART_RS485ReceiverCmd(UART_4,ENABLE);
#endif

	/*******************Uart enable****************************/
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART1,ENABLE);
	UART_TxCmd(LPC_UART0,ENABLE);
	UART_TxCmd(LPC_UART2,ENABLE);
	UART_TxCmd(LPC_UART3,ENABLE);
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART4,ENABLE);
	
}

/*********************************************************************************************
* name			:		LPC_RS485_Init
* func			:		RS485更改波特率
* para			:		RS485_UART_NUM UART_NUM,uint16_t Baudrate
* ret				:		none
* comment		:	
*********************************************************************************************/
void RS485_Baudrate_Config(RS485_UART_NUM UART_NUM,uint16_t Baudrate)
{
	
	/*********************初始化结构体***********************/
	UART_CFG_Type UartConfig;
	
	/**********************结构体赋值***********************/
	UartConfig.Baud_rate=Baudrate;											/***RS485***/
	UartConfig.Databits=UART_DATABIT_8;
	UartConfig.Stopbits=UART_STOPBIT_1;
	UartConfig.Parity=RS485PARITY;
	
	switch(UART_NUM)
	{
		case RS485_UART0:
			UART_Init(LPC_UART0,&UartConfig);									/***uart 0***/
			break;
		case RS485_UART2:
			UART_Init(LPC_UART2,&UartConfig);									/***uart 2***/
			break;
		case RS485_UART3:
			UART_Init(LPC_UART3,&UartConfig);									/***uart 3***/
			break;
		case RS485_UART4:
			UART_Init((LPC_UART_TypeDef *)LPC_UART4,&UartConfig);									/***uart 4***/
			break;
		default:
			break;
	}
	
}

/*********************************************************************************************
* name			:		UART1_IRQHandler
* func			:		Uart1中断处理函数
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/
void UART1_IRQHandler()
{
	uint8_t lineStatus;
	uint8_t rcvTemp;
	uint32_t intsrc;
	
	OSIntEnter();																	/****通知os进入中断*/
	
	intsrc=UART_GetIntId(UART_1);
	if((intsrc&0x0f)==UART_IIR_INTID_RLS)						/****接收线中断****/
	{
		//2.1 检查线状态
		lineStatus = UART_GetLineStatus(UART_1);//读取LSR时中断会被清除
		lineStatus &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
		//2.2 处理错误
		if(lineStatus)
		{
		}
					
	}
	/****接收数据或者超时中断****/
	else if (((intsrc&0x0f) == UART_IIR_INTID_RDA) || ((intsrc&0x0f) == UART_IIR_INTID_CTI))
	{
		if(readyToRcv==1)
		{
#ifdef UART1RCV_INT
			dataToRcv[uart1RcvIndex]=UART_ReceiveByte((LPC_UART_TypeDef *)LPC_UART1);
			uart1RcvIndex+=1;
			if(uart1RcvIndex<134)
			{
				OSIntExit();											/*****通知os退出中断******/
				return;
			}
#else
			UART_Receive((LPC_UART_TypeDef *)LPC_UART1,dataToRcv,0x86, BLOCKING);			/*从串口连续接收一个包*/
#endif
			if(dataToRcv[132]==0xE5&&dataToRcv[133]==0x5E)
			{
				uart1RcvOK=1;				/*设标记位，标志接收成功*/
				readyToRcv=0;
				uart1RcvIndex=0;
				OSIntExit();											/*****通知os退出中断******/
				return;
			}
			else 
			{
				uart1RcvOK=0;				/*接收失败*/
				readyToRcv=0;
				uart1RcvIndex=0;
				OSIntExit();											/*****通知os退出中断******/
				return;
			}
		}
		rcvTemp=UART_ReceiveByte((LPC_UART_TypeDef *)LPC_UART1);
		if(rcvTemp==0x5A&&uart1RcvStart==0)//若接收未开始且收到开始标记，则标记接收开始
		{
			uart1RcvStart=1;
		}
		else if(uart1RcvStart==1)
		{
			if(rcvTemp==0xA5)//若开始标记全部复合，则直接接收数据块
			{
				readyToRcv=1;
				uart1RcvStart=0;
				uart1RcvIndex=0;
			}
		}
	}
	/****	发送区为空中断	****/
	else if((intsrc&0x0f) == UART_IIR_INTID_THRE)
	{	
		uart1SendIndex+=1;
		UART_SendByte((LPC_UART_TypeDef *)LPC_UART1,dataToSend[uart1SendIndex]);
		
		if(uart1SendIndex>=7)
		{
			UART_IntConfig(UART_1, UART_INTCFG_THRE, DISABLE);						/* 		Enable the tr exit			*/
			uart1SendOK=1;
			uart1SendIndex=0;
		}
	}
	OSIntExit();											/*****通知os退出中断******/
	
}

/*********************************************************************************************
* name			:		UART0_IRQHandler
* func			:		Uart0中断处理函数
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/
void UART0_IRQHandler()
{
	uint8_t lineStatus;
	uint8_t rcvTemp;
	uint32_t intsrc;
	
	OSIntEnter();																	/****通知os进入中断*/
	
	intsrc=UART_GetIntId(UART_0);
	if((intsrc&0x0f)==UART_IIR_INTID_RLS)						/****接收线中断****/
	{
		//2.1 检查线状态
		lineStatus = UART_GetLineStatus(UART_0);//读取LSR时中断会被清除
		lineStatus &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
		//2.2 处理错误
		if(lineStatus)
		{
		}
					
	}
	/****接收数据或者超时中断****/
	else if (((intsrc&0x0f) == UART_IIR_INTID_RDA) || ((intsrc&0x0f) == UART_IIR_INTID_CTI))
	{
		if(RS485Uart0.readyToRcv==1)
		{

			RS485Uart0.Rcv[RS485Uart0.rcvIndex]=UART_ReceiveByte(LPC_UART0);
			RS485Uart0.rcvIndex+=1;
			if(RS485Uart0.Rcv[RS485Uart0.rcvIndex-1]!='E')													/*	未收到包尾		*/
			{
				OSIntExit();											/*****通知os退出中断******/
				return;
			}

			else if(RS485Uart0.Rcv[RS485Uart0.rcvIndex-1]=='E'&&RS485Uart0.rcvIndex>=5)			/*	收到包尾且长度大于5	*/
			{
				if(RS485Uart0.Rcv[RS485Uart0.rcvIndex-2]==(('S'+'E')&0xff))										/*	检测校验位						*/
				{
					uart0RcvOK=1;				/*设标记位，标志接收成功*/
					RS485Uart0.readyToRcv=0;
					RS485Uart0.rcvIndex=0;
					OSIntExit();											/*****通知os退出中断******/
					return;
				}
				else
				{
					uart0RcvOK=0;				/*设标记位，标志接收成功*/
					RS485Uart0.readyToRcv=0;
					RS485Uart0.rcvIndex=0;
					OSIntExit();											/*****通知os退出中断******/
					return;
				}
					
			}
			else 
			{
				OSIntExit();											/*****通知os退出中断******/
				return;
			}
		}
		rcvTemp=UART_ReceiveByte(LPC_UART0);
		if(rcvTemp=='S'&&RS485Uart0.readyToRcv==0)//若接收未开始且收到开始标记，则标记接收开始
		{
			RS485Uart0.readyToRcv=1;
		}
	}
	/****	发送区为空中断	****/
	else if((intsrc&0x0f) == UART_IIR_INTID_THRE)
	{	
		RS485Uart0.sendIndex+=1;
		UART_SendByte(LPC_UART0,RS485Uart0.Send[RS485Uart0.sendIndex]);
		if((RS485Uart0.sendIndex>=RS485Uart0.sendLen-1)&&RS485Uart0.Send[RS485Uart0.sendIndex]=='E')
		{
			UART_IntConfig(UART_0, UART_INTCFG_THRE, DISABLE);						/* 		Enable the tr exit			*/
			RS485Uart0.sendOK=1;
			RS485Uart0.sendIndex=0;
		}
	}
	OSIntExit();											/*****通知os退出中断******/
}

/*********************************************************************************************
* name			:		UART2_IRQHandler
* func			:		Uart2中断处理函数
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/
void UART2_IRQHandler()
{
	uint8_t lineStatus;
	uint8_t rcvTemp;
	uint32_t intsrc;
	
	OSIntEnter();																	/****通知os进入中断*/
	
	intsrc=UART_GetIntId(UART_2);
	if((intsrc&0x0f)==UART_IIR_INTID_RLS)						/****接收线中断****/
	{
		//2.1 检查线状态
		lineStatus = UART_GetLineStatus(UART_2);//读取LSR时中断会被清除
		lineStatus &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
		//2.2 处理错误
		if(lineStatus)
		{
		}
					
	}
	/****接收数据或者超时中断****/
	else if (((intsrc&0x0f) == UART_IIR_INTID_RDA) || ((intsrc&0x0f) == UART_IIR_INTID_CTI))
	{
		if(RS485Uart2.readyToRcv==1)
		{

			RS485Uart2.Rcv[RS485Uart2.rcvIndex]=UART_ReceiveByte(LPC_UART2);
			RS485Uart2.rcvIndex+=1;
			if(RS485Uart2.Rcv[RS485Uart2.rcvIndex-1]!='E')													/*	未收到包尾		*/
			{
				OSIntExit();											/*****通知os退出中断******/
				return;
			}

			else if(RS485Uart2.Rcv[RS485Uart2.rcvIndex-1]=='E'&&RS485Uart2.rcvIndex>=5)			/*	收到包尾且长度大于5	*/
			{
				if(RS485Uart2.Rcv[RS485Uart2.rcvIndex-2]==(('S'+'E')&0xff))										/*	检测校验位						*/
				{
					uart2RcvOK=1;				/*设标记位，标志接收成功*/
					RS485Uart2.readyToRcv=0;
					RS485Uart2.rcvIndex=0;
					OSIntExit();											/*****通知os退出中断******/
					return;
				}
				else
				{
					uart2RcvOK=0;				/*设标记位，标志接收成功*/
					RS485Uart2.readyToRcv=0;
					RS485Uart2.rcvIndex=0;
					OSIntExit();											/*****通知os退出中断******/
					return;
				}
					
			}
			else 
			{
				OSIntExit();											/*****通知os退出中断******/
				return;
			}
		}
		rcvTemp=UART_ReceiveByte(LPC_UART2);
		if(rcvTemp=='S'&&RS485Uart2.readyToRcv==0)//若接收未开始且收到开始标记，则标记接收开始
		{
			RS485Uart2.readyToRcv=1;
		}
	}
	/****	发送区为空中断	****/
	else if((intsrc&0x0f) == UART_IIR_INTID_THRE)
	{	
		RS485Uart2.sendIndex+=1;
		UART_SendByte(LPC_UART2,RS485Uart2.Send[RS485Uart2.sendIndex]);
		if((RS485Uart2.sendIndex>=RS485Uart2.sendLen-1)&&RS485Uart2.Send[RS485Uart2.sendIndex]=='E')
		{
			UART_IntConfig(UART_2, UART_INTCFG_THRE, DISABLE);						/* 		Enable the tr exit			*/
			RS485Uart2.sendOK=1;
			RS485Uart2.sendIndex=0;
		}
	}
	OSIntExit();											/*****通知os退出中断******/
}

/*********************************************************************************************
* name			:		UART3_IRQHandler
* func			:		Uart3中断处理函数
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/
void UART3_IRQHandler()
{
	uint8_t lineStatus;
	uint8_t rcvTemp;
	uint32_t intsrc;
	
	OSIntEnter();																	/****通知os进入中断*/
	
	intsrc=UART_GetIntId(UART_3);
	if((intsrc&0x0f)==UART_IIR_INTID_RLS)						/****接收线中断****/
	{
		//2.1 检查线状态
		lineStatus = UART_GetLineStatus(UART_3);//读取LSR时中断会被清除
		lineStatus &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
		//2.2 处理错误
		if(lineStatus)
		{
		}
					
	}
	/****接收数据或者超时中断****/
	else if (((intsrc&0x0f) == UART_IIR_INTID_RDA) || ((intsrc&0x0f) == UART_IIR_INTID_CTI))
	{
		if(RS485Uart3.readyToRcv==1)
		{

			RS485Uart3.Rcv[RS485Uart3.rcvIndex]=UART_ReceiveByte(LPC_UART3);
			RS485Uart3.rcvIndex+=1;
			if(RS485Uart3.Rcv[RS485Uart3.rcvIndex-1]!='E')													/*	未收到包尾		*/
			{
				OSIntExit();											/*****通知os退出中断******/
				return;
			}

			else if(RS485Uart3.Rcv[RS485Uart3.rcvIndex-1]=='E'&&RS485Uart3.rcvIndex>=5)			/*	收到包尾且长度大于5	*/
			{
				if(RS485Uart3.Rcv[RS485Uart3.rcvIndex-2]==(('S'+'E')&0xff))										/*	检测校验位						*/
				{
					uart3RcvOK=1;				/*设标记位，标志接收成功*/
					RS485Uart3.readyToRcv=0;
					RS485Uart3.rcvIndex=0;
					OSIntExit();											/*****通知os退出中断******/
					return;
				}
				else
				{
					uart3RcvOK=0;				/*设标记位，标志接收成功*/
					RS485Uart3.readyToRcv=0;
					RS485Uart3.rcvIndex=0;
					OSIntExit();											/*****通知os退出中断******/
					return;
				}
					
			}
			else 
			{
				OSIntExit();											/*****通知os退出中断******/
				return;
			}
		}
		rcvTemp=UART_ReceiveByte(LPC_UART3);
		if(rcvTemp=='S'&&RS485Uart3.readyToRcv==0)//若接收未开始且收到开始标记，则标记接收开始
		{
			RS485Uart3.readyToRcv=1;
		}
	}
	/****	发送区为空中断	****/
	else if((intsrc&0x0f) == UART_IIR_INTID_THRE)
	{	
		RS485Uart3.sendIndex+=1;
		UART_SendByte(LPC_UART3,RS485Uart3.Send[RS485Uart3.sendIndex]);
		if((RS485Uart3.sendIndex>=RS485Uart3.sendLen-1)&&RS485Uart3.Send[RS485Uart3.sendIndex]=='E')
		{
			UART_IntConfig(UART_3, UART_INTCFG_THRE, DISABLE);						/* 		Enable the tr exit			*/
			RS485Uart3.sendOK=1;
			RS485Uart3.sendIndex=0;
		}
	}
	OSIntExit();											/*****通知os退出中断******/
}

/*********************************************************************************************
* name			:		UART4_IRQHandler
* func			:		Uart4中断处理函数
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/
void UART4_IRQHandler()
{
	uint8_t lineStatus;
	uint8_t rcvTemp;
	uint32_t intsrc;
	
	OSIntEnter();																	/****通知os进入中断*/
	
	intsrc=UART_GetIntId(UART_4);
	if((intsrc&0x0f)==UART_IIR_INTID_RLS)						/****接收线中断****/
	{
		//2.1 检查线状态
		lineStatus = UART_GetLineStatus(UART_4);//读取LSR时中断会被清除
		lineStatus &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
		//2.2 处理错误
		if(lineStatus)
		{
		}
					
	}
	/****接收数据或者超时中断****/
	else if (((intsrc&0x0f) == UART_IIR_INTID_RDA) || ((intsrc&0x0f) == UART_IIR_INTID_CTI))
	{
		if(RS485Uart4.readyToRcv==1)
		{

			RS485Uart4.Rcv[RS485Uart4.rcvIndex]=UART_ReceiveByte((LPC_UART_TypeDef *)LPC_UART4);
			RS485Uart4.rcvIndex+=1;
			if(RS485Uart4.Rcv[RS485Uart4.rcvIndex-1]!='E')													/*	未收到包尾		*/
			{
				OSIntExit();											/*****通知os退出中断******/
				return;
			}

			else if(RS485Uart4.Rcv[RS485Uart4.rcvIndex-1]=='E'&&RS485Uart4.rcvIndex>=5)			/*	收到包尾且长度大于5	*/
			{
				if(RS485Uart4.Rcv[RS485Uart4.rcvIndex-2]==(('S'+'E')&0xff))										/*	检测校验位						*/
				{
					uart4RcvOK=1;				/*设标记位，标志接收成功*/
					RS485Uart4.readyToRcv=0;
					RS485Uart4.rcvIndex=0;
					OSIntExit();											/*****通知os退出中断******/
					return;
				}
				else
				{
					uart4RcvOK=0;				/*设标记位，标志接收成功*/
					RS485Uart4.readyToRcv=0;
					RS485Uart4.rcvIndex=0;
					OSIntExit();											/*****通知os退出中断******/
					return;
				}
					
			}
			else 
			{
				OSIntExit();											/*****通知os退出中断******/
				return;
			}
		}
		rcvTemp=UART_ReceiveByte((LPC_UART_TypeDef *)LPC_UART4);
		if(rcvTemp=='S'&&RS485Uart4.readyToRcv==0)//若接收未开始且收到开始标记，则标记接收开始
		{
			RS485Uart4.readyToRcv=1;
		}
	}
	/****	发送区为空中断	****/
	else if((intsrc&0x0f) == UART_IIR_INTID_THRE)
	{	
		RS485Uart4.sendIndex+=1;
		UART_SendByte((LPC_UART_TypeDef *)LPC_UART4,RS485Uart4.Send[RS485Uart4.sendIndex]);
		if((RS485Uart4.sendIndex>=RS485Uart4.sendLen-1)&&RS485Uart4.Send[RS485Uart4.sendIndex]=='E')
		{
			UART_IntConfig(UART_4, UART_INTCFG_THRE, DISABLE);						/* 		Enable the tr exit			*/
			RS485Uart4.sendOK=1;
			RS485Uart4.sendIndex=0;
		}
	}
	OSIntExit();											/*****通知os退出中断******/
}

/*********************************************************************************************
* name			:		Uart1_Send_Struct
* func			:		通过Uart1发送结构体
* para			:		SendStruct
* ret				:		none
* comment		:	
*********************************************************************************************/
void Uart1_Send_Struct(SendStruct* sstruct)
{
	if(uart1SendOK==0)
	{
		return;
	}
	dataToSend[0]=0x5B;
	dataToSend[1]=((sstruct->clearFreCount&0xfE00)>>8)|0x0;
	dataToSend[2]=((sstruct->clearFreCount&0x01FC)>>1)|0x0;
	dataToSend[3]=((sstruct->clearFreCount&0x3)<<6)|((sstruct->SEL&0xF800)>>10)|0x0;
	dataToSend[4]=((sstruct->SEL&0x07F0)>>3)|0x0;
	dataToSend[5]=((sstruct->SEL&0x0F)<<4)|((sstruct->outPut&0xe0)>>4)|0x0;
	dataToSend[6]=((sstruct->outPut&0x1f)<<3)|0x0;
	dataToSend[7]=0xB5;
	
	/*****Send the buffer*****/
#ifdef UART1SEND_INT
	UART_SendByte((LPC_UART_TypeDef *)LPC_UART1,dataToSend[0]);
	UART_IntConfig(UART_1, UART_INTCFG_THRE, ENABLE);						/* 		Enable the tr exit			*/
	uart1SendOK=0;
#else
	UART_Send((LPC_UART_TypeDef *)LPC_UART1,dataToSend,0x08, BLOCKING);
#endif
	
}

/*********************************************************************************************
* name			:		Uart1_Get_Struct
* func			:		获取Uart1数据
* para			:		none
* ret				:		RcvStruct
* comment		:	
*********************************************************************************************/
RcvStruct Uart1_Get_Struct()
{
	uint16_t i,num;
	RcvStruct rstruct;
	num=0;
	if(uart1RcvOK==0)
	{
	}
	else
	{
		for(i=0;i<16;i++)
		{
			rstruct.Fre[i]=(dataToRcv[num+1]<<8)|dataToRcv[num];
			num+=2;
		}
		for(i=0;i<16;i++)
		{
			rstruct.AD[i]=(dataToRcv[num+1]<<8)|dataToRcv[num];
			num+=2;
		}
		num+=2;
		for(i=0;i<16;i++)
		{
			rstruct.count[i]=(dataToRcv[num+2]<<24)|(dataToRcv[num+3]<<16)|(dataToRcv[num]<<8)|dataToRcv[num+1];
			num+=4;
		}
		rstruct.sel=dataToRcv[131]<<8|dataToRcv[130];
		uart1RcvOK=0;	
	}
	return rstruct;
}

/*********************************************************************************************
* name			:		RS485Send_Struct
* func			:		通过RS485发送结构体
* para			:		RS485_UART_NUM UART_NUM,UserRS485Send rs485Send_S
* ret				:		none
* comment		:	
*********************************************************************************************/
void RS485Send_Struct(RS485_UART_NUM UART_NUM,UserRS485Send* rs485Send_S)
{
	uint8_t i;
	switch(UART_NUM)
	{
		case RS485_UART0:
			if(RS485Uart0.sendOK==0)
			{
				return;
			}
			RS485Uart0.Send[0]='S';
			RS485Uart0.Send[1]=rs485Send_S->address;
			RS485Uart0.Send[2]=rs485Send_S->ctrl;
			RS485Uart0.Send[3]=rs485Send_S->len;
			for(i=1;i<=rs485Send_S->len;i++)
			{
				RS485Uart0.Send[3+i]=rs485Send_S->data[i-1];
			}
			RS485Uart0.Send[3+rs485Send_S->len+1]=('S'+'E')&0xff;
			RS485Uart0.Send[3+rs485Send_S->len+2]='E';
			RS485Uart0.sendLen=rs485Send_S->len+6;
			
			UART_SendByte(LPC_UART0,RS485Uart0.Send[0]);
			UART_IntConfig(UART_0, UART_INTCFG_THRE, ENABLE);						/* 		Enable the tr exit			*/
			RS485Uart0.sendOK=0;
			break;
		case RS485_UART2:
			if(RS485Uart2.sendOK==0)
			{
				return;
			}
			RS485Uart2.Send[0]='S';
			RS485Uart2.Send[1]=rs485Send_S->address;
			RS485Uart2.Send[2]=rs485Send_S->ctrl;
			RS485Uart2.Send[3]=rs485Send_S->len;
			for(i=1;i<=rs485Send_S->len;i++)
			{
				RS485Uart2.Send[3+i]=rs485Send_S->data[i-1];
			}
			RS485Uart2.Send[3+rs485Send_S->len+1]=('S'+'E')&0xff;
			RS485Uart2.Send[3+rs485Send_S->len+2]='E';
			RS485Uart2.sendLen=rs485Send_S->len+6;
			
			UART_SendByte(LPC_UART2,RS485Uart2.Send[0]);
			UART_IntConfig(UART_2, UART_INTCFG_THRE, ENABLE);						/* 		Enable the tr exit			*/
			RS485Uart2.sendOK=0;
			break;
		case RS485_UART3:
			if(RS485Uart3.sendOK==0)
			{
				return;
			}
			RS485Uart3.Send[0]='S';
			RS485Uart3.Send[1]=rs485Send_S->address;
			RS485Uart3.Send[2]=rs485Send_S->ctrl;
			RS485Uart3.Send[3]=rs485Send_S->len;
			for(i=1;i<=rs485Send_S->len;i++)
			{
				RS485Uart3.Send[3+i]=rs485Send_S->data[i-1];
			}
			RS485Uart3.Send[3+rs485Send_S->len+1]=('S'+'E')&0xff;
			RS485Uart3.Send[3+rs485Send_S->len+2]='E';
			RS485Uart3.sendLen=rs485Send_S->len+6;
			
			UART_SendByte(LPC_UART3,RS485Uart3.Send[0]);
			UART_IntConfig(UART_3, UART_INTCFG_THRE, ENABLE);						/* 		Enable the tr exit			*/
			RS485Uart3.sendOK=0;
			break;
		case RS485_UART4:
			if(RS485Uart4.sendOK==0)
			{
				return;
			}
			RS485Uart4.Send[0]='S';
			RS485Uart4.Send[1]=rs485Send_S->address;
			RS485Uart4.Send[2]=rs485Send_S->ctrl;
			RS485Uart4.Send[3]=rs485Send_S->len;
			for(i=1;i<=rs485Send_S->len;i++)
			{
				RS485Uart4.Send[3+i]=rs485Send_S->data[i-1];
			}
			RS485Uart4.Send[3+rs485Send_S->len+1]=('S'+'E')&0xff;
			RS485Uart4.Send[3+rs485Send_S->len+2]='E';
			RS485Uart4.sendLen=rs485Send_S->len+6;
			
			UART_SendByte((LPC_UART_TypeDef *)LPC_UART4,RS485Uart4.Send[0]);
			UART_IntConfig(UART_4, UART_INTCFG_THRE, ENABLE);						/* 		Enable the tr exit			*/
			RS485Uart4.sendOK=0;
			break;
		default:
			break;
	}
	
}

/*********************************************************************************************
* name			:		RS485Rcv_Struct
* func			:		通过RS485接收结构体
* para			:		RS485_UART_NUM UART_NUM
* ret				:		none
* comment		:	
*********************************************************************************************/
UserRS485Rcv RS485_Get_Struct(RS485_UART_NUM UART_NUM)
{
	uint8_t i;
	UserRS485Rcv rs485Rcv_S;
	switch(UART_NUM)
	{
		case RS485_UART0:
			if(uart0RcvOK==0)
			{
				break;
			}
			rs485Rcv_S.ctrl=RS485Uart0.Rcv[0];
			rs485Rcv_S.len=RS485Uart0.Rcv[1];
			for(i=0;i<rs485Rcv_S.len;i++)
			{
				rs485Rcv_S.data[i]=RS485Uart0.Rcv[1+i+1];
			}
			uart0RcvOK=0;
			return rs485Rcv_S;
		case RS485_UART2:
			if(uart2RcvOK==0)
			{
				break;
			}
			rs485Rcv_S.ctrl=RS485Uart2.Rcv[0];
			rs485Rcv_S.len=RS485Uart2.Rcv[1];
			for(i=0;i<rs485Rcv_S.len;i++)
			{
				rs485Rcv_S.data[i]=RS485Uart2.Rcv[1+i+1];
			}
			uart2RcvOK=0;
			return rs485Rcv_S;
		case RS485_UART3:
			if(uart3RcvOK==0)
			{
				break;
			}
			rs485Rcv_S.ctrl=RS485Uart3.Rcv[0];
			rs485Rcv_S.len=RS485Uart3.Rcv[1];
			for(i=0;i<rs485Rcv_S.len;i++)
			{
				rs485Rcv_S.data[i]=RS485Uart3.Rcv[1+i+1];
			}
			uart3RcvOK=0;
			return rs485Rcv_S;
		case RS485_UART4:
			if(uart4RcvOK==0)
			{
				break;
			}
			rs485Rcv_S.ctrl=RS485Uart4.Rcv[0];
			rs485Rcv_S.len=RS485Uart4.Rcv[1];
			for(i=0;i<rs485Rcv_S.len;i++)
			{
				rs485Rcv_S.data[i]=RS485Uart4.Rcv[1+i+1];
			}
			uart4RcvOK=0;
			return rs485Rcv_S;
		default:
			break;
	}
	return rs485Rcv_S;
}

