
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
#ifndef __CAN_DRIVE_H__
	#define __CAN_DRIVE_H__
#include "stm32f10x_can.h"
#include "_Type.h"
	/*-----宏定义---------------------*/
//CAN接收RX0中断使能
#define CAN_RX0_INT_ENABLE	1		//0,不使能;1,使能.
#define CO_RXCAN_NUM_MSGS       20
#define CO_TXCAN_NUM_MSGS       20//(BUF_SIZE/7+1)

#define MAIN_SLAVE              0x00   //主--从模式
#define SLAVE_MAIN              0x400   //从-主模式
//#define NODE_ID                 0x01
#define CAN_ID_RPDO0        	0x100   // + NODE ID 读X轴的值
#define CAN_ID_RPDO1        	0x200   // + NODE ID 读Y轴的值
#define CAN_ID_RPDO2        	0x300   // + NODE ID 读Z轴的值






/*-----结构体数据类型定义---------*/
	typedef struct{
                        uint32_t StdId;  /*!< Specifies the standard identifier.标准标识符
                        This parameter can be a value between 0 to 0x7FF. */

//                        uint32_t ExtId;  /*!< Specifies the extended identifier.设置扩展标示符（29位）
 //                       This parameter can be a value between 0 to 0x1FFFFFFF. */

//                        uint8_t IDE;  
                        /*!< Specifies the type of identifier for the message that 使用扩展标识符 CAN_ID_STD;//(标识符模式)// CAN_ID_EXT （扩展标识符模式）
                                            will be received. This parameter can be a value of 
                                           @ref CAN_identifier_type */

//                        uint8_t RTR;    
                        /*!< Specifies the type of frame for the received message.
                        This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

//                        uint8_t DLC;
                        /*!< Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8 */ 
      
      
      
                       uint8_t  NewMsg    :1; //flag bit, if new message has received (is waiting to be sent)
                       uint8_t  Data[8];
                       int8u 	SendCount;			//发送的次数      
       
        
        
        
	}	CO_CANBUSMESSAGE;


	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/

								    
										 							 				    
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode); //CAN初始化
u8 CAN_Mode_Init1(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化 
u8 Can_Send_Msg(CO_CANBUSMESSAGE* msg);					   //发送数据

u8 Can_Receive_Msg(u8 *buf);							  //接收数据
void Can_BaudSetGet (int16u *bitrate);                                              //波特率设置
int16u  NodeId_Rd(void);                                                        //节点Id读取
void NodeId_Wr(int16u id);                                                        //节点Id修改
uint8_t can_send(uint8_t *txd,uint8_t len);
u8 Text_Send_Msg(void);
u8 T_Send_Msg(int8u * msg);
 void CanStandbyMode_Init(void);
 
/*-------------变量声明--------------*/
extern int8u Can_Mode;     // 
extern int8u RD_MOD;
extern int16u CAN_data_buf_head;
extern int16u NODE_ID;       //節點ID
extern int16u CO_BitRate;         //波特率
extern CanRxMsg Re_CanBuf[8];
extern CO_CANBUSMESSAGE	Co_RxCan[CO_RXCAN_NUM_MSGS];
extern CO_CANBUSMESSAGE	Co_TxCan[CO_TXCAN_NUM_MSGS];


#endif

//***********************************************************************/
#ifdef   _CAN_DRIVE_C_
#define  CAN_EXT
#else
#define  CAN_EXT  extern
#endif

//***********************************************************************/












