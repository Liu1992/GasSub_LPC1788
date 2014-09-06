
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_gpio.h"
#include "Gpioconfig.h"


void GPIOIni(void)
{
    volatile uint32_t i;
#ifndef __RAM_MODE__
    /*****************************************/
    //      EMC
    /*****************************************/
    PINSEL_ConfigPin(2,16,1);//EMC_CAS
    PINSEL_ConfigPin(2,17,1);//EMC_RAS
    PINSEL_ConfigPin(2,18,1);//EMC_CLK0
    PINSEL_ConfigPin(2,20,1);//EMC_DYCS0
    PINSEL_ConfigPin(2,24,1);//EMC_OE
    PINSEL_ConfigPin(2,28,1);//EMC_DQM0
    PINSEL_ConfigPin(2,29,1);//EMC_DQM1
    PINSEL_ConfigPin(2,30,1);//EMC_DQM2
    PINSEL_ConfigPin(2,31,1);//EMC_DQM3


    for(i = 0; i < 32; i++)
    {
        PINSEL_ConfigPin(3,i,1);
    };
    for(i = 0; i < 21; i++)
    {
        PINSEL_ConfigPin(4,i,1);
    };
    PINSEL_ConfigPin(4,24,1);
    PINSEL_ConfigPin(4,25,1);
    PINSEL_ConfigPin(4,30,1);
    PINSEL_ConfigPin(4,31,1);
#endif
    
    PINSEL_ConfigPin(4,27,0);	//NAND FLASH的状态信号
    GPIO_SetDir(4,NAND_FLASH_READY,GPIO_DIRECTION_INPUT);

    /*****************************************/
    //      SPI
    /*****************************************/

    PINSEL_ConfigPin ( 0, 12, 2);
    PINSEL_ConfigPin ( 0, 13, 2);
    PINSEL_ConfigPin ( 1, 31, 2);

    PINSEL_ConfigPin ( 0, 14, 0);
    GPIO_SetDir(0,SSP_M25P128_CS,GPIO_DIRECTION_OUTPUT);
    GPIO_SetValue(0,SSP_M25P128_CS);


    PINSEL_ConfigPin ( 1, 20, 5);
    PINSEL_ConfigPin ( 0, 17, 2);
    PINSEL_ConfigPin ( 0, 18, 2);

    PINSEL_ConfigPin ( 2, 22, 0);
    GPIO_SetDir(2,SSP_GT21L16S2W_CS,GPIO_DIRECTION_OUTPUT);
    GPIO_SetValue(2,SSP_GT21L16S2W_CS);


    /*****************************************/
    //          LCD引脚
    /*****************************************/
    PINSEL_ConfigPin(2,9,6);
    PINSEL_ConfigPin(2,6,7);
    PINSEL_ConfigPin(2,7,7);
    PINSEL_ConfigPin(2,8,7);
    PINSEL_ConfigPin(4,29,5);


    PINSEL_ConfigPin(4,28,6);
    PINSEL_ConfigPin(1,21,7);
    PINSEL_ConfigPin(1,22,7);
    PINSEL_ConfigPin(1,23,7);
    PINSEL_ConfigPin(1,24,7);
    PINSEL_ConfigPin(1,25,7);

    PINSEL_ConfigPin(2,13,7);
    PINSEL_ConfigPin(1,26,7);
    PINSEL_ConfigPin(1,27,7);
    PINSEL_ConfigPin(1,28,7);
    PINSEL_ConfigPin(1,29,7);


    PINSEL_ConfigPin(2,0,7);
    PINSEL_ConfigPin(2,2,7);
    PINSEL_ConfigPin(2,3,7);
    PINSEL_ConfigPin(2,4,7);
    PINSEL_ConfigPin(2,5,7);


    PINSEL_ConfigPin(2,11,0);//背光灯开关
    GPIO_SetDir(2,LCD_BACK_LIGHT,GPIO_DIRECTION_OUTPUT);
    //GPIO_SetValue(2,LCD_BACK_LIGHT);
		GPIO_ClearValue(2,LCD_BACK_LIGHT);

    PINSEL_ConfigPin(2,1,0);//背光灯亮度
    GPIO_SetDir(2,LCD_BACK_POWER,GPIO_DIRECTION_OUTPUT);
    GPIO_SetValue(2,LCD_BACK_POWER);
    //GPIO_ClearValue(2,LCD_BACK_POWER);
    /*****************************************/
    //          HS0038B输入引脚作为EINT0
    /*****************************************/
    PINSEL_ConfigPin(0,29,2);

    /*****************************************/
    //          EXTWDI引脚
    /*****************************************/
    PINSEL_ConfigPin(5,0,0);
    GPIO_SetDir(2,EXTWDI,GPIO_DIRECTION_OUTPUT);
    /*****************************************/
    //          ENET引脚
    /*****************************************/
    PINSEL_ConfigPin(1,0,1);
    PINSEL_ConfigPin(1,1,1);
    PINSEL_ConfigPin(1,4,1);
    PINSEL_ConfigPin(1,8,1);
    PINSEL_ConfigPin(1,9,1);
    PINSEL_ConfigPin(1,10,1);
    PINSEL_ConfigPin(1,14,1);
    PINSEL_ConfigPin(1,15,1);
    PINSEL_ConfigPin(1,16,1);
    PINSEL_ConfigPin(1,17,1);
    /*****************************************/
    //          CAN引脚
    /*****************************************/
    PINSEL_ConfigPin(0,0,1);
    PINSEL_ConfigPin(0,1,1);
    PINSEL_ConfigPin(0,4,2);
    PINSEL_ConfigPin(0,5,2);
    /*****************************************/
    //          RS485引脚
    /*****************************************/
    PINSEL_ConfigPin(0,2,1);//U0_TXD
    PINSEL_ConfigPin(0,3,1);//U0_RXD
    PINSEL_ConfigPin(5,4,1);//U0_OE

    PINSEL_ConfigPin(0,15,1);//U1_TXD
    PINSEL_ConfigPin(0,16,1);//U1_RXD

    PINSEL_ConfigPin(4,22,2);//U2_TXD
    PINSEL_ConfigPin(4,23,2);//U2_RXD
    PINSEL_ConfigPin(1,19,6);//U2_OE

    PINSEL_ConfigPin(0,25,3);	//U3_TXD
    PINSEL_ConfigPin(0,26,3);	//U3_RXD
    PINSEL_ConfigPin(1,30,5);	//U3_OE

    PINSEL_ConfigPin(0,22,3);	//U4_TXD
    PINSEL_ConfigPin(5,3,4);	//U4_RXD
    PINSEL_ConfigPin(0,21,3);	//U4_OE



    /*****************************************/
    //          I2C脚
    /*****************************************/
    PINSEL_ConfigPin(0,27,1);
    PINSEL_ConfigPin(0,28,1);
    /*****************************************/
    //          GPIO脚
    /*****************************************/
	PINSEL_ConfigPin(0, 29, 2);	  //P0.29用做Eint0

}



