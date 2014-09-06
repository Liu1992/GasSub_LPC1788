/**
 * tft_hsd04i9w1.c
 *
 * HannStar 721Q310B63-A2 / HSD043I9W1-A driver
 *
 * Guo Jian
 * 2014.04.22
 */

#include <tft_hsd04i9w1.h>
#include <lpc177x_8x_gpio.h>

uint16_t LCD_Frame_Buffer[LCD_PPL * LCD_LPP]
#ifndef __RAM_MODE__
__attribute__ ((section("LCD_FRAME")))
#endif
;

/**
 * LCD backlight control routines
 * @param  none
 * @return none
 * @brief  Turn on/off lcd backlight
           Note that backlight work only if both switches are on
 */
void LCDBackLightOn(void)
{
	GPIO_OutputValue(2, 1<<11 ,1);
}
void LCDBackLightOff(void)
{
	GPIO_OutputValue(2,1 <<11 ,0);
}
void LCDBackLightBright(void)
{
	GPIO_OutputValue(2, 1<<1 ,1);
}
void LCDBackLightDark(void)
{
	GPIO_OutputValue(2, 1<<1 ,0);
}

/**
 * Init TFT
 * @param  none
 * @return none
 * @brief  Initialize TFT
 */
void TFT_Init(void)
{
    LCD_Config_Type config = {
        {
            2,
            2,
            41,
            LCD_PPL,
        },
        {
            2,
            2,
            10,
            LCD_LPP,
        },
        {
            LCD_PPL,
            1,
            0,
        },
        C_GLCD_PIX_CLK,
        LCD_BPP_16_565Mode,
        LCD_TFT,
        0,
        0,
        0,
        0,
        0,
        0,
        NULL,
    };

    config.lcd_panel_lower = config.lcd_panel_upper = (uint32_t)LCD_Frame_Buffer;
    
    LCD_Init(&config);
    LCDBackLightOn();
    LCDBackLightBright();
    
    {
        int i, j;
        for (j = 0; j < 272; ++ j)
        {
            for (i = 0; i < 160; ++ i)
                LCD_Frame_Buffer[j * 480 + i] = LCD_RED;
            for (i = 160; i < 320; ++ i)
                LCD_Frame_Buffer[j * 480 + i] = LCD_GREEN;
            for (i = 320; i < 480; ++ i)
                LCD_Frame_Buffer[j * 480 + i] = LCD_BLUE;
        }
    }
        
    LCD_Enable(TRUE);
}
