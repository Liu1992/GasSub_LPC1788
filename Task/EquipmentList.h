#ifndef EQUIPMENTLIST_H_INCLUDED
#define EQUIPMENTLIST_H_INCLUDED

#include "Sensor.h"
#include "ucos_ii.h"
#include "com_uart.h"

/* 测点显示信息 */
typedef struct
{
    char   testpointNo[34];																					//测点编号
    char   location[32];																						//安装地点
    char   sensorType[16];																					//传感器类型
    char   collectData[8];																					//采集数据
    char   portType[8];																							//端口类型
}TestpointShowInfo;

/* 测点信息：包含配置信息+上传信息 */
typedef struct Testpoint
{	
    int EquipmentType;																								// 测点类型
    void               *config;             													// 测点配置信息
    Upload_Info         upload;             													// 测点上传信息

    OS_EVENT *EquipmentSem;																						//每个测点的锁
    //struct Testpoint* prev;																					//指向前一个的指针（双向链表备用）
    struct Testpoint* next;																						//指向后一个的指针（单向链表，双向链表皆可）

    void (*setConfig)(void *dest, void *src);													//设置测点配置
    void (*setUpload)(Upload_Info *dest, Upload_Info *src);						//修改上传信息
    void (*showAllInfo)(struct Testpoint *tp);												//显示这一个测点的所有信息
    Upload_Info (*getUploadInfo)(Upload_Info src);										//获取该测点的上传信息
		TestpointShowInfo (*getShowInfo)(struct Testpoint *tp);						//获取该测点的显示信息
		void (*getDataFromUart)( RcvStruct *rev, struct Testpoint *tp );	//从16路端口获取该测点的采集数据
	
		BOOLEAN (*ifAlarm)(struct Testpoint* tp);													//判断是否报警
		BOOLEAN (*ifLiftAlarm)(struct Testpoint *tp);											//判断是否解除报警
		
		BOOLEAN (*ifOutput)(struct Testpoint *tp);												//判断是否断电
		BOOLEAN (*ifRecover)(struct Testpoint *tp);												//判断是否复电
	
		INT8U (*controlOutput)(struct Testpoint *tp);											//控制断电
		INT8U (*controlRecover)(struct Testpoint *tp);  								  //控制复电
}Testpoint;

#endif // EQUIPMENTLIST_H_INCLUDED
