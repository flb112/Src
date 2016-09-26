/***********************************************************************
* �ļ�����	canbus.h
* �桡����	v1.0
* ��  ����	canbusЭ��ջͷ�ļ�
* �����ˣ�	huangxinglong
* �������ڣ�2010-8-19 10:59

***********************************************************************/
/***********************************************************************/
#include	"_Type.h"
/***********************************************************************/
#ifndef __CANBUS_H__
	#define __CANBUS_H__
/*******************************************************************************
   CANopen NMT constants
*******************************************************************************/
	//head_message  data_frame     //��Ϣ�^
	#define HEADMSG_FRAMSEQUENC            0x00

	#define HEADMSG_FRAMSEQUENCE_SINGLE    0xc0//��Ϣ�^����������-�Ύ�   0000 0000
	#define HEADMSG_FRAMSEQUENCE_FIRST     0x40//��Ϣ�^����������-��һ�� 0100 0000
	#define HEADMSG_FRAMSEQUENCE_MIDDLE    0x80//��Ϣ�^����������-���g�� 1000 0000
	#define HEADMSG_FRAMSEQUENCE_LAST      0xc0//��Ϣ�^����������-���Ꭼ 1100 0000

	//MAIN COMMANDS  WORD                   //��������
    #define MAINCOMMANDS_BUFPOSI            0//�������֔�������λ��0
    #define MAINCOMMANDS_RD                 0x01//�������ֶ�ȡ��̬����
    #define MAINCOMMANDS_MACHINEPARAMETER   0x02//�������ֶ�д�豸����

    #define MAINCOMMANDS_RST                0x03//��λ
//    #define MAINCOMMANDS_BitRate            0x03//�������ֶ�д�����ʡ��豸��ַ������ʱ��
//    #define MAINCOMMANDS_WORD               0x04//




	//SUB COMMANDS  WORD                   //��������
    #define SUBCOMMANDS_BUFPOSI            1//�������֔�������λ��1
    #define SUBCOMMANDS_WORD_TIMERSTAMP    0x10//�������֡��r�g��
    #define SUBCOMMANDS_WORD_XDATATYPE     0x01//�������֡��������X�S����
    #define SUBCOMMANDS_WORD_YDATATYPE     0x02//�������֡��������Y�S����
    #define SUBCOMMANDS_WORD_ZDATATYPE     0x03//�������֡��������Z�S����
    #define SUBCOMMANDS_WORD_ALLDATATYPE   0x04//�������֡��������XYZ�S����

    #define SLAVECOMMANDS_DATALENGTH_L       1//�؏͔����L�Ⱦ���λ��1
    #define SLAVECOMMANDS_DATALENGTH_H       2//�͔����L�Ⱦ���λ��2

    #define SLAVECOMMANDS_TIMERSTAMP_L       3//ʱ���
    #define SLAVECOMMANDS_TIMERSTAMP_L1      4//ʱ���
    #define SLAVECOMMANDS_TIMERSTAMP_H       5//ʱ���
    #define SLAVECOMMANDS_TIMERSTAMP_H1      6//ʱ���
    #define SLAVECOMMANDS_DATA1              7//����1����λ��

    #define SLAVECOMMANDS_DATA2              1//����2����λ��
    #define SLAVECOMMANDS_DATA3              2//����2����λ��
    #define SLAVECOMMANDS_DATA4              3//����2����λ��
    #define SLAVECOMMANDS_DATA5              4//����2����λ��3
    #define SLAVECOMMANDS_DATA6              5//����2����λ��
    #define SLAVECOMMANDS_DATA7              6//����2����λ��
    #define SLAVECOMMANDS_DATA8              7//����2����λ��


    #define SENDCNT                          3//�ظ����͵Ĵ���


    #define SETSUCCESS                       0x20
    #define SETFAIL                          0x00


/*******************************************************************************
   COB-IDs in CANbus
*******************************************************************************/
	/*-----�ṹ���������Ͷ���---------*/
typedef struct 
{	
	int8u	    maincommands; //  ��������
	int8u	    slavecommands;// ��������
	int8u		attribute;	  //��д
    
	int16u* 	addr;         // POINTER to data (RAM or ROM memory) ���޸�             
//	int16u		saddr;               

} UM_TABLE;


extern uint16_t can_buf_head;
extern uint16_t buf_flag_head;
extern uint8_t  moves_flag;
   
/*************************************************
//   ��������
**************************************************/ 
void Rx_CANBUS_Proc (void);
void Tx_CANBUS_Proc (void);
void Co_ClrCan (void);
#endif



/*-----------------------------------*/
#ifdef   __CANBUS_C__
#define  CANBUS_EXT
#else
#define	 CANBUS_EXT  extern
#endif

CANBUS_EXT int16u XdataBuf;
CANBUS_EXT int16u YdataBuf;
CANBUS_EXT int16u ZdataBuf;
CANBUS_EXT int16u AlldataBuf[3];
CANBUS_EXT int16u TXC_INDEX;
CANBUS_EXT int16u DeviceFir_Type;
CANBUS_EXT int16u DeviceFir_Version;
CANBUS_EXT int16u DeviceFir_Number[2];
CANBUS_EXT int16u Acqu_Cycle;


