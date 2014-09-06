
/*********************************************************************************************
* File name	: eeprom_st24c02.c
* HardWare	: LPC1788 and eeprom_st24c02 
* Author		: Lius
* History		:	none
*	Data			:	2014-07-17
* Descript	: eeprom驱动
*	Version		: V1.0.0
*********************************************************************************************/

/********************************************************************************************
*			include files
********************************************************************************************/

#include "eeprom_st24c02.h"

/*****************************************************************************
 * Defines and typedefs
 ****************************************************************************/
#define EEPROM_SLVADDR			(0xA0>>1)
#define ADDRESS_OFFSET      (0)
#define ADDRESS_SIZE        (1)
#define DATA_OFFSET         (1)
#define PAGE_SIZE           (8)

/*********************************************************************************************
* name			:		EEPROM_24c02_Init
* func			:		EEPROM初始化
* para			:		none
* ret				:		none
* comment		:	
*********************************************************************************************/
void EEPROM_24c02_Init()
{
	I2C_Init(I2C_0, 100000);

	/* Enable I2C1 operation */
	I2C_Cmd(I2C_0, I2C_MASTER_MODE, ENABLE);
}

/*********************************************************************************************
* name			:		EEPROM_ReadData
* func			:		EEPROM读数据
* para			:		uint8_t data[],uint8_t size,uint8_t address
* ret				:		none
* comment		:	
*********************************************************************************************/
Bool EEPROM_ReadData(uint8_t data[],uint8_t size,uint8_t address)
{
	/* Receive setup */
	I2C_M_SETUP_Type rxsetup;
	uint8_t eeprom_address[1];
	
	eeprom_address[0]=address;
	rxsetup.sl_addr7bit = EEPROM_SLVADDR;
	rxsetup.tx_data = eeprom_address;	// Get address to read at writing address
	rxsetup.tx_length = ADDRESS_SIZE;
	rxsetup.rx_data = data;
	rxsetup.rx_length = size;
	rxsetup.retransmissions_max = 3;

	if (I2C_MasterTransferData(I2C_0, &rxsetup, I2C_TRANSFER_POLLING) == SUCCESS){
		return TRUE;
	} else {
		return FALSE;
	}
}


/*********************************************************************************************
* name			:		EEPROM_WriteDataOnePage
* func			:		EEPROM写数据
* para			:		uint8_t data[],uint8_t size,uint8_t address
* ret				:		none
* comment		:	
*********************************************************************************************/
Bool EEPROM_WriteDataOnePage(uint8_t data[],uint8_t size,uint8_t address)
{
	/* Transmit setup */
	I2C_M_SETUP_Type txsetup;
	uint8_t wrdata[9];
	uint8_t j;

	wrdata[ADDRESS_OFFSET]=address;
	for(j=0;j<8;j++)
	{
		wrdata[j+1]=data[j];
	}
	txsetup.sl_addr7bit = EEPROM_SLVADDR;
	txsetup.tx_data = wrdata;
	txsetup.tx_length = size+ADDRESS_SIZE;
	txsetup.rx_data = NULL;
	txsetup.rx_length = 0;
	txsetup.retransmissions_max = 3;

	if (I2C_MasterTransferData(I2C_0, &txsetup, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		
		return TRUE;
	} 
	else 
	{
		
		return FALSE;
	}
}

/*********************************************************************************************
* name			:		EEPROM_ReadDataOnePage
* func			:		EEPROM写数据
* para			:		uint8_t data[],uint8_t size,uint8_t address
* ret				:		none
* comment		:	
*********************************************************************************************/
Bool EEPROM_ReadDataOnePage(uint8_t data[],uint8_t size,uint8_t address)
{
	/* Transmit setup */
	I2C_M_SETUP_Type txsetup;
	uint8_t wrdata[1];

	wrdata[ADDRESS_OFFSET]=address;

	txsetup.sl_addr7bit = EEPROM_SLVADDR;
	txsetup.tx_data = wrdata;
	txsetup.tx_length = 1;
	txsetup.rx_data = data;
	txsetup.rx_length = size;
	txsetup.retransmissions_max = 3;

	if (I2C_MasterTransferData(I2C_0, &txsetup, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		
		return TRUE;
	} 
	else 
	{
		
		return FALSE;
	}
}

