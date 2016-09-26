/***********************************************************************
* 文件名：	canbus.h
* 版　本：	v1.0
* 描  述：	canbus协议栈头文件
* 创建人：	huangxinglong
* 创建日期：2010-8-19 10:59

***********************************************************************/
/***********************************************************************/
#include	"_Type.h"
/***********************************************************************/
#ifndef __CANBUS_H__
	#define __CANBUS_H__
/*******************************************************************************
   CANopen NMT constants
*******************************************************************************/
	//head_message  data_frame     //消息頭
	#define HEADMSG_FRAMSEQUENC            0x00

	#define HEADMSG_FRAMSEQUENCE_SINGLE    0xc0//消息頭數據幀序列-單幀   0000 0000
	#define HEADMSG_FRAMSEQUENCE_FIRST     0x40//消息頭數據幀序列-第一幀 0100 0000
	#define HEADMSG_FRAMSEQUENCE_MIDDLE    0x80//消息頭數據幀序列-中間幀 1000 0000
	#define HEADMSG_FRAMSEQUENCE_LAST      0xc0//消息頭數據幀序列-最後幀 1100 0000

	//MAIN COMMANDS  WORD                   //主命令字
    #define MAINCOMMANDS_BUFPOSI            0//主命令字數據緩存位置0
    #define MAINCOMMANDS_RD                 0x01//主命令字读取姿态数据
    #define MAINCOMMANDS_MACHINEPARAMETER   0x02//主命令字读写设备数据

    #define MAINCOMMANDS_RST                0x03//复位
//    #define MAINCOMMANDS_BitRate            0x03//主命令字读写波特率、设备地址、运行时间
//    #define MAINCOMMANDS_WORD               0x04//




	//SUB COMMANDS  WORD                   //子命令字
    #define SUBCOMMANDS_BUFPOSI            1//子命令字數據緩存位置1
    #define SUBCOMMANDS_WORD_TIMERSTAMP    0x10//子命令字—時間戳
    #define SUBCOMMANDS_WORD_XDATATYPE     0x01//子命令字—數據類型X軸數據
    #define SUBCOMMANDS_WORD_YDATATYPE     0x02//子命令字—數據類型Y軸數據
    #define SUBCOMMANDS_WORD_ZDATATYPE     0x03//子命令字—數據類型Z軸數據
    #define SUBCOMMANDS_WORD_ALLDATATYPE   0x04//子命令字—數據類型XYZ軸數據

    #define SLAVECOMMANDS_DATALENGTH_L       1//回復數據長度緩存位置1
    #define SLAVECOMMANDS_DATALENGTH_H       2//復數據長度緩存位置2

    #define SLAVECOMMANDS_TIMERSTAMP_L       3//时间戳
    #define SLAVECOMMANDS_TIMERSTAMP_L1      4//时间戳
    #define SLAVECOMMANDS_TIMERSTAMP_H       5//时间戳
    #define SLAVECOMMANDS_TIMERSTAMP_H1      6//时间戳
    #define SLAVECOMMANDS_DATA1              7//數據1緩存位置

    #define SLAVECOMMANDS_DATA2              1//數據2緩存位置
    #define SLAVECOMMANDS_DATA3              2//數據2緩存位置
    #define SLAVECOMMANDS_DATA4              3//數據2緩存位置
    #define SLAVECOMMANDS_DATA5              4//數據2緩存位置3
    #define SLAVECOMMANDS_DATA6              5//數據2緩存位置
    #define SLAVECOMMANDS_DATA7              6//數據2緩存位置
    #define SLAVECOMMANDS_DATA8              7//數據2緩存位置


    #define SENDCNT                          3//重复发送的次数


    #define SETSUCCESS                       0x20
    #define SETFAIL                          0x00


/*******************************************************************************
   COB-IDs in CANbus
*******************************************************************************/
	/*-----结构体数据类型定义---------*/
typedef struct 
{	
	int8u	    maincommands; //  主命令字
	int8u	    slavecommands;// 子命令字
	int8u		attribute;	  //读写
    
	int16u* 	addr;         // POINTER to data (RAM or ROM memory) 有修改             
//	int16u		saddr;               

} UM_TABLE;


extern uint16_t can_buf_head;
extern uint16_t buf_flag_head;
extern uint8_t  moves_flag;
   
/*************************************************
//   函数声明
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


