
//*****************��*��*��*Ϣ********************************************
// *�� �� ����Main.h
// *�桡  ����v1.0
// *�� �� �ˣ�Huangxinglong
// *�ա�  �ڣ�2014/5/23 10:15:00
// *�衡  ����
// *
// *��ǰ�汾��v1.0
// *�� �� �ˣ�Huangxinglong
// *�ա�  �ڣ�2014/5/23 10:15:02
// *�衡  ����
//************************************************************************
#ifndef __SENSOR_H__
	#define __SENSOR_H__
	/*-----�궨��---------------------*/
#define LPS25HB  0
#define HTS221   0
#define Kionx_Acc 1
#define    FILTERWINDOWLENGTH     16
#define    WINDOW 16


	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
int8u Lps25Hb_Init(void);
int8u HTS221_Init(void);								    
int8u RdCalib_Data(void);										 							 				    
int16u Temp_Calcu(void);
int16u Humi_Calcu(void);
void  Str_Init( void );
void TempCalib_Init(void);
void Temp_Calib(void);
#if LPS25HB 
float Hpa_Rd(void);
#endif




#endif

//***********************************************************************/
#ifdef   _SENSOR_C_
#define  SENSOR_EXT
#else
#define  SENSOR_EXT  extern
#endif
/*-------------��������--------------*/

//***********************************************************************/


SENSOR_EXT int16u PressureBuf;//��ѹֵ
SENSOR_EXT int16u TempBuf;//�¶�ֵ
SENSOR_EXT int16u HumiBuf;//ʪ��ֵ

SENSOR_EXT int16u TempCalib[2];//У׼�¶�ֵ
//SENSOR_EXT float TempCalib0;//У׼�¶�ֵ

SENSOR_EXT int16u TempActual[2];//��ǰ���¶�ֵ
//SENSOR_EXT float TempActual0;//��ǰ���¶�ֵ
SENSOR_EXT int16u TempCalibComp;
SENSOR_EXT float TempCalib_K;//У׼�¶�ֵ
SENSOR_EXT float TempCalib_A;//У׼�¶�ֵ
SENSOR_EXT int16u TempOffset;



