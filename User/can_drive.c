//*****************��*��*��*Ϣ********************************************
// *�� �� ����Can.c
// *�桡  ����v0.0
// *�� �� �ˣ�������
// *�ա�  �ڣ�2013-12-24 16:04
// *�衡  ����
// *
// *��ǰ�汾��v0.0
// *�� �� �ˣ�������
// *�ա�  �ڣ�2013-12-24 16:04
// *�衡  ����
//************************************************************************
#define   _CAN_DRIVE_C_
#include "_Include.h"




//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:1~3; CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.��Χ:1~8;
//tbs1:ʱ���1��ʱ�䵥Ԫ.��Χ:1~16;	  CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024;(ʵ��Ҫ��1,Ҳ����1~1024) tq=(brp)*tpclk1
//ע�����ϲ����κ�һ����������Ϊ0,�������.
//������=Fpclk1/((tsjw+tbs1+tbs2)*brp);
//mode:0,��ͨģʽ;1,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Normal_Init(1,8,7,5,1);
//������Ϊ:36M/((1+8+7)*5)=450Kbps
//����ֵ:0,��ʼ��OK;
//����,��ʼ��ʧ��;

//************************************************************************
// *��    �ܣ�CANģʽ��ʼ��	
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//************************************************************************
// CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_7tq,5,CAN_Mode_Normal)

//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Normal_Init(1,8,7,5,1);

//������Ϊ:36M/((8+9+1)*4)=500Kbps
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
 	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

    
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
//************************TX����**********************************************
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	    //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);		        //��ʼ��IO
//************************RX����**********************************************   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);              //��ʼ��IO
	  
 	//CAN��Ԫ����
 	CAN_InitStructure.CAN_TTCM=DISABLE;						 //��ʱ�䴥��ͨ��ģʽ  //
 	CAN_InitStructure.CAN_ABOM=ENABLE;//DISABLE;			//����Զ����߹���	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 //��ֹ�����Զ����� //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //���Ĳ�����,�µĸ��Ǿɵ� // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //���ȼ��ɱ��ı�ʶ������ //
  	CAN_InitStructure.CAN_Mode= mode;	                     //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; //


  	CAN_InitStructure.CAN_SJW=tsjw;				            //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1;                         //Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;                         //Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;                    //��Ƶϵ��(Fdiv)Ϊbrp+1	//


  	CAN_Init(CAN1, &CAN_InitStructure);                     // ��ʼ��CAN1 

 	CAN_FilterInitStructure.CAN_FilterNumber=0;	            //������0
 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //��ʶ��ģʽ
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ ������
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;        ////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;    //32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
 	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;    //���������0

  	CAN_FilterInit(&CAN_FilterInitStructure);               //�˲�����ʼ��
#if CAN_RX0_INT_ENABLE
	
//	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                  //FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 01;  // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                  //FIFO0��Ϣ�Һ��ж�����.		    
  
/*
    NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
*/
#endif
//    Can_BaudSetGet (125);
	return 0;
}   
 
#if CAN_RX0_INT_ENABLE	//ʹ��RX0�ж�
	
//************************************************************************
// *��    �ܣ��жϷ�����	
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//************************************************************************
void USB_LP_CAN1_RX0_IRQHandler(void)
{   u8 i,j;
    static u8 tmp=0;
  
    CanRxMsg RxMessage;
    CAN_Receive(CAN1, 0, &RxMessage);
  
    if((RxMessage.StdId&0x400)!=MAIN_SLAVE) return;  //����--�ӻ� ģʽ�ж�
    
    if((RxMessage.StdId&0x07f)!=NODE_ID) return;    //�жϽڵ�ID �Լ��㲥֡������

    if(tmp>CO_RXCAN_NUM_MSGS-1)tmp=0;  
    
    for(j=tmp;j<CO_RXCAN_NUM_MSGS;j++)
    {
        if((Co_RxCan[j].NewMsg)==0) 	//ƥ����ձ���COB��ID
        {
           tmp=j;
           Co_RxCan[j].NewMsg=1;
           
//           Co_RxCan[j].StdId=RxMessage.StdId;        //���R��
           Co_RxCan[j].StdId=(RxMessage.StdId|SLAVE_MAIN);        //���R��           
           for(i=0;i<8;i++)
           {
             
              Co_RxCan[j].Data[i] = RxMessage.Data[i];
              RxMessage.Data[i]=0;
           }
           break;
        }
      
    }
//    Rx_CANBUS_Proc();    
    
}
/*
USB_HP_CAN1_TX_IRQHandler
{



}*/
#endif


//************************************************************************
// *��    �ܣ�can����һ������  (�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
// *��    �ڣ�buf:���ݻ�����;
// *��    �ڣ�0,�ɹ�;;
// *��    ע��len:���ݳ���(���Ϊ8) msg:����ָ��,���Ϊ8���ֽ�.
// *�������ʣ�
//************************************************************************
u8 Can_Send_Msg(CO_CANBUSMESSAGE* msg)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=msg->StdId;					 // ��׼��ʶ��Ϊ0
//  TxMessage.ExtId=msg->ExtId;				     // ������չ��ʾ����29λ��
  TxMessage.IDE=CAN_ID_STD;			             // ʹ����չ��ʶ�� CAN_ID_STD;//(��ʶ��ģʽ)// CAN_ID_EXT ����չ��ʶ��ģʽ��
  TxMessage.RTR=CAN_RTR_DATA;                    //	CAN_RTR_REMOTE	                // ��Ϣ����Ϊ����֡��һ֡8λ CAN_RTR_DATA
  TxMessage.DLC=8;					             // 


  for(i=0;i<8;i++)
  TxMessage.Data[i]=msg->Data[i];				 //          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;		

}

//************************************************************************
// *��    �ܣ�can�ڽ������ݲ�ѯ
// *��    �ڣ�buf:���ݻ�����;
// *��    �ڣ�0,�����ݱ��յ�;
// *��    ע������,���յ����ݳ���;
// *�������ʣ�
//************************************************************************
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}





/***********************************************************************/
// *���ܣ�CANλ������
// *��ڣ�
// *���ڣ�
// *��ע��
// *�������ʣ�
/***********************************************************************/
void Can_BaudSetGet (int16u *bitrate)
{  
/*    u8 i=0;
   	CAN1->MCR|=1<<0;		//����CAN�����ʼ��ģʽ
	while((CAN1->MSR&1<<0)==0)
	{
		i++;
		if(i>100)return 2;//�����ʼ��ģʽʧ��
	} 
*/    

    
	switch(*bitrate)
	{
		case 0:			                    //125k
//                 CAN1->BTR=16;
                 
          CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,32,CAN_Mode_Normal);                 
                 
          break;
         
		case 1:	                            //250k
//                CAN1->BTR|=8;
          
          CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,16,CAN_Mode_Normal);          
          break;
          
		case 2:			                    //500k
//                 CAN1->BTR|=4;
          CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,CAN_Mode_Normal);


          break;
         
		case 3:	
                            //1000k
                
          CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);          
//          CAN1->BTR|=2;
          break;
          
          
		default:		
          CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);
          *bitrate=3;

//          CAN1->BTR|=4;
          break;		//500k
	}
}




/***********************************************************************/
// *���ܣ��޸Ľڵ��ַ
// *��ڣ�
// *���ڣ�
// *��ע��
// *�������ʣ�
/***********************************************************************/
void NodeId_Wr(int16u id)
{
    NODE_ID=id;

}


/***********************************************************************/
// *���ܣ���ȡ�ڵ��ַ
// *��ڣ�
// *���ڣ�
// *��ע��
// *�������ʣ�
/***********************************************************************/
int16u  NodeId_Rd(void)
{
    return NODE_ID;
}







//************************************************************************
// *��    �ܣ������� �ڷ����¶ȣ�ʪ�ȣ����ٶȣ���ѹ
// *��    �ڣ�buf:���ݻ�����;
// *��    �ڣ�0,�ɹ�;;
// *��    ע��len:���ݳ���(���Ϊ8) msg:����ָ��,���Ϊ8���ֽ�.
// *�������ʣ�
//************************************************************************
/*u8 Text_Send_Msg(void)
{	
  u8 mbox;
  u16 i=0;
  static u16 recvcnt=0;
  CanTxMsg TxMessage;

  if(!RdReturn) return 0;//�ظ��ź�

     TxMessage.StdId=0x480+NODE_ID;//msg->StdId;	// ��׼��ʶ��Ϊ0
     TxMessage.ExtId=00;//msg->ExtId;				// ������չ��ʾ����29λ��
     TxMessage.IDE=CAN_ID_STD;			            // ʹ����չ��ʶ�� CAN_ID_STD;//(��ʶ��ģʽ)// CAN_ID_EXT ����չ��ʶ��ģʽ��
     TxMessage.RTR=CAN_RTR_DATA;                    //	CAN_RTR_REMOTE	                // ��Ϣ����Ϊ����֡��һ֡8λ CAN_RTR_DATA
     TxMessage.DLC=7;					            // 

    if (recvcnt >=BUF_SIZE)//5376CAN_data_buf_head
    { 
	       
        TxMessage.Data[0]=(HEADMSG_FRAMSEQUENCE_LAST|8) ;   //β֡
        for(i=0;i<6;i++)
        {   
            TxMessage.Data[i+1]=0;	       
        }
        recvcnt =0;
        RdReturn=0;
        mbox= CAN_Transmit(CAN1, &TxMessage);   
        i=0;
       while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
        if(i>=0XFFF)return 1;
        return 0;    
       
    }  
    else
    {  
        TxMessage.Data[0]=(HEADMSG_FRAMSEQUENCE_MIDDLE|8); //�м�֡
        for(i=0;i<6;i++)
        {   
            TxMessage.Data[i+1]=raw_data_buf[i+recvcnt];	       
        }
        recvcnt=recvcnt+6;

        mbox= CAN_Transmit(CAN1, &TxMessage);   
       i=0;
      while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
       if(i>=0XFFF)return 1;
       return 0;		
    }
 
}
*/
u8 Text_Send_Msg(void)
{	
  u8 mbox;
  u16 i=0;
  static u16 recvcnt=0;
  CanTxMsg TxMessage;

  if(!RdReturn) return 0;//�ظ��ź�

     TxMessage.StdId=0x480+NODE_ID;//msg->StdId;	// ��׼��ʶ��Ϊ0
     TxMessage.ExtId=00;//msg->ExtId;				// ������չ��ʾ����29λ��
     TxMessage.IDE=CAN_ID_STD;			            // ʹ����չ��ʶ�� CAN_ID_STD;//(��ʶ��ģʽ)// CAN_ID_EXT ����չ��ʶ��ģʽ��
     TxMessage.RTR=CAN_RTR_DATA;                    //	CAN_RTR_REMOTE	                // ��Ϣ����Ϊ����֡��һ֡8λ CAN_RTR_DATA
     TxMessage.DLC=7;					            // 

 
      while(recvcnt<BUF_SIZE)
      {  
            TxMessage.Data[0]=(HEADMSG_FRAMSEQUENCE_MIDDLE|8); //�м�֡
            for(i=0;i<6;i++)//  HXL 0530
            {   
                TxMessage.Data[i+1]=raw_data_buf[i+recvcnt];	       
            }
//          recvcnt=recvcnt+6;
            mbox= CAN_Transmit(CAN1, &TxMessage);  
        
            i=0;
            while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
            if(i>=0XFFF)
            return 1;
            recvcnt=recvcnt+6;//  HXL 0530          
      }     
        TxMessage.Data[0]=(HEADMSG_FRAMSEQUENCE_LAST|8) ;   //β֡
        for(i=0;i<6;i++)
        {   
            TxMessage.Data[i+1]=0;	       
        }
        recvcnt =0;
        RdReturn=0;
        mbox= CAN_Transmit(CAN1, &TxMessage);             
 
}


/**********************SDA  Output****************************/
 void CanStandbyMode_Init(void)
 {    

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);  
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);     
   
    GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//stop
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�����������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO���ٶ�Ϊ50MHz   
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //HIGH 8BIT    
    GPIO_ResetBits(GPIOB,GPIO_Pin_3);   //LOW��ͨģʽ
    
   
}
