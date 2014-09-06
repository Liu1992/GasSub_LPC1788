/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_SetTime.c
** Last modified Date:      2014.04.25
** Last Version:            1.0
** Description:             Header file of the Set Time Dialog 设置时间对话框文件
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
#include "user_SetTime.h"
#include "DataStruct.h"
#include "MENU.h"
#include "user_menu.h"
#include "stdio.h"

Bool IsLeap(uint16_t year);
Bool CheckTime(MY_DATE currentTime);

extern MY_DATE currentTime;
extern MENU_Handle _hMenu;
extern int currentWin;

static int curPosSetTime;

/*******************************************
	      设置时间窗口所有控件

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateSetTime[9] = 
{	
	{ FRAMEWIN_CreateIndirect, "时间设置", 0, 10, 10, 260, 200, 0, 0 },
	
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT0,  ADDSTEP2,              ADDSTEP2,  EDITXSIZE3-4, EDITYSIZE, 0, 50 },	
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT1,  ADDSTEP5+8,            ADDSTEP2,  EDITXSIZE2-4, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT2,  ADDSTEP8+4,              ADDSTEP2,  EDITXSIZE2-4, EDITYSIZE, 0, 50 },	

	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT3,  ADDSTEP+EDITXSIZE2+4,    ADDSTEP3+12, EDITXSIZE2-4, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT4,  ADDSTEP5+8,            ADDSTEP3+12,  EDITXSIZE2-4, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT5,  ADDSTEP8+4,              ADDSTEP3+12,  EDITXSIZE2-4, EDITYSIZE, 0, 50 },

	{ BUTTON_CreateIndirect, "确定", GUI_ID_OK,      80, 150, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "取消", GUI_ID_CANCEL, 160, 150, 60, 20, 0, 0 }
	//
};

/*******************************************
	      绘制设置时间窗口

********************************************/
void _OnPaintSetTime( WM_HWIN hWin )
{
	GUI_RECT  r;
//	unsigned char s[2];

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);

	GUI_DispStringAt( "年", r.x0+ADDSTEP4,                 r.y0+ADDSTEP2+4 );
	GUI_DispStringAt( "月", r.x0+ADDSTEP6 + ADDSTEP-4,     r.y0+ADDSTEP2+4 );
	GUI_DispStringAt( "日", r.x0+ADDSTEP6 + ADDSTEP4-8,    r.y0+ADDSTEP2+4 );
	GUI_DispStringAt( "时", r.x0+ADDSTEP4,                 r.y0+ADDSTEP3+16 );
	GUI_DispStringAt( "分", r.x0+ADDSTEP6 + ADDSTEP-4,     r.y0+ADDSTEP3+16 );
	GUI_DispStringAt( "秒", r.x0+ADDSTEP6 + ADDSTEP4-8,    r.y0+ADDSTEP3+16 );

	return;
}

/*******************************************
	      设置时间窗口初始化
    编辑框内的时间初始化为当前系统时间
********************************************/

void _InitSetTimeDialog(WM_HWIN hWin)
{
	WM_HWIN hEdit;
	char s[8];

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT0 );
	sprintf( s, "%04d", currentTime.year );
	EDIT_SetText( hEdit, s );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT1 );
	sprintf( s, "%02d", currentTime.mon );
	EDIT_SetText( hEdit, s );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT2 );
	sprintf( s, "%02d", currentTime.day );
	EDIT_SetText( hEdit, s );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT3 );
	sprintf( s, "%02d", currentTime.hour );
	EDIT_SetText( hEdit, s );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT4 );
	sprintf( s, "%02d", currentTime.min );
	EDIT_SetText( hEdit, s );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT5 );
	sprintf( s, "%02d", currentTime.sec );
	EDIT_SetText( hEdit, s );

	return;
}

/*******************************************
	        设置时间的每一位

********************************************/

void _changeEdit( WM_HWIN hWin, int Id, int curPosSetTime )
{
	WM_HWIN hEdit;

    hEdit = WM_GetDialogItem( hWin, Id );
	EDIT_SetCursorAtChar( hEdit, curPosSetTime );

    return;
}

void _getUserInputTime( WM_HWIN hWin )
{
	WM_HWIN hEdit;
	char s[8];

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT0 );
	EDIT_GetText( hEdit, s, 4 );
	sscanf( s, "%d", &currentTime.year );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT1 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &currentTime.mon );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT2 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &currentTime.day );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT3 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &currentTime.hour );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT4 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &currentTime.min );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT5 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &currentTime.sec );
	return;
}


/*******************************************
	      设置时间窗口的回调函数

********************************************/


void _cbSetTime( WM_MESSAGE *pMsg )
{
	int Id, NCode;
	WM_HWIN hWin = pMsg->hWin;	

	switch( pMsg->MsgId )
	{
		case WM_INIT_DIALOG:
			_InitSetTimeDialog(hWin);
			break;
		
		case WM_KEY:
			Id    = WM_GetId(pMsg->hWinSrc);
			switch ( ((WM_KEY_INFO *)(pMsg->Data.p))->Key )
			{
				case GUI_KEY_UP:
				case GUI_KEY_DOWN:
				 	_changeEdit( hWin, Id, curPosSetTime );
					break;

				case GUI_KEY_ENTER:
					++curPosSetTime;
					if ( curPosSetTime > 3 ) curPosSetTime = 0;
					_changeEdit( hWin, Id, curPosSetTime );

				default: break;
			}
					
			break;

		case WM_PAINT:
			_OnPaintSetTime(hWin);
			break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch( NCode )
			{
				case WM_NOTIFICATION_RELEASED:
					if ( Id == GUI_ID_OK )
					{
						_getUserInputTime( hWin );
						
						/*	*/
						if(CheckTime(currentTime))
						{
						
							GUI_EndDialog( hWin, 0 );
							
							RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND,      currentTime.sec  );
							RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE,      currentTime.min  );
							RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR,        currentTime.hour );
							RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH,       currentTime.mon  );
							RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR,   	    currentTime.year );
							RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH,  currentTime.day );

							WM_SetFocus(_hMenu);
							currentWin = USER_ID_MAIN_WIN;
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

/*******************************************
	     检测时间设置是否合法

********************************************/
Bool CheckTime(MY_DATE currentTime)
{
	uint16_t dayOfMon;
	if(currentTime.sec>=60||currentTime.min>=60||currentTime.hour>=24||currentTime.mon>12||(currentTime.year<1990||currentTime.year>2100))
		return FALSE;
	if(currentTime.mon==1||currentTime.mon==3||currentTime.mon==5||currentTime.mon==7||currentTime.mon==8||currentTime.mon==10||currentTime.mon==12)
	{
		dayOfMon=31;
	}
	else if(currentTime.mon==2)
	{
		if(IsLeap(currentTime.year))
		{
			dayOfMon=29;
		}
		else
		{
			dayOfMon=28;
		}
	}
	else
	{
		dayOfMon=30;
	}
	if(currentTime.day>dayOfMon)
	{
		return FALSE;
	}
	return TRUE;
}
/*******************************************
	     检测年份是否为闰年

********************************************/
Bool IsLeap(uint16_t year)
{
	if(year%400==0)
		return TRUE;
	else if(year%100!=0&&year%4==0)
		return TRUE;
	else
		return FALSE;
}
