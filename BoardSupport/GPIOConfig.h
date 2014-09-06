/************************************************/
//            LPC1788�˿ڶ���
//            Gpioconfig.h
//***********************************************/
#ifndef __GPIOCONFIG_H
#define __GPIOCONFIG_H

#define SSP_M25P128_CS     (1<<14) //M25P128��Ƭѡ�ź�
#define SSP_GT21L16S2W_CS  (1<<22) //GT21L16S2W��Ƭѡ�ź�
#define LCD_BACK_LIGHT  (1<<11) //����ƿ�������
#define LCD_BACK_POWER  (1<<1)	//�������������

#define NAND_FLASH_READY (1<<27)//NAND FLASH��״̬���� 0��æ 1��׼����
#define EXTWDI					(1<<0)

extern void GPIOIni(void);

#endif

