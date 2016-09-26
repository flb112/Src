/*******************************Copyright (c)***************************/
/*
 *
 *
 *                       http://www.sinocomopto.com/
 */
/********************************文件信息*******************************/
/*
 * 文件名：	Comm.h
 *版　本:	v1.0
 *创建人:	孙娜
 *日　期:	2009-11-17 9:31
 *描　述:	
 *
 *当前版本:	v1.0
 *修改人:	孙娜
 *日　期:	2009-11-17 9:31
 *描　述:	
 */
/***********************************************************************/
#ifndef _TYPE_H_
	#define _TYPE_H_
//----------------------------------------------------
//数据类型重定义	
//	#define uchar unsigned char
//	#define byte unsigned char
	#define bool unsigned char
//	#define uint unsigned int
//	#define word unsinged int
//	
//	#define UCHAR uchar
//	#define BYTE byte
//	#define BOOL unsinge char
//	#define UINT uint
//	#define WORD word
	
//	#define int8u unsigned char
//	#define int16u unsigned int
//	#define int32u unsigned long
//	#define int8s signed char
//	#define int16s signed char
//	#define int32s long
	
//	#define INT8U int8u
//	#define INT16U int16u
//	#define INT32U int32u
//	#define INT8S int8s
//	#define INT16S int16s
//	#define INT32S int32s

    #define   int8u     unsigned  char
    #define   int16u    unsigned  short
    #define   int32u    unsigned  long
    #define   int8s     signed    char
    #define   int16s    signed    short
    #define   int32s    signed    long
/*	#define int8u  uint8_t
	#define int16u uint16_t
 
    #define int32u uint32_t
	#define int8s  int8_t
	#define int16s int16_t
	#define int32s int32_t   
*/  
    #define  FALSE      0
    #define   TRUE      1
    #define   NO        1
    #define   YES       0
    #define   LITTLE    1
    #define   BIG       0

//-----------------------------------------------
   typedef struct {
      int16u 	index;               // Index of OD entry
      int8u		subindex;            // Subindex of OD entry
      int8u		attribute;           // Attributes
      int8u		length;              // Data length in bytes
	  void*		pData;               // POINTER to data (RAM or ROM memory) 有修改
   } CO_OD_TABLE;	
   
   typedef struct{
      unsigned int bit0 :1;
      unsigned int bit1 :1;
      unsigned int bit2 :1;
      unsigned int bit3 :1;
      unsigned int bit4 :1;
      unsigned int bit5 :1;
      unsigned int bit6 :1;
      unsigned int bit7 :1;
      }tData8bits;

   typedef union{
      unsigned char BYTE[1];
      tData8bits    BYTEbits[1];
      }tData1byte;

   typedef union{
      unsigned int  WORD[1];
      unsigned char BYTE[2];
      tData8bits    BYTEbits[2];
      }tData2bytes;

   typedef union{
      unsigned long DWORD[1];
      unsigned int  WORD[2];
      unsigned char BYTE[4];
      tData8bits    BYTEbits[4];
      }tData4bytes;

   typedef union{
      unsigned long DWORD[2];
      unsigned int  WORD[4];
      unsigned char BYTE[8];
      tData8bits    BYTEbits[8];
      }tData8bytes;   
   
 //SDO通信参数结构
	typedef struct{
		int32u	COB_ID_Client_to_Server;
		int32u	COB_ID_Server_to_Client;
		int8u	Node_Id;
		}SDO_COMM_PARAM;


	typedef struct{
		int8u	NoOfEntries;
		int8u	Communication_Error;
		}ODEs_Error_Behavior;  
		
	typedef struct{
		tData2bytes  Ident;        //Can message identifier aligned with hardware registers
                           //11 bit COB-ID and Remote Transfer Request bit are included
                           //see CO_IDENT_WRITE macro in CO_driver.h
                           //Remote transfer request bit:
                           //Reception: receive message only if matched RTR
                           //Transmission: send normal or RTR message
		unsigned int NoOfBytes :4; //length of message, if>8, length is not monitored at receive
		unsigned int NewMsg    :1; //flag bit, if new message has received (is waiting to be sent)
		unsigned int Inhibit   :1; //Reception: Data are not copied, if Inhibit==1 AND NewMsg==1
                           //Transmission: message is not sent if Inhibit==1 AND time is outside
                           //CO_SYNCwindow (Flag for synchronous TPDO messages)
		tData8bytes  Data;         //Data
//        unsigned int  Data[8];
        
        
        
	}	CO_CANMESSAGE;		
//TPDO通信参数结构
	typedef struct{
		int32u   COB_ID;
		int8u    Trans_Type;		//TPDO发送类型
		int16u   Inhibit_Time;		//TPDO禁止时间
		int8u	 reserve;			//保留项		
		int16u   Event_Timer;		//TPDO定时发送 若定时发送时间为0则由远程帧触发
		}TPDO_COMM_PARAM;
		
	typedef struct{
		int8u      NoOfEntries;
		int32u     Vendor_ID;
		int32u     Product_Code;
		int32u     Revision_Number;
		int32u     Serial_Number;
	}CO_IDENTITY; 	
////////////////////////////////////2010.10.27修改增加
//	typedef struct{
//		int8u 		Inhibit;			//
//		int16u		Change_Angx;	//1-1000	= 0.01-10	
//		int16u		Chagne_Angy;	//
//	}TPDO_EVENT_PARAM;
	typedef struct{
		int32u   COB_ID;
		int8u    Trans_Type;
	}CO_sRPDO_param;
	
   typedef struct{
      unsigned char BYTE[2];
      }Data2bytes;	
///////////////////////////////////2010.10.27修改增加结束
#endif
