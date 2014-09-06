
#include "lpc_types.h"
#include "SSP_GT21L16S2W.h"
#include "lpc177x_8x_gpio.h"
#define BYTES_PER_FONT (9*72)
uint8_t GUI_FontDataBuf[BYTES_PER_FONT];
/*****************************************************************/
/*                                                               */
/*                                                               */
/*                                                               */
/*                                                               */
/*****************************************************************/
void GT21L16S2W_SSP_Init(void)
{
	SSP_CFG_Type SSP_ConfigStruct;
	SSP_ConfigStructInit(&SSP_ConfigStruct);
	SSP_Init(LPC_SSP0, &SSP_ConfigStruct); 
	SSP_Cmd(LPC_SSP0, ENABLE);

}
/*****************************************************************/
/*                                                               */
/*                                                               */
/*                                                               */
/*                                                               */
/*****************************************************************/
void GT21L16S2W_Read(uint32_t Add,uint8_t *DataBuffer,uint32_t Length)
{
	uint8_t Dummy;
	uint32_t i;
	while(LPC_SSP0->SR&SSP_STAT_RXFIFO_NOTEMPTY)
  {
	   Dummy = LPC_SSP0->DR;		/* clear the RxFIFO */
  };
	GPIO_ClearValue(2,SSP_GT21L16S2W_CS);	
	LPC_SSP0->DR=0x0b;	
	Dummy=Add>>16;
	LPC_SSP0->DR=Dummy;
	Dummy=Add>>8;
	LPC_SSP0->DR=Dummy;	
	Dummy=Add;
	LPC_SSP0->DR=Dummy;	
	LPC_SSP0->DR=0xff;
  while(LPC_SSP0->SR&SSP_STAT_BUSY);
	
	while(LPC_SSP0->SR&SSP_STAT_RXFIFO_NOTEMPTY)
	{
			Dummy = LPC_SSP0->DR;		
	};		
	for(i=0;i<Length;i++)
	{
			LPC_SSP0->DR=0xff;		
		  while(LPC_SSP0->SR&SSP_STAT_BUSY);		
		  *DataBuffer++=LPC_SSP0->DR;
	};
  GPIO_SetValue(2,SSP_GT21L16S2W_CS);			
}
