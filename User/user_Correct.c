/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_Corrent.c
** Last modified Date:      2014.08.27
** Last Version:            1.0
** Description:             Header file of the Correct Dialog У׼16·����˿ڶԻ����ļ�
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              GuoBingrong ������
** Created date:            2014.08.27
** Version:                 1.0
** Descriptions:            The original version ��ʼ�汾

*********************************************************************************************************/
#include "lpc177x_8x_rtc.h"

#include "GUI.h"
#include "DIALOG.h"
#include "DataStruct.h"
#include "MENU.h"
#include "user_menu.h"
#include "stdio.h"
#include "com_uart.h"
#include "string.h"
#include "user_SaveOption.h"


extern OS_EVENT  *UartRcvTaskSem;
extern RcvStruct  uart1RcvInfo;
extern INT8U      uart1RcvOK;

extern MENU_Handle _hMenu;
extern int currentWin;
static Correct_Input correct_input;

static int curPosCorrect = 0;

INT32S xCorrect[16][2], yCorrect[16][2];

INT32S prePort = 1000;         		//��һ��У׼�Ķ˿�
INT32S preCurrent = -1;		//��һ�βɼ��ĵ���ֵ
INT32S getDataCnt = 0;   			 		//ͬһ���˿ڲɼ����ݵĴ���

void GetAndSaveCorrectVal( int port );
void getPortData( WM_HWIN hWin );
BOOLEAN IfFinishedOneCorrect();

/*******************************************
	      У׼�������пؼ�

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateCorrect[7] = 
{	
	{ FRAMEWIN_CreateIndirect, "����У׼", 0, 10, 10, 280, 220, 0, 0 },

	{ DROPDOWN_CreateIndirect, NULL, GUI_ID_DROPDOWN0,  ADDSTEP6-4, EDITXSIZE,          DROPDOWNXSIZE, DROPDOWNYSIZE5, 0, 50 },
	
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT0,  ADDSTEP6-4,            ADDSTEP2,  EDITXSIZE4, EDITYSIZE, 0, 50 },	
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT1,  ADDSTEP6-4,            ADDSTEP3+4,  EDITXSIZE4, EDITYSIZE, 0, 50 },

	{ BUTTON_CreateIndirect, "��ȡ����", GUI_ID_OK,      140, 130, 100, 20, 0, 0 },
	
	{ BUTTON_CreateIndirect, "����", GUI_ID_YES,      100, 160, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "�ر�", GUI_ID_CLOSE, 180, 160, 60, 20, 0, 0 }
};

/*******************************************
	      ��������ʱ�䴰��

********************************************/
void _OnPaintCorrect( WM_HWIN hWin )
{
	GUI_RECT  r;
	char str[8];

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);

	GUI_DispStringAt( "У׼�˿ڣ�", r.x0+ADDSTEP,       r.y0+EDITXSIZE+2 );
	GUI_DispStringAt( "���������", r.x0+ADDSTEP,       r.y0+ADDSTEP2+4 );
	GUI_DispStringAt( "mA", r.x0+ADDSTEP9,             r.y0+ADDSTEP2+4 );
	GUI_DispStringAt( "������Ӧֵ��", r.x0+ADDSTEP,     r.y0+ADDSTEP3+8 );
	GUI_DispStringAt( "ʵ�ʲɼ�ֵ��", r.x0+ADDSTEP,     r.y0+ADDSTEP4+12 );
	
	//sprintf(str, "%d",  );
	GUI_DispStringAt( str, r.x0 + ADDSTEP8, r.y0+ADDSTEP4+12 );

	return;
}

/*******************************************
	      ����ʱ�䴰�ڳ�ʼ��
    �༭���ڵ�ʱ���ʼ��Ϊ��ǰϵͳʱ��
********************************************/

void _InitCorrectDialog(WM_HWIN hWin)
{
	WM_HWIN hEdit;
	WM_HWIN hDropdown;

	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN0 );
	DROPDOWN_AddString( hDropdown, "1·" );
	DROPDOWN_AddString( hDropdown, "2·" );
	DROPDOWN_AddString( hDropdown, "3·" );
	DROPDOWN_AddString( hDropdown, "4·" );
	DROPDOWN_AddString( hDropdown, "5·" );
	DROPDOWN_AddString( hDropdown, "6·" );
	DROPDOWN_AddString( hDropdown, "7·" );
	DROPDOWN_AddString( hDropdown, "8·" );
	DROPDOWN_AddString( hDropdown, "9·" );
	DROPDOWN_AddString( hDropdown, "10·" );
	DROPDOWN_AddString( hDropdown, "11·" );
	DROPDOWN_AddString( hDropdown, "12·" );
	DROPDOWN_AddString( hDropdown, "13·" );
	DROPDOWN_AddString( hDropdown, "14·" );
	DROPDOWN_AddString( hDropdown, "15·" );
	DROPDOWN_AddString( hDropdown, "16·" );
	DROPDOWN_SetAutoScroll( hDropdown, 1 );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT0 );
	EDIT_SetText( hEdit, "0000" );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT1 );
	EDIT_SetText( hEdit, "0000" );	

	return;
}

/*******************************************
	        ����ʱ���ÿһλ

********************************************/

void _changeEditCorrect( WM_HWIN hWin, int Id, int curPosCorrect )
{
	 WM_HWIN hEdit;

   hEdit = WM_GetDialogItem( hWin, Id );
	 EDIT_SetCursorAtChar( hEdit, curPosCorrect );

    return;
}

/*******************************************
	      ����ʱ�䴰�ڵĻص�����

********************************************/


void _cbCorrect( WM_MESSAGE *pMsg )
{
	int Id, NCode;
	WM_HWIN hWin = pMsg->hWin;	
	WM_HWIN hDropdown;
	
	hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN0 );

	switch( pMsg->MsgId )
	{
		case WM_INIT_DIALOG:
			_InitCorrectDialog(hWin);
			break;
		
		case WM_KEY:
			Id    = WM_GetId(pMsg->hWinSrc);
			switch ( ((WM_KEY_INFO *)(pMsg->Data.p))->Key )
			{
				case GUI_KEY_UP:
				case GUI_KEY_DOWN:
				 	_changeEditCorrect( hWin, Id, curPosCorrect );
					break;

				case GUI_KEY_ENTER:
					++curPosCorrect;
					if ( curPosCorrect > 3 ) curPosCorrect = 0;
					_changeEditCorrect( hWin, Id, curPosCorrect );

				default: break;
			}
					
			break;

		case WM_PAINT:
			_OnPaintCorrect(hWin);
			break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch( NCode )
			{
				case WM_NOTIFICATION_RELEASED:
					if ( Id == GUI_ID_YES )       //��ȡ����
					{
							getPortData( hWin );
					}
					if ( Id == GUI_ID_OK )        //��������
					{
							if ( IfFinishedOneCorrect() == TRUE )
							{
									GetAndSaveCorrectVal( DROPDOWN_GetSel( hDropdown ) );
									if ( EEPROM_WriteRati( correct_input ) == TRUE )
									{
											;
									}
							}
					}
					if ( Id == GUI_ID_CLOSE )     //�رմ���
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

/* �Ƿ����һ���˿ڵ�У׼ */
BOOLEAN IfFinishedOneCorrect()
{
		if ( getDataCnt >= 2 )
			return TRUE;
		else return FALSE;
}

/* ��ȡָ���˿ڵ�AD */
INT32S getPortAD( int port )
{
		OSTimeDly(3000);
		return uart1RcvInfo.AD[port];
}

/* ��ȡ�û��������ֵ */
void getPortData( WM_HWIN hWin )
{
		int port;
		WM_HWIN hDropdown, hEdit;
		INT32S current;
		char str[8];
		
		hDropdown = WM_GetDialogItem( hWin, GUI_ID_DROPDOWN0 );
	
		port = DROPDOWN_GetSel( hDropdown );
	
		if ( port == prePort )
		{
				hEdit 		= WM_GetDialogItem( hWin, GUI_ID_EDIT0 );
			  EDIT_GetText( hEdit, str, 4 );
				sscanf( str, "%d", &current );
			
				hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT1 );
				EDIT_GetText( hEdit, str, 4 );
			
				if ( current != preCurrent )
				{						
						++getDataCnt;
						//if ( getDataCnt >= 2 ) getDataCnt = 0;
				}
				sscanf( str, "%d", &xCorrect[port][ getDataCnt%2 ] );
				yCorrect[port][getDataCnt] = getPortAD(port);
		}
		else
		{
				preCurrent = -1;
				getDataCnt = 0;
		}
		prePort = port;
		
		return;
}

/* ����У׼ֵ������ */
void GetAndSaveCorrectVal( int port )
{
	  float k, b;
		k = (FP32)( xCorrect[port][1] - xCorrect[port][0] )/(FP32)( yCorrect[port][1] - yCorrect[port][0] );
		b = (FP32)yCorrect[port][0] - correct_input.slope[port]*xCorrect[port][0];
		correct_input.slope[port] = (INT32S)(k * 1000);
		correct_input.intercept[port] = (INT32S)(b * 1000);
		return;
}
