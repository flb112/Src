		    




//*****************��*��*��*Ϣ********************************************
// *�� �� ����SPI.h
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
#ifndef __SPI_H__
	#define __SPI_H__

	/*-----�궨��---------------------*/
// SPI�����ٶ����� 
#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7
						  	    													  

		 
	/*-----�ṹ���������Ͷ���---------*/
	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
void SPI2_Init(void);			 //��ʼ��SPI2��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI2�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI2���߶�дһ���ֽ�


#endif
//***********************************************************************/
#ifdef   __SPI_C__
#define  SPI_EXT
#else
#define  SPI_EXT  extern
#endif
/*-------------��������--------------*/
SPI_EXT int8u X_Axis_Buf;     // 
SPI_EXT int8u Y_Axis_Buf;
SPI_EXT int8u Z_Axis_Buf;
//***********************************************************************/