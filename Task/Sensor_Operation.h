#ifndef SENSOR_OPERATION_H_INCLUDED
#define SENSOR_OPERATION_H_INCLUDED

#include "Sensor.h"
#include "EquipmentList.h"

/* 配置传感器节点信息 */
void setConfig_Analogue( void* dest, void* src );     /* 配置模拟量传感器信息 */
void setConfig_Switch( void* dest, void* src );       /* 配置开关量传感器信息 */
void setConfig_Accumulate( void* dest, void* src );   /* 配置累计量传感器信息 */
void setConfig_Tristate( void* dest, void* src );     /* 配置三态开关量传感器信息 */

void setUpload(Upload_Info *dest, Upload_Info *src);  /* 获取或更改上传信息 */

void uploadData( Upload_Info *data );                 /* 网络获取需要上传的信息 */

Upload_Info 			getUploadInfo(Upload_Info src);     /* 获取上传信息 */
TestpointShowInfo getShowInfo( Testpoint *tp );       /* 获取测点显示信息 */
void showAllInfo(Testpoint *tp);                      /* 调试用：显示测点内所有信息 */


char *getSensorName( INT16U sensorName );   /* 传感器编号获取传感器名称对应 */
char *getPortType( INT16U portType );       /* 端口编号获取端口名称名称 */

/* 从串口获取传感器信息 */
void getDataFromUart_Analogue( RcvStruct *rev, Testpoint *tp );
void getDataFromUart_Switch( RcvStruct *rev, Testpoint *tp );
void getDataFromUart_Accumulate( RcvStruct *rev, Testpoint *tp );
void getDataFromUart_Tristate( RcvStruct *rev, Testpoint *tp );

/* 获取传感器的测点显示信息 */
TestpointShowInfo getShowInfo_Analogue(Testpoint *tp);    /* 获取模拟量传感器的测点显示信息 */
TestpointShowInfo getShowInfo_Switch(Testpoint *tp);      /* 获取开关量传感器的测点显示信息 */
TestpointShowInfo getShowInfo_Accumulate(Testpoint *tp);  /* 获取累计量传感器的测点显示信息 */
TestpointShowInfo getShowInfo_Tristate(Testpoint *tp);    /* 获取三态开关量传感器的测点显示信息 */

/* 判断传感器是否报警 */
BOOLEAN ifAlarm_Analogue( Testpoint *tp );
BOOLEAN ifAlarm_Switch( Testpoint *tp );
BOOLEAN ifAlarm_Accumulate( Testpoint *tp );
BOOLEAN ifAlarm_Tristate( Testpoint *tp );

/* 判断传感器解除报警 */
BOOLEAN ifLiftAlarm_Analogue( Testpoint *tp );
BOOLEAN ifLiftAlarm_Switch( Testpoint *tp );
BOOLEAN ifLiftAlarm_Accumulate( Testpoint *tp );
BOOLEAN ifLiftAlarm_Tristate( Testpoint *tp );

/* 判断传感器是否到达开出条件 */
BOOLEAN ifOutput_Analogue( Testpoint *tp );
BOOLEAN ifOutput_Switch( Testpoint *tp );
BOOLEAN ifOutput_Accumulate( Testpoint *tp );
BOOLEAN ifOutput_Tristate( Testpoint *tp );

/* 判断是否复电 */
BOOLEAN ifRecover_Analogue(Testpoint *tp);
BOOLEAN ifRecover_Switch( Testpoint *tp );
BOOLEAN ifRecover_Accumulate( Testpoint *tp );
BOOLEAN ifRecover_Tristate( Testpoint *tp );

/* 控制传感器开出 */
INT8U controlOutput_Analogue( Testpoint *tp );
INT8U controlOutput_Switch( Testpoint *tp );
INT8U controlOutput_Accumulate( Testpoint *tp );
INT8U controlOutput_Tristate( Testpoint *tp );

/* 复电 */
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

/* 修改测点中需要上传的信息 */
void setUpload(Upload_Info *dest, Upload_Info *src);

/* 获取测点中需要上传的信息 */
Upload_Info getUploadInfo(Upload_Info src);

#endif

