/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_TestIO.c
** Last modified Date:      2014.07.25
** Last Version:            1.0
** Description:             Header file of the IO Test Dialog 输入输出口测试
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              GuoBingrong 郭冰容
** Created date:            2014.07.25
** Version:                 1.0
** Descriptions:            The original version 初始版本

*********************************************************************************************************/
#include "lpc177x_8x_rtc.h"

#include "DataStruct.h"
#include "MENU.h"
#include "LISTVIEW.h"
#include "user_menu.h"
#include "com_uart.h"
#include "os_cpu.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

extern MENU_Handle _hMenu;
extern int currentWin;

extern OS_EVENT  *UartRcvTaskSem;
extern RcvStruct *uart1RcvInfo;
extern INT8U      uart1RcvOK;

char* _aTableUartRcv[5];						//测点显示的表格内数据

/*******************************************
	      输入输出口测试窗口所有控件

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateTestIO[12] = 
{	
	{ FRAMEWIN_CreateIndirect, "输入输出口测试", 0, 10, 10, 440, 240, 0, 0 },
	
	{ LISTVIEW_CreateIndirect, NULL, GUI_ID_LISTVIEW0,    ADDSTEP, EDITXSIZE2+5,        280, 140, 0, 50 },
	{ RADIO_CreateIndirect,    NULL, GUI_ID_RADIO0,      340, EDITXSIZE3,  EDITXSIZE3, EDITYSIZE, 0, 50 },
  { RADIO_CreateIndirect,    NULL, GUI_ID_RADIO1,      380, EDITXSIZE3,  EDITXSIZE3, EDITYSIZE, 0, 50 },
  { RADIO_CreateIndirect,    NULL, GUI_ID_RADIO2,      340, EDITXSIZE5,  EDITXSIZE3, EDITYSIZE, 0, 50 },
	{ RADIO_CreateIndirect,    NULL, GUI_ID_RADIO3,      380, EDITXSIZE5,  EDITXSIZE3, EDITYSIZE, 0, 50 },
  { RADIO_CreateIndirect,    NULL, GUI_ID_RADIO4,      340, EDITXSIZE7,  EDITXSIZE3, EDITYSIZE, 0, 50 },
  { RADIO_CreateIndirect,    NULL, GUI_ID_RADIO5,      380, EDITXSIZE7,  EDITXSIZE3, EDITYSIZE, 0, 50 },
	{ RADIO_CreateIndirect,    NULL, GUI_ID_RADIO6,      340, EDITXSIZE9,  EDITXSIZE3, EDITYSIZE, 0, 50 },
  { RADIO_CreateIndirect,    NULL, GUI_ID_RADIO7,      380, EDITXSIZE9,  EDITXSIZE3, EDITYSIZE, 0, 50 },

	{ BUTTON_CreateIndirect, "开始", GUI_ID_OK,     250, 184, 60, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_CLOSE,  340, 184, 60, 20, 0, 0 },
};

/*******************************************
	      输入输出口测试界面绘制

********************************************/
void _OnPaintTestIO( WM_HWIN hWin )
{
	int i;
	INT8U err;
	GUI_RECT  r;
	char s[16];
	WM_HWIN hListview;
	
	hListview = WM_GetDialogItem( hWin, GUI_ID_LISTVIEW0 );

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);

	GUI_DispStringAt( "输入口数据",        r.x0+ADDSTEP,     r.y0+EDITXSIZE-4 );
	GUI_DispStringAt( "输出控制",        r.x0+ADDSTEP10+ADDSTEP4,    r.y0+EDITXSIZE-4 );
	
	GUI_DispStringAt( "1.",        r.x0+324,     r.y0+EDITXSIZE3 );
	GUI_DispStringAt( "2.",        r.x0+364,     r.y0+EDITXSIZE3 );
	GUI_DispStringAt( "3.",        r.x0+324,     r.y0+EDITXSIZE5 );
	GUI_DispStringAt( "4.",        r.x0+364,     r.y0+EDITXSIZE5 );
	GUI_DispStringAt( "5.",        r.x0+324,     r.y0+EDITXSIZE7 );
	GUI_DispStringAt( "6.",        r.x0+364,     r.y0+EDITXSIZE7 );
	GUI_DispStringAt( "7.",        r.x0+324,     r.y0+EDITXSIZE9 );
	GUI_DispStringAt( "8.",        r.x0+364,     r.y0+EDITXSIZE9 );
	
	
	OSSemPend(UartRcvTaskSem, 0, &err );
	if ( uart1RcvOK == 1 )
	{
			for ( i = 0; i < 16; ++i )
			{
				sprintf( s, "%d", i + 1 );
				LISTVIEW_SetItemText( hListview, 0, i, s );
		
				sprintf( s, "%d", uart1RcvInfo->Fre[i] );
				LISTVIEW_SetItemText( hListview, 1, i, s );
		
				sprintf( s, "%d", uart1RcvInfo->AD[i] );
				LISTVIEW_SetItemText( hListview, 2, i, s );
		
				sprintf( s, "%d", uart1RcvInfo->count[i] );
				LISTVIEW_SetItemText( hListview, 3, i, s );
		
				sprintf( s, "%d", ( ( uart1RcvInfo->sel >> i) & 1 ) );
				LISTVIEW_SetItemText( hListview, 4, i, s );
			}
	}
	else 
	{
			for ( i = 0; i < 16; ++i )
			{
				sprintf( s, "%d", i + 1 );
				LISTVIEW_SetItemText( hListview, 0, i, s );
				
				LISTVIEW_SetItemText( hListview, 1, i, "NULL" );
				LISTVIEW_SetItemText( hListview, 2, i, "NULL" );
				LISTVIEW_SetItemText( hListview, 3, i, "NULL" );
				LISTVIEW_SetItemText( hListview, 4, i, "NULL" );
			}
	}
	OSSemPost(UartRcvTaskSem);
	
	return;
}

/*******************************************
	      串口测试窗口初始化
********************************************/

void _InitTestIODialog(WM_HWIN hWin)
{
	int i;
	WM_HWIN hListview;
	char s[4];

	hListview = WM_GetDialogItem( hWin, GUI_ID_LISTVIEW0 );
	
	LISTVIEW_AddColumn( hListview, EDITXSIZE3+5, "No.", GUI_TA_HCENTER );
	LISTVIEW_AddColumn( hListview, EDITXSIZE4, "频率", GUI_TA_HCENTER );
	LISTVIEW_AddColumn( hListview, EDITXSIZE4, "AD", GUI_TA_HCENTER );
	LISTVIEW_AddColumn( hListview, EDITXSIZE4, "计数", GUI_TA_HCENTER );
	LISTVIEW_AddColumn( hListview, EDITXSIZE3, "开关", GUI_TA_HCENTER );

	LISTVIEW_SetBkColor( hListview, LISTVIEW_CI_UNSEL, GUI_WHITE );	
	LISTVIEW_SetGridVis( hListview, 0 );
	LISTVIEW_SetTextColor( hListview, LISTVIEW_CI_UNSEL, GUI_BLACK );
	LISTVIEW_SetRowHeight( hListview, 23 );
	
	for ( i = 0; i < 5; ++i )
	{
		_aTableUartRcv[i]=(char *)malloc(sizeof(_aTableUartRcv[i]));
		memset(_aTableUartRcv[i],'\0',sizeof(_aTableUartRcv[i]));
		LISTVIEW_SetTextAlign( hListview, i, GUI_TA_VCENTER );
	}
	
	strcpy( _aTableUartRcv[1], "NULL" );
	strcpy( _aTableUartRcv[2], "NULL" );
	strcpy( _aTableUartRcv[3], "NULL" );
	strcpy( _aTableUartRcv[4], "NULL" );		

	for ( i = 0; i < 16; ++i )
	{
		sprintf( s, "%d", i + 1 );
		strcpy( _aTableUartRcv[0], s );
		LISTVIEW_AddRow( hListview, _aTableUartRcv );
	}
	
	return;
}

/*******************************************
	      串口测试窗口的回调函数

********************************************/


void _cbTestIO( WM_MESSAGE *pMsg )
{
	int Id, NCode;
	SendStruct uart1SendInfo;
	WM_HWIN hRadio;
	WM_HWIN hWin = pMsg->hWin;
	unsigned KeyVal = 0;
	
	uart1SendInfo.clearFreCount = 0;
	uart1SendInfo.outPut = 0;
	uart1SendInfo.SEL = 0;

	KeyVal = LISTVIEW_GetEnterInfo();
	
	//printf("%d %d %d\n", currentWin, USER_ID_MENU_TEST_IO, KeyVal );
	
	if ( currentWin == USER_ID_MENU_TEST_IO && KeyVal ){
				switch ( KeyVal ){
						case GUI_KEY_TAB:
								WM_SetFocus( WM_GetDialogItem( hWin, GUI_ID_RADIO0 ) );
								break;
						
				}
		}

	switch( pMsg->MsgId )
	{
		case WM_INIT_DIALOG:
			_InitTestIODialog(hWin);
			break;
					
		case WM_PAINT:
			_OnPaintTestIO(hWin);
			break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch( NCode )
			{
				case WM_NOTIFICATION_RELEASED:
					if ( Id == GUI_ID_OK )
					{
							hRadio =  WM_GetDialogItem( hWin, GUI_ID_RADIO0 );
							//printf("radio = %d\n", RADIO_GetValue(hRadio) );
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

