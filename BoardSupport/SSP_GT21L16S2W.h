/*********************************************************************/
/*                                                                   */
/*                   SSP_M25P128.h                                   */
/*                                                                   */
/*********************************************************************/
#ifndef __GT21L16S2W_H
#define __GT21L16S2W_H
/*********************************************************************/
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*********************************************************************/
#include "lpc_types.h"
#include "LPC177x_8x_ssp.h"
/*********************************************************************/
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*********************************************************************/
/*字模计算公式:(区码-0xa0)*94+(位码-0xa0)-95)*字模长度+字模首地址*/
#define GB2312_15X16_BASE 0x00000000 //15X16点GB2312 标准点阵字库
#define ASCII_7X8_BASE    0x000066C0 //7X8点ASCII字符
#define GB2312_8X16_BASE  0x0003B7D0 //8X16点国标扩展字符
#define ASCII_8X16_BASE   0x0003B7C0 //8X16点ASCII字符
#define ASCII_5X7_BASE    0x0003BFC0 //5X7点ASCII字符

#define GB2312_11X12_BASE 0x0003CF80
#define GB2312_6X12_BASE  0x00066D4C
#define ASCII_6X12_BASE   0x00066D40


#define SSP_GT21L16S2W_CS  (1<<22) //GT21L16S2W的片选信号


/*********************************************************************/
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*********************************************************************/
extern void GT21L16S2W_Read(uint32_t Add,uint8_t *DataBuffer,uint32_t Length);
extern void GT21L16S2W_SSP_Init(void);
#endif
