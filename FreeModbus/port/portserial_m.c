/*
 * FreeModbus Libary: STM32 Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

#include "FMBport.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_clkpwr.h"
#include "os_cpu.h"
#include "ucos_ii.h"
#include "DataStruct.h"

#define RS485FIFOTRGLEV		UART_FIFO_TRGLEV0			/*	RS485FIFO�ж��ֽ�����		*/


#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR(RS485_NUM RS485_X);
static void prvvUARTRxISR(RS485_NUM RS485_X);
/* ----------------------- Start implementation -----------------------------*/

void vMBMasterPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)//LPC1788 �� RS485�շ�ģʽ�Զ�ת��������������Ҫ
{
//	if (xRxEnable)
//	{
//		/* 485ͨ��ʱ���ȴ�������λ�Ĵ����е����ݷ�����ɺ���ȥʹ��485�Ľ��ա�ʧ��485�ķ���*/
//		while (!USART_GetFlagStatus(USART2,USART_FLAG_TC));
//		MASTER_RS485_RECEIVE_MODE;
//		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	}
//	else
//	{
//		MASTER_RS485_SEND_MODE;
//		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//	}
//	if (xTxEnable)
//	{
//		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
//	}
//	else
//	{
//		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
//	}
}

void vMBMasterPortClose(void)
{
	
	/******************DISABLE the uart NVIC	*******************/
	NVIC_DisableIRQ(UART0_IRQn);
	NVIC_DisableIRQ(UART2_IRQn);
	NVIC_DisableIRQ(UART3_IRQn);
	NVIC_DisableIRQ(UART4_IRQn);
	/*****************DISABLE the uartrx exit********************/

	UART_IntConfig(UART_0, UART_INTCFG_RBR, DISABLE);
	UART_IntConfig(UART_2, UART_INTCFG_RBR, DISABLE);
	UART_IntConfig(UART_3, UART_INTCFG_RBR, DISABLE);
	UART_IntConfig(UART_4, UART_INTCFG_RBR, DISABLE);
	
	/*******************Enable the uart line exit***************/

	UART_IntConfig(UART_0, UART_INTCFG_RLS, DISABLE);
	UART_IntConfig(UART_2, UART_INTCFG_RLS, DISABLE);
	UART_IntConfig(UART_3, UART_INTCFG_RLS, DISABLE);
	UART_IntConfig(UART_4, UART_INTCFG_RLS, DISABLE);
	
	UART_RS485ReceiverCmd(UART_0,DISABLE);
	UART_RS485ReceiverCmd(UART_2,DISABLE);
	UART_RS485ReceiverCmd(UART_3,DISABLE);
	UART_RS485ReceiverCmd(UART_4,DISABLE);
	
	/*******************Uart DISABLE****************************/

	UART_TxCmd(LPC_UART0,DISABLE);
	UART_TxCmd(LPC_UART2,DISABLE);
	UART_TxCmd(LPC_UART3,DISABLE);
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART4,DISABLE);
//	USART_ITConfig(USART2, USART_IT_TXE | USART_IT_RXNE, DISABLE);
//	USART_Cmd(USART2, DISABLE);
}

//Ĭ��һ������ ����2 �����ʿ�����  ��ż���������
BOOL xMBMasterPortSerialInit(RS485_NUM RS485_X, ULONG ulBaudRate, UCHAR ucDataBits,
		eMBParity eParity)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	//======================ʱ�ӳ�ʼ��=======================================
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//	//======================IO��ʼ��=======================================	
//	//USART2_TX
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	//USART2_RX
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	//����485���ͺͽ���ģʽ
////    TODO   ��ʱ��дB13 ��֮����������ʱ���޸�
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	//======================���ڳ�ʼ��=======================================
//	USART_InitStructure.USART_BaudRate = ulBaudRate;
//	//����У��ģʽ
//	switch (eParity)
//	{
//	case MB_PAR_NONE: //��У��
//		USART_InitStructure.USART_Parity = USART_Parity_No;
//		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//		break;
//	case MB_PAR_ODD: //��У��
//		USART_InitStructure.USART_Parity = USART_Parity_Odd;
//		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
//		break;
//	case MB_PAR_EVEN: //żУ��
//		USART_InitStructure.USART_Parity = USART_Parity_Even;
//		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
//		break;
//	default:
//		return FALSE;
//	}

//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_HardwareFlowControl =
//			USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	if (ucPORT != 2)
//		return FALSE;

//	ENTER_CRITICAL_SECTION(); //��ȫ���ж�

//	USART_Init(USART2, &USART_InitStructure);
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	USART_Cmd(USART2, ENABLE);

//	//=====================�жϳ�ʼ��======================================
//	//����NVIC���ȼ�����ΪGroup2��0-3��ռʽ���ȼ���0-3����Ӧʽ���ȼ�
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	EXIT_CRITICAL_SECTION(); //��ȫ���ж�
	/*********************struct define*************************/
	UART_CFG_Type UartConfig;
	UART_FIFO_CFG_Type UartFIFOConfig;
	UART1_RS485_CTRLCFG_Type rs485cfg;
	
	/********************		���ù��ò���			******************/
	
	UartConfig.Stopbits=UART_STOPBIT_1;
	UartConfig.Databits=UART_DATABIT_8;
	UartConfig.Baud_rate=ulBaudRate;
	//����У��ģʽ
	switch (eParity)
	{
	case MB_PAR_NONE: //��У��
//		USART_InitStructure.USART_Parity = USART_Parity_No;
//		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	UartConfig.Parity=UART_PARITY_NONE;
		break;
	case MB_PAR_ODD: //��У��
	UartConfig.Parity=UART_PARITY_ODD;
		break;
	case MB_PAR_EVEN: //żУ��
		UartConfig.Parity=UART_PARITY_EVEN;
		break;
	default:
		return FALSE;
	}
	
	UartFIFOConfig.FIFO_DMAMode=DISABLE;
	UartFIFOConfig.FIFO_ResetRxBuf=ENABLE;
	UartFIFOConfig.FIFO_ResetTxBuf=ENABLE;
	UartFIFOConfig.FIFO_Level=RS485FIFOTRGLEV; 	/* 8 character  */
	
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
	
	rs485cfg.Rx_State = ENABLE;
	rs485cfg.AutoAddrDetect_State = ENABLE;
	
	switch(RS485_X)
	{
		case RS485_1:
		{
			
			PINSEL_ConfigPin(0,2,1);/*TX*/										/***uart 0***/
			PINSEL_ConfigPin(0,3,1);/*RX*/
			PINSEL_ConfigPin(5,4,1);/*OE*/
			
			UART_Init(LPC_UART0,&UartConfig);									/***uart 0***/
			UART_FIFOConfig(UART_0,&UartFIFOConfig);
			UART_IntConfig(UART_0, UART_INTCFG_RBR, ENABLE);
			UART_IntConfig(UART_0, UART_INTCFG_RLS, ENABLE);
			NVIC_SetPriority(UART0_IRQn, ((0x01<<3)|0x02));
			NVIC_EnableIRQ(UART0_IRQn);
			UART_RS485ReceiverCmd(UART_0,ENABLE);
			UART_TxCmd(LPC_UART0,ENABLE);
			break;
		}
		
		case RS485_2:
		{
			PINSEL_ConfigPin(4,22,2);/*TX*/										/***uart 2***/
			PINSEL_ConfigPin(4,23,2);/*RX*/
			PINSEL_ConfigPin(1,19,6);/*OE*/
			
			UART_Init(LPC_UART2,&UartConfig);									/***uart 2***/
			UART_FIFOConfig(UART_2,&UartFIFOConfig);
			UART_IntConfig(UART_2, UART_INTCFG_RBR, ENABLE);
			UART_IntConfig(UART_2, UART_INTCFG_RLS, ENABLE);
			NVIC_SetPriority(UART2_IRQn, ((0x01<<3)|0x02));
			NVIC_EnableIRQ(UART2_IRQn);
			UART_RS485ReceiverCmd(UART_2,ENABLE);
			UART_TxCmd(LPC_UART2,ENABLE);
			break;
		}
		
		case RS485_3:
		{
			PINSEL_ConfigPin(0,25,3);/*TX*/										/***uart 3***/
			PINSEL_ConfigPin(0,26,3);/*RX*/
			PINSEL_ConfigPin(1,30,5);/*OE*/
			
			UART_Init(LPC_UART3,&UartConfig);									/***uart 3***/
			UART_FIFOConfig(UART_3,&UartFIFOConfig);
			UART_IntConfig(UART_3, UART_INTCFG_RBR, ENABLE);
			UART_IntConfig(UART_3, UART_INTCFG_RLS, ENABLE);
			NVIC_SetPriority(UART3_IRQn, ((0x01<<3)|0x02));
			NVIC_EnableIRQ(UART3_IRQn);
			UART_RS485ReceiverCmd(UART_3,ENABLE);
			UART_TxCmd(LPC_UART3,ENABLE);
			break;
		}
		
		case RS485_4:
		{
			PINSEL_ConfigPin(0,22,3);/*TX*/										/***uart 4***/
			PINSEL_ConfigPin(5,3,4); /*RX*/
			PINSEL_ConfigPin(0,21,3);/*OE*/
			
			UART_Init((LPC_UART_TypeDef *)LPC_UART4,&UartConfig);									/***uart 4***/
			UART_FIFOConfig(UART_4,&UartFIFOConfig);
			UART_IntConfig(UART_4, UART_INTCFG_RBR, ENABLE);
			UART_IntConfig(UART_4, UART_INTCFG_RLS, ENABLE);
			NVIC_SetPriority(UART4_IRQn, ((0x01<<3)|0x02));
			NVIC_EnableIRQ(UART4_IRQn);
			UART_RS485ReceiverCmd(UART_4,ENABLE);
			UART_TxCmd((LPC_UART_TypeDef *)LPC_UART4,ENABLE);
			break;
		}
		default:
			return FALSE;
			
	}

//#if RECEIVER_ALWAYS_EN
//	rs485cfg.Rx_State = ENABLE;
//	rs485cfg.AutoAddrDetect_State = DISABLE;
//#else
//	rs485cfg.Rx_State = DISABLE;
//#if AUTO_SLVADDR_DETECT
//	rs485cfg.AutoAddrDetect_State = ENABLE;
//#else
//	rs485cfg.AutoAddrDetect_State = DISABLE;
//	rs485cfg.MatchAddrValue = UART3_ADDR;
//#endif
//#endif
//	UART_RS485Config(UART_3, &rs485cfg);				/***Uart_3***/
	return TRUE;
}

BOOL xMBMasterPortSerialPutByte(RS485_NUM RS485_X,CHAR ucByte)
{
	switch(RS485_X)
	{
		case RS485_1:
			UART_SendByte(LPC_UART0,ucByte);
			break;
		case RS485_2:
			UART_SendByte(LPC_UART2,ucByte);
			break;
		case RS485_3:
			UART_SendByte(LPC_UART3,ucByte);
			break;
		case RS485_4:
			UART_SendByte((LPC_UART_TypeDef *)LPC_UART4,ucByte);
			break;
		default:
			return FALSE;
	}	
	return TRUE;
}

BOOL xMBMasterPortSerialGetByte(RS485_NUM RS485_X,CHAR * pucByte)
{
	switch(RS485_X)
	{
		case RS485_1:
			*pucByte=UART_ReceiveByte(LPC_UART0);
			break;
		case RS485_2:
			*pucByte=UART_ReceiveByte(LPC_UART2);
			break;
		case RS485_3:
			*pucByte=UART_ReceiveByte(LPC_UART3);
			break;
		case RS485_4:
			*pucByte=UART_ReceiveByte((LPC_UART_TypeDef *)LPC_UART4);
			break;
		default:
			return FALSE;
	}	
	return TRUE;
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR(RS485_NUM RS485_X)
{
	pxMBMasterFrameCBTransmitterEmpty(RS485_X);
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR(RS485_NUM RS485_X)
{
	pxMBMasterFrameCBByteReceived(RS485_X);
}

/*******************************************************************************
 * Function Name  : USART0_IRQHandler
 * Description    : This function handles USART0 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USART0_IRQHandler(void)
{
//	rt_interrupt_enter();
//	//�������
//	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) == SET)
//	{
//		prvvUARTRxISR();
//	}
//	//�����ж�
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
//	{
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//		prvvUARTRxISR();
//	}
//	//�����ж�
//	if (USART_GetITStatus(USART2, USART_IT_TXE) == SET)
//	{
//		prvvUARTTxReadyISR();
//	}
//	rt_interrupt_leave();
	
	uint32_t intsrc;
	uint8_t lineStatus;
	
	OSIntEnter();																		/****֪ͨos�����ж�*/
	
	intsrc=UART_GetIntId(UART_0);
	if((intsrc&0x0f)==UART_IIR_INTID_RLS)						/****�������ж�****/
	{
		//2.1 �����״̬
		lineStatus = UART_GetLineStatus(UART_0);//��ȡLSRʱ�жϻᱻ���
		prvvUARTRxISR(RS485_1);
					
	}
	/****�������ݻ��߳�ʱ�ж�****/
	else if (((intsrc&0x0f) == UART_IIR_INTID_RDA) || ((intsrc&0x0f) == UART_IIR_INTID_CTI))
	{
		prvvUARTRxISR(RS485_1);
	}
	/******�����ж�*************/
	else if((intsrc&0x0f) == UART_IIR_INTID_THRE)
	{	
		prvvUARTTxReadyISR(RS485_1);
	}
	OSIntExit();											/*****֪ͨos�˳��ж�******/
	
}

/*******************************************************************************
 * Function Name  : USART2_IRQHandler
 * Description    : This function handles USART0 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USART2_IRQHandler(void)
{
	uint32_t intsrc;
	uint8_t lineStatus;
	
	OSIntEnter();																		/****֪ͨos�����ж�*/
	
	intsrc=UART_GetIntId(UART_2);
	if((intsrc&0x0f)==UART_IIR_INTID_RLS)						/****�������ж�****/
	{
		//2.1 �����״̬
		lineStatus = UART_GetLineStatus(UART_2);//��ȡLSRʱ�жϻᱻ���
		prvvUARTRxISR(RS485_2);
					
	}
	/****�������ݻ��߳�ʱ�ж�****/
	else if (((intsrc&0x0f) == UART_IIR_INTID_RDA) || ((intsrc&0x0f) == UART_IIR_INTID_CTI))
	{
		prvvUARTRxISR(RS485_2);
	}
	/******�����ж�*************/
	else if((intsrc&0x0f) == UART_IIR_INTID_THRE)
	{	
		prvvUARTTxReadyISR(RS485_2);
	}
	OSIntExit();											/*****֪ͨos�˳��ж�******/
}

/*******************************************************************************
 * Function Name  : USART3_IRQHandler
 * Description    : This function handles USART0 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USART3_IRQHandler(void)
{
	uint32_t intsrc;
	uint8_t lineStatus;
	
	OSIntEnter();																		/****֪ͨos�����ж�*/
	
	intsrc=UART_GetIntId(UART_3);
	if((intsrc&0x0f)==UART_IIR_INTID_RLS)						/****�������ж�****/
	{
		//2.1 �����״̬
		lineStatus = UART_GetLineStatus(UART_3);//��ȡLSRʱ�жϻᱻ���
		prvvUARTRxISR(RS485_3);
					
	}
	/****�������ݻ��߳�ʱ�ж�****/
	else if (((intsrc&0x0f) == UART_IIR_INTID_RDA) || ((intsrc&0x0f) == UART_IIR_INTID_CTI))
	{
		prvvUARTRxISR(RS485_3);
	}
	/******�����ж�*************/
	else if((intsrc&0x0f) == UART_IIR_INTID_THRE)
	{	
		prvvUARTTxReadyISR(RS485_3);
	}
	OSIntExit();											/*****֪ͨos�˳��ж�******/
}

/*******************************************************************************
 * Function Name  : USART4_IRQHandler
 * Description    : This function handles USART0 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USART4_IRQHandler(void)
{
	uint32_t intsrc;
	uint8_t lineStatus;
	
	OSIntEnter();																		/****֪ͨos�����ж�*/
	
	intsrc=UART_GetIntId(UART_4);
	if((intsrc&0x0f)==UART_IIR_INTID_RLS)						/****�������ж�****/
	{
		//2.1 �����״̬
		lineStatus = UART_GetLineStatus(UART_4);//��ȡLSRʱ�жϻᱻ���
		prvvUARTRxISR(RS485_4);
					
	}
	/****�������ݻ��߳�ʱ�ж�****/
	else if (((intsrc&0x0f) == UART_IIR_INTID_RDA) || ((intsrc&0x0f) == UART_IIR_INTID_CTI))
	{
		prvvUARTRxISR(RS485_4);
	}
	/******�����ж�*************/
	else if((intsrc&0x0f) == UART_IIR_INTID_THRE)
	{	
		prvvUARTTxReadyISR(RS485_4);
	}
	OSIntExit();											/*****֪ͨos�˳��ж�******/
}

#endif
