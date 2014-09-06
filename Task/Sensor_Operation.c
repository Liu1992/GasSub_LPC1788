#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EquipmentList.h"
#include "Sensor.h"
#include "com_uart.h"


/* ���������Ʊ� */
char SensorTable[40][16] = {
		"��˹", "�ߵ�Ũ����˹", "����", "�¶�", "��ѹ",
		"��ѹ", "ˮλ", "�۳�", "����", "һ����̼", 
		"������̼", "�ܵ���˹", "�ܵ��¶�", "�ܵ���ѹ", "�ܵ�����",
	
	  "��ͣ", "����", "����", "���", "��Ͳ",
		"����", "����", "", "", "",
		
		"����", "����", "�ۼƷ�ʽ", "", "",
		
		"��ͣ", "����", "����", "���", "��Ͳ",
		"����", "����", "", "", "",
};

/* �˿ڱ� */
char PortTable[20][16] = {
		"1", "2", "3", "4", 
		"5", "6", "7", "8", 
		"9", "10", "11", "12", 
		"13", "14", "15", "16",
		"485-1", "485-2", "485-3", "485-4"
};

//extern Upload_Info netUploadData;    /* ��Ҫͨ�������ϴ������� */

void setConfig_Analogue( void* dest, void* src );     /* ����ģ������������Ϣ */
void setConfig_Switch( void* dest, void* src );       /* ���ÿ�������������Ϣ */
void setConfig_Accumulate( void* dest, void* src );   /* �����ۼ�����������Ϣ */
void setConfig_Tristate( void* dest, void* src );     /* ������̬��������������Ϣ */

void setUpload(Upload_Info *dest, Upload_Info *src);  /* ��ȡ������ϴ���Ϣ */

void uploadData( Upload_Info *data );                 /* �����ȡ��Ҫ�ϴ�����Ϣ */

Upload_Info 			getUploadInfo(Upload_Info src);     /* ��ȡ�ϴ���Ϣ */
TestpointShowInfo getShowInfo( Testpoint *tp );       /* ��ȡ�����ʾ��Ϣ */
void showAllInfo(Testpoint *tp);                      /* �����ã���ʾ�����������Ϣ */

/* �ɴ�������Ż�ȡ���������� */
char *getSensorName( INT16U sensorName )
{
		switch( sensorName / 10000 )
		{
				case SENSOR_ANALOGUE: return SensorTable[ sensorName%100-1 ];
				case SENSOR_SWITCH: return SensorTable[ sensorName%10-1+15 ];
				case SENSOR_ACCUMULATE: return SensorTable[ sensorName%10-1+25 ];
				case SENSOR_TRISTATE: return SensorTable[ sensorName%10-1+30 ];
		}
	
		return "\0";
}

/* �ɶ˿ڱ�Ż�ȡ�˿����� */
char *getPortType( INT16U portType )
{
		if ( portType < 64 )
		{
				return PortTable[portType%16];
		}
		else
		{
				return PortTable[16];
		}
}

void getDataFromUart_Analogue( RcvStruct *rev, Testpoint *tp )
{
		INT32U movebit;
		Analogue_Config     *_analogue = NULL;
	
		if ( rev == NULL || tp == NULL ) return;
		
		_analogue = (Analogue_Config *)tp->config;		
		movebit = _analogue->portType % 16;
	
		switch ( _analogue->portType / 16 )
		{
				case PORT_FREQ:
					tp->upload.collectData = rev->Fre[ movebit ];
					break;
				
				case PORT_AD:
					tp->upload.collectData = rev->AD[ movebit ];
					break;
				
				case PORT_COUNT:				
					tp->upload.collectData = rev->count[ movebit ];
					break;
				
				case PORT_SEL:
					tp->upload.collectData = ( rev->sel >> movebit ) & 1;
					break;
		}
	
		return;
}

void getDataFromUart_Switch( RcvStruct *rev, Testpoint *tp )
{
    Switch_Config       *_switch = NULL;
		INT32U movebit;
	
		if ( rev == NULL || tp == NULL ) return;
		
		_switch = (Switch_Config *)tp->config;		
		movebit = _switch->portType % 16;
	
		switch ( _switch->portType / 16 )
		{
				case PORT_FREQ:
					tp->upload.collectData = rev->Fre[ movebit ];
					break;
				
				case PORT_AD:
					tp->upload.collectData = rev->AD[ movebit ];
					break;
				
				case PORT_COUNT:				
					tp->upload.collectData = rev->count[ movebit ];
					break;
				
				case PORT_SEL:
					tp->upload.collectData = ( rev->sel >> movebit ) & 1;
					break;
		}
	
		return;
}

void getDataFromUart_Accumulate( RcvStruct *rev, Testpoint *tp )
{
		INT32U movebit;
    Accumulate_Config   *_accumulate = NULL;
	
		if ( rev == NULL || tp == NULL ) return;
		
		_accumulate = (Accumulate_Config *)tp->config;		
		movebit = _accumulate->portType % 16;
	
		switch ( _accumulate->portType / 16 )
		{
				case PORT_FREQ:
					tp->upload.collectData = rev->Fre[ movebit ];
					break;
				
				case PORT_AD:
					tp->upload.collectData = rev->AD[ movebit ];
					break;
				
				case PORT_COUNT:				
					tp->upload.collectData = rev->count[ movebit ];
					break;
				
				case PORT_SEL:
					tp->upload.collectData = ( rev->sel >> movebit ) & 1;
					break;
		}
	
		return;
}

void getDataFromUart_Tristate( RcvStruct *rev, Testpoint *tp )
{
    Tristate_Config     *_tristate = NULL;
		INT32U movebit;
	
		if ( rev == NULL || tp == NULL ) return;
		
		_tristate = (Tristate_Config *)tp->config;		
		movebit = _tristate->portType % 16;
	
		switch ( _tristate->portType / 16 )
		{
				case PORT_FREQ:
					tp->upload.collectData = rev->Fre[ movebit ];
					break;
				
				case PORT_AD:
					tp->upload.collectData = rev->AD[ movebit ];
					break;
				
				case PORT_COUNT:				
					tp->upload.collectData = rev->count[ movebit ];
					break;
				
				case PORT_SEL:
					tp->upload.collectData = ( rev->sel >> movebit ) & 1;
					break;
		}
	
		return;
}

/* ��ȡģ�����������Ĳ����ʾ��Ϣ */
TestpointShowInfo getShowInfo_Analogue(Testpoint *tp)
{
		INT32S aaa;
    Analogue_Config     *_analogue;
    TestpointShowInfo testShow = { "\0", "\0", "\0", "\0", "\0" };

    if ( tp == NULL ) return testShow;

    _analogue = ( Analogue_Config* )tp->config;
    if ( _analogue == NULL ) return testShow;
		strcpy( testShow.location, _analogue->location );
		
		aaa = _analogue->portType / 16;
		if ( aaa == PORT_FREQ )
			sprintf( testShow.sensorType, "%s������(F)", getSensorName(_analogue->sensorName) );
		else if ( aaa == PORT_AD )
			sprintf( testShow.sensorType, "%s������(D)", getSensorName(_analogue->sensorName) );

		strcpy( testShow.portType, getPortType(_analogue->portType) );

    sprintf( testShow.testpointNo, "%d", tp->upload.testpointNo );
    sprintf( testShow.collectData, "%d", tp->upload.collectData );

    return testShow;
}

/* ��ȡ�������������Ĳ����ʾ��Ϣ */
TestpointShowInfo getShowInfo_Switch(Testpoint *tp)
{
    Switch_Config       *_switch;
    TestpointShowInfo testShow = { "\0", "\0", "\0", "\0", "\0" };

    if ( tp == NULL ) return testShow;

		_switch = ( Switch_Config* )tp->config;
		if ( _switch == NULL ) return testShow;
		sprintf( testShow.location, "%s", _switch->location );
		sprintf( testShow.sensorType, "%s������", getSensorName(_switch->sensorName) );
		strcpy( testShow.portType, getPortType(_switch->portType) );
	 
    sprintf( testShow.testpointNo, "%d", tp->upload.testpointNo );
    sprintf( testShow.collectData, "%d", tp->upload.collectData );

    return testShow;
}

/* ��ȡ�ۼ����������Ĳ����ʾ��Ϣ */
TestpointShowInfo getShowInfo_Accumulate(Testpoint *tp)
{
    Accumulate_Config   *_accumulate;
    TestpointShowInfo testShow = { "\0", "\0", "\0", "\0", "\0" };

    if ( tp == NULL ) return testShow;

		_accumulate = ( Accumulate_Config* )tp->config;
		if ( _accumulate == NULL ) return testShow;
		sprintf( testShow.location, "%s", _accumulate->location );
		sprintf( testShow.sensorType, "%s������", getSensorName(_accumulate->sensorName) );
		strcpy( testShow.portType, getPortType(_accumulate->portType) );        

    sprintf( testShow.testpointNo, "%d", tp->upload.testpointNo );
    sprintf( testShow.collectData, "%d", tp->upload.collectData );

    return testShow;
}

/* ��ȡ��̬�������������Ĳ����ʾ��Ϣ */
TestpointShowInfo getShowInfo_Tristate(Testpoint *tp)
{
    Tristate_Config     *_tristate;
    TestpointShowInfo testShow = { "\0", "\0", "\0", "\0", "\0" };

    if ( tp == NULL ) return testShow;

		_tristate = ( Tristate_Config* )tp->config;
		if ( _tristate == NULL ) return testShow;
		sprintf( testShow.location, "%s", _tristate->location );
		sprintf( testShow.sensorType, "%s������", getSensorName(_tristate->sensorName) );
		strcpy( testShow.portType, getPortType(_tristate->portType) );

    sprintf( testShow.testpointNo, "%d", tp->upload.testpointNo );
    sprintf( testShow.collectData, "%d", tp->upload.collectData );

    return testShow;
}


/*
		Show All Config Info for Debug

*/
void showAllInfo(Testpoint *tp)
{
    Analogue_Config     *_analogue;
    Switch_Config       *_switch;
    Accumulate_Config   *_accumulate;
    Tristate_Config     *_tristate;

    if ( tp == NULL ) return;

    printf("The Upload Infomation below:\n");
    printf("testpointNo = %d\n", tp->upload.testpointNo );
    printf("collectData = %d\n", tp->upload.collectData );
    printf("sensorState = %d\n", tp->upload.sensorState );
    printf("collectTime = %s\n", tp->upload.collectTime );

    printf("The Config Infomation below:\n");
    switch( tp->EquipmentType )
    {
        case SENSOR_ANALOGUE:
            _analogue = ( Analogue_Config* )tp->config;
            if ( _analogue == NULL ) return;
            printf("testpointNo = %d\n", _analogue->testpointNo );
            printf("sensorName = %d\n",  _analogue->sensorName );
            printf("location = %s\n", _analogue->location );
            printf("portType = %d\n", _analogue->portType );
            printf("outputControl = %d\n", _analogue->outputControl );
            printf("outage : %d~%d\n", _analogue->outage.minval, _analogue->outage.maxval );
            printf("recover : %d~%d\n", _analogue->recover.minval, _analogue->recover.maxval );
            printf("alarm : %d~%d\n", _analogue->alarm.minval, _analogue->alarm.maxval );
            printf("liftAlarm : %d~%d\n", _analogue->liftAlarm.minval, _analogue->liftAlarm.maxval );
            printf("measureRange : %d~%d\n", _analogue->measureRange.minval, _analogue->measureRange.maxval );
            printf("coefficient = %.1f\n", _analogue->coefficient );
            break;

        case SENSOR_SWITCH:
            _switch = ( Switch_Config* )tp->config;
            if ( _switch == NULL ) return;
            printf("testpointNo = %d\n", _switch->testpointNo );
            printf("sensorName = %d\n",  _switch->sensorName );
            printf("location = %s\n", _switch->location );
            printf("portType = %d\n", _switch->portType );
            printf("outputControl = %d\n", _switch->outputControl );
            break;

        case SENSOR_ACCUMULATE:
            _accumulate = ( Accumulate_Config* )tp->config;
            if ( _accumulate == NULL ) return;
            printf("testpointNo = %d\n", _accumulate->testpointNo );
            printf("sensorName = %d\n",  _accumulate->sensorName );
            printf("location = %s\n", _accumulate->location );
            printf("portType = %d\n", _accumulate->portType );
            printf("measureMaxVal = %d\n", _accumulate->measureMaxVal );
            printf("coefficient = %.3f\n", _accumulate->coefficient );
            break;

        case SENSOR_TRISTATE:
            _tristate = ( Tristate_Config* )tp->config;
            if ( _tristate == NULL ) return;
            printf("testpointNo = %d\n", _tristate->testpointNo );
            printf("sensorName = %d\n",  _tristate->sensorName );
            printf("location = %s\n", _tristate->location );
            printf("portType = %d\n", _tristate->portType );
            printf("outputControl = %d\n", _tristate->outputControl );
            printf("0̬: %d, 1̬��%d, 2̬��%d\n", _tristate->current[0], _tristate->current[1], _tristate->current[2] );
            break;
        default:
            break;
    }

    puts("*****************************");
    return;
}

/* �޸Ĳ������Ҫ�ϴ�����Ϣ */
void setUpload(Upload_Info *dest, Upload_Info *src)
{
    dest->collectData = src->collectData;
    dest->sensorState = src->sensorState;
    strcpy( dest->collectTime, src->collectTime );
    return;
}

/* ��ȡ�������Ҫ�ϴ�����Ϣ */
Upload_Info getUploadInfo(Upload_Info src)
{
    return src;
}

/* ����ģ���������� */
void setConfig_Analogue( void* dest, void* src )
{
    Analogue_Config *_dest, *_src;
    if ( dest == NULL || src == NULL ) return;

    _dest = ( Analogue_Config* )dest;
    _src = ( Analogue_Config* )src;

    _dest->testpointNo   = _src->testpointNo;
    _dest->sensorName    = _src->sensorName;
    _dest->portType      = _src->portType;
    _dest->outputControl[0] = _src->outputControl[0];
		_dest->outputControl[1] = _src->outputControl[1];

    _dest->outage        = _src->outage;
    _dest->recover       = _src->recover;
    _dest->alarm         = _src->alarm;
    _dest->liftAlarm     = _src->liftAlarm;
    _dest->measureRange  = _src->measureRange;

    _dest->coefficient   = _src->coefficient;

    strcpy( _dest->location, _src->location );
    return;
}

/* ���ÿ����������� */
void setConfig_Switch( void* dest, void* src )
{
    Switch_Config *_dest, *_src;
    if ( dest == NULL || src == NULL ) return;

    _dest = ( Switch_Config* )dest;
    _src  = ( Switch_Config* )src;

    _dest->testpointNo   = _src->testpointNo;
    _dest->sensorName    = _src->sensorName;
    _dest->portType      = _src->portType;
    _dest->outputControl[0] = _src->outputControl[0];
		_dest->outputControl[1] = _src->outputControl[1];

    strcpy( _dest->location, _src->location );

    return;
}

/* �����ۼ��������� */
void setConfig_Accumulate( void* dest, void* src )
{
    Accumulate_Config *_dest, *_src;
    if ( dest == NULL || src == NULL ) return;

    _dest = (Accumulate_Config *)dest;
    _src  = (Accumulate_Config *)src;

    _dest->testpointNo   = _src->testpointNo;
    _dest->sensorName    = _src->sensorName;

    _dest->portType      = _src->portType;
    _dest->measureMaxVal = _src->measureMaxVal;
    _dest->coefficient   = _src->coefficient;
		_dest->shift 				 = _src->shift;

    strcpy( _dest->location, _src->location );
    return;
}

/* ������̬������������ */
void setConfig_Tristate( void* dest, void* src )
{
		int i = 0;
    Tristate_Config *_dest, *_src;
    if ( dest == NULL || src == NULL ) return;

    _dest = ( Tristate_Config * )dest;
    _src  = ( Tristate_Config * )src;

    _dest->testpointNo   = _src->testpointNo;
    _dest->sensorName    = _src->sensorName;

    _dest->portType      = _src->portType;
    _dest->outputControl[0] = _src->outputControl[0];
		_dest->outputControl[1] = _src->outputControl[1];
	
	
    for ( i = 0; i < 4; i++ )
			_dest->current[i]  = _src->current[i];

    strcpy( _dest->location, _src->location );
    
		return;
}

/* ģ�����Ƿ񱨾� */
BOOLEAN ifAlarm_Analogue( Testpoint *tp )
{
		Analogue_Config *analogue_conf;
		analogue_conf = (Analogue_Config *)tp->config;
		
		if ( analogue_conf->alarm.minval != VALUE_UNDEF )
		{
				if ( tp->upload.collectData <= analogue_conf->alarm.minval )
						return TRUE;
		}
		if ( analogue_conf->alarm.maxval != VALUE_UNDEF )
		{
				if ( tp->upload.collectData >= analogue_conf->alarm.maxval )
						return TRUE;
		}
	
		return FALSE;
}

/* ������û�б������� */
BOOLEAN ifAlarm_Switch( Testpoint *tp )
{
		return FALSE;
}

/* �ۼ���û�б������� */
BOOLEAN ifAlarm_Accumulate( Testpoint *tp )
{
		return FALSE;
}

/* ��̬������û�б������� */
BOOLEAN ifAlarm_Tristate( Testpoint *tp )
{
		return FALSE;
}

/* �ж�ģ�����������Ƿ������� */
BOOLEAN ifLiftAlarm_Analogue( Testpoint *tp )
{
		Analogue_Config *analogue_conf;
		analogue_conf = (Analogue_Config *)tp->config;
		
		if ( analogue_conf->liftAlarm.minval != VALUE_UNDEF )
		{
				if ( tp->upload.collectData < analogue_conf->liftAlarm.minval )
						return TRUE;
		}
		if ( analogue_conf->liftAlarm.maxval != VALUE_UNDEF )
		{
				if ( tp->upload.collectData > analogue_conf->liftAlarm.maxval )
						return TRUE;
		}
		return FALSE;
}

BOOLEAN ifLiftAlarm_Switch( Testpoint *tp )
{
		return FALSE;
}

BOOLEAN ifLiftAlarm_Accumulate( Testpoint *tp )
{
		return FALSE;
}

BOOLEAN ifLiftAlarm_Tristate( Testpoint *tp )
{
		return FALSE;
}

/* �ж�ģ�����������Ƿ񵽴￪������ */
BOOLEAN ifOutput_Analogue( Testpoint *tp )
{
		Analogue_Config *analogue_conf;
		analogue_conf = (Analogue_Config *)tp->config;
		
		if ( analogue_conf->outage.minval != VALUE_UNDEF )
		{
				if ( tp->upload.collectData <= analogue_conf->outage.minval )
						return TRUE;
		}
		if ( analogue_conf->outage.maxval != VALUE_UNDEF )
		{
				if ( tp->upload.collectData >= analogue_conf->outage.maxval )
						return TRUE;
		}	
		return FALSE;
}

/* ������û������ڿ��� */
BOOLEAN ifOutput_Switch( Testpoint *tp )
{
		return FALSE;
}

/* �ۼ���û��������� */
BOOLEAN ifOutput_Accumulate( Testpoint *tp )
{
		return FALSE;
}

/* ��̬�������Ƿ�ﵽ������� */
BOOLEAN ifOutput_Tristate( Testpoint *tp )
{
	/*
		int i;
		Tristate_Config *tristate_conf;
		tristate_conf = ( Tristate_Config* )tp->config;
		
		if ( tp->upload.collectData == tristate_conf->current[1] )
				return TRUE;
	*/	
		return FALSE;
}

/* ����ģ�������������� */
INT8U controlOutput_Analogue( Testpoint *tp )
{
		int i = 0;
		INT8U ret = 0, bit0, bit1;
		Analogue_Config *analogue_conf;
		analogue_conf = (Analogue_Config *)tp->config;
		
		for ( i = 0; i < 8; i++ )
		{
				bit0 = ( analogue_conf->outputControl[0] >> i ) & 1;
				bit1 = ( analogue_conf->outputControl[1] >> i ) & 1;
				ret |= ((bit0 & bit1) << i);
		}
		
		printf("In function controlOutput: ret = %u\n", ret );
		return ret;
}

INT8U controlOutput_Switch( Testpoint *tp )
{
		return 0;
}
	
INT8U controlOutput_Accumulate( Testpoint *tp )
{
		return 0;
}

INT8U controlOutput_Tristate( Testpoint *tp )
{
	/*
		int i = 0;
		INT8U ret = 0, bit0, bit1;
		Tristate_Config *tristate_conf;
		tristate_conf = (Tristate_Config *)tp->config;
		
		for ( i = 0; i < 8; i++ )
		{
				bit0 = ( tristate_conf->outputControl[0] >> i ) & 1;
				bit1 = ( tristate_conf->outputControl[1] >> i ) & 1;
				ret = bit0 & bit1;
		}
	
		return ret;
	*/
	return 0;
}

/* �ж�ģ�����������Ƿ񸴵� */
BOOLEAN ifRecover_Analogue(Testpoint *tp)
{
		Analogue_Config *analogue_conf;
		analogue_conf = (Analogue_Config *)tp->config;
		
		if ( analogue_conf->recover.minval != VALUE_UNDEF )
		{
				if ( tp->upload.collectData <= analogue_conf->recover.minval )
						return TRUE;
		}
		if ( analogue_conf->recover.maxval != VALUE_UNDEF )
		{
				if ( tp->upload.collectData >= analogue_conf->recover.maxval )
						return TRUE;
		}	
		return FALSE;
}

BOOLEAN ifRecover_Switch( Testpoint *tp )
{
		return FALSE;
}

BOOLEAN ifRecover_Accumulate( Testpoint *tp )
{
		return FALSE;
}

/* ��̬������ ������� ���� */
BOOLEAN ifRecover_Tristate( Testpoint *tp )
{
		return FALSE;
}

INT8U controlRecover_Analogue( Testpoint *tp )
{
		int i = 0;
		INT8U ret = 0, bit0, bit1;
		Analogue_Config *analogue_conf;
		analogue_conf = (Analogue_Config *)tp->config;
		
		for ( i = 0; i < 8; i++ )
		{
				bit0 = ( analogue_conf->outputControl[0] >> i ) & 1;
				bit1 = ( analogue_conf->outputControl[1] >> i ) & 1;
				ret |= ((!(bit0 & bit1)) << i);
		}
	
		return ret;
}

INT8U controlRecover_Switch( Testpoint *tp )
{
		return 0;
}

INT8U controlRecover_Accumulate( Testpoint *tp )
{
		return 0;
}

INT8U controlRecover_Tristate( Testpoint *tp )
{
		return 0;
}

/* �����ȡ��Ҫ�ϴ�����Ϣ *
void uploadData( Upload_Info *data )
{
		netUploadData.testpointNo = data->testpointNo;
		netUploadData.sensorState = data->sensorState;
		netUploadData.collectData = data->collectData;
		strcpy( netUploadData.collectTime, data->collectTime );
	
		return;
}
*/

