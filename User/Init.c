//*****************文*件*信*息********************************************
// *文 件 名：Init.c
// *版　  本：v1.0
// *创 建 人：Huangxinglong
// *日　  期：2014-06-05 12:54
// *描　  述：
// *
// *当前版本：v1.0
// *修 改 人：Huangxinglong
// *日　  期：2014-06-05 12:54
// *描　  述：
//************************************************************************
#define   _INIT_C_
#include "_Include.h"
//************************************************************************
// *功    能：IO设置成默认值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
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
// *功    能：时钟初始化
// *入    口：
// *出    口：
// *备    注：接入外部晶振为25MHZ
// *函数性质：
//************************************************************************
void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;
/*    RCC_HSEConfig(RCC_HSE_ON);    //使能外部晶振
    HSEStartUpStatus = RCC_WaitForHSEStartUp();    //等待外部晶振稳定
    //如果外部晶振启动成功，则进行下一步操作
    if(HSEStartUpStatus==SUCCESS)
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);        //设置HCLK（AHB时钟）=SYSCLK  72m
        RCC_PCLK1Config(RCC_HCLK_Div1);        //PCLK1(APB1) = HCLK/2
        RCC_PCLK2Config(RCC_HCLK_Div1);        //PCLK2(APB2) = HCLK        
        RCC_ADCCLKConfig(RCC_PCLK2_Div2);      //设置ADC时钟频率
        //FLASH时序控制
        //推荐值：SYSCLK = 0~24MHz   Latency=0
        //        SYSCLK = 24~48MHz  Latency=1
        //        SYSCLK = 48~72MHz  Latency=2
        FLASH_SetLatency(FLASH_Latency_2);
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //开启FLASH预取指功能
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //PLL设置 SYSCLK/1 * 9 = 8*1*9 = 72MHz
        RCC_PLLCmd(ENABLE);        //启动PLL
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);   //等待PLL稳定
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        //系统时钟SYSCLK来自PLL输出
        while(RCC_GetSYSCLKSource()!=0x08);             //切换时钟后等待系统时钟稳定
    }

    //下面是给各模块开启时钟
    //启动GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | 
                           RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD|
						   RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,
                           ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);      //启动AFIO
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE ) ;     //DMA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE ) ;     //AD时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);     //USART1 时钟
*/
    RCC_PLLCmd(DISABLE);        //启动PLL  ENABLE     
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);    //使能外部晶振
    HSEStartUpStatus = RCC_WaitForHSEStartUp();    //等待外部晶振稳定
    //如果外部晶振启动成功，则进行下一步操作
    if(HSEStartUpStatus==SUCCESS)
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);        //设置HCLK（AHB时钟）=SYSCLK  72m
        RCC_PCLK1Config(RCC_HCLK_Div1);        //PCLK1(APB1) = HCLK/2
        RCC_PCLK2Config(RCC_HCLK_Div1);        //PCLK2(APB2) = HCLK        
        RCC_ADCCLKConfig(RCC_PCLK2_Div8);      //设置ADC时钟频率
        //FLASH时序控制
        //推荐值：SYSCLK = 0~24MHz   Latency=0
        //        SYSCLK = 24~48MHz  Latency=1
        //        SYSCLK = 48~72MHz  Latency=2
        FLASH_SetLatency(FLASH_Latency_2);
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //开启FLASH预取指功能
       
//        RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_2); //PLL设置 SYSCLK/1 * 9 = 8*1*9 = 72MHz    8M
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //PLL设置 SYSCLK/1 * 9 = 8*1*9 = 72MHz    8M
        RCC_PLLCmd(ENABLE);        //启动PLL  ENABLE DISABLE
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);   //等待PLL稳定
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        //系统时钟SYSCLK来自PLL输出
        while(RCC_GetSYSCLKSource()!=0x08);            //切换时钟后等待系统时钟稳定
    

    //下面是给各模块开启时钟
    //启动GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| 
                           RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD|
						   RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,
                           ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);      //启动AFIO
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE ) ;     //DMA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE ) ;     //AD时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);     //USART1 时钟
    }
    else
    {
//	SystemInit();			// SYSCLK 48MHZ ,HCLK = SYSCLK  PCLK2 = HCLK=48MHZ  PCLK1 = HCLK/2=24MHZ    
    RCC_HSEConfig(RCC_HSE_OFF);
    RCC_HSICmd( ENABLE );//打开内部高速时钟
    RCC_PLLCmd(DISABLE);        //启动PLL  ENABLE     
    RCC_DeInit();    

    RCC_HCLKConfig(RCC_SYSCLK_Div1);        //设置HCLK（AHB时钟）=SYSCLK
    RCC_PCLK1Config(RCC_HCLK_Div1);        //PCLK1(APB1) = HCLK/2   18mhz
    RCC_PCLK2Config(RCC_HCLK_Div1);        //PCLK2(APB2) = HCLK        
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);      //设置ADC时钟频率
    //FLASH时序控制
    //推荐值：SYSCLK = 0~24MHz   Latency=0
    //        SYSCLK = 24~48MHz  Latency=1
    //        SYSCLK = 48~72MHz  Latency=2
    FLASH_SetLatency(FLASH_Latency_2);
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //开启FLASH预取指功能
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_9); //PLL设置 SYSCLK/2 * 9 = 8*9 = 72MHz
    RCC_PLLCmd(ENABLE);        //启动PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);   //等待PLL稳定
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        //系统时钟SYSCLK来自PLL输出
    while(RCC_GetSYSCLKSource()!=0x08);             //切换时钟后等待系统时钟稳定

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
// *功    能：
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//************************************************************************
void NVIC_Configuration(void)
{	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先
}


//************************************************************************
//系统参数初始化
//************************************************************************
 void  System_ParameterInit( void )
{	


 
#if HTS221
   NODE_ID=1;
   DeviceFir_Type=1;  //设备类型
   TempCalib_Init();   //温度校准
#endif    
#if Kionx_Acc
//    KxRange=8;       //4G 
//    MappingRange=4;  //4G      
    
    NODE_ID=2;
    DeviceFir_Type=2;//设备类型
    
#endif     
   DeviceFir_Number[0]=0x01;
   DeviceFir_Number[1]=0x00;

   CO_BitRate=3;
   DeviceFir_Version=0x38d;


   TempOffset=0; 
 
 
 
}
//************************************************************************
// *功    能：系统初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//************************************************************************
static void Sys_Init(void)
{

    NVIC_Configuration() ;
    RCC_Configuration() ;
    GPIO_Reset() ;
}
//************************************************************************
// *功    能：GPIO 初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//************************************************************************
static void Init_GPIO(void)
{
//    Switch_GPIO_Init();    //接近开关引脚配置
//    Inverter_GPIO_Init();   //变频器引脚配置
//    Relay_GPIO_Init();     //输出继电器引脚配置
//	  LCD_GPIO_Init();
//    PWM_GPIO_Init();
//    Ssi_GPIO_Init();	
//    Key_GPIO_Init();           //按键初始化    
}
//************************************************************************
// *功    能：初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
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
