		    




//*****************��*��*��*Ϣ********************************************
// *�� �� ����spi_drive.h
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

#ifndef __SPIDRIVE_H__
	#define __SPIDRIVE_H__

	/*-----�궨��---------------------*/
// SPI�����ٶ����� 
/*#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7
*/	

#define BUF_SIZE  16764//8964//7164//5376
		 
	/*-----�ṹ���������Ͷ���---------*/
	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
void SpiDrive_Init(void);
void kx122_read_acc_buf(void);
void AccTran_AngCalc(void);
uint8_t kx122_init(void);
void Times_Stamp(void);
int8u Rd_SegKX122(void);
int8u Select_g_Range(int8u gse);
#endif
//***********************************************************************/
#ifdef   __SPIDRIVE_C__
#define  SPIDRIVE_EXT
#else
#define  SPIDRIVE_EXT  extern
#endif
/*-------------��������--------------*/

SPIDRIVE_EXT uint8_t  flag_motion_det;
SPIDRIVE_EXT u16      raw_data_size ;
SPIDRIVE_EXT uint8_t  raw_data_buf[BUF_SIZE];
SPIDRIVE_EXT uint8_t  Ang_data_buf[6];
SPIDRIVE_EXT uint16_t accel_x;
SPIDRIVE_EXT uint16_t accel_y;
SPIDRIVE_EXT uint16_t accel_z;
SPIDRIVE_EXT uint16_t RdReturn;
SPIDRIVE_EXT uint16_t RdStart;
SPIDRIVE_EXT uint16_t TestMod;
SPIDRIVE_EXT uint16_t KxRange;
SPIDRIVE_EXT uint16_t MappingRange;
SPIDRIVE_EXT uint16_t BufData_Point;
SPIDRIVE_EXT uint16_t Osa_Hz;
//***********************************************************************/