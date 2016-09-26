
//*****************文*件*信*息********************************************
// *文 件 名：Main.h
// *版　  本：v1.0
// *创 建 人：Huangxinglong
// *日　  期：2014/5/23 10:15:00
// *描　  述：
// *
// *当前版本：v1.0
// *修 改 人：Huangxinglong
// *日　  期：2014/5/23 10:15:02
// *描　  述：
//************************************************************************
#ifndef __SENSOR_H__
	#define __SENSOR_H__
	/*-----宏定义---------------------*/
#define LPS25HB  0
#define HTS221   0
#define Kionx_Acc 1
#define    FILTERWINDOWLENGTH     16
#define    WINDOW 16


	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
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
/*-------------变量声明--------------*/

//***********************************************************************/


SENSOR_EXT int16u PressureBuf;//气压值
SENSOR_EXT int16u TempBuf;//温度值
SENSOR_EXT int16u HumiBuf;//湿度值

SENSOR_EXT int16u TempCalib[2];//校准温度值
//SENSOR_EXT float TempCalib0;//校准温度值

SENSOR_EXT int16u TempActual[2];//当前的温度值
//SENSOR_EXT float TempActual0;//当前的温度值
SENSOR_EXT int16u TempCalibComp;
SENSOR_EXT float TempCalib_K;//校准温度值
SENSOR_EXT float TempCalib_A;//校准温度值
SENSOR_EXT int16u TempOffset;



