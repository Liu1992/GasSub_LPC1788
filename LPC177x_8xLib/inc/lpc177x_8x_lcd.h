/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
Purpose     : Sample configuration file
----------------------------------------------------------------------
*/

#ifndef __LPC177X_8X_LCD_H
#define __LPC177X_8X_LCD_H

#include "sdram_mt48lc8m32lfb5.h"

/*********************************************************************
*
*                   General configuration of LCD
*
**********************************************************************
*/
	#define C_GLCD_PIX_CLK          (6.4*1000000l)		  
	#define C_GLCD_REFRESH_FREQ     (50HZ)
	#define C_GLCD_H_SIZE           480
	#define C_GLCD_H_PULSE          41
	#define C_GLCD_H_FRONT_PORCH    2
	#define C_GLCD_H_BACK_PORCH     2

	#define C_GLCD_V_SIZE           272
	#define C_GLCD_V_PULSE          10
	#define C_GLCD_V_FRONT_PORCH    2
	#define C_GLCD_V_BACK_PORCH     2
	#define C_GLCD_BitsPP			     16
	#define C_GLCD_BytesPP			   4		  //32位   没用到?
  

#define C_GLCD_CLK_PER_LINE     (C_GLCD_H_SIZE + C_GLCD_H_PULSE + C_GLCD_H_FRONT_PORCH + C_GLCD_H_BACK_PORCH)
#define C_GLCD_LINES_PER_FRAME  (C_GLCD_V_SIZE + C_GLCD_V_PULSE + C_GLCD_V_FRONT_PORCH + C_GLCD_V_BACK_PORCH)
#define LCD_VRAM_BASE_ADDR 	((uint32_t)FrameBuffer)


#define U_LCD_XSIZE     C_GLCD_H_SIZE    /*  LCD x size                  */
#define U_LCD_YSIZE     C_GLCD_V_SIZE    /*  LCD y size                  */
#define LCD_XSIZE       C_GLCD_H_SIZE   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE       C_GLCD_V_SIZE   /* Y-resolution of LCD, Logical coor. */
//#define LCD_SWAP_RB     (1)

#define LCD_ON
#define LCD_OFF

#define C_GLCD_PWR_ENA_DIS_DLY  10000
#define C_GLCD_ENA_DIS_DLY      10000

#define MIRROR_X        1                                               /*  x size mirror               */
#define MIRROR_Y        0                                               /*  y size mirror               */

#define HWORDMODE       0                                               /*  picture data                */

//#define LCD_RED         0xf800                                          /*  red color                   */
//#define LCD_GREEN       0x07e0                                          /*  green color                 */
//#define LCD_BLUE        0x001f                                          /*  blue color                  */
//#define LCD_BLACK       0x0000                                          /*  black color                 */
//#define LCD_WHITE       0xffff                                          /*  white color                 */

void delay_Ns (int idly);
void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex);
void LCDBackLightON(void);
void LCDBackLightOFF(void);
void LCDBackLightBright(void);
void LCDBackLightDark(void);

#define LCD_BITSPERPIXEL (16)
#define lpc1788 1788
#define LCD_CONTROLLER lpc1788
 
#endif /* LCDCONF_H */
