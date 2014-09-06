/*
 * FreeModbus Libary: BARE Port
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
 * File: $Id: port.h ,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

#ifndef _PORT_H
#define _PORT_H


#include "mbconfig.h"

#include "ucos_ii.h"

#include <assert.h>
#include <inttypes.h>

#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed. 
  *   If expr is true, it returns no value.
  * @retval : None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__,(uint32_t) __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#define	INLINE
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }

//TODO  暂时先写B13引脚，等组网测试时再确认
#define SLAVE_RS485_SEND_MODE  /*GPIO_SetBits(GPIOB,GPIO_Pin_13)*/
#define SLAVE_RS485_RECEIVE_MODE  /*GPIO_ResetBits(GPIOB,GPIO_Pin_13)*/
#define MASTER_RS485_SEND_MODE  /*GPIO_SetBits(GPIOB,GPIO_Pin_13)*/
#define MASTER_RS485_RECEIVE_MODE  /*GPIO_ResetBits(GPIOB,GPIO_Pin_13)*/

#define ENTER_CRITICAL_SECTION()	OS_ENTER_CRITICAL()
#define EXIT_CRITICAL_SECTION()    OS_EXIT_CRITICAL()


typedef uint8_t BOOL;
//#define BOOL bool


typedef unsigned char UCHAR;
typedef char    CHAR;

typedef uint16_t USHORT;
typedef int16_t SHORT;

typedef uint32_t ULONG;
typedef int32_t LONG;

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

void EnterCriticalSection(void);
void ExitCriticalSection(void);
void vMBDelay(ULONG nCount);

#endif
