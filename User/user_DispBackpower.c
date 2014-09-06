/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_DispBackpower.c
** Last modified Date:      2014.04.25
** Last Version:            1.0
** Description:             Header file of the Set Time Dialog 后备电源显示对话框文件
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              GuoBingrong 郭冰容
** Created date:            2014.04.25
** Version:                 1.0
** Descriptions:            The original version 初始版本

*********************************************************************************************************/
#include "lpc177x_8x_rtc.h"

#include "GUI.h"
#include "DIALOG.h"
#include "DataStruct.h"
#include "MENU.h"
#include "user_menu.h"

extern MENU_Handle _hMenu;
extern int currentWin;

/*******************************************
	      后备电源状态显示窗口所有控件

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateDispBackpower[3] = 
{	
	{ FRAMEWIN_CreateIndirect, "后备电源", 0, 10, 10, 260, 250, 0, 0 },
	{ RADIO_CreateIndirect, "", 0, 6, 14, 16, 16, 0, 0 },
	{ BUTTON_CreateIndirect, "确定", GUI_ID_OK,      160, 200, 60, 20, 0, 0 }
	//
};

/*******************************************
	      绘制显示后备电源状态

********************************************/
void _OnPaintDispBackpower( WM_HWIN hWin )
{
	GUI_RECT  r;
	//unsigned char s[2];

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);

	GUI_DispStringAt( "电池电压：", r.x0+ADDSTEP,               r.y0+ADDSTEP-10 );
	GUI_DispStringAt( "供电方式：",     r.x0+ADDSTEP,               r.y0+ADDSTEP3-10 );
	GUI_DispStringAt( "状态：",     r.x0+ADDSTEP,    			  r.y0+ADDSTEP2-10 );
	GUI_DispStringAt( "电池温度：",      r.x0+ADDSTEP,   			  r.y0+ADDSTEP3-10 );
	GUI_DispStringAt( "环境温度：",  r.x0+ADDSTEP,    			  r.y0+ADDSTEP4-10 );

	GUI_DispStringAt( "27.81V", 	r.x0+ADDSTEP6,               r.y0+ADDSTEP-10 );
	GUI_DispStringAt( "交流供电",   r.x0+ADDSTEP6,               r.y0+ADDSTEP3-10 );
	GUI_DispStringAt( "快速充电",   r.x0+ADDSTEP6,    			 r.y0+ADDSTEP2-10 );
	GUI_DispStringAt( "40摄氏度",   r.x0+ADDSTEP6,   			 r.y0+ADDSTEP3-10 );
	GUI_DispStringAt( "30摄氏度",  	r.x0+ADDSTEP6,    			 r.y0+ADDSTEP4-10 );

	GUI_DispStringAt( "1路：",  r.x0+ADDSTEP,               r.y0+ADDSTEP5 );
	GUI_DispStringAt( "3路：",  r.x0+ADDSTEP,               r.y0+ADDSTEP6 );
	GUI_DispStringAt( "5路：",  r.x0+ADDSTEP,               r.y0+ADDSTEP7 );
	GUI_DispStringAt( "7路：",  r.x0+ADDSTEP,               r.y0+ADDSTEP8 );

	GUI_DispStringAt( "2路：",  r.x0+ADDSTEP6,               r.y0+ADDSTEP5 );
	GUI_DispStringAt( "4路：",  r.x0+ADDSTEP6,               r.y0+ADDSTEP6 );
	GUI_DispStringAt( "6路：",  r.x0+ADDSTEP6,               r.y0+ADDSTEP7 );
	GUI_DispStringAt( "8路：",  r.x0+ADDSTEP6,               r.y0+ADDSTEP8 );
	
	GUI_SetColor(GUI_DARKGREEN);
	GUI_DispStringAt( "正常",  r.x0+ADDSTEP3,               r.y0+ADDSTEP5 );
	GUI_DispStringAt( "正常",  r.x0+ADDSTEP3,               r.y0+ADDSTEP6 );
	GUI_DispStringAt( "正常",  r.x0+ADDSTEP3,               r.y0+ADDSTEP7 );

	GUI_SetColor(GUI_RED);
	GUI_DispStringAt( "断开",  r.x0+ADDSTEP3,               r.y0+ADDSTEP8 );
	
	GUI_DispStringAt( "断开",  r.x0+ADDSTEP8,               r.y0+ADDSTEP5 );
	GUI_DispStringAt( "断开",  r.x0+ADDSTEP8,               r.y0+ADDSTEP7 );
	
	GUI_SetColor(GUI_DARKGREEN);
	GUI_DispStringAt( "正常",  r.x0+ADDSTEP8,               r.y0+ADDSTEP6 );	
	GUI_DispStringAt( "正常",  r.x0+ADDSTEP8,               r.y0+ADDSTEP8 );

	return;
}

/*******************************************
	     后备电源状态窗口初始化
  
********************************************/

void _InitDispBackpowerDialog(WM_HWIN hWin)
{
	//WM_HWIN hEdit;
	//unsigned char s[2];

	return;
}


/*******************************************
	      后备电源状态窗口的回调函数

********************************************/


void _cbDispBackpower( WM_MESSAGE *pMsg )
{
	int Id, NCode;
	WM_HWIN hWin = pMsg->hWin;	

	switch( pMsg->MsgId )
	{
		case WM_INIT_DIALOG:
			_InitDispBackpowerDialog(hWin);
			break;

		case WM_PAINT:
			_OnPaintDispBackpower(hWin);
			break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch( NCode )
			{
				case WM_NOTIFICATION_RELEASED:
					if ( Id == GUI_ID_OK )
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

