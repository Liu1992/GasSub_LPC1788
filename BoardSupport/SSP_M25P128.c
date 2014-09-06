#include "SSP_M25P128.h"
#include "lpc_types.h"
#include "LPC177x_8x_ssp.h"
#include "lpc177x_8x_gpio.h"
#include "Gpioconfig.h"
/*****************************************************************/
/*                                                               */
/*                                                               */
/*                                                               */
/*                                                               */
/*****************************************************************/
void M25P128_SSP_Init(void)
{
	SSP_CFG_Type SSP_ConfigStruct;
	SSP_ConfigStructInit(&SSP_ConfigStruct);
	SSP_Init(_USING_SSP, &SSP_ConfigStruct); 
	SSP_Cmd(_USING_SSP, ENABLE);

}
/*****************************************************************/
/*                                                               */
/*                                                               */
/*                                                               */
/*                                                               */
/*****************************************************************/
void M25P128_Read(uint32_t Add,uint8_t *DataBuffer,uint32_t Length)
{
	uint8_t Dummy;
	uint32_t i;
	while(LPC_SSP1->SR&SSP_STAT_RXFIFO_NOTEMPTY)
  {
	   Dummy = LPC_SSP1->DR;		/* clear the RxFIFO */
  };
	GPIO_ClearValue(0,SSP_M25P128_CS);	
	LPC_SSP1->DR=0x0b;	
	Dummy=Add>>16;
	LPC_SSP1->DR=Dummy;
	Dummy=Add>>8;
	LPC_SSP1->DR=Dummy;	
	Dummy=Add;
	LPC_SSP1->DR=Dummy;	
	LPC_SSP1->DR=0xff;
  while(LPC_SSP1->SR&SSP_STAT_BUSY);
	
	while(LPC_SSP1->SR&SSP_STAT_RXFIFO_NOTEMPTY)
	{
			Dummy = LPC_SSP1->DR;		
	};		
	for(i=0;i<Length;i++)
	{
			LPC_SSP1->DR=0xff;		
		  while(LPC_SSP1->SR&SSP_STAT_BUSY);		
		  *DataBuffer++=LPC_SSP1->DR;
	};
  GPIO_SetValue(0,SSP_M25P128_CS);			
}
