#include "stdlib.h"
#include "string.h"
#include "main.h"
#include "includes.h"
#include "os_cpu.h"

#include "task_UartRcv.h"
#include "com_uart.h"


extern OS_EVENT  *UartRcvTaskSem;
extern RcvStruct  uart1RcvInfo;
extern INT8U      uart1RcvOK;

SendStruct uart1SendInfo;
extern OS_EVENT  *UartSendTaskSem;

INT32U Uart1LostCnt = 0;

void UartRcv_Task (void) 
{
		INT8U err;
	
		uart1SendInfo.clearFreCount = 0;
		uart1SendInfo.outPut = 0x00;
		uart1SendInfo.SEL = 0;
	
		while(1)
		{
				
				printf("In function Uart: ret = %u\n", uart1SendInfo.outPut );
				OSSemPend( UartSendTaskSem, 1000, &err );
				Uart1_Send_Struct(&uart1SendInfo);
				OSSemPost( UartSendTaskSem );
				
				OSTimeDly(1000);
				if ( uart1RcvOK == 1 )
				{			
						OSSemPend(UartRcvTaskSem, 1000, &err );
						uart1RcvInfo = Uart1_Get_Struct();
						OSSemPost(UartRcvTaskSem);
						Uart1LostCnt = 0;
				}
				else 
				{
						Uart1LostCnt++;
				}
		}
		return;
}

