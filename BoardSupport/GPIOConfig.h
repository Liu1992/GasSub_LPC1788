/************************************************/
//            LPC1788端口定义
//            Gpioconfig.h
//***********************************************/
#ifndef __GPIOCONFIG_H
#define __GPIOCONFIG_H

#define SSP_M25P128_CS     (1<<14) //M25P128的片选信号
#define SSP_GT21L16S2W_CS  (1<<22) //GT21L16S2W的片选信号
#define LCD_BACK_LIGHT  (1<<11) //背光灯开关引脚
#define LCD_BACK_POWER  (1<<1)	//背光灯亮度引脚

#define NAND_FLASH_READY (1<<27)//NAND FLASH的状态引脚 0：忙 1：准备好
#define EXTWDI					(1<<0)

extern void GPIOIni(void);

#endif

