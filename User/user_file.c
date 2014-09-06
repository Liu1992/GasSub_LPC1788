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

/* �����ã������յ�������Ϣ�ṹ�� */
void initConf( Analogue_Config *an, Switch_Config *sw, Accumulate_Config *ac, Tristate_Config *tr )
{
    an->testpointNo=10001;
    an->sensorName = SENSOR_METHANE;
    strcpy( an->location, "������ҵ����Ԭ��ú��");
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
    strcpy( sw->location, "�д��ع�����װ��ʵ����");
    sw->portType = 17;
    sw->outputControl[0]=0x20;
		sw->outputControl[1]=0xff;

    ac->testpointNo=30003;
    ac->sensorName = SENSOR_HOOK_NUM;
    strcpy( ac->location, "������ҵ��������ׯú��");
    ac->portType = 24;
    ac->coefficient = 3.3;
		ac->shift = 3;
		ac->measureMaxVal = 12345;

    tr->testpointNo=40004;
    tr->sensorName = SENSOR_3_OPEN_OFF;
    strcpy( tr->location, "�����������豸���޹�˾");
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
				strcpy( analogue_conf.location, "������ҵ����Ԭ��ú��");
				File_Set_Config_Analogue( &f, &analogue_conf );
			strcpy( analogue_conf.location, "�д��ع�����װ��ʵ����");
				File_Set_Config_Switch( &f, &switch_conf );
			strcpy( analogue_conf.location, "������ҵ��������ׯú��");
				File_Set_Config_Accumulate( &f, &accumulate_conf );
			strcpy( analogue_conf.location, "�����������豸���޹�˾");
				File_Set_Config_Tristate( &f, &tristate_conf );
				
			/**********************************************************************
			*2014-09-04 ��˧�޸ģ����İ�װ�ص�
			**********************************************************************/
			
			strcpy( analogue_conf.location, "�Ĳ���832������");
				File_Set_Config_Analogue( &f, &analogue_conf );
			strcpy(switch_conf.location,"�ϴ�����綴�� ");
				File_Set_Config_Switch( &f, &switch_conf );
			strcpy(accumulate_conf.location,"������811���������");
				File_Set_Config_Accumulate( &f, &accumulate_conf );
				File_Set_Config_Tristate( &f, &tristate_conf );
				
			/**********************************************************************
			*2014-09-04 �޸�������
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

 		����������ļ��Ƿ���ڣ��粻�����򴴽�һ��

**************************************************/

void File_TpConfig_Exist()
{
//		int f;
//	
//		f = yaffs_open("/nand/config/TestpointConfig", O_RDONLY, 0 );    //����������ļ��Ƿ����
//		if (f >= 0)
//		{
//			 printf("Open Success! Testpoint Config Exist!\n");
//		}
//		else 																															//��������ڣ�����һ��
//		{
//			 f = yaffs_open( "/nand/config/TestpointConfig",  O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE  );
//			 if ( f >= 0 )
//			 {
//					printf("Create Success! Testpoint Config doesn't exist!\n");
//			 }
//		}
//		yaffs_close(f);
	/**********************************************************************
				*2014-09-04 ��˧�޸ģ����Ĭ������
				**********************************************************************/
				 Config_Struct *revConf;
				 File_Create_TpConfig( revConf );
				 /**********************************************************************
				*2014-09-04 �޸�������
				**********************************************************************/

		return;
}

/*************************************************

 		����վ�����ļ��Ƿ���ڣ��粻�����򴴽�һ��

**************************************************/

void File_SubstationConfig_Exist()
{
		int f;
	
		f = yaffs_open("/nand/config/SubstationConfig", O_RDONLY, 0 );    //����������ļ��Ƿ����
		if (f >= 0)
		{
			 printf("Open Success! Substation Config Exist!\n");
		}
		else 																															//��������ڣ�����һ��
		{
			 f = yaffs_open( "/nand/config/SubstationConfig",  O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE  );
			 if ( f >= 0 )
			 
				 printf("Create Success! Substation Config doesn't exist!\n");
		}
		yaffs_close(f);

		return;
}

/*************************************************

 		����ϴ������ļ��Ƿ���ڣ��粻�����򴴽�һ��

**************************************************/

void File_Data_Exist()
{
		int f;
		
		f = yaffs_open("/nand/data/SensorData", O_RDONLY, 0 );    //����������ļ��Ƿ����
		if (f >= 0)
		{
			 printf("Open Success! Sensor Data Exist!\n");
		}
		else 																															//��������ڣ�����һ��
		{
			 f = yaffs_open( "/nand/data/SensorData",  O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE  );
			 if ( f >= 0 )
					printf("Create Success! Sensor Data doesn't exist!\n");
		}
		yaffs_close(f);
		

		return;
}

/**************************************

						 ��ʼ���ļ�
						������Ҫ�ļ�

***************************************/

void File_Init(void)
{
		int f1, f2;
	
		yaffs_mount("/nand");											//����
    
		f1 = yaffs_open("nand/first_start", O_RDONLY, 0 );
	  if ( f1 >= 0 )    //���ǵ�һ�������ļ�ϵͳ 
		{
				printf("first_start open success!\n");
				File_TpConfig_Exist();
				//File_SubstationConfig_Exist();
				//File_Data_Exist();
		}
		else              //��һ�������ļ�ϵͳ
 	  {
			  yaffs_unmount("/nand");
				yaffs_format("/nand", 0, 0, 0);           //��ʽ��
				yaffs_mount("/nand");											//����
				yaffs_mkdir("/nand/config", 0666);				//���������ļ�Ŀ¼
				yaffs_mkdir("/nand/data", 0666);					//�����ϴ�����Ŀ¼
				
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

			ģ������������Ϣд���ļ�

***************************************/

void File_Set_Config_Analogue(int* f, Analogue_Config* analogue_conf)
{
		int n;
		char buf[BUFFERSIZE];
		memset(buf, '\0', sizeof(buf));
		
		sprintf( buf, "Type: %d", SENSOR_ANALOGUE );															//��������
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d %d %d", analogue_conf->testpointNo, analogue_conf->sensorName, analogue_conf->portType, 12 );   //����ţ����������ͣ��˿����ͣ��˿ڵ�ַ
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%s", analogue_conf->location );               //��װ�ص�
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

			��������������Ϣд���ļ�

***************************************/

void File_Set_Config_Switch(int* f, Switch_Config* switch_conf)
{
		int n;
		char buf[BUFFERSIZE];
		memset(buf, '\0', sizeof(buf));
		
		sprintf( buf, "Type: %d", SENSOR_SWITCH );															//��������
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d %d %d", switch_conf->testpointNo, switch_conf->sensorName, switch_conf->portType, 22 );   //����ţ����������ͣ��˿����ͣ��˿ڵ�ַ
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%s", switch_conf->location );               //��װ�ص�
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d", switch_conf->outputControl );
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );

		return;
}

/**************************************

			�ۼ�����������Ϣд���ļ�

***************************************/

void File_Set_Config_Accumulate(int* f, Accumulate_Config* accumulate_conf)
{
		int n;
		char buf[BUFFERSIZE];
	
		memset(buf, '\0', sizeof(buf));	
		sprintf( buf, "Type: %d", SENSOR_ACCUMULATE );															//��������
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d %d %d", accumulate_conf->testpointNo, accumulate_conf->sensorName, accumulate_conf->portType, 12 );   //����ţ����������ͣ��˿����ͣ��˿ڵ�ַ
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%s", accumulate_conf->location );               //��װ�ص�
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

			��̬��������������Ϣд���ļ�

***************************************/

void File_Set_Config_Tristate(int* f, Tristate_Config* tristate_conf)
{
		int n;
		char buf[BUFFERSIZE];
		memset(buf, '\0', sizeof(buf));
		
		sprintf( buf, "Type: %d", SENSOR_TRISTATE );															//��������
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%d %d %d %d", tristate_conf->testpointNo, tristate_conf->sensorName, tristate_conf->portType, 12 );   //����ţ����������ͣ��˿����ͣ��˿ڵ�ַ
		buf[BUFFERSIZE-1] = '\n';
		n = yaffs_write((*f), buf, BUFFERSIZE );
		
		memset(buf, '\0', sizeof(buf));
		sprintf( buf, "%s", tristate_conf->location );               //��װ�ص�
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

							��ȡģ��������������

***********************************************/

void File_Get_Config_Analogue(int* f, Analogue_Config* analogue_conf)
{
		int n;
		char buf[BUFFERSIZE];
	
		printf("In Analogue Config Function:\n");
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		printf( "buf=[%s]\n", buf );
		sscanf( buf, "%d %d %d %d", &analogue_conf->testpointNo, &analogue_conf->sensorName, &analogue_conf->portType, &n );   //����ţ����������ͣ��˿����ͣ��˿ڵ�ַ
				
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		printf( "buf=[%s]\n", buf );
		strcpy( analogue_conf->location, buf );               //��װ�ص�
				
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

			��ȡ������������������Ϣ

***************************************/

void File_Get_Config_Switch(int* f, Switch_Config* switch_conf)
{
		int n;
		char buf[BUFFERSIZE];
	
		printf("In Switch Config Function:\n");
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d %d %d %d", &switch_conf->testpointNo, &switch_conf->sensorName, &switch_conf->portType, &n );   //����ţ����������ͣ��˿����ͣ��˿ڵ�ַ
				
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		strcpy( switch_conf->location, buf );               //��װ�ص�
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d", &switch_conf->outputControl );	
		
		printf("Quit Switch Config Function\n");

		return;
}

/**************************************

			��ȡ�ۼ���������������Ϣ

***************************************/

void File_Get_Config_Accumulate(int* f, Accumulate_Config* accumulate_conf)
{
		int n;
		char buf[BUFFERSIZE];

		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d %d %d %d", &accumulate_conf->testpointNo, &accumulate_conf->sensorName, &accumulate_conf->portType, &n );   //����ţ����������ͣ��˿����ͣ��˿ڵ�ַ
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		strcpy( accumulate_conf->location, buf );               //��װ�ص�
		
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

			��ȡ��̬������������������Ϣ

***************************************/

void File_Get_Config_Tristate(int* f, Tristate_Config* tristate_conf)
{
		int n;
		char buf[BUFFERSIZE];
	
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d %d %d %d", &tristate_conf->testpointNo, &tristate_conf->sensorName, &tristate_conf->portType, &n );   //����ţ����������ͣ��˿����ͣ��˿ڵ�ַ
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		strcpy( tristate_conf->location, buf );               //��װ�ص�
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d %d %d", &tristate_conf->current[0], &tristate_conf->current[1], &tristate_conf->current[2] );
		
		memset(buf, '\0', sizeof(buf));
		n = yaffs_read( (*f), buf, BUFFERSIZE );
		sscanf( buf, "%d", &tristate_conf->outputControl );
	
		return;
}

/**************************************************

						�����ļ�����

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
