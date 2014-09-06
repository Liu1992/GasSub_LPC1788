#ifndef EQUIPMENTLIST_H_INCLUDED
#define EQUIPMENTLIST_H_INCLUDED

#include "Sensor.h"
#include "ucos_ii.h"
#include "com_uart.h"

/* �����ʾ��Ϣ */
typedef struct
{
    char   testpointNo[34];																					//�����
    char   location[32];																						//��װ�ص�
    char   sensorType[16];																					//����������
    char   collectData[8];																					//�ɼ�����
    char   portType[8];																							//�˿�����
}TestpointShowInfo;

/* �����Ϣ������������Ϣ+�ϴ���Ϣ */
typedef struct Testpoint
{	
    int EquipmentType;																								// �������
    void               *config;             													// ���������Ϣ
    Upload_Info         upload;             													// ����ϴ���Ϣ

    OS_EVENT *EquipmentSem;																						//ÿ��������
    //struct Testpoint* prev;																					//ָ��ǰһ����ָ�루˫�������ã�
    struct Testpoint* next;																						//ָ���һ����ָ�루��������˫������Կɣ�

    void (*setConfig)(void *dest, void *src);													//���ò������
    void (*setUpload)(Upload_Info *dest, Upload_Info *src);						//�޸��ϴ���Ϣ
    void (*showAllInfo)(struct Testpoint *tp);												//��ʾ��һ������������Ϣ
    Upload_Info (*getUploadInfo)(Upload_Info src);										//��ȡ�ò����ϴ���Ϣ
		TestpointShowInfo (*getShowInfo)(struct Testpoint *tp);						//��ȡ�ò�����ʾ��Ϣ
		void (*getDataFromUart)( RcvStruct *rev, struct Testpoint *tp );	//��16·�˿ڻ�ȡ�ò��Ĳɼ�����
	
		BOOLEAN (*ifAlarm)(struct Testpoint* tp);													//�ж��Ƿ񱨾�
		BOOLEAN (*ifLiftAlarm)(struct Testpoint *tp);											//�ж��Ƿ�������
		
		BOOLEAN (*ifOutput)(struct Testpoint *tp);												//�ж��Ƿ�ϵ�
		BOOLEAN (*ifRecover)(struct Testpoint *tp);												//�ж��Ƿ񸴵�
	
		INT8U (*controlOutput)(struct Testpoint *tp);											//���ƶϵ�
		INT8U (*controlRecover)(struct Testpoint *tp);  								  //���Ƹ���
}Testpoint;

#endif // EQUIPMENTLIST_H_INCLUDED
