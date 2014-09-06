/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File Name:               user_BrowsTestpoint.c
** Last modified Date:      2014.07.23
** Last Version:            1.0
** Description:             Header file of the Set Connect Dialog 测点查询
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              GuoBingrong 郭冰容
** Created date:            2014.07.23
** Version:                 1.0
** Descriptions:            

*********************************************************************************************************/
#include "lpc177x_8x_rtc.h"

#include "GUI.h"
#include "DIALOG.h"
#include "DataStruct.h"
#include "MENU.h"
#include "user_menu.h"
#include "EquipmentList.h"
#include "Sensor.h"
#include "stdio.h"

extern MENU_Handle _hListview;
extern int currentWin;

extern char *getPortType( INT16U portType );
extern char *getSensorName( INT16U sensorName );


Testpoint currentTp;

void ShowConfigInfomation( GUI_RECT r );

/*******************************************
	              测点查询

********************************************/
const GUI_WIDGET_CREATE_INFO _aDialogCreateBrowsTestpoint[3] = 
{	
	{ FRAMEWIN_CreateIndirect, "测点查询", 0, 10, 10, 280, 260, 0, 0 },
	{ RADIO_CreateIndirect, "", 0, 0, 0, 20, 20, 0, 0 },

	{ BUTTON_CreateIndirect, "退出", GUI_ID_CANCEL, 180, 210, 60, 20, 0, 0 },
};

/*******************************************
	      绘制通讯设置窗口

********************************************/
void _OnPaintBrowsTestpoint( WM_HWIN hWin )
{
		GUI_RECT  r;
		char s[32];
		Analogue_Config     *_analogue;
		Switch_Config       *_switch;
		Accumulate_Config   *_accumulate;
		Tristate_Config     *_tristate;
	
		WM_GetClientRectEx(hWin, &r);
		GUI_SetColor(GUI_BLACK);
		GUI_SetFont(&GUI_FontHZ16);
	
		if ( currentTp.EquipmentType == 0 ) return;
	
		switch( currentTp.EquipmentType )
		{
        case SENSOR_ANALOGUE:
            _analogue = ( Analogue_Config* )currentTp.config;
            if ( _analogue == NULL ) return;
						
						GUI_DispStringAt( "测点编号:",          r.x0+ADDSTEP,     r.y0+EDITXSIZE );
						GUI_DispStringAt( "传感器名称:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE2+7 );
						GUI_DispStringAt( "断电上下限:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE3+14 );
						GUI_DispStringAt( "复电上下限:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE4+21 );
						GUI_DispStringAt( "报警上下限:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE5+28 );
						GUI_DispStringAt( "解报上下限:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE6+35 );
						GUI_DispStringAt( "传感器量程:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE7+42 );
						GUI_DispStringAt( "比例系数:",          r.x0+ADDSTEP,     r.y0+EDITXSIZE8+49 );
						GUI_DispStringAt( "输出口控制:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE9+56 );
						
						sprintf( s, "%u", _analogue->testpointNo );
						GUI_DispStringAt( s,          r.x0+ADDSTEP6,     r.y0+EDITXSIZE );
						GUI_DispStringAt( getSensorName( _analogue->sensorName ),        r.x0+ADDSTEP6,     r.y0+EDITXSIZE2+7 );
						sprintf( s, "%u-%u", _analogue->outage.minval, _analogue->outage.maxval );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE3+14 );
						sprintf( s, "%u-%u", _analogue->recover.minval, _analogue->recover.maxval );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE4+21 );
						sprintf( s, "%u-%u", _analogue->alarm.minval, _analogue->alarm.maxval );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE5+28 );
						sprintf( s, "%u-%u", _analogue->liftAlarm.minval, _analogue->liftAlarm.maxval );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE6+35 );
						sprintf( s, "%u-%u", _analogue->measureRange.minval, _analogue->measureRange.maxval );						
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE7+42 );
						sprintf( s, "%.3f", _analogue->coefficient );
						GUI_DispStringAt( s,          r.x0+ADDSTEP6,     r.y0+EDITXSIZE8+49 );
						
						GUI_DispStringAt( "输出口控制:",        r.x0+ADDSTEP6,     r.y0+EDITXSIZE9+56 );
            
            break;

        case SENSOR_SWITCH:
            _switch = ( Switch_Config* )currentTp.config;
            if ( _switch == NULL ) return;
            GUI_DispStringAt( "测点编号:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE );
						GUI_DispStringAt( "传感器名称:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE2+7 );
						GUI_DispStringAt( "输出口控制:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE3+14 );
            
						sprintf( s, "%u", _switch->testpointNo );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE );
						GUI_DispStringAt( getSensorName( _switch->sensorName ),        r.x0+ADDSTEP6,     r.y0+EDITXSIZE2+7 );
						GUI_DispStringAt( "输出口控制:",        r.x0+ADDSTEP6,     r.y0+EDITXSIZE3+14 );
						break;

        case SENSOR_ACCUMULATE:
            _accumulate = ( Accumulate_Config* )currentTp.config;
            if ( _accumulate == NULL ) return;
						GUI_DispStringAt( "测点编号:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE );
						GUI_DispStringAt( "传感器名称:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE2+7 );
						GUI_DispStringAt( "输出范围:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE3+14 );
						GUI_DispStringAt( "比例系数:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE4+21 );
						
						sprintf( s, "%u", _accumulate->testpointNo );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE );
						GUI_DispStringAt( getSensorName( _accumulate->sensorName ),        r.x0+ADDSTEP6,     r.y0+EDITXSIZE2+7 );
						sprintf( s, "%u", _accumulate->measureMaxVal );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE3+14 );
						sprintf( s, "%u", _accumulate->coefficient );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE4+21 );
			
            break;

        case SENSOR_TRISTATE:
            _tristate = ( Tristate_Config* )currentTp.config;
            if ( _tristate == NULL ) return;
						GUI_DispStringAt( "测点编号:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE );
						GUI_DispStringAt( "传感器名称:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE2+7 );
						GUI_DispStringAt( "上限值:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE3+14 );
						GUI_DispStringAt( "下限值:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE4+21 );
						GUI_DispStringAt( "输出口控制:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE5+28 );
						
						sprintf( s, "%u", _tristate->testpointNo );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE );
						GUI_DispStringAt( getSensorName( _tristate->sensorName ),        r.x0+ADDSTEP6,     r.y0+EDITXSIZE2+7 );
						
						GUI_DispStringAt( "输出口控制:",        r.x0+ADDSTEP6,     r.y0+EDITXSIZE5+28 );
            break;
				
				default: break;
    }
		
		return;
}

/*******************************************
	      分站类型设置窗口初始化
       下拉菜单内的值为系统当前设置值
********************************************/

void _InitBrowsTestpointDialog(WM_HWIN hWin)
{
		GUI_RECT  r;
		char s[32];
		Analogue_Config     *_analogue;
		Switch_Config       *_switch;
		Accumulate_Config   *_accumulate;
		Tristate_Config     *_tristate;
	
		WM_GetClientRectEx(hWin, &r);
		GUI_SetColor(GUI_BLACK);
		GUI_SetFont(&GUI_FontHZ16);
	
		if ( currentTp.EquipmentType == 0 ) return;
	
		switch( currentTp.EquipmentType )
		{
        case SENSOR_ANALOGUE:
            _analogue = ( Analogue_Config* )currentTp.config;
            if ( _analogue == NULL ) return;
						
						GUI_DispStringAt( "测点编号:",          r.x0+ADDSTEP,     r.y0+EDITXSIZE );
						GUI_DispStringAt( "传感器名称:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE2+7 );
						GUI_DispStringAt( "断电上下限:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE3+14 );
						GUI_DispStringAt( "复电上下限:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE4+21 );
						GUI_DispStringAt( "报警上下限:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE5+28 );
						GUI_DispStringAt( "解报上下限:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE6+35 );
						GUI_DispStringAt( "传感器量程:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE7+42 );
						GUI_DispStringAt( "比例系数:",          r.x0+ADDSTEP,     r.y0+EDITXSIZE8+49 );
						GUI_DispStringAt( "输出口控制:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE9+56 );
						
						sprintf( s, "%u", _analogue->testpointNo );
						GUI_DispStringAt( s,          r.x0+ADDSTEP6,     r.y0+EDITXSIZE );
						GUI_DispStringAt( getSensorName( _analogue->sensorName ),        r.x0+ADDSTEP6,     r.y0+EDITXSIZE2+7 );
						sprintf( s, "%u-%u", _analogue->outage.minval, _analogue->outage.maxval );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE3+14 );
						sprintf( s, "%u-%u", _analogue->recover.minval, _analogue->recover.maxval );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE4+21 );
						sprintf( s, "%u-%u", _analogue->alarm.minval, _analogue->alarm.maxval );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE5+28 );
						sprintf( s, "%u-%u", _analogue->liftAlarm.minval, _analogue->liftAlarm.maxval );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE6+35 );
						sprintf( s, "%u-%u", _analogue->measureRange.minval, _analogue->measureRange.maxval );						
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE7+42 );
						sprintf( s, "%.3f", _analogue->coefficient );
						GUI_DispStringAt( s,          r.x0+ADDSTEP6,     r.y0+EDITXSIZE8+49 );
						
						GUI_DispStringAt( "输出口控制:",        r.x0+ADDSTEP6,     r.y0+EDITXSIZE9+56 );
            
            break;

        case SENSOR_SWITCH:
            _switch = ( Switch_Config* )currentTp.config;
            if ( _switch == NULL ) return;
            GUI_DispStringAt( "测点编号:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE );
						GUI_DispStringAt( "传感器名称:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE2+7 );
						GUI_DispStringAt( "输出口控制:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE3+14 );
            
						sprintf( s, "%u", _switch->testpointNo );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE );
						GUI_DispStringAt( getSensorName( _switch->sensorName ),        r.x0+ADDSTEP6,     r.y0+EDITXSIZE2+7 );
						GUI_DispStringAt( "输出口控制:",        r.x0+ADDSTEP6,     r.y0+EDITXSIZE3+14 );
						break;

        case SENSOR_ACCUMULATE:
            _accumulate = ( Accumulate_Config* )currentTp.config;
            if ( _accumulate == NULL ) return;
						GUI_DispStringAt( "测点编号:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE );
						GUI_DispStringAt( "传感器名称:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE2+7 );
						GUI_DispStringAt( "输出范围:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE3+14 );
						GUI_DispStringAt( "比例系数:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE4+21 );
						
						sprintf( s, "%u", _accumulate->testpointNo );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE );
						GUI_DispStringAt( getSensorName( _accumulate->sensorName ),        r.x0+ADDSTEP6,     r.y0+EDITXSIZE2+7 );
						sprintf( s, "%u", _accumulate->measureMaxVal );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE3+14 );
						sprintf( s, "%u", _accumulate->coefficient );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE4+21 );
			
            break;

        case SENSOR_TRISTATE:
            _tristate = ( Tristate_Config* )currentTp.config;
            if ( _tristate == NULL ) return;
						GUI_DispStringAt( "测点编号:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE );
						GUI_DispStringAt( "传感器名称:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE2+7 );
						GUI_DispStringAt( "上限值:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE3+14 );
						GUI_DispStringAt( "下限值:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE4+21 );
						GUI_DispStringAt( "输出口控制:",        r.x0+ADDSTEP,     r.y0+EDITXSIZE5+28 );
						
						sprintf( s, "%u", _tristate->testpointNo );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE );
						GUI_DispStringAt( getSensorName( _tristate->sensorName ),        r.x0+ADDSTEP6,     r.y0+EDITXSIZE2+7 );
				/*
						sprintf( s, "%u", _tristate->correctRange.maxval );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE3+14 );
						sprintf( s, "%u", _tristate->correctRange.minval );
						GUI_DispStringAt( s,        r.x0+ADDSTEP6,     r.y0+EDITXSIZE4+21 );
				*/		
						GUI_DispStringAt( "输出口控制:",        r.x0+ADDSTEP6,     r.y0+EDITXSIZE5+28 );
            break;
				
				default: break;
    }

		return;
}

/*******************************************
	      分站类型设置窗口的回调函数

********************************************/


void _cbBrowsTestpoint( WM_MESSAGE *pMsg )
{
	int Id, NCode;
	WM_HWIN hWin = pMsg->hWin;

	switch( pMsg->MsgId )
	{
		case WM_INIT_DIALOG:
			_InitBrowsTestpointDialog(hWin);
			break;
					
		case WM_PAINT:
			_OnPaintBrowsTestpoint(hWin);
			break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch( NCode )
			{
				case WM_NOTIFICATION_RELEASED:
					if ( Id == GUI_ID_CANCEL )
					{
						GUI_EndDialog( hWin, 0 );
						WM_SetFocus( _hListview );
						currentWin = USER_ID_MENU_DISP_TESTPOINT;
					}
					break;
				
				default: break;
			}
			break;

		default: 
		WM_DefaultProc(pMsg);
	}
}

