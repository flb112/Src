		    




//*****************文*件*信*息********************************************
// *文 件 名：SPI.h
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
#ifndef __SPI_H__
	#define __SPI_H__

	/*-----宏定义---------------------*/
// SPI总线速度设置 
#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7
						  	    													  

		 
	/*-----结构体数据类型定义---------*/
	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
void SPI2_Init(void);			 //初始化SPI2口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI2速度   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI2总线读写一个字节


#endif
//***********************************************************************/
#ifdef   __SPI_C__
#define  SPI_EXT
#else
#define  SPI_EXT  extern
#endif
/*-------------变量声明--------------*/
SPI_EXT int8u X_Axis_Buf;     // 
SPI_EXT int8u Y_Axis_Buf;
SPI_EXT int8u Z_Axis_Buf;
//***********************************************************************/