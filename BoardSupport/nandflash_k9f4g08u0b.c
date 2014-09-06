/**
 * nandflash_k9f4g08u0b.c
 *
 * K9F4G08U0B nandflash driver, modified from nandflash_k9f1g08u0a.c
 *
 * Guo Jian
 * 2014.04.24
 */
 /**********************************************************************
* $Id$		nandflash_k9f1g08u0a.c			2011-06-02
*//**
* @file		nandflash_k9f1g08u0a.c
* @brief	This c file contains all functions support for Nand Flash 
*			SamSung K9F1G08U0A
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
* 
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.`
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc177x_8x_libcfg.h"
#else
#include "lpc177x_8x_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */
#ifdef _EMC

#include "nandflash_k9f4g08u0b.h"
#include "lpc177x_8x_emc.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"
#include "stdio.h"

/*********************************************************************//**
 * @brief 		Ready/Busy check, no timeout, basically, R/B bit should
 * 				once to bail out from this routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void NandFlash_WaitForReady( void )
{
	TIM_Waitus(100);                    /* wait 100us */
    
	while( !(FIO4PIN & K9FXX_BUSY) );	/* from low to high once */

	return;
}

/*********************************************************************//**
 * @brief 		Initialize external NAND FLASH memory
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void NandFlash_Init( void )
{
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	EMC_STATIC_MEM_Config_Type config;

	/**************************************************************************
	* Initialize EMC for NAND FLASH
	**************************************************************************/
	config.CSn = 0;
	config.AddressMirror = 0;
	config.ByteLane = 1;
	config.DataWidth = 8;
	config.ExtendedWait = 0;
	config.PageMode = 0;
	config.WaitWEn = 2;
	config.WaitOEn = 2;
	config.WaitWr = 0x1f;
	config.WaitPage = 0x1f;
	config.WaitRd = 0x1f;
	config.WaitTurn = 0x1f;	
	StaticMem_Init(&config);
	 // init timer
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);

  	// wait 2ms
	TIM_Waitms(2);

	return;
}

/*********************************************************************//**
 * @brief 		Issue Reset command to NAND FLASH memory
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void NandFlash_Reset( void )
{
	volatile uint8_t *pCLE;

	/* Reset NAND FLASH  through NAND FLASH command */
	pCLE = K9F1G_CLE;
	*pCLE = K9FXX_RESET;

	TIM_Waitms(2);
	return;
}

/*********************************************************************//**
 * @brief 		Read status from NAND FLASH memory
 * @param[in]	Cmd	command for read operation, should be:
 * 					-  K9FXX_BLOCK_PROGRAM_1
 *					-  K9FXX_BLOCK_ERASE_1
 *					-  K9FXX_READ_1
 * @return 		Status, could be:
 *				- TRUE: pass
 *				- FALSE: Failure
 **********************************************************************/
Bool NandFlash_ReadStatus(uint32_t Cmd)
{
	volatile uint8_t *pCLE;
	volatile uint8_t *pDATA;
	uint8_t StatusData;

	pCLE  = K9F1G_CLE;
	pDATA = K9F1G_DATA;

	*pCLE = K9FXX_READ_STATUS;

	while ( (*pDATA & 0xC0) != 0xC0 );

	StatusData = *pDATA;

	switch (Cmd)
	{
		case K9FXX_BLOCK_PROGRAM_1:
		case K9FXX_BLOCK_ERASE_1:
		  	if (StatusData & 0x01)	/* Erase/Program failure(1) or pass(0) */
				return(FALSE);
		  	else
				return(TRUE);

		case K9FXX_READ_1:				/* bit 5 and 6, Read busy(0) or ready(1) */
		  	return(TRUE);

		default:
		  	break;
	}

	return(FALSE);
}
/*********************************************************************//**
 * @brief 		Read ID from external NAND FLASH memory
 * @param[in]	None
 * @return 		ID value
 **********************************************************************/
uint32_t NandFlash_ReadId( void )
{
	uint8_t a, b, c, d;
	volatile uint8_t *pCLE;
	volatile uint8_t *pALE;
	volatile uint8_t *pDATA;

	pCLE  = K9F1G_CLE;
	pALE  = K9F1G_ALE;
	pDATA = K9F1G_DATA;

	*pCLE = K9FXX_READ_ID;
	*pALE = 0;

	a = *pDATA;
	b = *pDATA;
	d = *pDATA;
	c = *pDATA;

	return ((a << 24) | (b << 16) | (c << 8) | d);
}

/*********************************************************************//**
 * @brief 		Erase the whole NAND FLASH memory block based on the
 *				block number
 * @param[in]	blockNum	number of block that will be erased, should
 *				be in range: 0 .. 1023
 * @return 		Erase status, could be:
 * 					- TRUE: pass
 *					- FALSE: failure
 **********************************************************************/
Bool NandFlash_BlockErase( uint32_t blockNum )
{
	volatile uint8_t *pCLE;
	volatile uint8_t *pALE;
	uint32_t rowAddr;

	pCLE  = K9F1G_CLE;
	pALE  = K9F1G_ALE;

	rowAddr = (NANDFLASH_BASE_ADDR + blockNum * NANDFLASH_BLOCK_FSIZE);
	rowAddr = rowAddr - (rowAddr % NANDFLASH_BLOCK_FSIZE);

	*pCLE = K9FXX_BLOCK_ERASE_1;

	*pALE = (uint8_t)((rowAddr & 0x000FF000) >> 12);	    /* row address low      A12 - A19 */
    
	*pALE = (uint8_t)((rowAddr & 0x0FF00000) >> 20);	    /* row address mid      A20 - A27 */

	*pALE = (uint8_t)((rowAddr & 0xF0000000) >> 28);	    /* row address high     A28 - A29 */

	*pCLE = K9FXX_BLOCK_ERASE_2;

	NandFlash_WaitForReady();

	return(NandFlash_ReadStatus(K9FXX_BLOCK_ERASE_1));
}

/*********************************************************************//**
 * @brief 		This routine is used to check if the block is valid or
 *				not.
 * @param[in]	None
 * @return 		Checking status, could be:
 * 					- TRUE: all blocks are valid
 *					- FALSE: invalid block is found, an initial invalid
 *						     table will be created
 **********************************************************************/
Bool NandFlash_ValidBlockCheck( uint32_t block )
{
	uint32_t page;
	uint8_t data = 0;

    for ( page = 0; page < 2; page++ )
    {
        /* Check column address 2048 at first page and second page */
        NandFlash_PageReadFromAddr(block, page, NANDFLASH_INVALIDBLOCK_CHECK_COLUMM, &data, 1);
        if(data != 0xFF)
            return FALSE;
    }
	return TRUE;
}

/*********************************************************************//**
 * @brief 		Write a full page of program into NAND flash based on the
 *				page number, write up to 2112 bytes of data at a time.
 * @param[in]	pageNum		number of page that will be programmed, should
 * 				be in range: 0..63
 * @param[in]	blockNum	number of block that will be programmed, should
 * 				be in range: 0..1023
 * @param[in]	bufPtr		pointer to the buffer that contain data will be
 * 				programmed in flash memory
 * @return 		Program status, could be:
 * 					- TRUE: success
 * 					- FALSE: fail
 **********************************************************************/
Bool NandFlash_PageProgram( uint32_t pageNum, uint32_t blockNum, uint8_t *bufPtr )
{
	volatile uint8_t *pCLE;
	volatile uint8_t *pALE;
	volatile uint8_t *pDATA;
	uint32_t i, curAddr, curColumm;

	pCLE  = K9F1G_CLE;
	pALE  = K9F1G_ALE;
	pDATA = K9F1G_DATA;

	curAddr = NANDFLASH_BASE_ADDR + blockNum * NANDFLASH_BLOCK_FSIZE
								+ pageNum * NANDFLASH_PAGE_FSIZE;

	curColumm = curAddr % NANDFLASH_PAGE_FSIZE;
	curAddr -= curColumm;

	*pCLE = K9FXX_BLOCK_PROGRAM_1;

	*pALE = (uint8_t)(curColumm & 0x000000FF);			/* column address low    A0 - A7 */

	*pALE = (uint8_t)((curColumm & 0x00000F00) >> 8);   /* column address high   A8 - A11 */

	*pALE = (uint8_t)((curAddr & 0x000FF000) >> 12);	    /* row address low      A12 - A19 */
    
	*pALE = (uint8_t)((curAddr & 0x0FF00000) >> 20);	    /* row address mid      A20 - A27 */

	*pALE = (uint8_t)((curAddr & 0xF0000000) >> 28);	    /* row address high     A28 - A29 */

	//Not write to spare area for the NandFlash valid block checking
	for ( i = 0; i < NANDFLASH_RW_PAGE_SIZE; i++ )
	{
		*pDATA = *bufPtr++;
	}

	*pCLE = K9FXX_BLOCK_PROGRAM_2;

	NandFlash_WaitForReady();

	return( NandFlash_ReadStatus( K9FXX_BLOCK_PROGRAM_1 ) );
}

/*********************************************************************//**
 * @brief 		Read the whole NAND FLASH memory page based on the
 *				page number, the data will be stored in the pointer
 *				to the buffer.
 * @param[in]	pageNum		number of page that will be read, should
 * 				be in range: 0..63
 * @param[in]	blockNum	number of block that will be read, should
 * 				be in range: 0..1023
 * @param[in]	bufPtr		pointer to the buffer that contain data will be
 * 				read from flash memory
 * @return 		Read status, could be:
 * 					- TRUE: success
 * 					- FALSE: fail
 **********************************************************************/
Bool NandFlash_PageRead( uint32_t pageNum, uint32_t blockNum, uint8_t *bufPtr )
{
	return ((NandFlash_PageReadFromBeginning(pageNum, blockNum, bufPtr) != 0) ? TRUE:FALSE);
}

/*********************************************************************//**
 * @brief 		Read the whole NAND FLASH memory page based on the
 *				page number, the data will be stored in the pointer
 *				to the buffer.
 * @param[in]	pageNum		number of page that will be read, should
 * 				be in range: 0..63
 * @param[in]	blockNum	number of block that will be read, should
 * 				be in range: 0..1023
 * @param[in]	bufPtr		pointer to the buffer that contain data will be
 * 				read from flash memory
 * @return 		number of byte(s) read til the end of the page
 **********************************************************************/
int NandFlash_PageReadFromBeginning(uint32_t blockNum, uint32_t pageNum, uint8_t* bufPtr)
{
	return (NandFlash_PageReadFromAddr(blockNum, pageNum, 0, bufPtr, NANDFLASH_PAGE_FSIZE));
}

/*********************************************************************//**
 * @brief 		Read the whole NAND FLASH memory page based on the
 *				page number, the data will be stored in the pointer
 *				to the buffer.
 * @param[in]	blockNum	number of block that will be read, should
 * 							be in range: 0..1023
 * @param[in]	pageNum		number of page that will be read, should
 * 				be in range: 0..63
 * @param[in]	addrInPage	the address in NandFlash to be read,
 * 							calculated from the beginning of page
 * @param[in]	bufPtr		pointer to the buffer that contain data will be
 * 							read from flash memory
 * @param[in]	size	the number of byte(s) to be read and stored to the buffer
 * @return 		number of byte(s) read til the end of the page
 **********************************************************************/
int NandFlash_PageReadFromAddr(uint32_t blockNum, uint32_t pageNum,
											uint32_t addrInPage, uint8_t* bufPtr, uint32_t size)
{
	uint32_t curAddr = 0;

	curAddr += NANDFLASH_BASE_ADDR + blockNum * NANDFLASH_BLOCK_FSIZE;

	curAddr += pageNum * NANDFLASH_PAGE_FSIZE;

	curAddr += addrInPage;

	return (NandFlash_ReadFromAddr(curAddr, bufPtr, size));
}

/*********************************************************************//**
 * @brief 		Read the whole NAND FLASH memory at an expected address,
 *				the data will be stored in the pointer to the buffer.
 * @param[in]	addrInWholeNand	the address in NandFlash to be read,
 * 						calculated from the beginning of Nand base address 0
 * @param[in]	bufPtr	pointer to the buffer that contain data will be
 * 						read from flash memory
 * @param[in]	size	the number of byte(s) to be read and stored to the buffer
 * @return 		number of byte(s) read til the end of the page
 **********************************************************************/
int NandFlash_ReadFromAddr(uint32_t addrInWholeNand, uint8_t* bufPtr, uint32_t size)
{
	volatile uint8_t *pCLE;
	volatile uint8_t *pALE;
	volatile uint8_t *pDATA;
	uint32_t i, curColumm, curRow;

	i = 0;

	pCLE  = K9F1G_CLE;
	pALE  = K9F1G_ALE;
	pDATA = K9F1G_DATA;

	curColumm = addrInWholeNand % NANDFLASH_PAGE_FSIZE;
	curRow = addrInWholeNand - curColumm;

	*pCLE = K9FXX_READ_1;

	*pALE = (uint8_t)(curColumm & 0x000000FF);			/* column address low    A0 - A7 */

	*pALE = (uint8_t)((curColumm & 0x00000F00) >> 8);   /* column address high   A8 - A11 */

	*pALE = (uint8_t)((curRow & 0x000FF000) >> 12);	    /* row address low      A12 - A19 */
    
	*pALE = (uint8_t)((curRow & 0x0FF00000) >> 20);	    /* row address mid      A20 - A27 */

	*pALE = (uint8_t)((curRow & 0xF0000000) >> 28);	    /* row address high     A28 - A29 */

	*pCLE = K9FXX_READ_2;

	NandFlash_WaitForReady();

	//Get data from the current address in the page til the end of the page
	for ( i = 0; i < (NANDFLASH_PAGE_FSIZE - curColumm); i++ )
	{
		*bufPtr = *pDATA;

		bufPtr++;

		if((i + 1) >= size)
			break;
	}

	// Ok, return
	return i;
}

int NandFlash_PageReadWithSpare(uint32_t page, uint8_t* dataPtr, uint8_t* sparePtr)
{
	volatile uint8_t *pCLE;
	volatile uint8_t *pALE;
	volatile uint8_t *pDATA;
	uint32_t i, off;
    
	pCLE  = K9F1G_CLE;
	pALE  = K9F1G_ALE;
	pDATA = K9F1G_DATA;

    off = dataPtr ? 0 : NANDFLASH_RW_PAGE_SIZE;
    
	*pCLE = K9FXX_READ_1;

	*pALE = off & 0x000000FF;	 	 	/* column address low    A0 - A7 */

	*pALE = (off & 0x00000F00) >> 8;    /* column address high   A8 - A11 */

	*pALE = page & 0x000000FF;	        /* row address low      A12 - A19 */
    
	*pALE = (page & 0x0000FF00) >> 8;   /* row address mid      A20 - A27 */

	*pALE = (page & 0x000F0000) >> 16;  /* row address high     A28 - A29 */

	*pCLE = K9FXX_READ_2;

	NandFlash_WaitForReady();

	/* Get data from the beginning til the end of the data */
    if (dataPtr)
        for ( i = 0; i < NANDFLASH_RW_PAGE_SIZE ; ++ i )
            * (dataPtr ++) = *pDATA;
        
	/* Get data from the current address in the page til the end of the page */
    if (sparePtr)
        for ( i = NANDFLASH_RW_PAGE_SIZE; i < NANDFLASH_PAGE_FSIZE ; ++ i )
            * (sparePtr ++) = *pDATA;

	// Ok, return
	return 1;
}

Bool NandFlash_PageProgramWithSpare( uint32_t page, const uint8_t *dataPtr, uint32_t dataLen, const uint8_t* sparePtr)
{
	volatile uint8_t *pCLE;
	volatile uint8_t *pALE;
	volatile uint8_t *pDATA;
	uint32_t i, off;

	pCLE  = K9F1G_CLE;
	pALE  = K9F1G_ALE;
	pDATA = K9F1G_DATA;

    if (sparePtr && *sparePtr != 0xFF)
    {
        /* in case wrongly marked */
        printf("writing bad mark!!!\n");
        for(;;);
    }
    
    off = dataPtr ? 0 : NANDFLASH_RW_PAGE_SIZE;

	*pCLE = K9FXX_BLOCK_PROGRAM_1;

	*pALE = off & 0x000000FF;	 	 	/* column address low    A0 - A7 */

	*pALE = (off & 0x00000F00) >> 8;    /* column address high   A8 - A11 */

	*pALE = page & 0x000000FF;	        /* row address low      A12 - A19 */
    
	*pALE = (page & 0x0000FF00) >> 8;   /* row address mid      A20 - A27 */

	*pALE = (page & 0x000F0000) >> 16;  /* row address high     A28 - A29 */

	/* write data from the beginning til the end of the data */
    if (dataPtr)
        for ( i = 0; i < dataLen ; ++ i )
            *pDATA = * (dataPtr ++);
        
	/* write data from the current address in the page til the end of the page */
    if (sparePtr)
    {
        if (dataPtr && dataLen != NANDFLASH_RW_PAGE_SIZE)
        {
            *pCLE = K9FXX_BLOCK_PROGRAM_3;
            
            off = NANDFLASH_RW_PAGE_SIZE;

            *pALE = off & 0x000000FF;	 	 	/* column address low    A0 - A7 */

            *pALE = (off & 0x00000F00) >> 8;    /* column address high   A8 - A11 */
        }
        for ( i = NANDFLASH_RW_PAGE_SIZE; i < NANDFLASH_PAGE_FSIZE ; ++ i )
            *pDATA = * (sparePtr ++);
    }

	*pCLE = K9FXX_BLOCK_PROGRAM_2;

	NandFlash_WaitForReady();

	return( NandFlash_ReadStatus( K9FXX_BLOCK_PROGRAM_1 ) );
}
#endif /*_EMC*/
