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
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "FMBport.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"
#include "lpc177x_8x_exti.h"
#include "lpc177x_8x_timer.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Variables ----------------------------------------*/
static USHORT usT35TimeOut50us;
static USHORT usPrescalerValue = 0;

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void);

/* ----------------------- Start implementation -----------------------------*/

//定时器中断初始化
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	//====================================时钟初始化===========================
//	//使能定时器2时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//	//====================================定时器初始化===========================
//	//定时器时间基配置说明
//	//HCLK为72MHz，APB1经过2分频为36MHz
//	//TIM2的时钟倍频后为72MHz（硬件自动倍频,达到最大）
//	//TIM2的分频系数为3599，时间基频率为72 / (1 + Prescaler) = 20KHz,基准为50us
//	//TIM最大计数值为usTim1Timerout50u	
//	usPrescalerValue = (uint16_t) (SystemCoreClock / 20000) - 1;
//	//保存T35定时器计数值
//	usT35TimeOut50us = usTimeOut50us; 

//	//预装载使能
//	TIM_ARRPreloadConfig(TIM2, ENABLE);
//	//====================================中断初始化===========================
//	//设置NVIC优先级分组为Group2：0-3抢占式优先级，0-3的响应式优先级
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	//清除溢出中断标志位
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	//定时器3溢出中断关闭
//	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
//	//定时器3禁能
//	TIM_Cmd(TIM2, DISABLE);

	//保存备份
	usPrescalerValue=50;
	usT35TimeOut50us=usTimeOut50us;

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER3_IRQn, ((0x01<<5)|0x01));

	/* Enable interrupt for timer 3 */
	NVIC_DisableIRQ(TIMER3_IRQn);

	// To start timer
	TIM_Cmd(LPC_TIM3, DISABLE);
	
	return TRUE;
}

/*********************************************************************************************
* name			:		vMBMasterPortTimersT35Enable
* func			:		设置定时器按照T3.5时间开始计数
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/
void vMBMasterPortTimersT35Enable()
{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

//	/* Set current timer mode,don't change it.*/
//	vMBMasterSetCurTimerMode(MB_TMODE_T35);

//	TIM_TimeBaseStructure.TIM_Prescaler = usPrescalerValue;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseStructure.TIM_Period = (uint16_t) usT35TimeOut50us;
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	TIM_SetCounter(TIM2, 0);
//	TIM_Cmd(TIM2, ENABLE);
	
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_MATCHCFG_Type TIM_MatchConfigStruct;
	//定时器0
	// Initialize timer 0, prescale count time of 50uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL; //微秒
	TIM_ConfigStruct.PrescaleValue	= usPrescalerValue;	//设定基准为50us
	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	//Toggle MR0.0 pin if MR0 matches it
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;
	// Set Match value, count value of 50us
	TIM_MatchConfigStruct.MatchValue   = usT35TimeOut50us;	//设定计数值
		

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM3, TIM_TIMER_MODE, &TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM3, &TIM_MatchConfigStruct);
	
	TIM_ClearIntPending(LPC_TIM3, TIM_MR0_INT);	//T3中断复位 T0IR
	
	/* Enable interrupt for timer 3 */
	NVIC_EnableIRQ(TIMER3_IRQn);

	// To start timer
	TIM_Cmd(LPC_TIM3, ENABLE);
}

/*********************************************************************************************
* name			:		vMBMasterPortTimersConvertDelayEnable
* func			:		设置定时器按照广播帧的转换延时时间开始计数(定时器每次延时MB_MASTER_DELAY_MS_CONVERT个毫秒)
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/
void vMBMasterPortTimersConvertDelayEnable()
{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_MATCHCFG_Type TIM_MatchConfigStruct;
	/* Set current timer mode,don't change it.*/
	vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);

//	TIM_TimeBaseStructure.TIM_Prescaler = usPrescalerValue;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseStructure.TIM_Period = (uint16_t)(MB_MASTER_DELAY_MS_CONVERT * 1000 / 50);
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	TIM_SetCounter(TIM2, 0);
//	TIM_Cmd(TIM2, ENABLE);

	// Initialize timer 0, prescale count time of 50uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL; //微秒
	TIM_ConfigStruct.PrescaleValue	= usPrescalerValue;	//设定基准为50us
	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	//Toggle MR0.0 pin if MR0 matches it
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;
	// Set Match value, count value of 50us
	TIM_MatchConfigStruct.MatchValue   = (uint16_t)(MB_MASTER_DELAY_MS_CONVERT * 1000 / 50);	//设定计数值多少个50us中断一次
		

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM3, TIM_TIMER_MODE, &TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM3, &TIM_MatchConfigStruct);
	
	TIM_ClearIntPending(LPC_TIM3, TIM_MR0_INT);	//T3中断复位 T0IR
	
	/* Enable interrupt for timer 3 */
	NVIC_EnableIRQ(TIMER3_IRQn);

	// To start timer
	TIM_Cmd(LPC_TIM3, ENABLE);
}

/*********************************************************************************************
* name			:		vMBMasterPortTimersRespondTimeoutEnable
* func			:		设置定时器按照响应超时时间开始计数
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/
void vMBMasterPortTimersRespondTimeoutEnable()
{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_MATCHCFG_Type TIM_MatchConfigStruct;
	/* Set current timer mode,don't change it.*/
	vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);

//	TIM_TimeBaseStructure.TIM_Prescaler = usPrescalerValue;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseStructure.TIM_Period = (uint16_t)(MB_MASTER_TIMEOUT_MS_RESPOND * 1000 / 50);
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	TIM_SetCounter(TIM2, 0);
//	TIM_Cmd(TIM2, ENABLE);
	
	// Initialize timer 0, prescale count time of 50uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL; //微秒
	TIM_ConfigStruct.PrescaleValue	= usPrescalerValue;	//设定基准为50us
	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	//Toggle MR0.0 pin if MR0 matches it
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;
	// Set Match value, count value of 50us
	TIM_MatchConfigStruct.MatchValue   = (uint16_t)(MB_TMODE_RESPOND_TIMEOUT * 1000 / 50);	//设定计数值多少个50us中断一次
		

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM3, TIM_TIMER_MODE, &TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM3, &TIM_MatchConfigStruct);
	
	TIM_ClearIntPending(LPC_TIM3, TIM_MR0_INT);	//T3中断复位 T0IR
	/* Enable interrupt for timer 3 */
	NVIC_EnableIRQ(TIMER3_IRQn);

	// To start timer
	TIM_Cmd(LPC_TIM3, ENABLE);
}

/*********************************************************************************************
* name			:		vMBMasterPortTimersDisable
* func			:		失能定时器，定时器将停止计数
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/
void vMBMasterPortTimersDisable()
{
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
//	TIM_SetCounter(TIM2, 0);
//	TIM_Cmd(TIM2, DISABLE);
	
	TIM_ClearIntPending(LPC_TIM3, TIM_MR0_INT);	//T3中断复位 T0IR
	/* Enable interrupt for timer 3 */
	NVIC_DisableIRQ(TIMER3_IRQn);

	// To start timer
	TIM_Cmd(LPC_TIM3, DISABLE);
}

/*********************************************************************************************
* name			:		prvvTIMERExpiredISR
* func			:		定时器中断服务程序接口，按照默认方式，直接引用pxMBMasterPortCBTimerExpired方法即可
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/
void prvvTIMERExpiredISR(void)
{
	(void) pxMBMasterPortCBTimerExpired();
}

/*********************************************************************************************
* name			:		TIM3_IRQHandler
* func			:		定时器中断服务程序
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/

void TIM3_IRQHandler(void)
{
//	rt_interrupt_enter();
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//	{
//		
//		TIM_ClearFlag(TIM2, TIM_FLAG_Update);	     //清中断标记
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	 //清除定时器TIM2溢出中断标志位
//		prvvTIMERExpiredISR();
//	}
//	rt_interrupt_leave();
	
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();
	OSIntNesting++;
	if (TIM_GetIntStatus(LPC_TIM2, TIM_MR0_INT)== SET)
	{
		TIM_ClearIntPending(LPC_TIM3, TIM_MR0_INT);	//T0中断复位 T0IR
		prvvTIMERExpiredISR();
	}
	OSIntExit();
	
}

#endif
