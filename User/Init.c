//*****************��*��*��*Ϣ********************************************
// *�� �� ����Init.c
// *�桡  ����v1.0
// *�� �� �ˣ�Huangxinglong
// *�ա�  �ڣ�2014-06-05 12:54
// *�衡  ����
// *
// *��ǰ�汾��v1.0
// *�� �� �ˣ�Huangxinglong
// *�ա�  �ڣ�2014-06-05 12:54
// *�衡  ����
//************************************************************************
#define   _INIT_C_
#include "_Include.h"
//************************************************************************
// *��    �ܣ�IO���ó�Ĭ��ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//************************************************************************
static  void  GPIO_Reset( void )
{
    GPIO_InitTypeDef      GPIO_InitStructure ;

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_All ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
    GPIO_Init( GPIOA, &GPIO_InitStructure ) ;
    GPIO_Init( GPIOB, &GPIO_InitStructure ) ;
    GPIO_Init( GPIOC, &GPIO_InitStructure ) ;
	GPIO_Init( GPIOD, &GPIO_InitStructure ) ;
	GPIO_Init( GPIOE, &GPIO_InitStructure ) ;

    GPIO_Write( GPIOA, 0x0000 ) ;
    GPIO_Write( GPIOB, 0x0000 ) ;
    GPIO_Write( GPIOC, 0x0000 ) ;
	GPIO_Write( GPIOD, 0x0000 ) ;
	GPIO_Write( GPIOE, 0x0000 ) ;

#if HTS221
    GPIO_SetBits(GPIOA,GPIO_Pin_5);  
#endif    


#if Kionx_Acc
    GPIO_SetBits(GPIOA,GPIO_Pin_1);
#endif 


}
//************************************************************************
// *��    �ܣ�ʱ�ӳ�ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע�������ⲿ����Ϊ25MHZ
// *�������ʣ�
//************************************************************************
void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;
/*    RCC_HSEConfig(RCC_HSE_ON);    //ʹ���ⲿ����
    HSEStartUpStatus = RCC_WaitForHSEStartUp();    //�ȴ��ⲿ�����ȶ�
    //����ⲿ���������ɹ����������һ������
    if(HSEStartUpStatus==SUCCESS)
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);        //����HCLK��AHBʱ�ӣ�=SYSCLK  72m
        RCC_PCLK1Config(RCC_HCLK_Div1);        //PCLK1(APB1) = HCLK/2
        RCC_PCLK2Config(RCC_HCLK_Div1);        //PCLK2(APB2) = HCLK        
        RCC_ADCCLKConfig(RCC_PCLK2_Div2);      //����ADCʱ��Ƶ��
        //FLASHʱ�����
        //�Ƽ�ֵ��SYSCLK = 0~24MHz   Latency=0
        //        SYSCLK = 24~48MHz  Latency=1
        //        SYSCLK = 48~72MHz  Latency=2
        FLASH_SetLatency(FLASH_Latency_2);
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //����FLASHԤȡָ����
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //PLL���� SYSCLK/1 * 9 = 8*1*9 = 72MHz
        RCC_PLLCmd(ENABLE);        //����PLL
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);   //�ȴ�PLL�ȶ�
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        //ϵͳʱ��SYSCLK����PLL���
        while(RCC_GetSYSCLKSource()!=0x08);             //�л�ʱ�Ӻ�ȴ�ϵͳʱ���ȶ�
    }

    //�����Ǹ���ģ�鿪��ʱ��
    //����GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | 
                           RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD|
						   RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,
                           ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);      //����AFIO
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE ) ;     //DMAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE ) ;     //ADʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);     //USART1 ʱ��
*/
    RCC_PLLCmd(DISABLE);        //����PLL  ENABLE     
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);    //ʹ���ⲿ����
    HSEStartUpStatus = RCC_WaitForHSEStartUp();    //�ȴ��ⲿ�����ȶ�
    //����ⲿ���������ɹ����������һ������
    if(HSEStartUpStatus==SUCCESS)
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);        //����HCLK��AHBʱ�ӣ�=SYSCLK  72m
        RCC_PCLK1Config(RCC_HCLK_Div1);        //PCLK1(APB1) = HCLK/2
        RCC_PCLK2Config(RCC_HCLK_Div1);        //PCLK2(APB2) = HCLK        
        RCC_ADCCLKConfig(RCC_PCLK2_Div8);      //����ADCʱ��Ƶ��
        //FLASHʱ�����
        //�Ƽ�ֵ��SYSCLK = 0~24MHz   Latency=0
        //        SYSCLK = 24~48MHz  Latency=1
        //        SYSCLK = 48~72MHz  Latency=2
        FLASH_SetLatency(FLASH_Latency_2);
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //����FLASHԤȡָ����
       
//        RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_2); //PLL���� SYSCLK/1 * 9 = 8*1*9 = 72MHz    8M
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //PLL���� SYSCLK/1 * 9 = 8*1*9 = 72MHz    8M
        RCC_PLLCmd(ENABLE);        //����PLL  ENABLE DISABLE
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);   //�ȴ�PLL�ȶ�
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        //ϵͳʱ��SYSCLK����PLL���
        while(RCC_GetSYSCLKSource()!=0x08);            //�л�ʱ�Ӻ�ȴ�ϵͳʱ���ȶ�
    

    //�����Ǹ���ģ�鿪��ʱ��
    //����GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| 
                           RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD|
						   RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,
                           ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);      //����AFIO
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE ) ;     //DMAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE ) ;     //ADʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);     //USART1 ʱ��
    }
    else
    {
//	SystemInit();			// SYSCLK 48MHZ ,HCLK = SYSCLK  PCLK2 = HCLK=48MHZ  PCLK1 = HCLK/2=24MHZ    
    RCC_HSEConfig(RCC_HSE_OFF);
    RCC_HSICmd( ENABLE );//���ڲ�����ʱ��
    RCC_PLLCmd(DISABLE);        //����PLL  ENABLE     
    RCC_DeInit();    

    RCC_HCLKConfig(RCC_SYSCLK_Div1);        //����HCLK��AHBʱ�ӣ�=SYSCLK
    RCC_PCLK1Config(RCC_HCLK_Div1);        //PCLK1(APB1) = HCLK/2   18mhz
    RCC_PCLK2Config(RCC_HCLK_Div1);        //PCLK2(APB2) = HCLK        
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);      //����ADCʱ��Ƶ��
    //FLASHʱ�����
    //�Ƽ�ֵ��SYSCLK = 0~24MHz   Latency=0
    //        SYSCLK = 24~48MHz  Latency=1
    //        SYSCLK = 48~72MHz  Latency=2
    FLASH_SetLatency(FLASH_Latency_2);
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //����FLASHԤȡָ����
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_9); //PLL���� SYSCLK/2 * 9 = 8*9 = 72MHz
    RCC_PLLCmd(ENABLE);        //����PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);   //�ȴ�PLL�ȶ�
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        //ϵͳʱ��SYSCLK����PLL���
    while(RCC_GetSYSCLKSource()!=0x08);             //�л�ʱ�Ӻ�ȴ�ϵͳʱ���ȶ�

	RCC_APB1PeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE ) ;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE ) ;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE ) ;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE ) ;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE ) ;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE ) ;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE ) ;  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE ) ; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE ) ; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE ) ; 
	
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE ) ;
    }

}

//************************************************************************
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//************************************************************************
void NVIC_Configuration(void)
{	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ����
}


//************************************************************************
//ϵͳ������ʼ��
//************************************************************************
 void  System_ParameterInit( void )
{	


  
   DeviceFir_Number[0]=0x01;
   DeviceFir_Number[1]=0x00;

   CO_BitRate=3;
   DeviceFir_Version=0x38d; 
}
//************************************************************************
// *��    �ܣ�ϵͳ��ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//************************************************************************
static void Sys_Init(void)
{

    NVIC_Configuration() ;
    RCC_Configuration() ;
    GPIO_Reset() ;
}
//************************************************************************
// *��    �ܣ�GPIO ��ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//************************************************************************
static void Init_GPIO(void)
{
//    Switch_GPIO_Init();    //�ӽ�������������
//    Inverter_GPIO_Init();   //��Ƶ����������
//    Relay_GPIO_Init();     //����̵�����������
//	  LCD_GPIO_Init();
//    PWM_GPIO_Init();
//    Ssi_GPIO_Init();	
//    Key_GPIO_Init();           //������ʼ��    
}
//************************************************************************
// *��    �ܣ���ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//************************************************************************
void  Init_All( void )
{
	Sys_Init();  
	Init_GPIO();
    Timer2_Init(10-1,1800-1);
    Time3_Init();
    System_ParameterInit();
    CanStandbyMode_Init();
    
//    Adc_Init();
//    Modm_Init(Baud);
}

//************************************************************************
//
//            E N D
//
//************************************************************************
