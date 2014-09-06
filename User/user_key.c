/***********************************************************
 *
 *
 *
 *
 *
************************************************************/
						 
 
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_pinsel.h"
#include "GUI.h"
#include "LPC177x_8x.h"
#include "includes.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_exti.h"
#include "lpc177x_8x_timer.h"
#include "GUI.h"
#include "os_cpu.h"
#include "main.h"
	
//按键红外信号16位
#define KEY_ADD		(0x0000E280)
#define KEY_SUB		(0x0000E240)
#define KEY_FUNC 	(0x0000E2C0)
#define	KEY_EXEC	(0x0000E200)
#define KEY_OK 		(0x807D40BF)
#define KEY_UP		(0X807D00FF)
#define KEY_DOWN	(0X807D807F)
#define KEY_LEFT	(0X807D40DF)
#define KEY_RIGHT	(0X807DA05F)

static void task_Key            (void);

/*-------------- Key Value --------------*/
int8_t 		KeyData=0xff;                   //初始值为0xff，按键值
int8_t 		KeyBuffer=0xff;                 //按键缓冲区，当判断有按键按下的时候，此缓冲区记录当前的按键值
#define 	KEYQSIZE   7            		//按键消息队列大小

/*------------- Timer init ------------- */
TIM_MATCHCFG_Type TIM_MatchConfigStruct;

unsigned int times, validtime;
uint32_t keyvalue, oldkeyvalue, KeyValueSaved, OldVal;
unsigned char keycnt,cntall, cntskip;
unsigned char badd;
uint8_t keyKeep;//是否为连续按键
uint8_t keyTmp;	//连续按键次数

extern OS_EVENT    *KeyQueueHead;             		//按键消息队列指针
extern void        *KeyMsg[KEYQSIZE]; 		   		//按键消息指针数组 

static OS_STK  TaskKeyStk[TASK_KEY_STK_SIZE];

//遥控器16位编码
//1110 0010 1000 0000	0xE280	加
//1110 0010 0000 0000	0xE200	执行
//1110 0010 0100 0000	0xE240	减
//1110 0010 1100 0000 	0xE2C0	功能
//中断时间
//1为2.4-1.8	ms，时间较长
//0为1.42-0.76	ms，时间较短
//每次按键，每隔22ms捕获一次。一次按键，要产生3次以上。后面的要忽略掉。	按住按键中，每46ms一次有效按键。
//按键添加到队列中

/***********************************************************************
函数名称: EINT0_IRQHandler
功能    ：EINT0中断，接收红外遥控器按键
输入参数：
输出    ：
修改日期：2012.4.23
************************************************************************/
void EINT0_IRQHandler(void)
{

	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();
	OSIntNesting++;
	OS_EXIT_CRITICAL();

	if (times>300)	//开始按键，前面的无效
	{
		keyvalue=0;		
	}
	else if (times>=6 && times<=15)	 //0
	{
		keyvalue=keyvalue<<1|0;
	}
	else if (times>18 && times<=26)	 //1
	{
		keyvalue=keyvalue<<1|1;
	}
	else if (times>105 && times<=115)	 //连续
	{
		if(keyKeep==0)
		{
			keyKeep=1;			
		}
		else if(keyKeep==1)
		{
			keyTmp+=1;
			if(keyTmp>=2)
			{
				keyvalue=oldkeyvalue;
				keyTmp=0;
			}	
			else
			{
				keyvalue=0;
			}
		}
		validtime=0;	
	}
	else if (times>=125 && times<=145)	 //新按键开始
	{
		keyKeep=0;
		keyTmp=0;
		validtime=0;
	}
	else 
	{
		keyvalue=0;		
	}
	times=0;

	switch (keyvalue)
	{
		case	KEY_ADD:	
		{			
			if (keyvalue==oldkeyvalue)//消抖
			{
				keyvalue=0;
				break;					//此处不能return，因为需要执行到最后的OSIntExit()，如果使用return，不执行OSIntExit就返回了，造成QSQPend返回OSIntNesting>0的错误。
			}
			KeyValueSaved=keyvalue;		//保存以便存入队列
			OSQPost(KeyQueueHead,&KeyValueSaved); 
			oldkeyvalue=keyvalue;
			validtime=0;	//400ms重新计时
			
			keyvalue=0;
			break;
		}
		case	KEY_SUB:
		{
			if (keyvalue==oldkeyvalue)
			{
				keyvalue=0;
				break;
			}
			KeyValueSaved=keyvalue;		//保存以便存入队列
			OSQPost(KeyQueueHead,&KeyValueSaved);
			oldkeyvalue=keyvalue;
			validtime=0;	//400ms重新计时
			keyvalue=0;
			break;
		}
		case	KEY_FUNC:
		{
			if (keyvalue==oldkeyvalue)
			{
				keyvalue=0;
				break;
			}
			KeyValueSaved=keyvalue;		//保存以便存入队列
			OSQPost(KeyQueueHead,&KeyValueSaved); 
			oldkeyvalue=keyvalue;
			validtime=0;	//400ms重新计时
			keyvalue=0;
			break;
		}
		case	KEY_EXEC:
		{
			if (keyvalue==oldkeyvalue)
			{
				keyvalue=0;
				break;
			}
			KeyValueSaved=keyvalue;		//保存以便存入队列
			OSQPost(KeyQueueHead,&KeyValueSaved);
			oldkeyvalue=keyvalue;
			validtime=0;	//400ms重新计时
			keyvalue=0;
			break;
		}
		case	KEY_OK:	
		{			

			KeyValueSaved=keyvalue;		//保存以便存入队列
			OSQPost(KeyQueueHead,&KeyValueSaved); 
			oldkeyvalue=keyvalue;
			validtime=0;	//400ms重新计时
			
			keyvalue=0;
			break;
		}
		case	KEY_UP:	
		{			

			KeyValueSaved=keyvalue;		//保存以便存入队列
			OSQPost(KeyQueueHead,&KeyValueSaved); 
			oldkeyvalue=keyvalue;
			validtime=0;	//400ms重新计时
			
			keyvalue=0;
			break;
		}
		case	KEY_DOWN:	
		{			

			KeyValueSaved=keyvalue;		//保存以便存入队列
			OSQPost(KeyQueueHead,&KeyValueSaved); 
			oldkeyvalue=keyvalue;
			validtime=0;	//400ms重新计时
			
			keyvalue=0;
			break;
		}
		case	KEY_LEFT:	
		{			

			KeyValueSaved=keyvalue;		//保存以便存入队列
			OSQPost(KeyQueueHead,&KeyValueSaved); 
			oldkeyvalue=keyvalue;
			validtime=0;	//400ms重新计时
			
			keyvalue=0;
			break;
		}
		case	KEY_RIGHT:	
		{			
			KeyValueSaved=keyvalue;		//保存以便存入队列
			OSQPost(KeyQueueHead,&KeyValueSaved); 
			oldkeyvalue=keyvalue;
			validtime=0;	//400ms重新计时
			
			keyvalue=0;
			break;
		}
		default: break;
	}
	
	//printf( "old = %d\n", oldkeyvalue );
	OldVal = oldkeyvalue;
	EXTI_ClearEXTIFlag(EXTI_EINT0);
	OSIntExit();
}

/***********************************************************************
函数名称: TIMER0_IRQHandler
功能    ：定时器0中断，按键计时
输入参数：
输出    ：
修改日期：2012.4.23
************************************************************************/
void TIMER2_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();
	OSIntNesting++;
	OS_EXIT_CRITICAL();
	//printf("zhongduan: %d %d\n", LPC_TIM2->IR, TIM_MR0_INT );
	if (TIM_GetIntStatus(LPC_TIM2, TIM_MR0_INT)== SET)
	{
	   if (times<=1000)	//小于300ms	  
	   {
	   		times++;
	   }
	   else
	   {
			keyvalue=0;	
	   }
	   validtime++;
	   if (validtime>=2100)	//若一直按，则每210ms分出一次有效按键，示波器实测为232ms。可以有效区分连续按键。但是不能区分按住不放还是连续按键。
	   {
	   		keyvalue=0;
	   		validtime=0;
				oldkeyvalue=0;				
	   }
	}	
	TIM_ClearIntPending(LPC_TIM2, TIM_MR0_INT);	//T0中断复位 T0IR
	OSIntExit();
}


/***********************************************************************
函数名称: KeyInit
功能    ：按键初始化操作，初始化EINT0_IRQn和TIMER1_IRQn
输入参数：
输出    ：
修改日期：2012.4.23
************************************************************************/
void KeyInit(void)
{
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	//外部中断
	EXTI_InitTypeDef EXTICfg; 							   
	EXTI_Init();	 
	EXTICfg.EXTI_Line = EXTI_EINT0;
	/* edge sensitive */		 //下降沿中断
	EXTICfg.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
	EXTICfg.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
	EXTI_Config(&EXTICfg);
	NVIC_EnableIRQ(EINT0_IRQn);

  //临时变量初始化
	keyTmp=0;
	keyKeep=0;
	
	//定时器0
	// Initialize timer 0, prescale count time of 100uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;  //微秒
	TIM_ConfigStruct.PrescaleValue	= 10;				   //100微秒 10微秒
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
	// Set Match value, count value of 10000 (10000 * 100uS = 1000000us = 1s --> 1 Hz)
	TIM_MatchConfigStruct.MatchValue   = 9;	//定时100微秒，值为9时，为100微秒。为10时，大于100微秒，为110微秒

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM2, TIM_TIMER_MODE, &TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM2, &TIM_MatchConfigStruct);

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER2_IRQn, ((0x01<<3)|0x01));

	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER2_IRQn);


	//GPIO_SetDir(1, 1<<3, 1);	//P1.3输出，测试timer0中断时间

	// To start timer
	TIM_Cmd(LPC_TIM2, ENABLE);
}

void  App_KeyTaskCreate (void)
{
    INT8U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) task_Key,				
                          (void          * ) 0,							
                          (OS_STK        * )&TaskKeyStk[TASK_KEY_STK_SIZE - 1],		
                          (INT8U           ) TASK_KEY_PRIO  );							

}

static void task_Key (void) 
{ 	
	uint32_t *KeyValue;
	uint8_t Error;		
	//KeyInit();
	while(1)
	{
		KeyValue=(uint32_t *)OSQPend(KeyQueueHead, 0, &Error);
		if(Error == OS_ERR_NONE)
		{
			switch(*KeyValue)
			{
				case  KEY_ADD:					
					GUI_StoreKeyMsg(GUI_KEY_UP, 1);
					break;
				case  KEY_SUB:					
					GUI_StoreKeyMsg(GUI_KEY_DOWN, 1);
					break;
				case  KEY_FUNC:					
					GUI_StoreKeyMsg(GUI_KEY_TAB, 1);
					break;
				case  KEY_EXEC:
					GUI_StoreKeyMsg(GUI_KEY_ENTER, 1);
					break;
				case  KEY_OK:
					GUI_StoreKeyMsg(GUI_KEY_ENTER, 1);
					break;
				case  KEY_UP:
					GUI_StoreKeyMsg(GUI_KEY_UP, 1);
					break;
				case  KEY_DOWN:
					GUI_StoreKeyMsg(GUI_KEY_DOWN, 1);
					break;
				case  KEY_LEFT:
					GUI_StoreKeyMsg(GUI_KEY_LEFT, 1);
					break;
				case  KEY_RIGHT:
					GUI_StoreKeyMsg(GUI_KEY_TAB, 1);
					break;
				default:				
					break;
			}
		}
		OSTimeDly(10);
		
		//printf("%d %d\n", *KeyValue, OldVal );
		
		switch( OldVal )
		{
			case  KEY_ADD:					
				GUI_StoreKeyMsg(GUI_KEY_UP, 0);
				OldVal = 0;
				break;
			case  KEY_SUB:					
				GUI_StoreKeyMsg(GUI_KEY_DOWN, 0);
				OldVal = 0;
				break;
			case  KEY_FUNC:					
				GUI_StoreKeyMsg(GUI_KEY_TAB, 0);
				OldVal = 0;
				break;
			case  KEY_EXEC:
				GUI_StoreKeyMsg(GUI_KEY_ENTER, 0);
				OldVal = 0;
				break;
			case  KEY_OK:
					GUI_StoreKeyMsg(GUI_KEY_ENTER, 0);
					break;
				case  KEY_UP:
					GUI_StoreKeyMsg(GUI_KEY_UP, 0);
					break;
				case  KEY_DOWN:
					GUI_StoreKeyMsg(GUI_KEY_DOWN, 0);
					break;
				case  KEY_LEFT:
					GUI_StoreKeyMsg(GUI_KEY_LEFT, 0);
					break;
				case  KEY_RIGHT:
					GUI_StoreKeyMsg(GUI_KEY_TAB, 0);
					break;			
			default:
				break;
		}
					  
	}

}

