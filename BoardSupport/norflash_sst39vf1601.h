/**
 * norflash_sst39vf1601.h
 *
 * SST39VF1601 norflash driver, modified from norflash_sst39vf3201.c
 * the only difference between SST39VF3201 and SST39VF1601 is the size (32MB / 16MB)
 *
 * Guo Jian
 * 2014.04.22
 */
/**********************************************************************
* $Id$		norflash_sst39vf1601.h			2011-06-02
*//**
* @file		norflash_sst39vf1601.h
* @brief	Contains all macro definitions and function prototypes
*			support for external NOR Flash SamSung SST39VF1601
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
* products. This software is supplied "AS IS" without any warranties.
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

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup NorFlash_SST39VF1601 NorFlash SST39VF1601
 * @ingroup LPC177x_8xCMSIS_Board_Support
 * @{
 */
#ifndef __NORFLASH_SST39VF1601_H_
#define __NORFLASH_SST39VF1601_H_

#include "lpc177x_8x_emc.h"

/*****************************************************************************
 * Defines and typedefs
 ****************************************************************************/
 
 /* conected to CS1 */
#define NOR_FLASH_BASE		0x90000000

#define NOR_FLASH_SIZE		0x00200000

#define GET_ADDR(addr)	(volatile uint16_t *)(NOR_FLASH_BASE | ((addr)<<1))

#define SECTOR_SIZE		0x800	/* Must be 2048 words for 39VF160 */
#define BLOCK_SIZE		0x8000	/* Must be 32K words for 39VF160  */

#define SST_ID			0xBF    /* SST Manufacturer's ID code   */
#define SST_39VF160		0x234B  /* SST 39VF160 device code      */

#define PROGRAM_TIMEOUT	0x08000000

extern void NORFLASHInit( void );
extern void NORFLASHErase( void );
extern uint32_t NORFLASHCheckID( void );
extern uint32_t NORFLASHWriteWord( uint32_t Addr, uint16_t Data );
extern uint32_t ToggleBitCheck( uint32_t Addr, uint16_t Data );

#endif /* __NORFLASH_SST39VF1601_H_ */

/**
 * @}
 */
