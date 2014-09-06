#ifndef SENSOR_H_INCLUDED
#define SENSOR_H_INCLUDED

#include "os_cpu.h"

#define VALUE_UNDEF													-2147483640

#define PORT_FREQ										  			0
#define PORT_AD															1
#define PORT_COUNT                          2
#define PORT_SEL                            3

#define SENSOR_UNSURE                       0

#define SENSOR_ANALOGUE                     1
#define SENSOR_SWITCH                       2
#define SENSOR_ACCUMULATE                   3
#define SENSOR_TRISTATE                     4

#define SENSOR_METHANE                      10001
#define SENSOR_HIGH_LOW_METHANE             10002
#define SENSOR_WIND_SPEED                   10003
#define SENSOR_TEMPERATURE                  10004
#define SENSOR_WIND_PRESSURE                10005
#define SENSOR_NEGATIVE_PRESSURE            10006
#define SENSOR_WATER_LEVEL                  10007
#define SENSOR_DUST                         10008
#define SENSOR_OXYGEN                       10009
#define SENSOR_CARBON_MONOXIDE              10010
#define SENSOR_CARBON_DIOXIDE               10011
#define SENSOR_PIPE_METHANE                 10012
#define SENSOR_PIPE_TEMPERATURE             10013
#define SENSOR_PIPE_NEG_PRESSURE            10014
#define SENSOR_PIPE_FLOW                    10015

#define SENSOR_2_OPEN_OFF                   20001
#define SENSOR_2_SWITCH                     20002
#define SENSOR_2_AIR_DOOR                   20003
#define SENSOR_2_DRAUGHT_FAN                20004
#define SENSOR_2_AIR_DUCT                   20005
#define SENSOR_2_FEED                       20006
#define SENSOR_2_SMOKE                      20007

#define SENSOR_HOOK_NUM                     30001
#define SENSOR_PRODUCTION                   30002
#define SENSOR_TOTAL                        30003

#define SENSOR_3_OPEN_OFF                   40001
#define SENSOR_3_SWITCH                     40002
#define SENSOR_3_AIR_DOOR                   40003
#define SENSOR_3_DRAUGHT_FAN                40004
#define SENSOR_3_AIR_DUCT                   40005
#define SENSOR_3_FEED                       40006
#define SENSOR_3_SMOKE                      40007

typedef struct
{
    INT32S minval;
    INT32S maxval;
}Range;

typedef struct
{
    INT32S testpointNo;				//测点编号
    INT16S sensorName;				//传感器名称
    char location[30];				//安装地点
    INT16U portType;					//端口类型
    INT8U outputControl[2];		//输出控制

    Range outage;							//断电值
    Range recover;						//复电值
    Range alarm;							//报警值
    Range liftAlarm;					//解报值
    Range measureRange;				//传感器量程

    float coefficient;				//比例系数
}Analogue_Config;

typedef struct
{
    INT32S testpointNo;				//测点编号
    INT16S sensorName;				//传感器名称
    char location[30];				//安装地点
    INT16U portType;					//端口类型
    INT8U outputControl[2];	  //输出控制
}Switch_Config;

typedef struct
{
    INT32S testpointNo;				//测点编号
    INT16S sensorName;				//传感器名称
    char location[30];				//安装地点
    INT16U portType;					//端口类型
    INT32S measureMaxVal;			//最大值
		INT32S shift;							//班次（多久清零）
    float coefficient;				//比例系数
		
}Accumulate_Config;

typedef struct
{
    INT32S testpointNo;				//测点编号
    INT16S sensorName;				//传感器名称
    char location[30];				//安装地点
    INT16U portType;					//端口类型
    INT8U outputControl[2];		//输出控制
    INT32S current[4];				//三态对应的电流值
}Tristate_Config;

typedef struct
{
    INT32S testpointNo;				//测点编号
    char collectTime[20];			//采集时间
    INT32S collectData;				//采集数据
    INT8S  sensorState;				//传感器状态
}Upload_Info;


//调试用：综合配置结构体封装
typedef struct
{
	Analogue_Config analogueConfig;
	Switch_Config	switchConfig;
	Accumulate_Config accumulateConfig;
	Tristate_Config  tristateConfig;
}Config_Struct;

#endif // SENSOR_H_INCLUDED
