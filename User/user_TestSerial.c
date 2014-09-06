/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_TestRS485.c
** Last modified Date:      2014.07.25
** Last Version:            1.0
** Description:             Header file of the Set Connect Dialog ���ڲ���
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              GuoBingrong ������
** Created date:            2014.07.25
** Version:                 1.0
** Descriptions:            The original version ��ʼ�汾

*********************************************************************************************************/
#include "lpc177x_8x_rtc.h"

#include "GUI.h"
#include "DIALOG.h"
#include "user_SetTime.h"
#include "DataStruct.h"
#include "MENU.h"
#include "user_menu.h"

extern MENU_Handle _hMenu;
extern int currentWin;

/*******************************************
	      ���ڲ��Դ������пؼ�

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateTestSerial[7] = 
{	
	{ FRAMEWIN_CreateIndirect, "���ڲ���", 0, 10, 10, 300, 220, 0, 0 },
	
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN0,  ADDSTEP, EDITXSIZE,          DROPDOWNXSIZE, DROPDOWNYSIZE3-6, 0, 50 },
	{ EDIT_CreateIndirect,     NULL, GUI_ID_EDIT0,      ADDSTEP, EDITXSIZE2+14,  240, EDITYSIZE5, 0, 50 },

	{ BUTTON_CreateIndirect, "��ͣ", GUI_ID_OK,     10, 170, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "����", GUI_ID_YES,    80, 170, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "���", GUI_ID_NO,     150, 170, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "�˳�", GUI_ID_CLOSE,  220, 170, 60, 20, 0, 0 },
};

/*******************************************
	      ���ڲ��Խ������

********************************************/
void _OnPaintTestSerial( WM_HWIN hWin )
{
	GUI_RECT  r;
	//unsigned char s[2];

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);

	GUI_DispStringAt( "��ʵʱ����",        r.x0+ADDSTEP7,     r.y0+EDITXSIZE+2 );
		
	GUI_DispStringAt( "���ʹ�����",        r.x0+ADDSTEP,      r.y0+EDITXSIZE+ADDSTEP6 );
	GUI_DispStringAt( "���մ�����",        r.x0+ADDSTEP7,     r.y0+EDITXSIZE+ADDSTEP6 );

	return;
}

/*******************************************
	      ���ڲ��Դ��ڳ�ʼ��
********************************************/

void _InitTestSerialDialog(WM_HWIN hWin)
{
	WM_HWIN hDropdown;

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN0 );
	DROPDOWN_AddString( hDropdown, "RS485-1" );
	DROPDOWN_AddString( hDropdown, "RS485-2" );
	DROPDOWN_AddString( hDropdown, "RS485-3" );
	DROPDOWN_AddString( hDropdown, "RS485-4" );
	//DROPDOWN_SetBkColor( hDropdown, DROPDOWN_CI_UNSEL, GUI_WHITE );
	DROPDOWN_SetAutoScroll( hDropdown, 1 );
	
	return;
}

/*******************************************
	      ���ڲ��Դ��ڵĻص�����

********************************************/


void _cbTestSerial( WM_MESSAGE *pMsg )
{
	int Id, NCode;
	WM_HWIN hEdit;
	WM_HWIN hWin = pMsg->hWin;

	switch( pMsg->MsgId )
	{
		case WM_INIT_DIALOG:
			_InitTestSerialDialog(hWin);
			break;
					
		case WM_PAINT:
			_OnPaintTestSerial(hWin);
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
					if ( Id == GUI_ID_YES )
					{
						
					}
					if ( Id == GUI_ID_NO )
					{
							hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT0 );
							EDIT_SetText( hEdit, "" );
					}
					if ( Id == GUI_ID_CLOSE )
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

