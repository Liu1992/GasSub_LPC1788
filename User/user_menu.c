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
File        : Reversi.c
Purpose     : Simple 'reversi' game.
----------------------------------------------------------------------
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lpc177x_8x_lcd.h"
#include "lpc177x_8x_rtc.h"
#include "LPC177x_8x.h"

#include "GUI.h"
#include "DIALOG.h"
#include "MENU.h"
#include "LISTVIEW.h"
#include "FRAMEWIN_Private.h"
#include "user_menu.h"
#include "DataStruct.h"
#include "Gpioconfig.h"
#include "GlobalVar.h"
#include "ucos_ii.h"

#include "user_SetTime.h"

#include "EquipmentList.h"
#include "Sensor.h"
#include "com_uart.h"

const unsigned int Xpos[5]= {20,105,245,60,60};

const GUI_COLOR Colorsconnect1[] =
{
    0xC0C0C0,0xCBBABA,0xCFA689,0x808080
    ,0xC6B693,0x5E3838,0xCB7979,0x9C5D5D
    ,0xCD8484,0xD9C5C5,0xD09797,0xCC6E6E
    ,0xDFDEDE,0xD6B0B0,0xCC9393,0xC08F8F
    ,0xD48989,0xC66161,0xDBC7C7,0xE1E0E0
    ,0xDDD9D9,0xDDCDCD,0xCCB9B9,0xC09999
    ,0xC09797,0xE0C0C0,0xCC7B7B,0xBF8A8A
    ,0xF3F3F3,0xF2F2F2,0xDBC0C0,0xCCA0A0
    ,0xC09292,0xDBD2D2,0xC5ADAD,0xB58987
};

const GUI_LOGPALETTE Palconnect1 =
{
    36,	/* number of entries */
    0, 	/* No transparency */
    &Colorsconnect1[0]
};

const unsigned char acconnect1[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x03,
    0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x06, 0x05, 0x05, 0x05, 0x05, 0x03,
    0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x05, 0x05, 0x05, 0x05, 0x03,
    0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x05, 0x05, 0x05, 0x05, 0x03,
    0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x07, 0x07, 0x07, 0x03, 0x03,
    0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x08, 0x08, 0x06, 0x03, 0x03,
    0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x09, 0x0A, 0x0B, 0x03, 0x00,
    0x04, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x06, 0x0C, 0x0D, 0x0E, 0x0F, 0x00,
    0x03, 0x03, 0x10, 0x10, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x00,
    0x03, 0x18, 0x19, 0x09, 0x0A, 0x1A, 0x1B, 0x18, 0x18, 0x18, 0x18, 0x03, 0x00,
    0x0A, 0x1C, 0x1D, 0x0C, 0x1E, 0x1F, 0x20, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00,
    0x0A, 0x14, 0x13, 0x21, 0x15, 0x22, 0x23, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x18, 0x18, 0x18, 0x18, 0x18, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const GUI_BITMAP bmconnect1 =
{
    13, /* XSize */
    16, /* YSize */
    13, /* BytesPerLine */
    8, /* BitsPerPixel */
    acconnect1,  /* Pointer to picture data (indices) */
    &Palconnect1  /* Pointer to palette */
};

const GUI_COLOR Colorsledon[] =
{
    0xC0C0C0,0xB2B4C3,0xA1A6C7,0x808080
    ,0x7A89D4,0xB2B5C3,0xBFBFC0,0x2244EB
    ,0x0C04FC,0x7886CD,0xADB0C3,0x2F4FE8
    ,0x2840E7,0x161DF5,0x1012F7,0x1212F7
    ,0x0933C4,0xADB1C3,0x2442E8,0x0E09FB
    ,0x274DE5,0x161FF4,0x072CA9,0x2249E7
    ,0x0F13F6,0x254AE8,0x1114F6,0x1923F3
    ,0x4862D3,0x7A86CA,0x274CE4,0x120DFB
    ,0x7785C9,0xAEB0C2,0x304CE5,0x2026F4
    ,0x5469D4,0xADB0C1,0xA1A7C5,0x5E74D9
    ,0x5D75D5,0x9FA7C4,0xBEBFC0,0x7B88D0
    ,0x4F66DE,0x7B89D3,0xB2B5C2
};

const GUI_LOGPALETTE Palledon =
{
    47,	/* number of entries */
    0, 	/* No transparency */
    &Colorsledon[0]
};

const unsigned char acledon[] =
{
    0x00, 0x00, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04, 0x04, 0x05, 0x00, 0x00,
    0x06, 0x02, 0x03, 0x03, 0x03, 0x07, 0x08, 0x08, 0x03, 0x09, 0x09, 0x06,
    0x0A, 0x03, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x08, 0x10, 0x10, 0x09, 0x11,
    0x03, 0x03, 0x12, 0x13, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x03, 0x09,
    0x03, 0x14, 0x15, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x16, 0x03,
    0x03, 0x17, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x16, 0x16,
    0x03, 0x19, 0x1A, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x16, 0x16,
    0x03, 0x03, 0x1B, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x16, 0x1C,
    0x1D, 0x03, 0x1E, 0x1F, 0x08, 0x08, 0x08, 0x10, 0x10, 0x16, 0x16, 0x20,
    0x21, 0x03, 0x03, 0x22, 0x23, 0x10, 0x10, 0x10, 0x16, 0x16, 0x24, 0x25,
    0x06, 0x26, 0x27, 0x03, 0x03, 0x10, 0x10, 0x16, 0x03, 0x28, 0x29, 0x2A,
    0x00, 0x00, 0x05, 0x2B, 0x2C, 0x03, 0x03, 0x28, 0x2D, 0x2E, 0x00, 0x00
};

const GUI_BITMAP bmledon =
{
    12, /* XSize */
    12, /* YSize */
    12, /* BytesPerLine */
    8, /* BitsPerPixel */
    acledon,  /* Pointer to picture data (indices) */
    &Palledon  /* Pointer to palette */
};
const GUI_COLOR Colorsledoff[] =
{
    0xC0C0C0,0xBEBFC0,0xA1A6C7,0x808080
    ,0x7A89D4,0xB2B5C3,0xBFBFC0,0x400080
    ,0x7886CD,0x02176C,0x2F2F2F,0x7A86CA
    ,0xADB0C1,0x5E74D9,0x4F66DE
};

const GUI_LOGPALETTE Palledoff =
{
    15,	/* number of entries */
    0, 	/* No transparency */
    &Colorsledoff[0]
};

const unsigned char acledoff[] =
{
    0x00, 0x11, 0x23, 0x34, 0x15, 0x00,
    0x61, 0x33, 0x77, 0x77, 0x38, 0x16,
    0x13, 0x77, 0x77, 0x77, 0x99, 0x81,
    0x33, 0x77, 0x77, 0x77, 0x79, 0x91,
    0x37, 0x77, 0x77, 0x77, 0x99, 0xA3,
    0x37, 0x77, 0x77, 0x77, 0x79, 0xAA,
    0x37, 0x77, 0x77, 0x77, 0x99, 0xAA,
    0x33, 0x77, 0x77, 0x77, 0x99, 0xA1,
    0xB3, 0x77, 0x77, 0x79, 0x9A, 0xA1,
    0x13, 0x37, 0x79, 0x99, 0xAA, 0x3C,
    0x61, 0xD3, 0x39, 0x9A, 0x31, 0x11,
    0x00, 0x51, 0xE3, 0x31, 0x11, 0x00
};

const GUI_BITMAP bmledoff =
{
    12, /* XSize */
    12, /* YSize */
    6, /* BytesPerLine */
    4, /* BitsPerPixel */
    acledoff,  /* Pointer to picture data (indices) */
    &Palledoff  /* Pointer to palette */
};

const GUI_COLOR Colorsunconnect[] =
{
    0xC0C0C0,0xCBBABA,0xCFA689,0x808080
    ,0x0000FF,0xC6B693,0x5E3838,0xCB7979
    ,0x9C5D5D,0xCD8484,0xD9C5C5,0xD09797
    ,0xCC6E6E,0xD6B0B0,0xCC9393,0xC08F8F
    ,0xD48989,0xC66161,0xDBC7C7,0xE1E0E0
    ,0xCCB9B9,0xC09999,0xC09797,0xCC7B7B
    ,0xBF8A8A,0xDFDEDE,0xDBC0C0,0xCCA0A0
    ,0xC09292,0xDBD2D2,0xDDCDCD,0xC5ADAD
    ,0xB58987
};

const GUI_LOGPALETTE Palunconnect =
{
    33,	/* number of entries */
    0, 	/* No transparency */
    &Colorsunconnect[0]
};

const unsigned char acunconnect[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
    0x04, 0x04, 0x00, 0x00, 0x00, 0x05, 0x06, 0x06, 0x06, 0x06, 0x06, 0x04, 0x04,
    0x04, 0x04, 0x00, 0x00, 0x00, 0x05, 0x06, 0x06, 0x06, 0x06, 0x06, 0x04, 0x04,
    0x01, 0x04, 0x04, 0x01, 0x02, 0x02, 0x02, 0x07, 0x06, 0x06, 0x04, 0x04, 0x03,
    0x05, 0x06, 0x04, 0x04, 0x06, 0x06, 0x06, 0x07, 0x06, 0x04, 0x04, 0x06, 0x03,
    0x05, 0x06, 0x06, 0x04, 0x04, 0x06, 0x06, 0x07, 0x04, 0x04, 0x06, 0x06, 0x03,
    0x05, 0x06, 0x06, 0x06, 0x04, 0x04, 0x06, 0x04, 0x04, 0x08, 0x08, 0x03, 0x03,
    0x05, 0x06, 0x06, 0x06, 0x06, 0x04, 0x04, 0x04, 0x09, 0x09, 0x07, 0x03, 0x03,
    0x05, 0x06, 0x06, 0x06, 0x06, 0x04, 0x04, 0x04, 0x0A, 0x0B, 0x0C, 0x03, 0x00,
    0x05, 0x08, 0x08, 0x08, 0x04, 0x04, 0x08, 0x04, 0x04, 0x0D, 0x0E, 0x0F, 0x00,
    0x03, 0x03, 0x10, 0x04, 0x04, 0x11, 0x12, 0x13, 0x04, 0x04, 0x14, 0x15, 0x00,
    0x03, 0x16, 0x04, 0x04, 0x0B, 0x17, 0x18, 0x16, 0x16, 0x04, 0x04, 0x03, 0x00,
    0x0B, 0x04, 0x04, 0x19, 0x1A, 0x1B, 0x1C, 0x03, 0x03, 0x03, 0x04, 0x04, 0x00,
    0x04, 0x04, 0x13, 0x1D, 0x1E, 0x1F, 0x20, 0x03, 0x00, 0x00, 0x00, 0x04, 0x04,
    0x04, 0x04, 0x16, 0x16, 0x16, 0x16, 0x03, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04,
    0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const GUI_BITMAP bmunconnect =
{
    13, /* XSize */
    16, /* YSize */
    13, /* BytesPerLine */
    8, /* BitsPerPixel */
    acunconnect,  /* Pointer to picture data (indices) */
    &Palunconnect  /* Pointer to palette */
};
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/* Global data */
static WM_HWIN  _hFrame;				 //主窗口
MENU_Handle _hMenu;						 //主窗口菜单
LISTVIEW_Handle _hListview;
int currentWin = USER_ID_MAIN_WIN;
INT8U substType = 0;

/* My Add */
static RTC_TIME_Type RTCFullTime;		 //当前系统时间
MY_DATE currentTime;					 //当前系统时间

extern WM_HWIN  _hErrorRateRS485;

extern IPConfigStruct ipConfig;								//IP地址配置参数全局结构体

extern Testpoint currentTp;

extern Testpoint* EquipmentList;		//设备表头指针
extern OS_EVENT *EquipmentTaskSem;  //任务初始化先后顺序锁

extern SendStruct uart1SendInfo;
extern OS_EVENT  *UartSendTaskSem;


extern GUI_WIDGET_CREATE_INFO _aDialogCreateSetTime[9];
extern GUI_WIDGET_CREATE_INFO _aDialogCreateSetConnect[9];
extern GUI_WIDGET_CREATE_INFO _aDialogCreateDispConnect[3];
extern GUI_WIDGET_CREATE_INFO _aDialogCreateDispBackpower[3];
extern GUI_WIDGET_CREATE_INFO _aDialogCreateSetAddr[21];
extern GUI_WIDGET_CREATE_INFO _aDialogCreateSetPassword[15];
extern GUI_WIDGET_CREATE_INFO _aDialogCreateSetType[4];
extern GUI_WIDGET_CREATE_INFO _aDialogCreateBrowsTestpoint[3];
extern GUI_WIDGET_CREATE_INFO _aDialogCreateTestRS485[4];
extern GUI_WIDGET_CREATE_INFO _aDialogCreateTestSerial[7];
extern GUI_WIDGET_CREATE_INFO _aDialogCreateTestIO[12];
extern GUI_WIDGET_CREATE_INFO _aDialogCreateCorrect[7];

unsigned int k=0;
unsigned int testNo = 0;

extern void _cbSetTime( WM_MESSAGE *pMsg );
extern void _cbSetConnect( WM_MESSAGE *pMsg );
extern void _cbDispConnect( WM_MESSAGE *pMsg );
extern void _cbDispBackpower( WM_MESSAGE *pMsg );
extern void _cbSetAddr( WM_MESSAGE *pMsg );
extern void _cbSetPassword( WM_MESSAGE *pMsg );
extern void _cbCallback_kb(WM_MESSAGE * pMsg);
extern void _cbSetType( WM_MESSAGE *pMsg );
extern void _cbBrowsTestpoint( WM_MESSAGE *pMsg );
extern void _cbTestRS485( WM_MESSAGE *pMsg );
extern void _cbTestSerial( WM_MESSAGE *pMsg );
extern void _cbTestIO( WM_MESSAGE *pMsg );
extern void _cbCorrect( WM_MESSAGE *pMsg );

/*******************************************************************
*		给菜单中添加选项
*       _AddMenuItem
*/
static void _AddMenuItem(MENU_Handle hMenu, MENU_Handle hSubmenu,  char* pText, U16 Id, U16 Flags)
{
    MENU_ITEM_DATA Item;
    Item.pText    = pText;
    Item.hSubmenu = hSubmenu;
    Item.Flags    = Flags;
    Item.Id       = Id;
    MENU_AddItem(hMenu, &Item);
}

/*******************************************************************
*		创建菜单
*       _CreateMenu
*/
static void _CreateMenu(WM_HWIN hWin)
{
    MENU_Handle hMenuGame, hMenuOptions, hMenuHelp;

    MENU_SetDefaultFont(&GUI_FontHZ16);

	  //_hMenu = MENU_CreateEx(0, 22,  LCD_XSIZE, 20, _hFrame, WM_CF_SHOW, MENU_CF_HORIZONTAL, 0);

    /* Create menu 'Game' */
    hMenuGame = MENU_CreateEx(0, 0, 0, 0, WM_UNATTACHED, 0, MENU_CF_VERTICAL, 0);
    _AddMenuItem(hMenuGame, 0, "测点显示", 			USER_ID_MENU_DISP_TESTPOINT,  0);
		_AddMenuItem(hMenuGame, 0, 0,          0,  		MENU_IF_SEPARATOR);
    _AddMenuItem(hMenuGame, 0, "通讯状态显示",  	USER_ID_MENU_DISP_CONNECT, 0);
   // _AddMenuItem(hMenuGame, 0, "后备电源显示",  	USER_ID_MENU_DISP_BACKPOWER, 0);
    
	/* Create menu 'Options' */
    hMenuOptions = MENU_CreateEx(0, 0, 0, 0, WM_UNATTACHED, 0, MENU_CF_VERTICAL, 0);
    _AddMenuItem(hMenuOptions, 0, "IP地址设置", 		USER_ID_MENU_SET_ADDR, 0);
    _AddMenuItem(hMenuOptions, 0, "通讯设置", 		USER_ID_MENU_SET_CONNECT, 0);
    _AddMenuItem(hMenuOptions, 0, 0,          0,            MENU_IF_SEPARATOR);
    _AddMenuItem(hMenuOptions, 0, "时间设置", 		USER_ID_MENU_SET_TIMER, 0);
    _AddMenuItem(hMenuOptions, 0, "闭锁设置", 	USER_ID_MENU_SET_LOCK, 0);
    //_AddMenuItem(hMenuOptions, 0, 0,          0,            MENU_IF_SEPARATOR);
		//_AddMenuItem(hMenuOptions, 0, "分站类型设置", USER_ID_MENU_SET_TYPE, 0);
    //_AddMenuItem(hMenuOptions, 0, "密码设置", 		USER_ID_MENU_SET_PASSWORD, 0);

    /* Create menu 'Help' */
    hMenuHelp = MENU_CreateEx(0, 0, 0, 0,  WM_UNATTACHED, 0, MENU_CF_VERTICAL, 0);
		/*
    _AddMenuItem(hMenuHelp, 0, "RS485测试", 			USER_ID_MENU_TEST_RS485, 0);
		_AddMenuItem(hMenuHelp, 0, "输入输出口测试", 	USER_ID_MENU_TEST_IO, 0);
		_AddMenuItem(hMenuHelp, 0, "累计量测试", 			USER_ID_MENU_TEST_TOTAL, 0);
		_AddMenuItem(hMenuHelp, 0, "串口测试", 				USER_ID_MENU_TEST_SERIAL, 0);
    _AddMenuItem(hMenuHelp, 0, 0,          0,              MENU_IF_SEPARATOR);
    _AddMenuItem(hMenuHelp, 0, "关于...",  USER_ID_MENU_ABOUT, 0);
		*/
		_AddMenuItem(hMenuHelp, 0, "本机校准", 			USER_ID_MENU_CORRECT, 0);
		//_AddMenuItem(hMenuHelp, 0, "RS-485误码率测试", 			USER_ID_MENU_ERROR_RATE_RS485, 0);
    
		/* Create main menu */
    _hMenu = MENU_CreateEx(0, 0, 0, 0, WM_UNATTACHED, 0, MENU_CF_HORIZONTAL, 0);

    _AddMenuItem(_hMenu, hMenuGame,    "显示",    0, 0);
    _AddMenuItem(_hMenu, hMenuOptions, "设置",    0, 0);
    _AddMenuItem(_hMenu, hMenuHelp,    "本机测试",    0, 0);
    
	/* Attach menu to framewin */
    FRAMEWIN_AddMenu(hWin, _hMenu);
    
	WM_SetFocus(_hMenu);
	currentWin = USER_ID_MAIN_WIN;
}

/*******************************************************************
*		在菜单中选择相应的条目产生的结果
*       _OnMenu
*/
static void _OnMenu(WM_MESSAGE* pMsg)
{
    MENU_MSG_DATA* pData = (MENU_MSG_DATA*)pMsg->Data.p;
    //MENU_Handle    hMenu = pMsg->hWinSrc;
    switch (pData->MsgType)
    {
    case MENU_ON_ITEMSELECT:
        switch (pData->ItemId)
        {
        case USER_ID_MENU_DISP_TESTPOINT:

						WM_SetFocus( _hListview );
						currentWin = USER_ID_MENU_DISP_TESTPOINT;
            break;
/*
        case USER_ID_MENU_DISP_BACKPOWER:
						GUI_CreateDialogBox( _aDialogCreateDispBackpower, GUI_COUNTOF(_aDialogCreateDispBackpower), &_cbDispBackpower, NULL, 80, 0 );   //非阻塞方式创建对话框			
            currentWin = USER_ID_MENU_DISP_BACKPOWER;
						break;
*/
        case USER_ID_MENU_DISP_CONNECT:
						GUI_CreateDialogBox( _aDialogCreateDispConnect, GUI_COUNTOF(_aDialogCreateDispConnect), &_cbDispConnect, NULL, 80, 0 );   //非阻塞方式创建对话框
						currentWin = USER_ID_MENU_DISP_CONNECT;
            break;
				
        case USER_ID_MENU_SET_ADDR:
						//GUI_CreateDialogBox( _aDialogCreateSetTime, GUI_COUNTOF(_aDialogCreateSetTime), &_cbSetTime, _hFrame, 80, 0 );   //非阻塞方式创建对话框
						GUI_CreateDialogBox( _aDialogCreateSetAddr, GUI_COUNTOF(_aDialogCreateSetAddr), &_cbSetAddr, NULL, 80, 0 );   //非阻塞方式创建对话框			
            currentWin = USER_ID_MENU_SET_ADDR;
						break;
				
        case USER_ID_MENU_SET_CONNECT:
						GUI_CreateDialogBox( _aDialogCreateSetConnect, GUI_COUNTOF(_aDialogCreateSetConnect), &_cbSetConnect, NULL, 80, 0 );   //非阻塞方式创建对话框				
            currentWin = USER_ID_MENU_SET_CONNECT;
						break;

				case USER_ID_MENU_SET_TIMER:			
		
						currentTime.year = RTCFullTime.YEAR;
						currentTime.mon  = RTCFullTime.MONTH;
						currentTime.day  = RTCFullTime.DOM;
						currentTime.hour = RTCFullTime.HOUR;
						currentTime.min  = RTCFullTime.MIN;
						currentTime.sec  = RTCFullTime.SEC;
				
						GUI_CreateDialogBox( _aDialogCreateSetTime, GUI_COUNTOF(_aDialogCreateSetTime), &_cbSetTime, NULL, 80, 0 );   //非阻塞方式创建对话框
						//GUI_ExecDialogBox( _aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, _hFrame, 100, 0 );	 //阻塞方式创建对话框
						currentWin = USER_ID_MENU_SET_TIMER;
						break;
/*
				case USER_ID_MENU_SET_TYPE:		
						GUI_CreateDialogBox( _aDialogCreateSetType, GUI_COUNTOF(_aDialogCreateSetType), &_cbSetType, NULL, 80, 0 );   //非阻塞方式创建对话框
						currentWin = USER_ID_MENU_SET_TYPE;
						break;
*/
				case USER_ID_MENU_SET_LOCK:		
						currentWin = USER_ID_MENU_SET_LOCK;
						break;
/*
				case USER_ID_MENU_SET_PASSWORD:		
						GUI_CreateDialogBox( _aDialogCreateSetPassword, GUI_COUNTOF(_aDialogCreateSetPassword), &_cbSetPassword, NULL, 80, 0 );
						currentWin = USER_ID_MENU_SET_PASSWORD;
						break;

				case USER_ID_MENU_UNIT_TEST:		
						GUI_CreateDialogBox( _aDialogCreateBrowsTestpoint, GUI_COUNTOF(_aDialogCreateBrowsTestpoint), &_cbBrowsTestpoint, NULL, 80, 0 );
						break;
*/
				/*
				case USER_ID_MENU_TEST_RS485:
						GUI_CreateDialogBox( _aDialogCreateTestRS485, GUI_COUNTOF(_aDialogCreateTestRS485), &_cbTestRS485, NULL, 80, 0 );
						currentWin = USER_ID_MENU_TEST_RS485;
						break;
				
				case USER_ID_MENU_TEST_IO:
						GUI_CreateDialogBox( _aDialogCreateTestIO, GUI_COUNTOF(_aDialogCreateTestIO), &_cbTestIO, NULL, 15, 0 );
						currentWin = USER_ID_MENU_TEST_IO;
						break;
				
				case USER_ID_MENU_TEST_TOTAL:
						currentWin = USER_ID_MENU_TEST_TOTAL;
						break;
				
				case USER_ID_MENU_TEST_SERIAL:
						GUI_CreateDialogBox( _aDialogCreateTestSerial, GUI_COUNTOF(_aDialogCreateTestSerial), &_cbTestSerial, NULL, 80, 0 );
						currentWin = USER_ID_MENU_TEST_SERIAL;
						break;
				
				case USER_ID_MENU_ABOUT:
						currentWin = USER_ID_MENU_ABOUT;
						break;
						*/
						
				case USER_ID_MENU_CORRECT:
						GUI_CreateDialogBox( _aDialogCreateCorrect, GUI_COUNTOF(_aDialogCreateCorrect), &_cbCorrect, NULL, 80, 0 );
						currentWin = USER_ID_MENU_CORRECT;
						break;
				case USER_ID_MENU_ERROR_RATE_RS485:
					
						WM_ShowWindow(_hErrorRateRS485);
						WM_SetFocus(_hErrorRateRS485);
						currentWin = USER_ID_MENU_ERROR_RATE_RS485;
					
						break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}
/*******************************************************************
*		显示主要信息，时间、IP等
*       MainDisplayStatus
*/
static void MainDisplayStatus(GUI_RECT  r)
{
		INT8U currentOutput;
    unsigned int i;
    char s[30];
    //x=GUI_GetFontDistY()+4;
	
    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_FillRect(r.x0,r.y0,r.x1,r.y1);
		
		GUI_SetFont(&GUI_FontHZ16);
		GUI_SetTextMode(LCD_DRAWMODE_TRANS);
    GUI_SetColor(GUI_DARKBLUE);

		RTC_GetFullTime (LPC_RTC, &RTCFullTime);
		sprintf(s,"%0.4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d",RTCFullTime.YEAR,RTCFullTime.MONTH,RTCFullTime.DOM,RTCFullTime.HOUR,RTCFullTime.MIN,RTCFullTime.SEC);
		//sprintf(s,"%0.4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d",System_Date.year,System_Date.mon,System_Date.day,System_Date.hour,System_Date.min,System_Date.sec);
    GUI_DispStringAt(s,r.x1-180, r.y1-18);

		sprintf(s, "IP=%d.%d.%d.%d", ipConfig.ip[0], ipConfig.ip[1], ipConfig.ip[2], ipConfig.ip[3] );
    GUI_DispStringAt( s ,r.x0+150, r.y1-18);
    GUI_DrawBitmap(&bmunconnect, r.x1-20, r.y1-18);
    
		currentOutput = uart1SendInfo.outPut;
		for(i=0; i<4; i++)
    {
				if ( ( currentOutput >> i ) & 1 )
						GUI_DrawBitmap(&bmledon, i*16+2, r.y1-16);
				else 
						GUI_DrawBitmap(&bmledoff, i*16+2, r.y1-16);
    };
    for(i=0; i<4; i++)
    {
				if ( ( currentOutput >> ( i + 4 ) ) & 1 )
						GUI_DrawBitmap(&bmledon, i*16+80, r.y1-16);
				else 
						GUI_DrawBitmap(&bmledoff, i*16+80, r.y1-16);
    };

}

/*******************************************************************
*		绘制表格主体
*       _OnPaint
*/

static void _OnPaint(WM_HWIN hWin)
{	
		INT8U err;
		INT8U ret = 0;
		GUI_RECT  r;
		unsigned int i = 0;
		char str[16];
		Testpoint *pList = NULL;
		TestpointShowInfo tpShow;
	
		if ( EquipmentList != NULL )
				pList = EquipmentList->next;
	
		if ( substType == SUBST_TYPE_A )
				FRAMEWIN_SetText(_hFrame, "KJ719-F(A型)监控分站");	  //设置标题文本
		else 
				FRAMEWIN_SetText(_hFrame, "KJ719-F(B型)监控分站");
		
		if ( k % 30 == 0 )
		{
			if ( currentWin == 0 )
				LISTVIEW_IncSel( _hListview );
			++testNo;
			if ( currentWin == 0 && testNo >= 15 )
			{
				for ( i = 0; i < 15; ++i )
					LISTVIEW_DecSel( _hListview );
				testNo = 0;
			}
		}
		
		i = 0;
		//pList = pList->next;	
		while ( pList != NULL )
		{
			sprintf( str, "%d", i + 1 );
			
			OSSemPend( pList->EquipmentSem , 0, &err );
			tpShow = pList->getShowInfo( pList );
			if ( pList->ifAlarm( pList ) )						 //报警
			{
					LISTVIEW_SetItemTextColor( _hListview, 0, i, LISTVIEW_CI_UNSEL, GUI_RED );
					LISTVIEW_SetItemTextColor( _hListview, 1, i, LISTVIEW_CI_UNSEL, GUI_RED );
					LISTVIEW_SetItemTextColor( _hListview, 2, i, LISTVIEW_CI_UNSEL, GUI_RED );
					LISTVIEW_SetItemTextColor( _hListview, 3, i, LISTVIEW_CI_UNSEL, GUI_RED );
					LISTVIEW_SetItemTextColor( _hListview, 4, i, LISTVIEW_CI_UNSEL, GUI_RED );
			}
			if ( pList->ifLiftAlarm( pList ) )				//解除报警
			{
					LISTVIEW_SetItemTextColor( _hListview, 0, i, LISTVIEW_CI_UNSEL, GUI_WHITE );
					LISTVIEW_SetItemTextColor( _hListview, 1, i, LISTVIEW_CI_UNSEL, GUI_WHITE );
					LISTVIEW_SetItemTextColor( _hListview, 2, i, LISTVIEW_CI_UNSEL, GUI_WHITE );
					LISTVIEW_SetItemTextColor( _hListview, 3, i, LISTVIEW_CI_UNSEL, GUI_WHITE );
					LISTVIEW_SetItemTextColor( _hListview, 4, i, LISTVIEW_CI_UNSEL, GUI_WHITE );
			}
			if ( pList->ifOutput(pList) )							//断电
			{
					ret |= pList->controlOutput( pList );
					printf("In function menu paint1: ret = %u\n", ret );
			}
			if ( pList->ifRecover(pList) )						//复电
			{
					ret |= pList->controlRecover( pList );
					printf("In function menu paint2: ret = %u\n", ret );
			}
			
			OSSemPost( pList->EquipmentSem );
			
			LISTVIEW_SetItemText( _hListview, 0, i, str );
			LISTVIEW_SetItemText( _hListview, 1, i, tpShow.sensorType );
			LISTVIEW_SetItemText( _hListview, 2, i, tpShow.location );
			LISTVIEW_SetItemText( _hListview, 3, i, tpShow.collectData );
			LISTVIEW_SetItemText( _hListview, 4, i, tpShow.portType );
					
			++i;
			if ( i >= 15 ) i = 0;
			pList = pList->next;		
		}
		
		printf("In function menu paint before send: ret = %u\n", ret );
		OSSemPend( UartSendTaskSem, 1000, &err );
		uart1SendInfo.outPut = ~ret;
		OSSemPost( UartSendTaskSem );
	
    WM_GetClientRectEx(hWin, &r);
		MainDisplayStatus(r);							 //显示表格外的关键信息 
}

void InitListview()
{
	INT32U i = 0;
	//char s[4];
	char* _aTable[5];						//测点显示的表格内数据
	TestpointShowInfo tpShow;
	Testpoint *pList = NULL;

	LISTVIEW_SetDefaultGridColor( GUI_GREEN );
	_hListview = LISTVIEW_CreateEx( 0, 40, LCD_XSIZE, LCD_YSIZE-64, _hFrame, WM_CF_SHOW, 0, 0 );
	
	LISTVIEW_AddColumn( _hListview, EDITXSIZE2-4, "No.", GUI_TA_LEFT );
	LISTVIEW_AddColumn( _hListview, EDITXSIZE7, "类型", GUI_TA_HCENTER );
	LISTVIEW_AddColumn( _hListview, LCD_XSIZE-EDITXSIZE9-EDITXSIZE7+4, "安装地点", GUI_TA_HCENTER );
	LISTVIEW_AddColumn( _hListview, EDITXSIZE4, "状态", GUI_TA_HCENTER );
	LISTVIEW_AddColumn( _hListview, EDITXSIZE3, "端口", GUI_TA_HCENTER );

	LISTVIEW_SetBkColor( _hListview, LISTVIEW_CI_UNSEL, GUI_BLACK );	
	LISTVIEW_SetGridVis( _hListview, 1 );
	LISTVIEW_SetTextColor( _hListview, LISTVIEW_CI_UNSEL, GUI_WHITE );
	LISTVIEW_SetRowHeight( _hListview, 23 );
	
	if ( EquipmentList != NULL )
				pList = EquipmentList->next;
	
	_aTable[0] = tpShow.testpointNo;
	_aTable[1] = tpShow.sensorType;
	_aTable[2] = tpShow.location;
	_aTable[3] = tpShow.collectData;
	_aTable[4] = tpShow.portType;

	while ( pList != NULL )
	{
		tpShow = pList->getShowInfo( pList );
		LISTVIEW_AddRow( _hListview, _aTable );
		++i;
		if ( i >= 15 ) i = 0;
		pList = pList->next;		
	}	
	
	return;
}

/*******************************************************************
*		回调函数
*       _cbReversiWin
*/
static void _cbReversiWin(WM_MESSAGE* pMsg)
{
		INT8U err;
		INT16U i = 0;
		INT16U curSel;
		WM_HWIN hWin = pMsg->hWin;
		unsigned KeyVal = 0;
		Testpoint *pList = NULL;
	
		if ( EquipmentList != NULL )
		{
				pList = EquipmentList->next;
		}

		KeyVal = LISTVIEW_GetEnterInfo();
    	
		if ( currentWin == USER_ID_MENU_DISP_TESTPOINT && KeyVal ){
				i = 0;
				switch ( KeyVal ){
						case GUI_KEY_ENTER:
								curSel = LISTVIEW_GetSel( _hListview );
								while ( pList != NULL ) 
								{
										OSSemPend( pList->EquipmentSem , 0, &err );
										currentTp = *pList;			
										OSSemPost( pList->EquipmentSem );
										if ( i == curSel ) break;
										++i;
										pList = pList->next;
								}
								GUI_CreateDialogBox( _aDialogCreateBrowsTestpoint, GUI_COUNTOF(_aDialogCreateBrowsTestpoint), &_cbBrowsTestpoint, NULL, 80, 0 );
								break;
						
						case GUI_KEY_TAB:
								WM_SetFocus( _hMenu );
								currentWin = USER_ID_MAIN_WIN;
								break;						
				}
		}

    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        _OnPaint(hWin);							//绘制客户区主体
        break;

    case WM_MENU:
        _OnMenu(pMsg);							//
        break;
				
    default:
        WM_DefaultProc(pMsg);
    }
}

/*********************************************************************
*		创建主体窗口
*       _StartReversi
*/
static void _StartReversi(void)
{

	//创建框架窗口
    _hFrame = FRAMEWIN_CreateEx(0, 0, LCD_XSIZE,LCD_YSIZE, WM_HBKWIN, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, NULL, &_cbReversiWin);

    FRAMEWIN_SetClientColor(_hFrame, GUI_INVALID_COLOR);	  //设置客户区背景
    FRAMEWIN_SetFont       (_hFrame, &GUI_FontHZ16);		  //为标题文本选择字体
    FRAMEWIN_SetTextAlign  (_hFrame, GUI_TA_HCENTER);		  //设置标题文本对齐方式
		//FRAMEWIN_SetBarColor   (_hFrame, 1, GUI_BLUE );
		if ( substType == SUBST_TYPE_A )
			FRAMEWIN_SetText(_hFrame, "KJ218-F(A型)通用监控分站");	  //设置标题文本
		else 
			FRAMEWIN_SetText(_hFrame, "KJ218-F(B型)通用监控分站");	  //设置标题文本
    
		_CreateMenu(_hFrame);									  //创建菜单
		InitListview();
}

/*********************************************************************
*
*       MainTask
*/
unsigned char ss,ll=0;

void Menu_Task(void)
{
	INT8U err;
	
	WM_EnableMemdev(WM_HBKWIN);
  WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
    
	OSSemPend(EquipmentTaskSem, 1000, &err);	//等待设备链表初始化完毕

	while (1)
	{		
				_StartReversi();
		
        while (_hFrame)
        {
            GUI_Delay(1000);
            k++;
						//RTC_GetFullTime (LPC_RTC, &RTCFullTime);
						//System_Date.sec=(System_Date.sec+1)%60;
            WM_InvalidateWindow(WM_GetClientWindow(_hFrame));	//制定一个窗口无效 
        }
	}
}
