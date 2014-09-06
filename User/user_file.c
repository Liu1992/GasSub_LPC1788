#include "yaffsfs.h"
#include "Sensor.h"
#include "stdio.h"
#include "EquipmentList.h"
#include "DataStruct.h"
#include "LPC177x_8x.h"

extern OS_EVENT    *ConfigQueueHead;

void File_Set_Config_Analogue(int* f, Analogue_Config* analogue_conf);
void File_Get_Config_Analogue(int* f, Analogue_Config* analogue_conf);

void File_Set_Config_Switch(int* f, Switch_Config* switch_conf);
void File_Get_Config_Switch(int* f, Switch_Config* switch_conf);

void File_Set_Config_Accumulate(int* f, Accumulate_Config* accumulate_conf);
void File_Get_Config_Accumulate(int* f, Accumulate_Config* accumulate_conf);

void File_Set_Config_Tristate(int* f, Tristate_Config* tristate_conf);
void File_Get_Config_Tristate(int* f, Tristate_Config* tristate_conf);

/* 调试用：创建空的配置信息结构体 */
void initConf( Analogue_Config *an, Switch_Config *sw, Accumulate_Config *ac, Tristate_Config *tr )
{
    an->testpointNo=10001;
    an->sensorName = SENSOR_METHANE;
    strcpy( an->location, "淮北矿业集团袁店煤矿");
    an->portType = 17;
    an->outputControl[0]=0x80;
		an->outputControl[1]=0xff;
    an->coefficient = 1.1;
	
		an->outage.minval = VALUE_UNDEF;
		an->outage.maxval = 70;
		
		an->alarm.minval = VALUE_UNDEF;
		an->alarm.maxval = 70;
		
		an->liftAlarm.minval = 70;
		an->liftAlarm.maxval = VALUE_UNDEF;
		
		an->recover.minval = VALUE_UNDEF;
		an->recover.maxval = 70;
	
		an->measureRange.minval = 0;
		an->measureRange.maxval = 200;

    sw->testpointNo=20002;
    sw->sensorName = SENSOR_2_OPEN_OFF;
    strcpy( sw->location, "中船重工矿用装备实验室");
    sw->portType = 17;
    sw->outputControl[0]=0x20;
		sw->outputControl[1]=0xff;

    ac->testpointNo=30003;
    ac->sensorName = SENSOR_HOOK_NUM;
    strcpy( ac->location, "淮北矿业集团朱仙庄煤矿");
    ac->portType = 24;
    ac->coefficient = 3.3;
		ac->shift = 3;
		ac->measureMaxVal = 12345;

    tr->testpointNo=40004;
    tr->sensorName = SENSOR_3_OPEN_OFF;
    strcpy( tr->location, "淮北创奇监测设备有限公司");
    tr->portType = 36;
    tr->outputControl[0]=0x08;
		tr->outputControl[1]=0xff;
		tr->current[0] = 0;
		tr->current[1] = 1;
		tr->current[2] = 5;

    return;
}

void File_Create_TpConfig(Config_Struct* rev_config)
{
		int f;
		//char buf[BUFFERSIZE];
	
		Analogue_Config     analogue_conf;
		Switch_Config       switch_conf;
		Accumulate_Config   accumulate_conf;
		Tristate_Config     tristate_conf;
	
		initConf( &analogue_conf, &switch_conf, &accumulate_conf, &tristate_conf );
	
		f = yaffs_open( "/nand/config/TestpointConfig",  O_CREAT |O_RDWR | O_TRUNC, S_IREAD | S_IWRITE  );
		if ( f >= 0 )
		{
				printf("InfileCre: %u %u\n",rev_config->analogueConfig.testpointNo, rev_config->analogueConfig.sensorName );
	/*			
				File_Set_Config_Analogue( &f, &rev_config->analogueConfig );
				File_Set_Config_Switch( &f, &rev_config->switchConfig );
				File_Set_Config_Accumulate( &f, &rev_config->accumulateConfig );
				File_Set_Config_Tristate( &f, &rev_config->tristateConfig );
	*/		
				strcpy( analogue_conf.location, "淮北矿业集团袁店煤矿");
				File_Set_Config_Analogue( &f, &analogue_conf );
			strcpy( analogue_conf.location, "中船重工矿用装备实验室");
				File_Set_Config_Switch( &f, &switch_conf );
			strcpy( analogue_conf.location, "淮北矿业集团朱仙庄煤矿");
				File_Set_Config_Accumulate( &f, &accumulate_conf );
			strcpy( analogue_conf.location, "淮北创奇监测设备有限公司");
				File_Set_Config_Tristate( &f, &tristate_conf );
				
			/**********************************************************************
			*2014-09-04 刘帅修改，更改安装地点
			**********************************************************************/
			
			strcpy( analogue_conf.location, "四采区832工作面");
				File_Set_Config_Analogue( &f, &analogue_conf );
			strcpy(switch_conf.location,"南大巷机电洞室 ");
				File_Set_Config_Switch( &f, &switch_conf );
			strcpy(accumulate_conf.location,"二采区811掘进工作面");
				File_Set_Config_Accumulate( &f, &accumulate_conf );
				File_Set_Config_Tristate( &f, &tristate_conf );
				
			/**********************************************************************
			*2014-09-04 修改区结束
			**********************************************************************/	
				
				File_Set_Config_Analogue( &f, &analogue_conf );
				File_Set_Config_Switch( &f, &switch_conf );
				File_Set_Config_Accumulate( &f, &accumulate_conf );
				File_Set_Config_Tristate( &f, &tristate_conf );
				
				File_Set_Config_Analogue( &f, &analogue_conf );
				File_Set_Config_Switch( &f, &switch_conf );
				File_Set_Config_Accumulate( &f, &accumulate_conf );
				File_Set_Config_Tristate( &f, &tristate_conf );
			
				yaffs_close(f);
		}
		return;
}

void File_Create_SubstationConfig()
{
		int f;
		//char buf[BUFFERSIZE];
	
		f = yaffs_open( "/nand/config/SubstationConfig",  O_RDWR | O_TRUNC, S_IREAD | S_IWRITE  );
		if ( f >= 0 )
		{			
				yaffs_close(f);
		}
		return;
}

void File_Update(void)
{
		return;
}

/*************************************************

 		检查测点配置文件是否存在，如不存在则创建一个

**************************************************/

void File_TpConfig_Exist()
{
//		int f;
//	
//		f = yaffs_open("/nand/config/TestpointConfig", O_RDONLY, 0 );    //检查测点配置文件是否存在
//		if (f >= 0)
//		{
//			 printf("Open Success! Testpoint Config Exist!\n");
//		}
//		else 																															//如果不存在，创建一个
//		{
//			 f = yaffs_open( "/nand/config/TestpointConfig",  O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE  );
//			 if ( f >= 0 )
//			 {
//					printf("Create Success! Testpoint Config doesn't exist!\n");
//			 }
//		}
//		yaffs_close(f);
	/**********************************************************************
				*2014-09-04 刘帅修改，添加默认配置
				**********************************************************************/
				 Config_Struct *revConf;
				 File_Create_TpConfig( revConf );
				 /**********************************************************************
				*2014-09-04 修改区结束
				**********************************************************************/

		return;
}

/*************************************************

 		检查分站配置文件是否存在，如不存在则创建一个

**************************************************/

void File_SubstationConfig_Exist()
{
		int f;
	
		f = yaffs_open("/nand/config/SubstationConfig", O_RDONLY, 0 );    //检查测点配置文件是否存在
		if (f >= 0)
		{
			 printf("Open Success! Substation Config Exist!\n");
		}
		else 																															//如果不存在，创建一个
		{
			 f = yaffs_open( "/nand/config/SubstationConfig",  O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE  );
			 if ( f >= 0 )
			 
				 printf("Create Success! Substation Config doesn't exist!\n");
		}
		yaffs_close(f);

		return;
}

/*************************************************

 		检查上传数据文件是否存在，如不存在则创建一个

**************************************************/

void File_Data_Exist()
{
		int f;
		
		f = yaffs_open("/nand/data/SensorData", O_RDONLY, 0 );    //检查测点配置文件是否存在
		if (f >= 0)
		{
			 printf("Open Success! Sensor Data Exist!\n");
		}
		else 																															//如果不存在，创建一个
		{
			 f = yaffs_open( "/nand/data/SensorData",  O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE  );
			 if ( f >= 0 )
					printf("Create Success! Sensor Data doesn't exist!\n");
		}
		yaffs_close(f);
		

		return;
}

/**************************************

						 初始化文件
						建立主要文件

***************************************/

void File_Init(void)
{
		int f1, f2;
	
		yaffs_mount("/nand");											//挂载
    
		f1 = yaffs_open("nand/first_start", O_RDONLY, 0 );
	  if ( f1 >= 0 )    //不是第一次启动文件系统 
		{
				printf("first_start open success!\n");
				File_TpConfig_Exist();
				//File_SubstationConfig_Exist();
				//File_Data_Exist();
		}
		else              //第一次启动文件系统
 	  {
			  yaffs_unmount("/nand");
				yaffs_format("/nand", 0, 0, 0);           //格式化
				yaffs_mount("/nand");											//挂载
				yaffs_mkdir("/nand/config", 0666);				//创建配置文件目录
				yaffs_mkdir("/nand/data", 0666);					//创建上传数据目录
				
				f1 = yaffs_open( "/nand/first_start",  O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE  );
				if ( f1 >= 0 )
				{
						printf("first_start create success!\n");
				}
/*			
				f2 = yaffs_open( "/nand/config/TestpointConfig",  O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE  );			
				if ( f2 >= 0 )
				{
						printf("Testpoint Config Create Success!\n");
						yaffs_close(f2);
				}
				
				f2 = yaffs_open( "/nand/config/SubstationConfig",  O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE  );			
				if ( f2 >= 0 )
				{
						printf("Substation Config Create Success!\n");
						yaffs_close(f2);
				}
				
				f2 = yaffs_open( "/nand/data/SensorData",  O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE  );			
				if ( f2 >= 0 )
				{
						printf("SensorData Create Success!\n");
						yaffs_close(f2);
				}
				*/
		}
		
		yaffs_close(f1);
				
		return;
}

/**************************************

			模拟量传感器信息写入文件

***************************************/

void File_Set_Config_Analogue(int* f, Analogue_Config* analogue_conf)
{
		int n;
		char buf[BUFFERSIZE];
		memset(buf, '\0', sizeof(buf));
		
		sprintf( buf, "Type: %d", SENSOR_ANALOGUE );															//参数长度
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d %d %d", analogue_conf->testpointNo, analogue_conf->sensorName, analogue_conf->portType, 12 );   //测点编号，传感器类型，端口类型，端口地址
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%s", analogue_conf->location );               //安装地点
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d", analogue_conf->alarm.minval, analogue_conf->alarm.maxval );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d", analogue_conf->liftAlarm.minval, analogue_conf->liftAlarm.maxval );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d", analogue_conf->measureRange.minval, analogue_conf->measureRange.maxval );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d", analogue_conf->outage.minval, analogue_conf->outage.maxval );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d", analogue_conf->recover.minval, analogue_conf->recover.maxval );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %.3f", analogue_conf->outputControl, analogue_conf->coefficient );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		return;
}

/**************************************

			开关量传感器信息写入文件

***************************************/

void File_Set_Config_Switch(int* f, Switch_Config* switch_conf)
{
		int n;
		char buf[BUFFERSIZE];
		memset(buf, '\0', sizeof(buf));
		
		sprintf( buf, "Type: %d", SENSOR_SWITCH );															//参数长度
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d %d %d", switch_conf->testpointNo, switch_conf->sensorName, switch_conf->portType, 22 );   //测点编号，传感器类型，端口类型，端口地址
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%s", switch_conf->location );               //安装地点
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d", switch_conf->outputControl );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );

		return;
}

/**************************************

			累计量传感器信息写入文件

***************************************/

void File_Set_Config_Accumulate(int* f, Accumulate_Config* accumulate_conf)
{
		int n;
		char buf[BUFFERSIZE];
	
		memset(buf, '\0', sizeof(buf));	
		sprintf( buf, "Type: %d", SENSOR_ACCUMULATE );															//参数长度
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d %d %d", accumulate_conf->testpointNo, accumulate_conf->sensorName, accumulate_conf->portType, 12 );   //测点编号，传感器类型，端口类型，端口地址
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%s", accumulate_conf->location );               //安装地点
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d", accumulate_conf->measureMaxVal );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%.3f", accumulate_conf->coefficient );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));	
		sprintf( buf, "%d", accumulate_conf->shift );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );

		return;
}

/**************************************

			三态开关量传感器信息写入文件

***************************************/

void File_Set_Config_Tristate(int* f, Tristate_Config* tristate_conf)
{
		int n;
		char buf[BUFFERSIZE];
		memset(buf, '\0', sizeof(buf));
		
		sprintf( buf, "Type: %d", SENSOR_TRISTATE );															//参数长度
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d %d %d", tristate_conf->testpointNo, tristate_conf->sensorName, tristate_conf->portType, 12 );   //测点编号，传感器类型，端口类型，端口地址
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%s", tristate_conf->location );               //安装地点
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d %d", tristate_conf->current[0], tristate_conf->current[1], tristate_conf->current[2] );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d", tristate_conf->outputControl );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
	
		return;
}


/**********************************************

							获取模拟量传感器配置

***********************************************/

void File_Get_Config_Analogue(int* f, Analogue_Config* analogue_conf)
{
		int n;
		char buf[BUFFERSIZE];
	
		printf("In Analogue Config Function:\n");
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		printf( "buf=[%s]\n", buf );
		sscanf( buf, "%d %d %d %d", &analogue_conf->testpointNo, &analogue_conf->sensorName, &analogue_conf->portType, &n );   //测点编号，传感器类型，端口类型，端口地址
				
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		printf( "buf=[%s]\n", buf );
		strcpy( analogue_conf->location, buf );               //安装地点
				
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		printf( "buf=[%s]\n", buf );
		sscanf( buf, "%d %d", &analogue_conf->alarm.minval, &analogue_conf->alarm.maxval );
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		printf( "buf=[%s]\n", buf );
		sscanf( buf, "%d %d", &analogue_conf->liftAlarm.minval, &analogue_conf->liftAlarm.maxval );
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		printf( "buf=[%s]\n", buf );
		sscanf( buf, "%d %d", &analogue_conf->measureRange.minval, &analogue_conf->measureRange.maxval );
				
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		printf( "buf=[%s]\n", buf );
		sscanf( buf, "%d %d", &analogue_conf->outage.minval, &analogue_conf->outage.maxval );
				
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		printf( "buf=[%s]\n", buf );
		sscanf( buf, "%d %d", &analogue_conf->recover.minval, &analogue_conf->recover.maxval );
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read((*f), buf, BUFFERSIZE );
		printf( "buf=[%s]\n", buf );
		sscanf( buf, "%d %f", &analogue_conf->outputControl, &analogue_conf->coefficient );
		
		printf("Quit Analogue Config Function\n");
	
		return;
}

/**************************************

			获取开关量传感器配置信息

***************************************/

void File_Get_Config_Switch(int* f, Switch_Config* switch_conf)
{
		int n;
		char buf[BUFFERSIZE];
	
		printf("In Switch Config Function:\n");
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d %d %d %d", &switch_conf->testpointNo, &switch_conf->sensorName, &switch_conf->portType, &n );   //测点编号，传感器类型，端口类型，端口地址
				
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		strcpy( switch_conf->location, buf );               //安装地点
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d", &switch_conf->outputControl );	
		
		printf("Quit Switch Config Function\n");

		return;
}

/**************************************

			获取累计量传感器配置信息

***************************************/

void File_Get_Config_Accumulate(int* f, Accumulate_Config* accumulate_conf)
{
		int n;
		char buf[BUFFERSIZE];

		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d %d %d %d", &accumulate_conf->testpointNo, &accumulate_conf->sensorName, &accumulate_conf->portType, &n );   //测点编号，传感器类型，端口类型，端口地址
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		strcpy( accumulate_conf->location, buf );               //安装地点
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d", &accumulate_conf->measureMaxVal );
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%f", &accumulate_conf->coefficient );
	
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d", &accumulate_conf->shift );

		return;
}

/**************************************

			获取三态开关量传感器配置信息

***************************************/

void File_Get_Config_Tristate(int* f, Tristate_Config* tristate_conf)
{
		int n;
		char buf[BUFFERSIZE];
	
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d %d %d %d", &tristate_conf->testpointNo, &tristate_conf->sensorName, &tristate_conf->portType, &n );   //测点编号，传感器类型，端口类型，端口地址
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		strcpy( tristate_conf->location, buf );               //安装地点
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d %d %d", &tristate_conf->current[0], &tristate_conf->current[1], &tristate_conf->current[2] );
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d", &tristate_conf->outputControl );
	
		return;
}

/**************************************************

						接收文件任务

***************************************************/

void FileReceive_Task (void) 
{
		INT8U err;
		Config_Struct *revConf;
		
		while (1)
		{
				revConf=(Config_Struct *)OSQPend(ConfigQueueHead, 0, &err);
				File_Create_TpConfig( revConf );
				NVIC_SystemReset();
		}
}
