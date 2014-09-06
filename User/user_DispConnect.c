/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_SetConnect.c
** Last modified Date:      2014.04.25
** Last Version:            1.0
** Description:             Header file of the Set Connect Dialog ����ͨ��״̬�Ի����ļ�
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              GuoBingrong ������
** Created date:            2014.04.25
** Version:                 1.0
** Descriptions:            The original version ��ʼ�汾

*********************************************************************************************************/
#include "lpc177x_8x_rtc.h"

#include "GUI.h"
#include "DIALOG.h"
#include "DataStruct.h"
#include "MENU.h"
#include "user_menu.h"

extern MENU_Handle _hMenu;
extern GUI_BITMAP bmledon;
extern GUI_BITMAP bmledoff;
extern int currentWin;

/*******************************************
	      ����ͨ��״̬��ʾ�������пؼ�

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateDispConnect[3] = 
{	
	{ FRAMEWIN_CreateIndirect, "ͨ��״̬", 0, 10, 10, 260, 230, 0, 0 },
	{ RADIO_CreateIndirect, "", 0, 6, 14, 16, 16, 0, 0 },
	{ BUTTON_CreateIndirect, "ȷ��", GUI_ID_OK,      160, 180, 60, 20, 0, 0 }
	//
};

/*******************************************
	      ������ʾ����ͨ��״̬

********************************************/
void _OnPaintDispConnect( WM_HWIN hWin )
{
	GUI_RECT  r;
	//unsigned char s[2];

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);

	GUI_DispStringAt( "ͨ�ŷ�ʽ", r.x0+ADDSTEP,               r.y0+ADDSTEP-10 );
	GUI_DispStringAt( "����",     r.x0+ADDSTEP6,              r.y0+ADDSTEP-10 );
	GUI_DispStringAt( "����",     r.x0+ADDSTEP6 + ADDSTEP2+10,r.y0+ADDSTEP-10 );
	GUI_DispStringAt( "CAN1",     r.x0+ADDSTEP,               r.y0+ADDSTEP3-6 );
	GUI_DispStringAt( "CAN2",     r.x0+ADDSTEP,    			  r.y0+ADDSTEP2-6 );
	GUI_DispStringAt( "LAN",      r.x0+ADDSTEP,   			  r.y0+ADDSTEP3-6 );
	GUI_DispStringAt( "RS485-1",  r.x0+ADDSTEP,    			  r.y0+ADDSTEP4-6 );
	GUI_DispStringAt( "RS485-2",  r.x0+ADDSTEP,               r.y0+ADDSTEP5-6 );
	GUI_DispStringAt( "RS485-3",  r.x0+ADDSTEP,               r.y0+ADDSTEP6-6 );
	GUI_DispStringAt( "RS485-4",  r.x0+ADDSTEP,               r.y0+ADDSTEP7-6 );

	GUI_DrawBitmap(&bmledon, r.x0+ADDSTEP6+8,            r.y0+ADDSTEP3-6 );
	GUI_DrawBitmap(&bmledon, r.x0+ADDSTEP6+8,            r.y0+ADDSTEP2-6 );
	GUI_DrawBitmap(&bmledoff, r.x0+ADDSTEP6+8,            r.y0+ADDSTEP3-6 );
	GUI_DrawBitmap(&bmledon, r.x0+ADDSTEP6+8,            r.y0+ADDSTEP4-6 );
	GUI_DrawBitmap(&bmledon, r.x0+ADDSTEP6+8,            r.y0+ADDSTEP5-6 );
	GUI_DrawBitmap(&bmledoff, r.x0+ADDSTEP6+8,            r.y0+ADDSTEP6-6 );
	GUI_DrawBitmap(&bmledoff, r.x0+ADDSTEP6+8,            r.y0+ADDSTEP7-6 );

	GUI_DrawBitmap(&bmledoff, r.x0+ADDSTEP9,            r.y0+ADDSTEP3-6 );
	GUI_DrawBitmap(&bmledon, r.x0+ADDSTEP9,            r.y0+ADDSTEP2-6 );
	GUI_DrawBitmap(&bmledoff, r.x0+ADDSTEP9,            r.y0+ADDSTEP3-6 );
	GUI_DrawBitmap(&bmledon, r.x0+ADDSTEP9,            r.y0+ADDSTEP4-6 );
	GUI_DrawBitmap(&bmledon, r.x0+ADDSTEP9,            r.y0+ADDSTEP5-6 );
	GUI_DrawBitmap(&bmledoff, r.x0+ADDSTEP9,            r.y0+ADDSTEP6-6 );
	GUI_DrawBitmap(&bmledon, r.x0+ADDSTEP9,            r.y0+ADDSTEP7-6 );

	return;
}

/*******************************************
	      ͨ��״̬���ڳ�ʼ��
  
********************************************/

void _InitDispConnectDialog(WM_HWIN hWin)
{
	//WM_HWIN hYear;
	//WM_HWIN hMonth, hDay;
	//WM_HWIN hHour, hMinute, hSecond;
	//unsigned char s[2];

	return;
}


/*******************************************
	      ͨ��״̬���ڵĻص�����

********************************************/


void _cbDispConnect( WM_MESSAGE *pMsg )
{
	int Id, NCode;
	WM_HWIN hWin = pMsg->hWin;	

	switch( pMsg->MsgId )
	{
		case WM_INIT_DIALOG:
			_InitDispConnectDialog(hWin);
			break;

		case WM_PAINT:
			_OnPaintDispConnect(hWin);
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

