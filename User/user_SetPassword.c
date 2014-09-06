/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_SetTime.c
** Last modified Date:      2014.04.25
** Last Version:            1.0
** Description:             Header file of the Set Time Dialog 设置密码对话框文件
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

extern MENU_Handle _hMenu;
extern int currentWin;
extern EEPROMDataStruct optionSaveStruct;

char BVtxt_temp[64];

/*******************************************
	      设置时间窗口所有控件

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateSetPassword[15] =
{
    { FRAMEWIN_CreateIndirect, "密码设置", 0, 10, 10, 320, 230, 0, 0 },

	
    { BUTTON_CreateIndirect, "1", GUI_ID_BUTTON0,      210, 40, 24, 24, 0, 0 },
    { BUTTON_CreateIndirect, "2", GUI_ID_BUTTON1,      240, 40, 24, 24, 0, 0 },
    { BUTTON_CreateIndirect, "3", GUI_ID_BUTTON2,      270, 40, 24, 24, 0, 0 },
    { BUTTON_CreateIndirect, "4", GUI_ID_BUTTON3,      210, 70, 24, 24, 0, 0 },
    { BUTTON_CreateIndirect, "5", GUI_ID_BUTTON4,      240, 70, 24, 24, 0, 0 },
    { BUTTON_CreateIndirect, "6", GUI_ID_BUTTON5,      270, 70, 24, 24, 0, 0 },
    { BUTTON_CreateIndirect, "7", GUI_ID_BUTTON6,      210, 100, 24, 24, 0, 0 },
    { BUTTON_CreateIndirect, "8", GUI_ID_BUTTON7,      240, 100, 24, 24, 0, 0 },
    { BUTTON_CreateIndirect, "9", GUI_ID_BUTTON8,      270, 100, 24, 24, 0, 0 },
    { BUTTON_CreateIndirect, "0", GUI_ID_BUTTON9,      210, 130, 24, 24, 0, 0 },
    
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 94, 40, 100, 24, 0, 0 },
	/*
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT1, 40, 70, 120, 24, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT2, 40, 100, 120, 24, 0, 0 },
	*/
	
	{ BUTTON_CreateIndirect, "删除", GUI_ID_BUTTON10, 240, 130, 54, 24, 0, 0},	
	{ BUTTON_CreateIndirect, "确定", GUI_ID_OK,       100, 170, 60, 24, 0, 0},
    { BUTTON_CreateIndirect, "取消", GUI_ID_CANCEL,   200, 170, 60, 24, 0, 0}    
    //
};

void PaintDialog_kb(WM_MESSAGE * pMsg)
{
	GUI_RECT  r;	
    WM_HWIN hWin = pMsg->hWin;

	WM_GetClientRectEx(hWin, &r);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ16);

	GUI_DispStringAt( "原密码:",     r.x0+14,  r.y0+42 );
	GUI_DispStringAt( "新密码:",     r.x0+14,  r.y0+74 );
	GUI_DispStringAt( "确认密码:",   r.x0+14,  r.y0+106 );

}
/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitDialog_kb(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMWIN
    //
    FRAMEWIN_SetTitleHeight(hWin,16);
    FRAMEWIN_SetFont(hWin,&GUI_FontHZ16);
    /*
    //GUI_ID_BUTTON0
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),&GUI_Font24B_ASCII);
    //
    //GUI_ID_BUTTON1
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON1),&GUI_Font24B_ASCII);
    //
    //GUI_ID_BUTTON2
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON2),&GUI_Font24B_ASCII);
    //
    //GUI_ID_BUTTON3
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON3),&GUI_Font24B_ASCII);
    //
    //GUI_ID_BUTTON4
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON4),&GUI_Font24B_ASCII);
    //
    //GUI_ID_BUTTON5
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON5),&GUI_Font24B_ASCII);
    //
    //GUI_ID_BUTTON6
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON6),&GUI_Font24B_ASCII);
    //
    //GUI_ID_BUTTON7
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON7),&GUI_Font24B_ASCII);
    //
    //GUI_ID_BUTTON8
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON8),&GUI_Font24B_ASCII);
    //
    //GUI_ID_BUTTON9
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON9),&GUI_Font24B_ASCII);
    //
    //GUI_ID_EDIT0
    // */
    //EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT0),&GUI_Font24_ASCII);
//        EDIT_SetValue(WM_GetDialogItem(hWin,GUI_ID_EDIT0),0);
    //EDIT_SetMaxLen(WM_GetDialogItem(hWin,GUI_ID_EDIT0),9);
//        EDIT_SetDecMode(WM_GetDialogItem(hWin,GUI_ID_EDIT0),0,0,999.9999,4,0);
    //
    //GUI_ID_BUTTON12
    //
    //BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON10),&GUI_Font24B_ASCII);
}

/*********************************************************************
*
*       Dialog callback routine
*/
void _cbSetPassword(WM_MESSAGE * pMsg)
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        PaintDialog_kb(pMsg);
        break;
    case WM_INIT_DIALOG:
        InitDialog_kb(pMsg);
        break;
    case WM_KEY:
        switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key)
        {
		/*
        case GUI_KEY_ESCAPE:
            GUI_EndDialog(hWin, 1);
            break;
        case GUI_KEY_ENTER:
            GUI_EndDialog(hWin, 0);
            break;
		*/
        }
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(NCode)
        {
        case  WM_NOTIFICATION_RELEASED:
            switch (Id)
            {
            case GUI_ID_OK:
                GUI_EndDialog(hWin, 0);
								WM_SetFocus(_hMenu);
                currentWin = USER_ID_MAIN_WIN;
								break;
            case GUI_ID_CANCEL:
                GUI_EndDialog(hWin, 0);
								WM_SetFocus(_hMenu);
                currentWin = USER_ID_MAIN_WIN;
								break;
            case GUI_ID_BUTTON0:
                EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),'1');
                break;
            case GUI_ID_BUTTON1:
                EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),'2');
                break;
            case GUI_ID_BUTTON2:
                EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),'3');
                break;
            case GUI_ID_BUTTON3:
                EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),'4');
                break;
            case GUI_ID_BUTTON4:
                EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),'5');
                break;
            case GUI_ID_BUTTON5:
                EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),'6');
                break;
            case GUI_ID_BUTTON6:
                EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),'7');

                break;
            case GUI_ID_BUTTON7:
                EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),'8');

                break;
            case GUI_ID_BUTTON8:
                EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),'9');

                break;
            case GUI_ID_BUTTON9:
                EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),'0');
                break;
            case GUI_ID_BUTTON10:
                EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),NULL);//清空输入框，挨个删除用下面两行
//                  WM_SetFocus(WM_GetDialogItem(hWin,GUI_ID_EDIT0));
//                  GUI_SendKeyMsg(GUI_KEY_BACKSPACE,1);
                break;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}
