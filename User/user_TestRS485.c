/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_TestRS485.c
** Last modified Date:      2014.07.25
** Last Version:            1.0
** Description:             Header file of the Set Connect Dialog RS485测试
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              GuoBingrong 郭冰容
** Created date:            2014.07.25
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
#include "os_cpu.h"
#include "com_uart.h"
#include "stdio.h"

extern MENU_Handle _hMenu;
extern int currentWin;
INT32U Uart1SendCnt = 0;
INT32U Uart2SendCnt = 0;
INT32U Uart3SendCnt = 0;
INT32U Uart4SendCnt = 0;

/*******************************************
	      RS485测试窗口所有控件

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateTestRS485[4] = 
{	
	{ FRAMEWIN_CreateIndirect, "RS485测试", 0, 10, 10, 300, 220, 0, 0 },
	
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN0,  ADDSTEP7, EDITXSIZE,          DROPDOWNXSIZE, DROPDOWNYSIZE4-10, 0, 50 },

	{ BUTTON_CreateIndirect, "开始", GUI_ID_OK,     110, 170, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_CANCEL, 190, 170, 60, 20, 0, 0 },
};

/*******************************************
	      RS485测试界面绘制

********************************************/
void _OnPaintTestRS485( WM_HWIN hWin )
{
	WM_HWIN hDropdown;
	GUI_RECT  r;
	char s[32];
	int RS485_Selc;

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);

	GUI_DispStringAt( "RS485发送：",        r.x0+ADDSTEP,     r.y0+EDITXSIZE );
	
	GUI_DispStringAt( "信息框",        r.x0+ADDSTEP5+11,     r.y0+EDITXSIZE+ADDSTEP2+10 );
	GUI_DispStringAt( "—————————————————",        r.x0+10,     r.y0+EDITXSIZE+ADDSTEP3+12 );
	GUI_DispStringAt( "发送次数：",        r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP4+14 );
	
	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN0 );
	RS485_Selc = DROPDOWN_GetSel(hDropdown);
	
	switch( RS485_Selc )
	{
			case 0:
					sprintf(s, "%d", Uart1SendCnt );					
					break;
			case 1:
					sprintf(s, "%d", Uart2SendCnt );					
					break;
			case 2:
					sprintf(s, "%d", Uart3SendCnt );				
					break;
			case 3: 
					sprintf(s, "%d", Uart4SendCnt );					
					break;
			default: break;
	}
	
	GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE+ADDSTEP4+15 );

	return;
}

/*******************************************
			
								Test RS485

********************************************/

void _InitTestRS485Dialog(WM_HWIN hWin)
{
	WM_HWIN hDropdown;

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN0 );
	DROPDOWN_AddString( hDropdown, "RS485-1" );
	DROPDOWN_AddString( hDropdown, "RS485-2" );
	DROPDOWN_AddString( hDropdown, "RS485-3" );
	DROPDOWN_AddString( hDropdown, "RS485-4" );
	//DROPDOWN_SetBkColor( hDropdown, DROPDOWN_CI_UNSEL, GUI_WHITE );
	DROPDOWN_SetAutoScroll( hDropdown, 1 );
	
	Uart1SendCnt = 0;
	Uart2SendCnt = 0;
	Uart3SendCnt = 0;
	Uart4SendCnt = 0;

	return;
}

/*******************************************
	      设置时间窗口的回调函数

********************************************/


void _cbTestRS485( WM_MESSAGE *pMsg )
{
	int RS485_Selc;
	int Id, NCode;
	WM_HWIN hDropdown;
	WM_HWIN hWin = pMsg->hWin;
	UserRS485Send RS485Send_s;
		
	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN0 );

	switch( pMsg->MsgId )
	{
		case WM_INIT_DIALOG:
			_InitTestRS485Dialog(hWin);
			break;
					
		case WM_PAINT:
			_OnPaintTestRS485(hWin);
			break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch( NCode )
			{
				case WM_NOTIFICATION_RELEASED:
					if ( Id == GUI_ID_OK )
					{
							RS485Send_s.address=0x11;
							RS485Send_s.ctrl=0x12;
							RS485Send_s.data[0]=0x13;
							RS485Send_s.len=0x01;
							RS485_Selc = DROPDOWN_GetSel(hDropdown);
							switch( RS485_Selc )
							{
									case 0:
										Uart1SendCnt++;
									  RS485Send_Struct(RS485_UART0,&RS485Send_s);
										break;
								  case 1:
										Uart2SendCnt++;
										RS485Send_Struct(RS485_UART2,&RS485Send_s);
										break;
									case 2:
										Uart3SendCnt++;
										RS485Send_Struct(RS485_UART3,&RS485Send_s);
										break;
									case 3: 
										Uart4SendCnt++;
										RS485Send_Struct(RS485_UART4,&RS485Send_s);
										break;
									default: break;
							}
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

