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
File        : GUICharP.C
Purpose     : Implementation of Proportional fonts
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Private.h"
 
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIPROP_FindChar
*/

static const GUI_FONT_PROP GUI_UNI_PTR * GUIPROP_FindChar(const GUI_FONT_PROP GUI_UNI_PTR* pProp, U16P c) {
  for (; pProp; pProp = pProp->pNext) {
    if ((c>=pProp->First) && (c<=pProp->Last))
      break;
  }
  return pProp;
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIPROP_DispChar
*
* Purpose:
*   This is the routine that displays a character. It is used by all
*   other routines which display characters as a subroutine.
*  此处被修改，从SPI芯片里面读字模
*/
void GUIPROP_DispChar(U16P c) {
  int BytesPerLine;
	U16 BytesPerFont;
	U32 base,oft;
	U8  MSB,LSB;

  GUI_DRAWMODE DrawMode = GUI_Context.TextMode;
  const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUIPROP_FindChar(GUI_Context.pAFont->p.pProp, c);
  if (pProp) 
	{
    GUI_DRAWMODE OldDrawMode;
		const GUI_CHARINFO GUI_UNI_PTR * pCharInfo;
		if(GUI_Context.pAFont == &GUI_FontHZ16)
		{
				 pCharInfo = pProp->paCharInfo;

				 base = (U32)pProp->paCharInfo->pData;
				 BytesPerFont = GUI_Context.pAFont->YSize * pProp->paCharInfo->BytesPerLine; //?????????? 
				 if (BytesPerFont > BYTES_PER_FONT)
				 {
						 BytesPerFont = BYTES_PER_FONT;
				 }; 
				 if (c < 0x80)
				 { 
						 oft = base +(c-0x20)*BytesPerFont; 
						 GT21L16S2W_Read(oft, GUI_FontDataBuf, BytesPerFont);
					 
						 BytesPerLine = pCharInfo->BytesPerLine;
						 OldDrawMode = LCD_SetDrawMode(DrawMode); 

						 LCD_DrawBitmap( GUI_Context.DispPosX, 
														 GUI_Context.DispPosY,
														 pCharInfo->XSize/2,
														 GUI_Context.pAFont->YSize,
														 GUI_Context.pAFont->XMag,
														 GUI_Context.pAFont->YMag,
														 1, /* Bits per Pixel */
														 BytesPerLine,
														 GUI_FontDataBuf,
														 &LCD_BKCOLORINDEX);					 
					 
					 
				 } 
				 else 
				 {
					   MSB=c>>8;
					   LSB=c&0xff;
					   if(MSB == 0xA9 && LSB >=0xA1)
									oft =(282 + (LSB - 0xA1)) * BytesPerFont+ base;
						 else if(MSB >=0xA1 && MSB <= 0xA3 && LSB >=0xA1)
									oft =( (MSB - 0xA1) * 94 + (LSB - 0xA1)) * BytesPerFont+ base;
						 else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
									oft = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846) * BytesPerFont+ base;

						 GT21L16S2W_Read(oft, GUI_FontDataBuf, BytesPerFont);
						 
						 BytesPerLine = pCharInfo->BytesPerLine;
						 OldDrawMode = LCD_SetDrawMode(DrawMode); 

						 LCD_DrawBitmap( GUI_Context.DispPosX, 
														 GUI_Context.DispPosY,
														 pCharInfo->XSize,
														 GUI_Context.pAFont->YSize,
														 GUI_Context.pAFont->XMag,
														 GUI_Context.pAFont->YMag,
														 1, /* Bits per Pixel */
														 BytesPerLine,
														 GUI_FontDataBuf,
														 &LCD_BKCOLORINDEX);
				 };
    }
		else
		{
				pCharInfo = pProp->paCharInfo+(c-pProp->First);
				BytesPerLine = pCharInfo->BytesPerLine;
				OldDrawMode  = LCD_SetDrawMode(DrawMode);
				LCD_DrawBitmap( GUI_Context.DispPosX, GUI_Context.DispPosY,
													 pCharInfo->XSize,
													 GUI_Context.pAFont->YSize,
													 GUI_Context.pAFont->XMag,
													 GUI_Context.pAFont->YMag,
													 1,     /* Bits per Pixel */
													 BytesPerLine,
													 pCharInfo->pData,
													 &LCD_BKCOLORINDEX
					
													 );
		};
    /* Fill empty pixel lines */
    if (GUI_Context.pAFont->YDist > GUI_Context.pAFont->YSize) 
		{
      int YMag = GUI_Context.pAFont->YMag;
      int YDist = GUI_Context.pAFont->YDist * YMag;
      int YSize = GUI_Context.pAFont->YSize * YMag;
      if (DrawMode != LCD_DRAWMODE_TRANS) {
        LCD_COLOR OldColor = GUI_GetColor();
        GUI_SetColor(GUI_GetBkColor());
        LCD_FillRect(GUI_Context.DispPosX, 
                     GUI_Context.DispPosY + YSize, 
                     GUI_Context.DispPosX + pCharInfo->XSize, 
                     GUI_Context.DispPosY + YDist);
        GUI_SetColor(OldColor);
      }
    };
    LCD_SetDrawMode(OldDrawMode); /* Restore draw mode */
    GUI_Context.DispPosX += pCharInfo->XDist * GUI_Context.pAFont->XMag;
  }
}

/*********************************************************************
*
*       GUIPROP_GetCharDistX此处为了汉字显示，被修改
*/
int GUIPROP_GetCharDistX(U16P c) {
  const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUIPROP_FindChar(GUI_Context.pAFont->p.pProp, c);
  //return (pProp) ? (pProp->paCharInfo+(c-pProp->First))->XSize * GUI_Context.pAFont->XMag : 0;
	if(GUI_Context.pAFont == &GUI_FontHZ16)
	{	
		return (pProp) ? pProp->paCharInfo->XDist * GUI_Context.pAFont->XMag : 0;
	}
	else
	{
		return (pProp) ? (pProp->paCharInfo+(c-pProp->First))->XDist * GUI_Context.pAFont->XMag : 0;
	};

}

/*********************************************************************
*
*       GUIPROP_GetFontInfo
*/
void GUIPROP_GetFontInfo(const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO* pfi) {
  GUI_USE_PARA(pFont);
  pfi->Flags = GUI_FONTINFO_FLAG_PROP;
}

/*********************************************************************
*
*       GUIPROP_IsInFont
*/
char GUIPROP_IsInFont(const GUI_FONT GUI_UNI_PTR * pFont, U16 c) {
  const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUIPROP_FindChar(pFont->p.pProp, c);
  return (pProp==NULL) ? 0 : 1;
}

/*************************** End of file ****************************/
