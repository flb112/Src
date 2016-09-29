//*****************文*件*信*息********************************************
// *文 件 名：Can.c
// *版　  本：v0.0
// *创 建 人：黄兴龙
// *日　  期：2013-12-24 16:04
// *描　  述：
// *
// *当前版本：v0.0
// *修 改 人：黄兴龙
// *日　  期：2013-12-24 16:04
// *描　  述：
//************************************************************************
#define   _CAN_DRIVE_C_
#include "_Include.h"




//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:1~3; CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
//tbs2:时间段2的时间单元.范围:1~8;
//tbs1:时间段1的时间单元.范围:1~16;	  CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024;(实际要加1,也就是1~1024) tq=(brp)*tpclk1
//注意以上参数任何一个都不能设为0,否则会乱.
//波特率=Fpclk1/((tsjw+tbs1+tbs2)*brp);
//mode:0,普通模式;1,回环模式;
//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Normal_Init(1,8,7,5,1);
//则波特率为:36M/((1+8+7)*5)=450Kbps
//返回值:0,初始化OK;
//其他,初始化失败;

//************************************************************************
// *功    能：CAN模式初始化	
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//************************************************************************
// CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_7tq,5,CAN_Mode_Normal)

//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Normal_Init(1,8,7,5,1);

//则波特率为:36M/((8+9+1)*4)=500Kbps
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
 	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

    
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
//************************TX配置**********************************************
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	    //复用推挽
    GPIO_Init(GPIOA, &GPIO_InitStructure);		        //初始化IO
//************************RX配置**********************************************   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       //上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);              //初始化IO
	  
 	//CAN单元设置
 	CAN_InitStructure.CAN_TTCM=DISABLE;						 //非时间触发通信模式  //
 	CAN_InitStructure.CAN_ABOM=ENABLE;//DISABLE;			//软件自动离线管理	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 //禁止报文自动传送 //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //报文不锁定,新的覆盖旧的 // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //优先级由报文标识符决定 //
  	CAN_InitStructure.CAN_Mode= mode;	                     //模式设置： mode:0,普通模式;1,回环模式; //


  	CAN_InitStructure.CAN_SJW=tsjw;				            //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1;                         //Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;                         //Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;                    //分频系数(Fdiv)为brp+1	//


  	CAN_Init(CAN1, &CAN_InitStructure);                     // 初始化CAN1 

 	CAN_FilterInitStructure.CAN_FilterNumber=0;	            //过滤器0
 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //标识符模式
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 过滤器
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;        ////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;    //32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
 	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;    //激活过滤器0

  	CAN_FilterInit(&CAN_FilterInitStructure);               //滤波器初始化
#if CAN_RX0_INT_ENABLE
	
//	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                  //FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 01;  // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                  //FIFO0消息挂号中断允许.		    
  
/*
    NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
*/
#endif
//    Can_BaudSetGet (125);
	return 0;
}   
 
#if CAN_RX0_INT_ENABLE	//使能RX0中断
	
//************************************************************************
// *功    能：中断服务函数	
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//************************************************************************
void USB_LP_CAN1_RX0_IRQHandler(void)
{   u8 i,j;
    static u8 tmp=0;
  
    CanRxMsg RxMessage;
    CAN_Receive(CAN1, 0, &RxMessage);
  
    if((RxMessage.StdId&0x400)!=MAIN_SLAVE) return;  //主机--从机 模式判断
    
    if((RxMessage.StdId&0x07f)!=NODE_ID) return;    //判断节点ID 以及广播帧不接收

    if(tmp>CO_RXCAN_NUM_MSGS-1)tmp=0;  
    
    for(j=tmp;j<CO_RXCAN_NUM_MSGS;j++)
    {
        if((Co_RxCan[j].NewMsg)==0) 	//匹配接收报文COB—ID
        {
           tmp=j;
           Co_RxCan[j].NewMsg=1;
           
//           Co_RxCan[j].StdId=RxMessage.StdId;        //標識符
           Co_RxCan[j].StdId=(RxMessage.StdId|SLAVE_MAIN);        //標識符           
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
// *功    能：can发送一组数据  (固定格式:ID为0X12,标准帧,数据帧)	
// *入    口：buf:数据缓存区;
// *出    口：0,成功;;
// *备    注：len:数据长度(最大为8) msg:数据指针,最大为8个字节.
// *函数性质：
//************************************************************************
u8 Can_Send_Msg(CO_CANBUSMESSAGE* msg)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=msg->StdId;					 // 标准标识符为0
//  TxMessage.ExtId=msg->ExtId;				     // 设置扩展标示符（29位）
  TxMessage.IDE=CAN_ID_STD;			             // 使用扩展标识符 CAN_ID_STD;//(标识符模式)// CAN_ID_EXT （扩展标识符模式）
  TxMessage.RTR=CAN_RTR_DATA;                    //	CAN_RTR_REMOTE	                // 消息类型为数据帧，一帧8位 CAN_RTR_DATA
  TxMessage.DLC=8;					             // 


  for(i=0;i<8;i++)
  TxMessage.Data[i]=msg->Data[i];				 //          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;		

}

//************************************************************************
// *功    能：can口接收数据查询
// *入    口：buf:数据缓存区;
// *出    口：0,无数据被收到;
// *备    注：其他,接收的数据长度;
// *函数性质：
//************************************************************************
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}





/***********************************************************************/
// *功能：CAN位率设置
// *入口：
// *出口：
// *备注：
// *函数性质：
/***********************************************************************/
void Can_BaudSetGet (int16u *bitrate)
{  
/*    u8 i=0;
   	CAN1->MCR|=1<<0;		//请求CAN进入初始化模式
	while((CAN1->MSR&1<<0)==0)
	{
		i++;
		if(i>100)return 2;//进入初始化模式失败
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
// *功能：修改节点地址
// *入口：
// *出口：
// *备注：
// *函数性质：
/***********************************************************************/
void NodeId_Wr(int16u id)
{
    NODE_ID=id;

}


/***********************************************************************/
// *功能：读取节点地址
// *入口：
// *出口：
// *备注：
// *函数性质：
/***********************************************************************/
int16u  NodeId_Rd(void)
{
    return NODE_ID;
}







//************************************************************************
// *功    能：调试用 于发送温度，湿度，加速度，气压
// *入    口：buf:数据缓存区;
// *出    口：0,成功;;
// *备    注：len:数据长度(最大为8) msg:数据指针,最大为8个字节.
// *函数性质：
//************************************************************************
/*u8 Text_Send_Msg(void)
{	
  u8 mbox;
  u16 i=0;
  static u16 recvcnt=0;
  CanTxMsg TxMessage;

  if(!RdReturn) return 0;//回复信号

     TxMessage.StdId=0x480+NODE_ID;//msg->StdId;	// 标准标识符为0
     TxMessage.ExtId=00;//msg->ExtId;				// 设置扩展标示符（29位）
     TxMessage.IDE=CAN_ID_STD;			            // 使用扩展标识符 CAN_ID_STD;//(标识符模式)// CAN_ID_EXT （扩展标识符模式）
     TxMessage.RTR=CAN_RTR_DATA;                    //	CAN_RTR_REMOTE	                // 消息类型为数据帧，一帧8位 CAN_RTR_DATA
     TxMessage.DLC=7;					            // 

    if (recvcnt >=BUF_SIZE)//5376CAN_data_buf_head
    { 
	       
        TxMessage.Data[0]=(HEADMSG_FRAMSEQUENCE_LAST|8) ;   //尾帧
        for(i=0;i<6;i++)
        {   
            TxMessage.Data[i+1]=0;	       
        }
        recvcnt =0;
        RdReturn=0;
        mbox= CAN_Transmit(CAN1, &TxMessage);   
        i=0;
       while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
        if(i>=0XFFF)return 1;
        return 0;    
       
    }  
    else
    {  
        TxMessage.Data[0]=(HEADMSG_FRAMSEQUENCE_MIDDLE|8); //中间帧
        for(i=0;i<6;i++)
        {   
            TxMessage.Data[i+1]=raw_data_buf[i+recvcnt];	       
        }
        recvcnt=recvcnt+6;

        mbox= CAN_Transmit(CAN1, &TxMessage);   
       i=0;
      while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
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

  if(!RdReturn) return 0;//回复信号

     TxMessage.StdId=0x480+NODE_ID;//msg->StdId;	// 标准标识符为0
     TxMessage.ExtId=00;//msg->ExtId;				// 设置扩展标示符（29位）
     TxMessage.IDE=CAN_ID_STD;			            // 使用扩展标识符 CAN_ID_STD;//(标识符模式)// CAN_ID_EXT （扩展标识符模式）
     TxMessage.RTR=CAN_RTR_DATA;                    //	CAN_RTR_REMOTE	                // 消息类型为数据帧，一帧8位 CAN_RTR_DATA
     TxMessage.DLC=7;					            // 

 
      while(recvcnt<BUF_SIZE)
      {  
            TxMessage.Data[0]=(HEADMSG_FRAMSEQUENCE_MIDDLE|8); //中间帧
            for(i=0;i<6;i++)//  HXL 0530
            {   
                TxMessage.Data[i+1]=raw_data_buf[i+recvcnt];	       
            }
//          recvcnt=recvcnt+6;
            mbox= CAN_Transmit(CAN1, &TxMessage);  
        
            i=0;
            while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
            if(i>=0XFFF)
            return 1;
            recvcnt=recvcnt+6;//  HXL 0530          
      }     
        TxMessage.Data[0]=(HEADMSG_FRAMSEQUENCE_LAST|8) ;   //尾帧
        for(i=0;i<6;i++)
        {   
            TxMessage.Data[i+1]=0;	       
        }
        recvcnt =0;
        RdReturn=0;
        mbox= CAN_Transmit(CAN1, &TxMessage);             

        return 0;
}


/**********************SDA  Output****************************/
 void CanStandbyMode_Init(void)
 {    

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);  
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);     
   
    GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//stop
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置推挽输出  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz   
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //HIGH 8BIT    
    GPIO_ResetBits(GPIOB,GPIO_Pin_3);   //LOW普通模式
    
   
}
