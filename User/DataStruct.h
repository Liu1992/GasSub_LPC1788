#ifndef  DATASTRUCT
#define  DATASTRUCT

#include "os_cpu.h"

#define   ADDSTEP     	22
#define   ADDSTEP2    	44
#define   ADDSTEP3    	66
#define   ADDSTEP4    	88
#define   ADDSTEP5    	110
#define   ADDSTEP6    	132
#define   ADDSTEP7    	154
#define   ADDSTEP8    	176
#define   ADDSTEP9    	198
#define   ADDSTEP10    	220

#define   OTHERADDSTEP     	  30
#define   OTHERADDSTEP2     	60
#define   OTHERADDSTEP3     	90
#define   OTHERADDSTEP4     	120
#define   OTHERADDSTEP5     	150
#define   OTHERADDSTEP6     	180
#define   OTHERADDSTEP7     	210
#define   OTHERADDSTEP8     	240

#define   EDITXSIZE   	15
#define   EDITXSIZE2  	30
#define   EDITXSIZE3  	45
#define   EDITXSIZE4  	60
#define   EDITXSIZE5  	75
#define   EDITXSIZE6  	90
#define   EDITXSIZE7  	105
#define   EDITXSIZE8  	120
#define   EDITXSIZE9  	135
#define   EDITXSIZE10  	150

#define   EDITYSIZE  	20
#define   EDITYSIZE2    40
#define   EDITYSIZE3    60
#define   EDITYSIZE4    80
#define   EDITYSIZE5    100
#define   EDITYSIZE6    120
#define   EDITYSIZE7    140
#define   EDITYSIZE8    160
#define   EDITYSIZE9    180
#define   EDITYSIZE10    200

#define DROPDOWNXSIZE   100
#define DROPDOWNYSIZE   20
#define DROPDOWNYSIZE2   40
#define DROPDOWNYSIZE3   60
#define DROPDOWNYSIZE4   80
#define DROPDOWNYSIZE5   100

#define SUBST_TYPE_A     0
#define SUBST_TYPE_B     1

#define BUFFERSIZE  64

typedef struct {
 		unsigned int  year;
		unsigned int  mon;
		unsigned int  day;
		unsigned int  hour;
		unsigned int  min;
		unsigned int  sec;	
}MY_DATE;

typedef struct {
	 unsigned int No;
	 char type[14];
	 char address[32];
	 char port[8];
	 float  state;
}MY_TESTPOINT;


typedef struct
{
	INT8U ip[4];
	INT8U sub[4];
	INT8U gw[4];
	
	INT8U mac[6];
}IPConfigStruct;		/*		网络配置结构体	*/

typedef struct
{
	IPConfigStruct ipConfig;
	INT8U 		uartConfig[6];
	INT8U			password[6];
}EEPROMDataStruct;

typedef struct
{
		INT32S slope[16];
		INT32S intercept[16];
}Correct_Input;

typedef enum {											/*	US485串口号			*/	
	RS485_1 	= 0,								/*	RS485_UART0		*/
	RS485_2,	 							/*	RS485_UART2 	*/
	RS485_3, 								/*	RS485_UART3 	*/
	RS485_4, 								/*	RS485_UART4		*/
} RS485_NUM;

#endif 
