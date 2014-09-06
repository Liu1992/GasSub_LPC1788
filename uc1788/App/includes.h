/*
*********************************************************************************************************
*                                              uC/OS-II
*                                        The Real-Time Kernel
*
*                          (c) Copyright 2004-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/OS-II is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/OS-II in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to
*               help you experience uC/OS-II.  The fact that the source code is provided
*               does NOT mean that you can use it without paying a licensing fee.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                        IAR Development Kits
*                                               on the
*
*                                            NXP LPC1768
*                                      LPC1766-SK KICKSTART KIT
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : FT
*                 DC
*********************************************************************************************************
*/

#ifndef  INCLUDES_MODULES_PRESENT
#define  INCLUDES_MODULES_PRESENT

/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
#include "lpc_types.h"

/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

/* not used ?
#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>
*/

/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/

#include  <app_cfg.h>
/* not used
#include  <bsp.h>
#include  <csp.h>
*/
#include <lpc177x_8x_emc.h>
#include <lpc177x_8x_clkpwr.h>
#include <sdram_mt48lc8m32lfb5.h>
#include <nandflash_k9f4g08u0b.h>
#include <norflash_sst39vf1601.h>

/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <ucos_ii.h>
#include "GUI.h"

/*
*********************************************************************************************************
*                                               FS
*********************************************************************************************************
*/

#include  <yaffsfs.h>

/*
*********************************************************************************************************
*                                                 TCP-IP
*********************************************************************************************************
*/

#include  <lwip/init.h>
#include  <lwip/tcpip.h>

/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/

#endif
