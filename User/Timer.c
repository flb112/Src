//*****************��*��*��*Ϣ********************************************
// *�� �� ����Timer.c
// *�桡  ����v1.0
// *�� �� �ˣ�Huangxinglong
// *�ա�  �ڣ�2014/5/23 15:43:38
// *�衡  ����
// *
// *��ǰ�汾��v1.0
// *�� �� �ˣ�Huangxinglong
// *�ա�  �ڣ�2014/5/23 15:43:43
// *�衡  ����
//************************************************************************
#define   __TIMER_C__
#include  "_Include.h"
/*******************************************************************************
* Function Name  :��ʱ
* Description    : ��ʱ��2��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void Timer2_Init(void)           //psc/72000000*arr
{	
    int16u	PrescalerValue = 0;
  
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 10-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ

	PrescalerValue = (uint16_t) (SystemCoreClock / 100000) - 1; // 1s/100000=10us 

	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;	    
    
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_ARRPreloadConfig(TIM2, ENABLE);			                //ʹ��Ԥװ�ؼĴ���    

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 	

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	TIM_SetCounter(TIM2,0x0000);
    
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx		

        
  
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
* Function Name  : TIM2_IRQHandler	1ms��ʱ
* Description    : ��ʱ��2�ж�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)  
{
	static int16u count;
	 //100us��ʱ
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
	{          
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
    	//-------------------------------------------------------------
                             
        if(count==65535)
       		count=0;
        else              
          	count++; 
//        Text_Send_Msg();        
        if(count%10==0)  
            T10MSFlag=1;		
        if(count%100==0)
            Times_Stamp();//10ms ��ʱ�� ʱ���
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
/*         if(RdReturn==0)   //�������ݽ׶β�����
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
// *��    �ܣ�Time_Proc()
// *��    �ڣ�
// *��    �ڣ�
// *��    ע����ģʽ�л�����־����
// *�������ʣ�
//***********************************************************************/
void Time_Proc(void)
{
	
    if(T10MSFlag)
    {
         
     
   	}    
}

hk_tm_proc timer3_hook;
//************************************************************************
// *��    �ܣ�TIM4 500us��ʱ��ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע�����ڱ�Ƶ����ͨ��
// *�������ʣ�
//************************************************************************
void Time3_Init( void)
{	
	int16u	PrescalerValue = 0;
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure ;
    NVIC_InitTypeDef 	NVIC_InitStructure;

	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock*9 / 1000000) - 1; // 1s/1000000=1us 
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = TIM3_TICK-1;			//��ʱ�Զ�����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ֵΪ0�ù��ܲ�ʹ��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	
	TIM_ARRPreloadConfig(TIM3, ENABLE);			//ʹ��Ԥװ�ؼĴ���
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); ////��ռ���ȼ�2λ�������ȼ�1λ
	/* Enable the TIM4 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
	
	/* TIM IT DISABLE */
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_SetCounter(TIM3,0x0000); 
	TIM_Cmd(TIM3, ENABLE);					//�жϼ����ر�
    timer3_hook = NULL;
}

//************************************************************************
// *��    �ܣ�TIM4 �жϷ������
// *��    �ڣ�
// *��    �ڣ�
// *��    ע�����Ƶ��ͨ��
// *�������ʣ�
////************************************************************************
void  TIM3_IRQHandler( void ) // 
{  // static int16u count=0;
	if ( SET == TIM_GetITStatus( TIM3, TIM_IT_Update ) )  // check interrupt source
    {
        TIM_ClearITPendingBit( TIM3, TIM_IT_Update ) ;
//          	 Modm_Timout();  
        if(timer3_hook != NULL)
        {
            (*timer3_hook)();
        }
   	 }	
}


/*
 *
 */
int8u timer_set_hook(int8u timer_num,hk_tm_proc fn)
{
    if(3 == timer_num)
    {
        timer3_hook = fn;
        return 1;
    }
    return 0;
}
