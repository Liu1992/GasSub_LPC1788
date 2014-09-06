/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_SetTime.c
** Last modified Date:      2014.04.25
** Last Version:            1.0
** Description:             Header file of the Set Connect Dialog 设置通讯对话框文件
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              GuoBingrong 郭冰容
** Created date:            2014.05.01
** Version:                 1.0
** Descriptions:            The original version 初始版本

*********************************************************************************************************/
#include "lpc177x_8x_rtc.h"

#include "GUI.h"
#include "DIALOG.h"
#include "user_SetTime.h"
#include "DataStruct.h"
#include "MENU.h"
#include "user_menu.h"
#include "user_SaveOption.h"

extern MY_DATE currentTime;
extern MENU_Handle _hMenu;
extern int currentWin;
extern EEPROMDataStruct optionSaveStruct;

/*******************************************
	      通讯设置窗口所有控件

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateSetConnect[9] = 
{	
	{ FRAMEWIN_CreateIndirect, "通讯设置", 0, 10, 10, 300, 220, 0, 0 },
	
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN0,  ADDSTEP7, EDITXSIZE,          DROPDOWNXSIZE, DROPDOWNYSIZE3-6, 0, 50 },
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN1,  ADDSTEP7, EDITXSIZE+ADDSTEP,  DROPDOWNXSIZE, DROPDOWNYSIZE3-6, 0, 50 },
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN2,  ADDSTEP7, EDITXSIZE+ADDSTEP2, DROPDOWNXSIZE, DROPDOWNYSIZE4-6, 0, 50 },
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN3,  ADDSTEP7, EDITXSIZE+ADDSTEP3, DROPDOWNXSIZE, DROPDOWNYSIZE4-6, 0, 50 },	
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN4,  ADDSTEP7, EDITXSIZE+ADDSTEP4, DROPDOWNXSIZE, DROPDOWNYSIZE4-6, 0, 50 },
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN5,  ADDSTEP7, EDITXSIZE+ADDSTEP5, DROPDOWNXSIZE, DROPDOWNYSIZE2, 0, 50 },

	{ BUTTON_CreateIndirect, "确定", GUI_ID_OK,     110, 170, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "取消", GUI_ID_CANCEL, 190, 170, 60, 20, 0, 0 },
};

/*******************************************
	      绘制通讯设置窗口

********************************************/
void _OnPaintSetConnect( WM_HWIN hWin )
{
	GUI_RECT  r;
	//unsigned char s[2];

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);

	GUI_DispStringAt( "CAN1速率：",        r.x0+ADDSTEP,     r.y0+EDITXSIZE );
	GUI_DispStringAt( "CAN2速率：",        r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP+1 );
	GUI_DispStringAt( "RS485-1波特率：",   r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP2+2 );
	GUI_DispStringAt( "RS485-2波特率：",   r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP3+3 );
	GUI_DispStringAt( "RS485-3波特率：",   r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP4+4 );
	GUI_DispStringAt( "RS485-4波特率：",   r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP5+5 );

	return;
}

/*******************************************
	      通讯设置窗口初始化
       下拉菜单内的值为系统当前设置值
********************************************/

void _InitSetConnectDialog(WM_HWIN hWin)
{
	WM_HWIN hDropdown;

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN0 );
	DROPDOWN_AddString( hDropdown, "2.5Kbps" );
	DROPDOWN_AddString( hDropdown, "5.0Kbps" );
	DROPDOWN_AddString( hDropdown, "10Kbps" );
	DROPDOWN_SetSel( hDropdown, optionSaveStruct.uartConfig[0] );
	//DROPDOWN_SetBkColor( hDropdown, DROPDOWN_CI_UNSEL, GUI_WHITE );

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN1 );
	DROPDOWN_AddString( hDropdown, "2.5Kbps" );
	DROPDOWN_AddString( hDropdown, "5.0Kbps" );
	DROPDOWN_AddString( hDropdown, "10Kbps" );
	DROPDOWN_SetSel( hDropdown, optionSaveStruct.uartConfig[1] );
	//DROPDOWN_SetBkColor( hDropdown, DROPDOWN_CI_UNSEL, GUI_WHITE );

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN2 );
	DROPDOWN_AddString( hDropdown, "1200bps" );
	DROPDOWN_AddString( hDropdown, "2400bps" );
	DROPDOWN_AddString( hDropdown, "4800bps" );
	DROPDOWN_AddString( hDropdown, "9600bps" );
	DROPDOWN_SetSel( hDropdown, optionSaveStruct.uartConfig[2] );
	DROPDOWN_SetAutoScroll( hDropdown, 1 );
	//DROPDOWN_SetBkColor( hDropdown, DROPDOWN_CI_UNSEL, GUI_WHITE );

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN3 );
	DROPDOWN_AddString( hDropdown, "1200bps" );
	DROPDOWN_AddString( hDropdown, "2400bps" );
	DROPDOWN_AddString( hDropdown, "4800bps" );
	DROPDOWN_AddString( hDropdown, "9600bps" );
	DROPDOWN_SetSel( hDropdown, optionSaveStruct.uartConfig[3] );
	DROPDOWN_SetAutoScroll( hDropdown, 1 );
	//DROPDOWN_SetBkColor( hDropdown, DROPDOWN_CI_UNSEL, GUI_WHITE );

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN4 );
	DROPDOWN_AddString( hDropdown, "1200bps" );
	DROPDOWN_AddString( hDropdown, "2400bps" );
	DROPDOWN_AddString( hDropdown, "4800bps" );
	DROPDOWN_AddString( hDropdown, "9600bps" );
	DROPDOWN_SetSel( hDropdown, optionSaveStruct.uartConfig[4] );
	DROPDOWN_SetAutoScroll( hDropdown, 1 );
	//DROPDOWN_SetBkColor( hDropdown, DROPDOWN_CI_UNSEL, GUI_WHITE );

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN5 );
	DROPDOWN_AddString( hDropdown, "1200bps" );
	DROPDOWN_AddString( hDropdown, "2400bps" );
	DROPDOWN_AddString( hDropdown, "4800bps" );
	DROPDOWN_AddString( hDropdown, "9600bps" );
	DROPDOWN_SetSel( hDropdown, optionSaveStruct.uartConfig[5] );
	DROPDOWN_SetAutoScroll( hDropdown, 1 );
	//DROPDOWN_SetBkColor( hDropdown, DROPDOWN_CI_UNSEL, GUI_WHITE );

	return;
}

/*******************************************
	      通讯设置窗口初始化
       下拉菜单内的值为系统当前设置值
********************************************/

void _GetConnectDialog(WM_HWIN hWin)
{
	WM_HWIN hDropdown;

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN0 );
	optionSaveStruct.uartConfig[0]=DROPDOWN_GetSel(hDropdown);

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN1 );
	optionSaveStruct.uartConfig[1]=DROPDOWN_GetSel(hDropdown);

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN2 );
	optionSaveStruct.uartConfig[2]=DROPDOWN_GetSel(hDropdown);

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN3 );
	optionSaveStruct.uartConfig[3]=DROPDOWN_GetSel(hDropdown);

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN4 );
	optionSaveStruct.uartConfig[4]=DROPDOWN_GetSel(hDropdown);

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN5 );
	optionSaveStruct.uartConfig[5]=DROPDOWN_GetSel(hDropdown);

	return;
}

/*******************************************
	      设置时间窗口的回调函数

********************************************/


void _cbSetConnect( WM_MESSAGE *pMsg )
{
	int Id, NCode;
	WM_HWIN hWin = pMsg->hWin;	

	switch( pMsg->MsgId )
	{
		case WM_INIT_DIALOG:
			_InitSetConnectDialog(hWin);
			break;
					
		case WM_PAINT:
			_OnPaintSetConnect(hWin);
			break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch( NCode )
			{
				case WM_NOTIFICATION_RELEASED:
					if ( Id == GUI_ID_OK )
					{
						_GetConnectDialog(hWin);
						
						GUI_EndDialog( hWin, 0 );
						if(EEPROM_WriteStruct(optionSaveStruct)==FALSE)
						{
							printf("no!");
						}
						WM_SetFocus(_hMenu);
						currentWin = USER_ID_MAIN_WIN;
					}
					if ( Id == GUI_ID_CANCEL )
					{
						GUI_EndDialog( hWin, 0 );
						WM_SetFocus(_hMenu);
						currentWin = USER_ID_MAIN_WIN;
					}
					break;

				
				
				default: break;
			}
			break;

		default: 
		WM_DefaultProc(pMsg);
	}
}

