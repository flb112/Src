//*****************文*件*信*息********************************************
// *文 件 名：Timer.c
// *版　  本：v1.0
// *创 建 人：Huangxinglong
// *日　  期：2014/5/23 15:43:38
// *描　  述：
// *
// *当前版本：v1.0
// *修 改 人：Huangxinglong
// *日　  期：2014/5/23 15:43:43
// *描　  述：
//************************************************************************
#define   __TIMER_C__
#include  "_Include.h"
/*******************************************************************************
* Function Name  :定时
* Description    : 定时器2初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void Timer2_Init(void)           //psc/72000000*arr
{	
    int16u	PrescalerValue = 0;
  
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 10-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值

	PrescalerValue = (uint16_t) (SystemCoreClock / 100000) - 1; // 1s/100000=10us 

	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;	    
    
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_ARRPreloadConfig(TIM2, ENABLE);			                //使能预装载寄存器    

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 	

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	TIM_SetCounter(TIM2,0x0000);
    
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx		

        
  
}

void Times_Stamp(void)
{
		timestamp++;
		time_buf[0] = (uint8_t)(timestamp & 0xff);
		time_buf[1] = (uint8_t)(timestamp>>8 & 0xff);
		time_buf[2] = (uint8_t)(timestamp>>16 & 0xff);
		time_buf[3] = (uint8_t)(timestamp>>24 & 0xff);

}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler	1ms定时
* Description    : 定时器2中断
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)  
{
	static int16u count;
	 //100us定时
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{          
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
    	//-------------------------------------------------------------
                             
        if(count==65535)
       		count=0;
        else              
          	count++; 
//        Text_Send_Msg();        
        if(count%10==0)  
            T10MSFlag=1;		
        if(count%100==0)
            Times_Stamp();//10ms 的时基 时间戳
            T100MSFlag=1;
        if(count%500==0)
        {

            T500MSFlag=1;

        }
        if(count%1000==0)
        {  
          if(++T1SFlag>10)
          {
            T1SFlag=1;
/*         if(RdReturn==0)   //发送数据阶段不触发
        {
            RdStart=1;
        }            
*/
          }
        }
    	//----------------------------------------------------------------------
   }
}
/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
 	 while (1);
}
//***********************************************************************/
// *功    能：Time_Proc()
// *入    口：
// *出    口：
// *备    注：子模式切换，标志处理
// *函数性质：
//***********************************************************************/
void Time_Proc(void)
{
	
    if(T10MSFlag)
    {
         
     
   	}    
}
//************************************************************************
// *功    能：TIM4 500us定时初始化
// *入    口：
// *出    口：
// *备    注：用于变频器的通信
// *函数性质：
//************************************************************************
//int8u  Time4_Init( int16u  timerb50us)
//{	
//	int16u	PrescalerValue = 0;
//    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure ;
//    NVIC_InitTypeDef 	NVIC_InitStructure;
//
//	/* TIM4 clock enable */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	/* Compute the prescaler value */
//	PrescalerValue = (uint16_t) (SystemCoreClock / 2000) - 1; // 1s/2000=500us 
//	/* Time base configuration */
//	TIM_TimeBaseStructure.TIM_Period = timerb50us;			//定时自动载入值
//	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;	
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//值为0该功能不使用
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	
//	TIM_ARRPreloadConfig(TIM4, ENABLE);			//使能预装载寄存器
//	
//	/* Configure one bit for preemption priority */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); ////抢占优先级2位，从优先级1位
//	/* Enable the TIM4 Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
//	NVIC_Init(&NVIC_InitStructure);
//	
//	/* TIM IT DISABLE */
//	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);		//清除中断标志位
//	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);		//溢出中断关闭
//	/* TIM4 DISABLE counter */
//	TIM_Cmd(TIM4,  DISABLE);						//中断计数关闭
//	return TRUE;;
//}
////************************************************************************
//// *功    能：TIM3 50us定时初始化
//// *入    口：
//// *出    口：
//// *备    注：
//// *函数性质：
////************************************************************************
//void Timer4Enable( int32u timercount ) //打开时钟
//{
//	Time4_Init((int16u)timercount);
//	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
//	TIM_SetCounter(TIM4,0x0000); 
//	TIM_Cmd(TIM4, ENABLE);
//}
//
//void Timer4Disable(void) //关闭时钟
//{
//	TIM_Cmd(TIM4, DISABLE);
//	TIM_SetCounter(TIM4,0x0000); 
//	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
//	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//}
//************************************************************************
// *功    能：TIM4 中断服务程序
// *入    口：
// *出    口：
// *备    注：与变频器通信
// *函数性质：
////************************************************************************
//void  TIM4_IRQHandler( void ) // 
//{  // static int16u count=0;
//	if ( SET == TIM_GetITStatus( TIM4, TIM_IT_Update ) )  // check interrupt source
//    {
//        TIM_ClearITPendingBit( TIM4, TIM_IT_Update ) ;
////          	 Modm_Timout();  
//
//   	 }	
//}
