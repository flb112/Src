
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
#ifndef __CAN_DRIVE_H__
	#define __CAN_DRIVE_H__
#include "stm32f10x_can.h"
#include "_Type.h"
	/*-----�궨��---------------------*/
//CAN����RX0�ж�ʹ��
#define CAN_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.
#define CO_RXCAN_NUM_MSGS       20
#define CO_TXCAN_NUM_MSGS       20//(BUF_SIZE/7+1)

#define MAIN_SLAVE              0x00   //��--��ģʽ
#define SLAVE_MAIN              0x400   //��-��ģʽ
//#define NODE_ID                 0x01
#define CAN_ID_RPDO0        	0x100   // + NODE ID ��X���ֵ
#define CAN_ID_RPDO1        	0x200   // + NODE ID ��Y���ֵ
#define CAN_ID_RPDO2        	0x300   // + NODE ID ��Z���ֵ






/*-----�ṹ���������Ͷ���---------*/
	typedef struct{
                        uint32_t StdId;  /*!< Specifies the standard identifier.��׼��ʶ��
                        This parameter can be a value between 0 to 0x7FF. */

//                        uint32_t ExtId;  /*!< Specifies the extended identifier.������չ��ʾ����29λ��
 //                       This parameter can be a value between 0 to 0x1FFFFFFF. */

//                        uint8_t IDE;  
                        /*!< Specifies the type of identifier for the message that ʹ����չ��ʶ�� CAN_ID_STD;//(��ʶ��ģʽ)// CAN_ID_EXT ����չ��ʶ��ģʽ��
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
                       int8u 	SendCount;			//���͵Ĵ���      
       
        
        
        
	}	CO_CANBUSMESSAGE;


	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/

								    
										 							 				    
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode); //CAN��ʼ��
u8 CAN_Mode_Init1(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ�� 
u8 Can_Send_Msg(CO_CANBUSMESSAGE* msg);					   //��������

u8 Can_Receive_Msg(u8 *buf);							  //��������
void Can_BaudSetGet (int16u *bitrate);                                              //����������
int16u  NodeId_Rd(void);                                                        //�ڵ�Id��ȡ
void NodeId_Wr(int16u id);                                                        //�ڵ�Id�޸�
uint8_t can_send(uint8_t *txd,uint8_t len);
u8 Text_Send_Msg(void);
u8 T_Send_Msg(int8u * msg);
 void CanStandbyMode_Init(void);
 
/*-------------��������--------------*/
extern int8u Can_Mode;     // 
extern int8u RD_MOD;
extern int16u CAN_data_buf_head;
extern int16u NODE_ID;       //���cID
extern int16u CO_BitRate;         //������
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












