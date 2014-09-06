/**
 * tft_hsd04i9w1.h
 *
 * HannStar 721Q310B63-A2 / HSD043I9W1-A driver
 *
 * Guo Jian
 * 2014.04.22
 */
#ifndef __TFT_HSD043I9W1_H
#define __TFT_HSD043I9W1_H

#include <lpc177x_8x_lcd.h>
#include <sdram_mt48lc8m32lfb5.h>

#define C_GLCD_PIX_CLK      (6.4*1000000l)

#define LCD_PPL         480
#define LCD_LPP         272
#define LCD_RED         0xf800                                          /*  red color                   */
#define LCD_GREEN       0x07e0                                          /*  green color                 */
#define LCD_BLUE        0x001f                                          /*  blue color                  */
#define LCD_BLACK       0x0000                                          /*  black color                 */
#define LCD_WHITE       0xffff                                          /*  white color                 */

extern uint16_t LCD_Frame_Buffer[LCD_PPL * LCD_LPP];

void TFT_Init(void);
void LCDBackLightOn(void);
void LCDBackLightOff(void);
void LCDBackLightBright(void);
void LCDBackLightDark(void);

#endif /* __TFT_HSD043I9W1_H */
