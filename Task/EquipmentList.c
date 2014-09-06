#include "stdlib.h"
#include "string.h"
#include "main.h"
#include "includes.h"
#include "os_cpu.h"

#include "EquipmentList.h"
#include "Sensor.h"
#include "com_uart.h"
#include "DataStruct.h"
#include "Sensor_Operation.h"


extern OS_EVENT *EquipmentTaskSem;				//控制设备表任务建立顺序
extern OS_EVENT *UploadQueueHead;        	//上传信息消息队列

Testpoint* EquipmentList = NULL;           //设备表头指针

extern RcvStruct  uart1RcvInfo;     //串口接收数据
extern OS_EVENT  *UartRcvTaskSem;   //串口缓冲区互斥锁
extern INT8U      uart1RcvOK;				//串口1接受数据成功

extern INT32U Uart1LostCnt; 				//串口1丢失数据次数

//extern INT8U  isUploading;

/* 从文件中获取配置 */
extern void File_Create_TpConfig(void);
extern void File_Get_Config_Analogue(int* f, Analogue_Config* analogue_conf);
extern void File_Get_Config_Switch(int* f, Switch_Config* switch_conf);
extern void File_Get_Config_Accumulate(int* f, Accumulate_Config* accumulate_conf);
extern void File_Get_Config_Tristate(int* f, Tristate_Config* tristate_conf);

Upload_Info Upload_NULL = { 0, "\0", 0, 0 };

/**
 * initTestpoint
 *
 * @param   Testpoint*    -the Testpoint point that need init
 *					sersorName    -the type of sensor
 * @return  none
 *
 * @brief   init one testpoint
 */

void initTestpoint( Testpoint* thisp, INT16U sensorName, void *config )
{
    //thisp->prev = NULL;
    thisp->next = NULL;
    thisp->EquipmentType = sensorName;
		thisp->EquipmentSem = OSSemCreate(1);

    /**  initialize function  **/
    thisp->setUpload = &setUpload;
    thisp->showAllInfo = &showAllInfo;
    thisp->getUploadInfo = &getUploadInfo;

    printf("EquipmentType = %d\n\n", thisp->EquipmentType );

    switch( thisp->EquipmentType )
    {
        case SENSOR_ANALOGUE:
            thisp->config = malloc( sizeof(Analogue_Config) );
            thisp->setConfig = &setConfig_Analogue;						            
						thisp->getShowInfo = &getShowInfo_Analogue;
						thisp->upload.testpointNo = ((Analogue_Config*)config)->testpointNo;
						thisp->getDataFromUart = &getDataFromUart_Analogue;
						thisp->ifAlarm = &ifAlarm_Analogue;
						thisp->ifLiftAlarm = &ifLiftAlarm_Analogue;
						
						thisp->ifOutput = &ifOutput_Analogue;
						thisp->controlOutput = &controlOutput_Analogue;
				
						thisp->ifRecover = &ifRecover_Analogue;
						thisp->controlRecover = &controlRecover_Analogue;
						
            break;

        case SENSOR_SWITCH:
            thisp->config = malloc( sizeof(Switch_Config) );
            thisp->setConfig = &setConfig_Switch;
						thisp->getShowInfo = &getShowInfo_Switch;
						thisp->upload.testpointNo = ((Switch_Config*)config)->testpointNo;
						thisp->getDataFromUart = &getDataFromUart_Switch;
						thisp->ifAlarm = &ifAlarm_Switch;
						thisp->ifLiftAlarm = &ifLiftAlarm_Switch;
						
						thisp->ifOutput = &ifOutput_Switch;
						thisp->controlOutput = &controlOutput_Switch;
				
						thisp->ifRecover = &ifRecover_Switch;
						thisp->controlRecover = &controlRecover_Switch;
				
            break;

        case SENSOR_ACCUMULATE:
            thisp->config = malloc( sizeof(Accumulate_Config) );
            thisp->setConfig = &setConfig_Accumulate;
						thisp->getShowInfo = &getShowInfo_Accumulate;
						thisp->upload.testpointNo = ((Accumulate_Config*)config)->testpointNo;
						thisp->getDataFromUart = &getDataFromUart_Accumulate;
						thisp->ifAlarm = &ifAlarm_Accumulate;
						thisp->ifLiftAlarm = &ifLiftAlarm_Accumulate;
				
						thisp->ifOutput = &ifOutput_Accumulate;
						thisp->controlOutput = &controlOutput_Accumulate;
						
						thisp->ifRecover = &ifRecover_Accumulate;
						thisp->controlRecover = &controlRecover_Accumulate;
				
						break;

        case SENSOR_TRISTATE:
            thisp->config = malloc( sizeof(Tristate_Config) );
            thisp->setConfig = &setConfig_Tristate;
						thisp->getShowInfo = &getShowInfo_Tristate;
						thisp->upload.testpointNo = ((Tristate_Config*)config)->testpointNo;
            thisp->getDataFromUart = &getDataFromUart_Tristate;
						thisp->ifAlarm = &ifAlarm_Tristate;
						thisp->ifLiftAlarm = &ifLiftAlarm_Tristate;
				
						thisp->ifOutput = &ifOutput_Tristate;
						thisp->controlOutput = &controlOutput_Tristate;
				
						thisp->ifRecover = &ifRecover_Tristate;
						thisp->controlRecover = &controlRecover_Tristate;
				
						break;

        default:
            thisp->config = NULL;
            thisp->setConfig = NULL;
						thisp->getShowInfo = NULL;
						thisp->getDataFromUart = NULL;
						thisp->ifAlarm = NULL;
						thisp->ifOutput = NULL;
						thisp->controlOutput = NULL;
						thisp->ifRecover = NULL;
						thisp->controlRecover = NULL;
				
            break;
    }

    thisp->upload.testpointNo = 0;
    setUpload( &thisp->upload, &Upload_NULL );
		if ( thisp->setConfig != NULL )
			thisp->setConfig( thisp->config, config );
    
		return;
}

/**
 * AddEquipment
 *
 * @param   Testpoint* head       -the EquipmentList head point
 *					Testpoint* equipment  -the equipment to be added
 * @return  none
 *
 * @brief   add one testpoint to EquipmentList
 */

void AddEquipment( Testpoint* head, Testpoint* equipment )
{
    Testpoint* phead = head->next;

    equipment->next = head->next;
	
	/*
    if ( phead != NULL )
    {
        phead->prev = equipment;
    }

    equipment->prev = head;
	*/
	
    head->next = equipment;
    return;
}

/**
 * DeleteEquipment
 *
 * @param   Testpoint* head       -the EquipmentList head point
 *					Testpoint* equipment  -the equipment to be deleted
 * @return  none
 *
 * @brief   delete one testpoint from EquipmentList
 */

void DeleteEquipment(Testpoint* head, Testpoint *equipment)    //从设备表中删除设备
{
    Testpoint* pList = head;
    Testpoint* pNext = equipment->next;

    while( pList != NULL && pList->next != equipment )
    {
        pList = pList->next;
    }

    if ( pList == NULL ) return;

    pList->next = equipment->next;
    
		/*
		if ( pNext != NULL )
    {
        pNext->prev = pList;
    }
		*/

    free(equipment);
    return;
}

/**
 * InitEquipmentList
 *
 * @param   none
 * @return  none
 *
 * @brief   init EquipmentList, create a empty head point to represent empty list
 */
BOOLEAN InitEquipmentList()
{
    char str[] = "I am a head of List.";
    EquipmentList = (Testpoint *)malloc(sizeof(Testpoint));
		if ( EquipmentList == NULL ) return FALSE;
		
    //EquipmentList->prev = NULL;
    EquipmentList->next = NULL;
    EquipmentList->EquipmentType = 0;      // the number of testpoints

    EquipmentList->config = (char*)malloc(sizeof(str));
    strcpy( EquipmentList->config, str );
    return TRUE;
}

int Tp_GetConfig_()
{
		return 0;
}

//测试用，待修改
/**
 * UpdateEquipment
 *
 * @param   none
 * @return  none
 *
 * @brief   get data from sensor and update the EquipmentList
 */

void UpdateEquipmentList()
{
		int i = 0;
		INT8U err;
		Testpoint *pList = NULL;
		//char s[30];
		RcvStruct rcvTemp;
		INT8U rcvOK = uart1RcvOK;
		
		OSSemPend(UartRcvTaskSem, 0, &err );
		if ( rcvOK == 1 )
		{
				rcvTemp = uart1RcvInfo;
		}
		OSSemPost(UartRcvTaskSem);
		
		if( EquipmentList != NULL )
		{
				pList = EquipmentList->next;
		}	
	
				
		if ( rcvOK == 1 )
		{
				while ( pList != NULL )
				{
						OSSemPend( pList->EquipmentSem , 0, &err );
												
						pList->getDataFromUart( &rcvTemp, pList );
						//pList->upload.sensorState = 11;
						//strcpy( pList->upload.collectTime, "2014-8-24" );						
						OSQPost( UploadQueueHead, &pList->upload );
/*						
						if ( isUploading == 0 )
						{
								uploadData( &(pList->upload) );
								
								printf("collectData=%d\n", pList->upload.collectData );
								printf("sensorState=%d\n", pList->upload.sensorState );
								printf("testpointNo=%d\n", pList->upload.testpointNo );
								printf("collectTime=%s\n", pList->upload.collectTime );
								
						}
*/						
						OSSemPost( pList->EquipmentSem );
						
						++i;
						if ( i >= 15 ) i = 0;
						pList = pList->next;		
				}
		}
		/*
		else if ( Uart1LostCnt >= 3 ) 
		{
				while ( pList != NULL )
				{
						OSSemPend( pList->EquipmentSem , 0, &err );
						sprintf( (char *)pList->Equipment, "NULL" );
						OSSemPost( pList->EquipmentSem );
		
						++i;
						if ( i >= 15 ) i = 0;
						pList = pList->next;		
				}
		}
	*/
		return;
}

void Read_ConfigFile()
{
		INT8U err;
		int n;
		int f;
		INT32U sensor_type;
	  char buf[BUFFERSIZE];
		Testpoint *testP;
		
		Analogue_Config     analogue_conf;
		Switch_Config       switch_conf;
		Accumulate_Config   accumulate_conf;
		Tristate_Config     tristate_conf;
		
		f = yaffs_open("/nand/config/TestpointConfig", O_RDONLY , 0 );
		printf( "EquipInit F = %d\n", f );
	
		if ( f >= 0 )
		{		
				while ( ( n = yaffs_read( f, buf, 64 ) ) != 0 )
				{
						testP = (Testpoint *)malloc( sizeof(Testpoint) );
						sscanf( buf, "Type: %d", &sensor_type );
						printf("Type=%d\n", sensor_type);
						switch( sensor_type )
						{
								case SENSOR_ANALOGUE:
									File_Get_Config_Analogue( &f, &analogue_conf );
									initTestpoint( testP, sensor_type, (void *)&analogue_conf );
									AddEquipment( EquipmentList, testP );
									EquipmentList->EquipmentType++;
									break;
								
								case SENSOR_SWITCH:
									File_Get_Config_Switch( &f, &switch_conf );
									initTestpoint( testP, sensor_type, (void *)&switch_conf );
									AddEquipment( EquipmentList, testP );
									EquipmentList->EquipmentType++;
									break;
								
								case SENSOR_ACCUMULATE:
									File_Get_Config_Accumulate( &f, &accumulate_conf );
									initTestpoint( testP, sensor_type, (void *)&accumulate_conf );
									AddEquipment( EquipmentList, testP );
									EquipmentList->EquipmentType++;
									break;
								
								case SENSOR_TRISTATE:
									File_Get_Config_Tristate( &f, &tristate_conf );
									initTestpoint( testP, sensor_type, (void *)&tristate_conf );
									AddEquipment( EquipmentList, testP );
									EquipmentList->EquipmentType++;
									break;
								
								default:
									free(testP);
									break;
						}
				}
				
				yaffs_close(f);
		}
		
		return;
}

/* 动态更新设备表任务 */
void Equipment_Task (void) 
{ 		

    //initConf( &analogue_conf, &switch_conf, &accumulate_conf, &tristate_conf );
	
    if ( InitEquipmentList()== TRUE )
		{
				Read_ConfigFile();
				
		}
		OSSemPost(EquipmentTaskSem);
		
		
		while (1)
		{
				UpdateEquipmentList();
				OSTimeDly(2000);
		}
}

