/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_SetType.c
** Last modified Date:      2014.07.22
** Last Version:            1.0
** Description:             Header file of the Set Connect Dialog ��վ��������
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              GuoBingrong ������
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
	      ͨѶ���ô������пؼ�

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateSetType[4] = 
{	
	{ FRAMEWIN_CreateIndirect, "��վ��������", 0, 20, 40, 280, 200, 0, 0 },
	
	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN0,  ADDSTEP6, EDITXSIZE,        DROPDOWNXSIZE, DROPDOWNYSIZE2, 0, 50 },

	{ BUTTON_CreateIndirect, "ȷ��", GUI_ID_OK,     100, 150, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "ȡ��", GUI_ID_CANCEL, 180, 150, 60, 20, 0, 0 },
};

/*******************************************
	      ����ͨѶ���ô���

********************************************/
void _OnPaintSetType( WM_HWIN hWin )
{
	GUI_RECT  r;
	//unsigned char s[2];

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);
	
	GUI_DispStringAt( "��վ���ͣ�",        r.x0+ADDSTEP,     r.y0+EDITXSIZE );
	
	//GUI_SetColor(GUI_BLUE);
	GUI_DispStringAt( "A��Ϊ16�������",        r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP2+10 );
	GUI_DispStringAt( "B��Ϊ40�������,17-40Ϊ����ʽ",   r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP3+14 );
	GUI_DispStringAt( "���ִ�����",   r.x0+ADDSTEP,     r.y0+EDITXSIZE+ADDSTEP4+15 );
	
	return;
}

/*******************************************
	      ��վ�������ô��ڳ�ʼ��
       �����˵��ڵ�ֵΪϵͳ��ǰ����ֵ
********************************************/

void _InitSetTypeDialog(WM_HWIN hWin)
{
	WM_HWIN hDropdown;

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN0 );
	DROPDOWN_AddString( hDropdown, "A��" );
	DROPDOWN_AddString( hDropdown, "B��" );
	//DROPDOWN_SetBkColor( hDropdown, DROPDOWN_CI_UNSEL, GUI_WHITE );

	return;
}

/*******************************************
	      ��վ�������ô��ڵĻص�����

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

