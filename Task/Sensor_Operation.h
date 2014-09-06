#ifndef SENSOR_OPERATION_H_INCLUDED
#define SENSOR_OPERATION_H_INCLUDED

#include "Sensor.h"
#include "EquipmentList.h"

/* ���ô������ڵ���Ϣ */
void setConfig_Analogue( void* dest, void* src );     /* ����ģ������������Ϣ */
void setConfig_Switch( void* dest, void* src );       /* ���ÿ�������������Ϣ */
void setConfig_Accumulate( void* dest, void* src );   /* �����ۼ�����������Ϣ */
void setConfig_Tristate( void* dest, void* src );     /* ������̬��������������Ϣ */

void setUpload(Upload_Info *dest, Upload_Info *src);  /* ��ȡ������ϴ���Ϣ */

void uploadData( Upload_Info *data );                 /* �����ȡ��Ҫ�ϴ�����Ϣ */

Upload_Info 			getUploadInfo(Upload_Info src);     /* ��ȡ�ϴ���Ϣ */
TestpointShowInfo getShowInfo( Testpoint *tp );       /* ��ȡ�����ʾ��Ϣ */
void showAllInfo(Testpoint *tp);                      /* �����ã���ʾ�����������Ϣ */


char *getSensorName( INT16U sensorName );   /* ��������Ż�ȡ���������ƶ�Ӧ */
char *getPortType( INT16U portType );       /* �˿ڱ�Ż�ȡ�˿��������� */

/* �Ӵ��ڻ�ȡ��������Ϣ */
void getDataFromUart_Analogue( RcvStruct *rev, Testpoint *tp );
void getDataFromUart_Switch( RcvStruct *rev, Testpoint *tp );
void getDataFromUart_Accumulate( RcvStruct *rev, Testpoint *tp );
void getDataFromUart_Tristate( RcvStruct *rev, Testpoint *tp );

/* ��ȡ�������Ĳ����ʾ��Ϣ */
TestpointShowInfo getShowInfo_Analogue(Testpoint *tp);    /* ��ȡģ�����������Ĳ����ʾ��Ϣ */
TestpointShowInfo getShowInfo_Switch(Testpoint *tp);      /* ��ȡ�������������Ĳ����ʾ��Ϣ */
TestpointShowInfo getShowInfo_Accumulate(Testpoint *tp);  /* ��ȡ�ۼ����������Ĳ����ʾ��Ϣ */
TestpointShowInfo getShowInfo_Tristate(Testpoint *tp);    /* ��ȡ��̬�������������Ĳ����ʾ��Ϣ */

/* �жϴ������Ƿ񱨾� */
BOOLEAN ifAlarm_Analogue( Testpoint *tp );
BOOLEAN ifAlarm_Switch( Testpoint *tp );
BOOLEAN ifAlarm_Accumulate( Testpoint *tp );
BOOLEAN ifAlarm_Tristate( Testpoint *tp );

/* �жϴ������������ */
BOOLEAN ifLiftAlarm_Analogue( Testpoint *tp );
BOOLEAN ifLiftAlarm_Switch( Testpoint *tp );
BOOLEAN ifLiftAlarm_Accumulate( Testpoint *tp );
BOOLEAN ifLiftAlarm_Tristate( Testpoint *tp );

/* �жϴ������Ƿ񵽴￪������ */
BOOLEAN ifOutput_Analogue( Testpoint *tp );
BOOLEAN ifOutput_Switch( Testpoint *tp );
BOOLEAN ifOutput_Accumulate( Testpoint *tp );
BOOLEAN ifOutput_Tristate( Testpoint *tp );

/* �ж��Ƿ񸴵� */
BOOLEAN ifRecover_Analogue(Testpoint *tp);
BOOLEAN ifRecover_Switch( Testpoint *tp );
BOOLEAN ifRecover_Accumulate( Testpoint *tp );
BOOLEAN ifRecover_Tristate( Testpoint *tp );

/* ���ƴ��������� */
INT8U controlOutput_Analogue( Testpoint *tp );
INT8U controlOutput_Switch( Testpoint *tp );
INT8U controlOutput_Accumulate( Testpoint *tp );
INT8U controlOutput_Tristate( Testpoint *tp );

/* ���� */
INT8U controlRecover_Analogue( Testpoint *tp );
INT8U controlRecover_Switch( Testpoint *tp );
INT8U controlRecover_Accumulate( Testpoint *tp );
INT8U controlRecover_Tristate( Testpoint *tp );


/**/

/**/

/**/

/**/

/**/

/**/

/* �޸Ĳ������Ҫ�ϴ�����Ϣ */
void setUpload(Upload_Info *dest, Upload_Info *src);

/* ��ȡ�������Ҫ�ϴ�����Ϣ */
Upload_Info getUploadInfo(Upload_Info src);

#endif

