/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_ErrorRateRS485.c
** Last modified Date:      2014.08.29
** Last Version:            1.0
** Description:             Header file of Test Error Rate Dialog RS-485误码率测试对话框文件
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              GuoBingrong 郭冰容
** Created date:            2014.08.29
** Version:                 1.0
** Descriptions:            The original version

*********************************************************************************************************/
#include "lpc177x_8x_rtc.h"

#include "GUI.h"
#include "DIALOG.h"
#include "DataStruct.h"
#include "MENU.h"
#include "user_menu.h"
#include "stdio.h"

WM_HWIN  _hErrorRateRS485;

extern MENU_Handle _hMenu;
extern int currentWin;

//static int curPosErrorRate;

/*******************************************
	      设置时间窗口所有控件

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateErrorRate[9] = 
{	
	{ FRAMEWIN_CreateIndirect, "RS-485误码率测试", 0, 10, 10, 260, 200, 0, 0 },
	
	{ EDIT_CreateIndirect,     NULL, GUI_ID_EDIT0,      ADDSTEP7, EDITXSIZE,          DROPDOWNXSIZE, DROPDOWNYSIZE3-6, 0, 50 },
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN0,  ADDSTEP7, EDITXSIZE+ADDSTEP,  DROPDOWNXSIZE, DROPDOWNYSIZE3-6, 0, 50 },
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN1,  ADDSTEP7, EDITXSIZE+ADDSTEP2, DROPDOWNXSIZE, DROPDOWNYSIZE4-6, 0, 50 },
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN2,  ADDSTEP7, EDITXSIZE+ADDSTEP3, DROPDOWNXSIZE, DROPDOWNYSIZE4-6, 0, 50 },	
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN3,  ADDSTEP7, EDITXSIZE+ADDSTEP4, DROPDOWNXSIZE, DROPDOWNYSIZE4-6, 0, 50 },

	{ BUTTON_CreateIndirect, "开始", GUI_ID_OK,      80, 150, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "停止", GUI_ID_CANCEL, 160, 150, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "关闭", GUI_ID_CLOSE,  220, 150, 60, 20, 0, 0 }
};

/*******************************************
	      绘制设置时间窗口

********************************************/
void OnPaintErrorRateRS485( WM_HWIN hWin )
{
	
	GUI_RECT  r;

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);

	GUI_DispStringAt( "设备地址：",        r.x0+ADDSTEP,     r.y0+EDITXSIZE );
	GUI_DispStringAt( "发送命令：",        r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP+1 );
	GUI_DispStringAt( "寄存器起始地址：",   r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP2+2 );
	GUI_DispStringAt( "读取寄存器个数：",   r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP3+3 );
	GUI_DispStringAt( "发送时间间隔：",   r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP4+4 );

	return;
}

/*******************************************
	      设置时间窗口初始化
    编辑框内的时间初始化为当前系统时间
********************************************/

void InitErrorRateRS485()
{
	WM_HWIN hEdit;
	WM_HWIN hDropdown[4];
	
	hEdit = EDIT_CreateEx( GUI_ID_EDIT0, ADDSTEP7, EDITXSIZE,          DROPDOWNXSIZE, DROPDOWNYSIZE3-6, _hErrorRateRS485, WM_CF_SHOW, 0, GUI_ID_EDIT0 );
	EDIT_SetText( hEdit, "000" );

	hDropdown[0] = DROPDOWN_CreateEx( ADDSTEP7, EDITXSIZE+ADDSTEP,  DROPDOWNXSIZE, DROPDOWNYSIZE3-6, _hErrorRateRS485, WM_CF_SHOW, 0, GUI_ID_DROPDOWN0 );
	DROPDOWN_AddString( hDropdown[0], "01 Read Coils(0x)" );
	DROPDOWN_AddString( hDropdown[0], "02 Read Discrete Inputs(1x)" );
	DROPDOWN_AddString( hDropdown[0], "03 Read Holding Registers(4x)" );
	DROPDOWN_AddString( hDropdown[0], "04 Read Input Registers(3x)" );
	DROPDOWN_AddString( hDropdown[0], "05 Write Single Coil" );
	DROPDOWN_AddString( hDropdown[0], "06 Write Single Register" );
	DROPDOWN_AddString( hDropdown[0], "15 Write Multiple Coils" );
	DROPDOWN_AddString( hDropdown[0], "16 Write Multiple Regristers" );
	DROPDOWN_SetAutoScroll( hDropdown[0], 1 );

	hDropdown[1] = DROPDOWN_CreateEx( ADDSTEP7, EDITXSIZE+ADDSTEP2, DROPDOWNXSIZE, DROPDOWNYSIZE4-6, _hErrorRateRS485, WM_CF_SHOW, 0, GUI_ID_DROPDOWN1 );
	DROPDOWN_AddString( hDropdown[1], "2.5Kbps" );
	DROPDOWN_AddString( hDropdown[1], "5.0Kbps" );
	DROPDOWN_AddString( hDropdown[1], "10Kbps" );
	DROPDOWN_SetAutoScroll( hDropdown[1], 1 );

	hDropdown[2] = DROPDOWN_CreateEx( ADDSTEP7, EDITXSIZE+ADDSTEP3, DROPDOWNXSIZE, DROPDOWNYSIZE4-6, _hErrorRateRS485, WM_CF_SHOW, 0, GUI_ID_DROPDOWN2 );
	DROPDOWN_AddString( hDropdown[2], "1" );
	DROPDOWN_AddString( hDropdown[2], "2" );
	DROPDOWN_AddString( hDropdown[2], "4" );
	DROPDOWN_AddString( hDropdown[2], "8" );
	DROPDOWN_AddString( hDropdown[2], "16" );
	DROPDOWN_SetAutoScroll( hDropdown[2], 1 );

	hDropdown[3] = DROPDOWN_CreateEx( ADDSTEP7, EDITXSIZE+ADDSTEP4, DROPDOWNXSIZE, DROPDOWNYSIZE4-6, _hErrorRateRS485, WM_CF_SHOW, 0, GUI_ID_DROPDOWN3 );
	DROPDOWN_AddString( hDropdown[3], "1s" );
	DROPDOWN_AddString( hDropdown[3], "2s" );
	DROPDOWN_AddString( hDropdown[3], "4s" );
	DROPDOWN_AddString( hDropdown[3], "8s" );
	DROPDOWN_SetAutoScroll( hDropdown[3], 1 );
}

/*******************************************
	        设置时间的每一位

********************************************

void _changeEdit( WM_HWIN hWin, int Id, int curPosErrorRate )
{
		WM_HWIN hEdit;

    hEdit = WM_GetDialogItem( hWin, Id );
		EDIT_SetCursorAtChar( hEdit, curPosErrorRate );

    return;
}

void _getUserInputTime( WM_HWIN hWin )
{
	WM_HWIN hEdit;
	char s[8];

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT0 );
	EDIT_GetText( hEdit, s, 4 );
	return;
}
*/

/*******************************************************************
*		RS485测试-回调函数
*       _cbErrorRateRS485Win
*/
static void cbErrorRateRS485Win(WM_MESSAGE* pMsg)
{
		WM_HWIN hWin = pMsg->hWin;
		int Id, NCode;

    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        OnPaintErrorRateRS485(hWin);							//绘制客户区主体
        break;
		
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch( NCode )
			{
				case WM_NOTIFICATION_RELEASED:
					if ( Id == GUI_ID_OK )
					{
						
					}
					if ( Id == GUI_ID_CANCEL )
					{
						
					}
					if ( Id == GUI_ID_CLOSE )
					{
							GUI_EndDialog( hWin, 0 );
							WM_SetFocus(_hMenu);
							currentWin = USER_ID_MAIN_WIN;
					}
					break;
				
			default:
					WM_DefaultProc(pMsg);
			}
		}
}

void StartErrorRateRS485(void)
{
	//创建框架窗口
    _hErrorRateRS485 = FRAMEWIN_CreateEx(10, 10, 280, 260, WM_HBKWIN, WM_CF_HIDE, FRAMEWIN_CF_MOVEABLE, 0, NULL, &cbErrorRateRS485Win);

    FRAMEWIN_SetClientColor( _hErrorRateRS485 , GUI_INVALID_COLOR);	  //设置客户区背景
    FRAMEWIN_SetFont       ( _hErrorRateRS485 , &GUI_FontHZ16);		  //为标题文本选择字体

	  FRAMEWIN_SetText(_hErrorRateRS485, "RS-485误码率测试");	  //设置标题文本
    
		InitErrorRateRS485();
}

void ErrorRate_RS485_Task(void)
{

	//WM_EnableMemdev(WM_HBKWIN);
  //WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */

	while (1)
	{		
				StartErrorRateRS485();
		
        while (_hErrorRateRS485)
        {
            GUI_Delay(1000);
            WM_InvalidateWindow(WM_GetClientWindow(_hErrorRateRS485));	//制定一个窗口无效 
        }
	}
}


