/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_SetType.c
** Last modified Date:      2014.07.22
** Last Version:            1.0
** Description:             Header file of the Set Connect Dialog 分站类型设置
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              GuoBingrong 郭冰容
** Created date:            2014.07.22
** Version:                 1.0
** Descriptions:            

*********************************************************************************************************/
#include "lpc177x_8x_rtc.h"

#include "GUI.h"
#include "DIALOG.h"
#include "DataStruct.h"
#include "MENU.h"
#include "user_menu.h"
#include "os_cpu.h"

extern MENU_Handle _hMenu;
extern INT8U substType;
extern int currentWin;

/*******************************************
	      通讯设置窗口所有控件

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateSetType[4] = 
{	
	{ FRAMEWIN_CreateIndirect, "分站类型设置", 0, 20, 40, 280, 200, 0, 0 },
	
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN0,  ADDSTEP6, EDITXSIZE,        DROPDOWNXSIZE, DROPDOWNYSIZE2, 0, 50 },

	{ BUTTON_CreateIndirect, "确定", GUI_ID_OK,     100, 150, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "取消", GUI_ID_CANCEL, 180, 150, 60, 20, 0, 0 },
};

/*******************************************
	      绘制通讯设置窗口

********************************************/
void _OnPaintSetType( WM_HWIN hWin )
{
	GUI_RECT  r;
	//unsigned char s[2];

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);
	
	GUI_DispStringAt( "分站类型：",        r.x0+ADDSTEP,     r.y0+EDITXSIZE );
	
	//GUI_SetColor(GUI_BLUE);
	GUI_DispStringAt( "A型为16测点输入",        r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP2+10 );
	GUI_DispStringAt( "B型为40测点输入,17-40为总线式",   r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP3+14 );
	GUI_DispStringAt( "数字传感器",   r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP4+15 );
	
	return;
}

/*******************************************
	      分站类型设置窗口初始化
       下拉菜单内的值为系统当前设置值
********************************************/

void _InitSetTypeDialog(WM_HWIN hWin)
{
	WM_HWIN hDropdown;

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN0 );
	DROPDOWN_AddString( hDropdown, "A型" );
	DROPDOWN_AddString( hDropdown, "B型" );
	//DROPDOWN_SetBkColor( hDropdown, DROPDOWN_CI_UNSEL, GUI_WHITE );

	return;
}

/*******************************************
	      分站类型设置窗口的回调函数

********************************************/


void _cbSetType( WM_MESSAGE *pMsg )
{
	int Id, NCode;
	WM_HWIN hWin = pMsg->hWin;
	WM_HWIN hDropdown;

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN0 );

	switch( pMsg->MsgId )
	{
		case WM_INIT_DIALOG:
			_InitSetTypeDialog(hWin);
			break;
					
		case WM_PAINT:
			_OnPaintSetType(hWin);
			break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch( NCode )
			{
				case WM_NOTIFICATION_RELEASED:
					if ( Id == GUI_ID_OK )
					{
						substType = DROPDOWN_GetSel(hDropdown);
						GUI_EndDialog( hWin, 0 );
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

