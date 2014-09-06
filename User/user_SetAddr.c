/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_SetTime.c
** Last modified Date:      2014.04.25
** Last Version:            1.0
** Description:             Header file of the Set Time Dialog IP地址设置对话框文件
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
#include "user_SaveOption.h"


extern MY_DATE currentTime;
extern MENU_Handle _hMenu;
extern int currentWin;
extern EEPROMDataStruct optionSaveStruct;

static int curPosSetAddr;

/*******************************************
	      设置时间窗口所有控件

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateSetAddr[21] = 
{	
	{ FRAMEWIN_CreateIndirect, "IP地址设置", 0, 10, 10, 300, 200, 0, 0 },
	
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT0,  ADDSTEP4+8,    OTHERADDSTEP-5,  EDITXSIZE2+4, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT1,  ADDSTEP6+8,    OTHERADDSTEP-5,  EDITXSIZE2+4, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT2,  ADDSTEP8+8,    OTHERADDSTEP-5,  EDITXSIZE2+4, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT3,  ADDSTEP10+8,   OTHERADDSTEP-5,  EDITXSIZE2+4, EDITYSIZE, 0, 50 },
	
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT4,  ADDSTEP4+8,    OTHERADDSTEP2-5,  EDITXSIZE2+4, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT5,  ADDSTEP6+8,    OTHERADDSTEP2-5,  EDITXSIZE2+4, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT6,  ADDSTEP8+8,    OTHERADDSTEP2-5,  EDITXSIZE2+4, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT7,  ADDSTEP10+8,   OTHERADDSTEP2-5,  EDITXSIZE2+4, EDITYSIZE, 0, 50 },
	
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT8,  ADDSTEP4+8,    OTHERADDSTEP3-5,  EDITXSIZE2+4, EDITYSIZE, 0, 50},
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT9,  ADDSTEP6+8,    OTHERADDSTEP3-5,  EDITXSIZE2+4, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT10, ADDSTEP8+8,    OTHERADDSTEP3-5,  EDITXSIZE2+4, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT11, ADDSTEP10+8,   OTHERADDSTEP3-5,  EDITXSIZE2+4, EDITYSIZE, 0, 50},

	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT12, ADDSTEP2+OTHERADDSTEP+12,    OTHERADDSTEP4-5,  EDITXSIZE+10, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT13, ADDSTEP2+OTHERADDSTEP2+12,   OTHERADDSTEP4-5,  EDITXSIZE+10, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT14, ADDSTEP2+OTHERADDSTEP3+12,   OTHERADDSTEP4-5,  EDITXSIZE+10, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT15, ADDSTEP2+OTHERADDSTEP4+12,   OTHERADDSTEP4-5,  EDITXSIZE+10, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT16, ADDSTEP2+OTHERADDSTEP5+12,   OTHERADDSTEP4-5,  EDITXSIZE+10, EDITYSIZE, 0, 50 },
	{ EDIT_CreateIndirect, NULL,   GUI_ID_EDIT17, ADDSTEP2+OTHERADDSTEP6+12,   OTHERADDSTEP4-5,  EDITXSIZE+10, EDITYSIZE, 0, 50 },

	{ BUTTON_CreateIndirect, "确定", GUI_ID_OK,      80, 150, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "取消", GUI_ID_CANCEL, 160, 150, 60, 20, 0, 0 }
	//
};

/*******************************************
	      绘制设置时间窗口

********************************************/
void _OnPaintSetAddr( WM_HWIN hWin )
{
	GUI_RECT  r;
	//unsigned char s[2];

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);

	GUI_DispStringAt( "IP地址:",   r.x0+ADDSTEP-4,    r.y0+OTHERADDSTEP-5 );
	GUI_DispStringAt( "子网掩码:", r.x0+ADDSTEP-4,    r.y0+OTHERADDSTEP2-5 );
	GUI_DispStringAt( "默认网关:", r.x0+ADDSTEP-4,    r.y0+OTHERADDSTEP3-5 );
	GUI_DispStringAt( "MAC:",      r.x0+ADDSTEP-4,    r.y0+OTHERADDSTEP4-5 );
	
	GUI_DispStringAt( ".", r.x0+ADDSTEP4+8+EDITXSIZE2+6 , r.y0+OTHERADDSTEP );
	GUI_DispStringAt( ".", r.x0+ADDSTEP6+8+EDITXSIZE2+6 , r.y0+OTHERADDSTEP );
	GUI_DispStringAt( ".", r.x0+ADDSTEP8+8+EDITXSIZE2+6 , r.y0+OTHERADDSTEP );

	GUI_DispStringAt( ".", r.x0+ADDSTEP4+8+EDITXSIZE2+6 , r.y0+OTHERADDSTEP2 );
	GUI_DispStringAt( ".", r.x0+ADDSTEP6+8+EDITXSIZE2+6 , r.y0+OTHERADDSTEP2 );
	GUI_DispStringAt( ".", r.x0+ADDSTEP8+8+EDITXSIZE2+6 , r.y0+OTHERADDSTEP2 );

	GUI_DispStringAt( ".", r.x0+ADDSTEP4+8+EDITXSIZE2+6 , r.y0+OTHERADDSTEP3 );
	GUI_DispStringAt( ".", r.x0+ADDSTEP6+8+EDITXSIZE2+6 , r.y0+OTHERADDSTEP3 );
	GUI_DispStringAt( ".", r.x0+ADDSTEP8+8+EDITXSIZE2+6 , r.y0+OTHERADDSTEP3 );

	return;
}

/*******************************************
	      设置IP窗口初始化
    编辑框内的时间初始化为当前系统时间
********************************************/

void _InitSetAddrDialog(WM_HWIN hWin)
{
	WM_HWIN hEdit;
	unsigned char s[4];

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT0 );
	sprintf( s, "%03d", optionSaveStruct.ipConfig.ip[0] );
	EDIT_SetText( hEdit, s );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT1 );
	sprintf( s, "%03d", optionSaveStruct.ipConfig.ip[1] );
	EDIT_SetText( hEdit, s );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT2 );
	sprintf( s, "%03d", optionSaveStruct.ipConfig.ip[2] );
	EDIT_SetText( hEdit, s );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT3 );
	sprintf( s, "%03d", optionSaveStruct.ipConfig.ip[3] );
	EDIT_SetText( hEdit, s );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT4 );
	sprintf( s, "%03d", optionSaveStruct.ipConfig.sub[0] );
	EDIT_SetText( hEdit, s );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT5 );
	sprintf( s, "%03d", optionSaveStruct.ipConfig.sub[1] );
	EDIT_SetText( hEdit, s );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT6 );
	sprintf( s, "%03d", optionSaveStruct.ipConfig.sub[2] );
	EDIT_SetText( hEdit, s );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT7 );
	sprintf( s, "%03d", optionSaveStruct.ipConfig.sub[3] );
	EDIT_SetText( hEdit, s );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT8 );
	sprintf( s, "%03d", optionSaveStruct.ipConfig.gw[0] );
	EDIT_SetText( hEdit, s );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT9 );
	sprintf( s, "%03d", optionSaveStruct.ipConfig.gw[1] );
	EDIT_SetText( hEdit, s );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT10 );
	sprintf( s, "%03d", optionSaveStruct.ipConfig.gw[2] );
	EDIT_SetText( hEdit, s );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT11 );
	sprintf( s, "%03d", optionSaveStruct.ipConfig.gw[3] );
	EDIT_SetText( hEdit, s );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT12 );
	sprintf( s, "%02d", optionSaveStruct.ipConfig.mac[0] );
	EDIT_SetText( hEdit, s );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT13 );
	sprintf( s, "%02d", optionSaveStruct.ipConfig.mac[1] );
	EDIT_SetText( hEdit, s );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT14 );
	sprintf( s, "%02d",optionSaveStruct.ipConfig.mac[2] );
	EDIT_SetText( hEdit, s );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT15 );
	sprintf( s, "%02d", optionSaveStruct.ipConfig.mac[3] );
	EDIT_SetText( hEdit, s );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT16 );
	sprintf( s, "%02d", optionSaveStruct.ipConfig.mac[4] );
	EDIT_SetText( hEdit, s );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT17 );
	sprintf( s, "%02d", optionSaveStruct.ipConfig.mac[5] );
	EDIT_SetText( hEdit, s );

	return;
}

Bool _checkBitIP( char *s )
{
	  INT8U check[3]={'2','5','5'};
		
		if(s[0]==check[0])
		{
			if(s[1]>check[1])
				return FALSE;
			else if(s[1]==check[1]&&s[2]>check[2])
				return FALSE;
		}
		else if ( s[0] > check[0] ) return FALSE;
		
		return TRUE;
}

Bool _checkUserInputIP( WM_HWIN hWin )
{
	WM_HWIN hEdit;
	char s[4];
	

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT0 );
	EDIT_GetText( hEdit, s, 3 );
	if ( _checkBitIP(s) == FALSE ) return FALSE;
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT1 );
	EDIT_GetText( hEdit, s, 3 );
	if ( _checkBitIP(s) == FALSE ) return FALSE;
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT2 );
	EDIT_GetText( hEdit, s, 3 );
	if ( _checkBitIP(s) == FALSE ) return FALSE;
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT3 );
	EDIT_GetText( hEdit, s, 3 );
	if ( _checkBitIP(s) == FALSE ) return FALSE;
		
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT4 );
	EDIT_GetText( hEdit, s, 3 );
	if ( _checkBitIP(s) == FALSE ) return FALSE;
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT5 );
	EDIT_GetText( hEdit, s, 3 );
	if ( _checkBitIP(s) == FALSE ) return FALSE;
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT6 );
	EDIT_GetText( hEdit, s, 3 );
	if ( _checkBitIP(s) == FALSE ) return FALSE;
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT7 );
	EDIT_GetText( hEdit, s, 3 );
	if ( _checkBitIP(s) == FALSE ) return FALSE;
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT8 );
	EDIT_GetText( hEdit, s, 3 );
	if ( _checkBitIP(s) == FALSE ) return FALSE;
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT9 );
	EDIT_GetText( hEdit, s, 3 );
	if ( _checkBitIP(s) == FALSE ) return FALSE;
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT10 );
	EDIT_GetText( hEdit, s, 3 );
	if ( _checkBitIP(s) == FALSE ) return FALSE;
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT11 );
	EDIT_GetText( hEdit, s, 3 );
	if ( _checkBitIP(s) == FALSE ) return FALSE;
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT12 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.mac[0] );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT13 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.mac[1] );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT14 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.mac[2] );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT15 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.mac[3] );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT16 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.mac[4] );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT17 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.mac[5] );
	return TRUE;
}

void _getUserInputIP( WM_HWIN hWin )
{
	WM_HWIN hEdit;
	char s[4];
	INT8U check[3]={'2','5','5'};

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT0 );
	EDIT_GetText( hEdit, s, 3 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.ip[0] );
	printf("%d",optionSaveStruct.ipConfig.ip[0]);
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT1 );
	EDIT_GetText( hEdit, s, 3 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.ip[1] );
	printf("%d",optionSaveStruct.ipConfig.ip[1]);
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT2 );
	EDIT_GetText( hEdit, s, 3 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.ip[2] );
	printf("%d",optionSaveStruct.ipConfig.ip[2]);
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT3 );
	EDIT_GetText( hEdit, s, 3 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.ip[3] );
	printf("%d",optionSaveStruct.ipConfig.ip[3]);
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT4 );
	EDIT_GetText( hEdit, s, 3 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.sub[0] );
	printf("%d",optionSaveStruct.ipConfig.sub[0]);
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT5 );
	EDIT_GetText( hEdit, s, 3 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.sub[1] );
	printf("%d",optionSaveStruct.ipConfig.sub[1]);
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT6 );
	EDIT_GetText( hEdit, s, 3 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.sub[2] );
	printf("%d",optionSaveStruct.ipConfig.sub[2]);
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT7 );
	EDIT_GetText( hEdit, s, 3 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.sub[3] );
	printf("%d",optionSaveStruct.ipConfig.sub[3]);
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT8 );
	EDIT_GetText( hEdit, s, 3 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.gw[0] );
	printf("%d",optionSaveStruct.ipConfig.gw[0]);
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT9 );
	EDIT_GetText( hEdit, s, 3 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.gw[1] );
	printf("%d",optionSaveStruct.ipConfig.gw[1]);
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT10 );
	EDIT_GetText( hEdit, s, 3 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.gw[2] );
	printf("%d",optionSaveStruct.ipConfig.gw[2]);
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT11 );
	EDIT_GetText( hEdit, s, 3 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.gw[3] );
	printf("%d",optionSaveStruct.ipConfig.gw[3]);
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT12 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.mac[0] );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT13 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.mac[1] );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT14 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.mac[2] );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT15 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.mac[3] );
	
	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT16 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.mac[4] );

	hEdit = WM_GetDialogItem( hWin, GUI_ID_EDIT17 );
	EDIT_GetText( hEdit, s, 2 );
	sscanf( s, "%d", &optionSaveStruct.ipConfig.mac[5] );
	return;
}

/*******************************************
	        设置IP地址的每一位
			   更改光标位置
********************************************/

void _changeAddrEdit( WM_HWIN hWin, int Id, char op, int curPosSetAddr )
{
    WM_HWIN hEdit;

    hEdit = WM_GetDialogItem( hWin, Id );
	EDIT_SetCursorAtChar( hEdit, curPosSetAddr );			

    return;
}


/*******************************************
	      设置时间窗口的回调函数

********************************************/


void _cbSetAddr( WM_MESSAGE *pMsg )
{
	int Id, NCode;
	WM_HWIN hWin = pMsg->hWin;	
	
	switch( pMsg->MsgId )
	{
		case WM_INIT_DIALOG:
			_InitSetAddrDialog(hWin);
			break;
		
		case WM_KEY: 
			Id    = WM_GetId(pMsg->hWinSrc);
			switch ( ((WM_KEY_INFO *)(pMsg->Data.p))->Key )
			{
				case GUI_KEY_UP:
					_changeAddrEdit( hWin, Id, GUI_KEY_UP, curPosSetAddr );
					break;

				case GUI_KEY_DOWN:
				 	_changeAddrEdit( hWin, Id, GUI_KEY_DOWN, curPosSetAddr );
					break;

				case GUI_KEY_ENTER:
					++curPosSetAddr;
					if ( curPosSetAddr > 2 ) curPosSetAddr = 0;
					_changeAddrEdit( hWin, Id, GUI_KEY_ENTER, curPosSetAddr );
					break;
/*
				case GUI_KEY_TAB:
					curPos = 0;
					EDIT_SetCursorAtChar( hWin, curPos );
					break;
  */
				default: WM_DefaultProc(pMsg);
			}
					
			break;

		case WM_PAINT:
			_OnPaintSetAddr(hWin);
			break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch( NCode )
			{
				case WM_NOTIFICATION_RELEASED:
					if ( Id == GUI_ID_OK )
					{
						if(_checkUserInputIP(hWin)==TRUE)
						{	
							_getUserInputIP(hWin);
														
							GUI_EndDialog( hWin, 0 );						
							WM_SetFocus(_hMenu);
							currentWin = USER_ID_MAIN_WIN;
							if(EEPROM_WriteStruct(optionSaveStruct)==FALSE)
							{
								printf("IP Save Error!");
							}
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

