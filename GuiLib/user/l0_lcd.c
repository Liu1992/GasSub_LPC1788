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
*/
#include "LCD_Private.h"      /* private modul definitions & config */
#include "GUI_Private.h"
#include "GUIDebug.h"
#include "L0_LCD.h"
#include <LPC177x_8x.H>
#include <LPC177x_8x_GPIO.H>
#include <LPC177x_8x_clkpwr.H>

/**********************************************************************/

#if (LCD_CONTROLLER == lpc1788) \
    && (!defined(WIN32) | defined(LCD_SIMCONTROLLER))

/*********************************************************************
*
*       Macros
*
**********************************************************************
*/
#define WORDS_PER_LINE  (LCD_VXSIZE * 3 /4)

#define Y2OFF(y)    (WORDS_PER_LINE * y)

/*********************************************
*
*       Macros for MIRROR_, SWAP_ and LUT_
*
**********************************************
*/

#if (!defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
  #if   (!LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) y
  #elif (!LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) y
    #define LOG2PHYS_Y(x, y) x
  #elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) LCD_YSIZE - 1 - (y)
  #elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_YSIZE - 1 - (y)
    #define LOG2PHYS_Y(x, y) x
  #elif ( LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_XSIZE - 1 - (x)
    #define LOG2PHYS_Y(x, y) y
  #elif ( LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_YSIZE - 1 - (y)
    #define LOG2PHYS_Y(x, y) x
  #elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_XSIZE - 1 - (x)
    #define LOG2PHYS_Y(x, y) LCD_YSIZE - 1 - (y)
  #elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #error This combination of mirroring/swapping not yet supported
  #endif
#else
  #if   ( defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) LCD__aLine2Com0[y]
  #elif (!defined (LCD_LUT_COM) &&  defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) LCD__aCol2Seg0[x]
    #define LOG2PHYS_Y(x, y) y
  #elif ( defined (LCD_LUT_COM) &&  defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) LCD__aCol2Seg0[x]
    #define LOG2PHYS_Y(x, y) LCD__aLine2Com0[y]
  #endif
#endif

/*********************************************************************************************************
** Function name :        delay_Ns
** Function Description : delay time 
** Input:                 idly   the necessary time
** Output:                none
*********************************************************************************************************/                                               
void delay_Ns (int idly)
{
    int i;
    
    for(; idly > 0; idly--) {
        for (i = 0; i < 5000; i++);
    }
}

/*********************************************
*
*       Draw Bitmap 1 BPP
*
**********************************************
*/

static void  _DrawBitLine1BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX Index0 = *(pTrans+0);
  LCD_PIXELINDEX Index1 = *(pTrans+1);
  x += Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      do {
        LCD_L0_SetPixelIndex(x++, y, (*p & (0x80 >> Diff)) ? Index1 : Index0);
			  if (++Diff == 8) {
          Diff = 0;
				  p++;
			  }
		  } while (--xsize);
      break;
    case LCD_DRAWMODE_TRANS:
      do {
  		  if (*p & (0x80 >> Diff))
          LCD_L0_SetPixelIndex(x, y, Index1);
        x++;
			  if (++Diff == 8) {
          Diff = 0;
				  p++;
			  }
		  } while (--xsize);
      break;
    case LCD_DRAWMODE_XOR:;
      do {
  		  if (*p & (0x80 >> Diff)) {
          int Pixel = LCD_L0_GetPixelIndex(x, y);
          LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - 1 - Pixel);
        }
        x++;
			  if (++Diff == 8) {
          Diff = 0;
				  p++;
			  }
		  } while (--xsize);
      break;
	}
}

/*********************************************
*
*       Draw Bitmap 2 BPP
*
**********************************************
*/

#if (LCD_MAX_LOG_COLORS > 2)
static void  _DrawBitLine2BPP(int x, int y, U8 const * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixels = *p;
  int CurrentPixel = Diff;
  x += Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      do {
        int Shift = (3 - CurrentPixel) << 1;
        int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
        LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
        LCD_L0_SetPixelIndex(x++, y, PixelIndex);
        if (++CurrentPixel == 4) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
		  } while (--xsize);
      break;
    case LCD_DRAWMODE_TRANS:
      do {
        int Shift = (3 - CurrentPixel) << 1;
        int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
        if (Index) {
          LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
          LCD_L0_SetPixelIndex(x, y, PixelIndex);
        }
        x++;
        if (++CurrentPixel == 4) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
		  } while (--xsize);
      break;
  }
}
#endif

/*********************************************
*
*       Draw Bitmap 4 BPP
*
**********************************************
*/

#if (LCD_MAX_LOG_COLORS > 4)
static void  _DrawBitLine4BPP(int x, int y, U8 const * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixels = *p;
  int CurrentPixel = Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      do {
        int Shift = (1 - CurrentPixel) << 2;
        int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
        LCD_L0_SetPixelIndex(x++, y, PixelIndex);
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
		  } while (--xsize);
      break;
    case LCD_DRAWMODE_TRANS:
      do {
        int Shift = (1 - CurrentPixel) << 2;
        int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        if (Index) {
          LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
          LCD_L0_SetPixelIndex(x, y, PixelIndex);
        }
        x++;
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
		  } while (--xsize);
      break;
  }
}
#endif

/*********************************************
*
*       Draw Bitmap 8 BPP
*
**********************************************
*/

#if (LCD_MAX_LOG_COLORS > 16)
static void  _DrawBitLine8BPP(int x, int y, U8 const * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixel;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      if (pTrans) {
        for (; xsize > 0; xsize--, x++, p++) {
          Pixel = *p;
          LCD_L0_SetPixelIndex(x, y, *(pTrans + Pixel));
        }
      } else {
        for (; xsize > 0; xsize--, x++, p++) {
          LCD_L0_SetPixelIndex(x, y, *p);
        }
      }
      break;
    case LCD_DRAWMODE_TRANS:
      if (pTrans) {
        for (; xsize > 0; xsize--, x++, p++) {
          Pixel = *p;
          if (Pixel) {
            LCD_L0_SetPixelIndex(x, y, *(pTrans + Pixel));
          }
        }
      } else {
        for (; xsize > 0; xsize--, x++, p++) {
          Pixel = *p;
          if (Pixel) {
            LCD_L0_SetPixelIndex(x, y, Pixel);
          }
        }
      }
      break;
  }
}
#endif

/*********************************************
*
*       Draw Bitmap 16 BPP
*
**********************************************
*/

#if (LCD_BITSPERPIXEL > 8)
static void  DrawBitLine16BPP(int x, int y, U16 const * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) == 0) {
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        LCD_L0_SetPixelIndex(x, y, *(pTrans + pixel));
      }
    } else {
      for (;xsize > 0; xsize--, x++, p++) {
        LCD_L0_SetPixelIndex(x, y, *p);
      }
    }
  } else {
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          LCD_L0_SetPixelIndex(x, y, *(pTrans + pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          LCD_L0_SetPixelIndex(x, y, pixel);
        }
      }
    }
  }
}
#endif



/*********************************************************************
*
*       Exported functions
*
**********************************************************************
*/

/*********************************************
*
*       LCD_L0_SetPixelIndex
*
**********************************************
Purpose:
  Sets the index of the given pixel. The upper layers of emWin
  calling this routine make sure that the coordinates are in range, so
  that no check on the parameters needs to be performed.
*/

void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex) {
  /* Convert logical into physical coordinates (Dep. on LCDConf.h) */

  #if LCD_SWAP_XY | LCD_MIRROR_X| LCD_MIRROR_Y
    int xPhys = LOG2PHYS_X(x, y);
    int yPhys = LOG2PHYS_Y(x, y);
  #else
    #define xPhys x
    #define yPhys y
  #endif
  /* Write into hardware ... Adapt to your system */

 if ((x < LCD_XSIZE) && (y < LCD_YSIZE)) {
        *(volatile uint16_t *)(LCD_VRAM_BASE_ADDR + 2*(xPhys + LCD_XSIZE * yPhys)) = (unsigned short)PixelIndex;
    }

}

/*********************************************
*
*       LCD_L0_GetPixelIndex
*
**********************************************
Purpose:
  Returns the index of the given pixel. The upper layers of emWin
  calling this routine make sure that the coordinates are in range, so
  that no check on the parameters needs to be performed.
*/

unsigned int LCD_L0_GetPixelIndex(int x, int y) {
  LCD_PIXELINDEX PixelIndex;
  /* Convert logical into physical coordinates (Dep. on LCDConf.h) */
  #if LCD_SWAP_XY | LCD_MIRROR_X| LCD_MIRROR_Y
    int xPhys = LOG2PHYS_X(x, y);
    int yPhys = LOG2PHYS_Y(x, y);
  #else
    #define xPhys x
    #define yPhys y
  #endif
  /* Write into hardware ... Adapt to your system */

    if ((x < U_LCD_XSIZE) && (y < U_LCD_YSIZE)) {
        PixelIndex = *((volatile uint16_t *)(LCD_VRAM_BASE_ADDR + 2*(xPhys + U_LCD_XSIZE * yPhys))); 
		return PixelIndex;
    } 
	return 0;
}

/*********************************************
*
*       LCD_L0_XorPixel
*
**********************************************
*/

void LCD_L0_XorPixel(int x, int y) {
  LCD_PIXELINDEX PixelIndex = LCD_L0_GetPixelIndex(x, y);
  LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - PixelIndex - 1);
}

/*********************************************
*
*       LCD_L0_DrawHLine
*
**********************************************
*/

void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (; x0 <= x1; x0++) {
      LCD_L0_XorPixel(x0, y);
    }
  } else {
    for (; x0 <= x1; x0++) {
      LCD_L0_SetPixelIndex(x0, y, LCD_COLORINDEX);
    }
  }
}

/*********************************************
*
*       LCD_L0_DrawVLine
*
**********************************************
*/

void LCD_L0_DrawVLine  (int x, int y0,  int y1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (; y0 <= y1; y0++) {
      LCD_L0_XorPixel(x, y0);
    }
  } else {
    for (; y0 <= y1; y0++) {
      LCD_L0_SetPixelIndex(x, y0, LCD_COLORINDEX);
    }
  }
}

/*********************************************
*
*       LCD_L0_FillRect
*
**********************************************
*/

void LCD_L0_FillRect(int x0, int y0, int x1, int y1) {
  for (; y0 <= y1; y0++) {
    LCD_L0_DrawHLine(x0, y0, x1);
  }
}

/*********************************************
*
*       LCD_L0_DrawBitmap
*
**********************************************
*/
void LCD_L0_DrawBitmap(int x0, int y0,
                       int xsize, int ysize,
                       int BitsPerPixel, 
                       int BytesPerLine,
                       const U8* pData, int Diff,
                       const LCD_PIXELINDEX* pTrans)
{
  int i;
  /* Use _DrawBitLineXBPP */
  for (i=0; i<ysize; i++) {
    switch (BitsPerPixel) {
    case 1:
      _DrawBitLine1BPP(x0, i + y0, pData, Diff, xsize, pTrans);
      break;
      case 2:
        _DrawBitLine2BPP(x0, i + y0, pData, Diff, xsize, pTrans);
        break;
      case 4:
        _DrawBitLine4BPP(x0, i + y0, pData, Diff, xsize, pTrans);
        break;
      case 8:
        _DrawBitLine8BPP(x0, i + y0, pData, xsize, pTrans);
        break;
      case 16:
        DrawBitLine16BPP(x0, i + y0, (const U16 *)pData, xsize, pTrans);
        break;
    }
    pData += BytesPerLine;
  }
}

/*********************************************
*
*       LCD_L0_SetOrg
*
**********************************************
*/

void LCD_L0_SetOrg(int x, int y) {
  GUI_USE_PARA(x);
  GUI_USE_PARA(y);
}

/*********************************************
*
*       LCD_On / LCD_Off
*
**********************************************
*/

void LCD_On (void) {
#ifdef LCD_ON
  	volatile uint32_t i;

    LPC_LCD->CTRL |= (1<<0);
    for(i = C_GLCD_PWR_ENA_DIS_DLY; i; i--);
    LPC_LCD->CTRL |= (1<<11);
#endif
}

void LCD_Off (void) {
  	volatile uint32_t i;
#ifdef LCD_OFF
    LPC_LCD->CTRL &= ~(1<<11);
    for(i = C_GLCD_PWR_ENA_DIS_DLY; i; i--);
    LPC_LCD->CTRL &= ~(1<<0);
#endif
}	   


void LCDBackLightON(void)
{
	GPIO_SetDir(2, 1<<11, 1);		//输出引脚
	GPIO_OutputValue(2,1<<11 ,1);  	//背光开启
}

void LCDBackLightOFF(void)
{
	GPIO_SetDir(2, 1<<11, 1);		//输出引脚
	GPIO_OutputValue(2,1<<11 ,0);  	//背光关闭
}

void LCDBackLightBright(void)
{
	GPIO_SetDir(2, 1<<1, 1);		//输出引脚
	GPIO_OutputValue(2,1<<1 ,1);   	//背光亮
}

void LCDBackLightDark(void)
{
	GPIO_SetDir(2, 1<<1, 1);		//输出引脚
	GPIO_OutputValue(2,1<<1 ,0);   	//背光暗	
}

/*********************************************
*
*       LCD_L0_Init
*
**********************************************
Purpose:
  Initialises the LCD-controller.
*/

int  LCD_L0_Init(void) {

  uint32_t i;

	uint32_t *pDst = (uint32_t *)LCD_VRAM_BASE_ADDR;
//	uint32_t p0,p1,p2,p3;
	// Assign pins

	
	
	//Turn on LCD clock
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCLCD, ENABLE);
	
	// Disable cursor
	LPC_LCD->CRSR_CTRL &=~(1<<0);
	
	// disable LCD controller	
	LPC_LCD->CTRL = 0;
	// 16 bpp
	LPC_LCD->CTRL &= ~(0x07 <<1);
	LPC_LCD->CTRL |=(6<<1);
	
	// TFT panel
	LPC_LCD->CTRL |= (1<<5);
	// single panel
	LPC_LCD->CTRL &= ~(1<<7);
	// notmal output
//	LPC_LCD->CTRL &= ~(1<<8);
	LPC_LCD->CTRL |= (1<<8);
	// little endian byte order
	LPC_LCD->CTRL &= ~(1<<9);
	// little endian pix order
	LPC_LCD->CTRL &= ~(1<<10);
	// disable power
	LPC_LCD->CTRL &= ~(1<<11);
	// init pixel clock
	LPC_SC->LCD_CFG = CLKPWR_GetCLK(CLKPWR_CLKTYPE_PER) / ((uint32_t)C_GLCD_PIX_CLK);
	// bypass inrenal clk divider
	LPC_LCD->POL |=(1<<26);
	// clock source for the LCD block is HCLK
	LPC_LCD->POL &= ~(1<<5);
	// LCDFP pin is active LOW and inactive HIGH
	LPC_LCD->POL |= (1<<11);
	// LCDLP pin is active LOW and inactive HIGH
//	LPC_LCD->POL |= (1<<12);
	LPC_LCD->POL &= ~(1<<12);
	// data is driven out into the LCD on the falling edge
//	LPC_LCD->POL |= (1<<13);
	LPC_LCD->POL &= ~(1<<13);
	// active high
	LPC_LCD->POL &= ~(1<<14);
	LPC_LCD->POL &= ~(0x3FF <<16);
	LPC_LCD->POL |= (C_GLCD_H_SIZE-1)<<16;
	
	// init Horizontal Timing
	LPC_LCD->TIMH = 0; //reset TIMH before set value
	LPC_LCD->TIMH |= (C_GLCD_H_FRONT_PORCH - 1)<<16;
	LPC_LCD->TIMH |= (C_GLCD_H_PULSE - 1)<<8;
	LPC_LCD->TIMH |= ((C_GLCD_H_SIZE/16) - 1)<<2;
	
	// init Vertical Timing
	LPC_LCD->TIMV = 0;  //reset TIMV value before setting
	LPC_LCD->TIMV |= (C_GLCD_V_BACK_PORCH)<<24;
	LPC_LCD->TIMV |= (C_GLCD_V_FRONT_PORCH)<<16;
	LPC_LCD->TIMV |= (C_GLCD_V_PULSE - 1)<<10;
	LPC_LCD->TIMV |= C_GLCD_V_SIZE - 1;
	// Frame Base Address doubleword aligned
	LPC_LCD->UPBASE = LCD_VRAM_BASE_ADDR & ~7UL ;
	LPC_LCD->LPBASE = LCD_VRAM_BASE_ADDR & ~7UL ;
        // init colour pallet

	// clear display memory
	for(i = 0; (C_GLCD_H_SIZE *90 / 2)> i; i++)
	{
	  		*pDst++ = 0x1f001f;
	};
	for(; (C_GLCD_H_SIZE * 180 / 2) > i; i++)
	{
	  		*pDst++ = 0x7e007e0;
	};
	for(; (C_GLCD_H_SIZE * C_GLCD_V_SIZE / 2)> i; i++)
	{
	  		*pDst++ = 0xf800f800;
	};

	
 	for(i = C_GLCD_ENA_DIS_DLY; i; i--);	


	
  return 0;
}

/*********************************************
*
*       LCD_L0_SetLUTEntry
*
**********************************************
*/

void LCD_L0_SetLUTEntry(U8 Pos, LCD_COLOR Color) {
  GUI_USE_PARA(Pos);
  GUI_USE_PARA(Color);
}

#else

void LCD444_c(void) { } /* avoid empty object files */

#endif /* (LCD_CONTROLLER undefined) */

