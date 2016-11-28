//*****************文*件*信*息****************************************
// *项 目 名：CAN ST-Stm32103x8
// *文 件 名：Main.c
// *版　  本：v1.0
// *创 建 人：huangxinglong
// *日　  期：2014/5/23 10:15:56
// *描　  述：
// *				
// *当前版本：v1.0
// *修 改 人：huangxinglong
// *日　  期：2014/5/23 10:16:00
// *描　  述：CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式
//************************************************************************
#define   __MAIN_C__
#include  "_Include.h"
#include "stm32_spi.h"
#include "gyro_drv.h"


//************************************************************************
// *功    能：主函数
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//************************************************************************
 int main(void)
 {	 
   Init_All();
   // TIMInit();
//    AccStr_Init();    
    delay_init(72); 
    Flash_ReadUserPara(); 
    Can_BaudSetGet(&CO_BitRate); 
 
    spi_init();
    gyro_ang_init();
   
   

     
 //   TestMod=1;
 //   IWDG_Init(4,322);    //与分频数为64,重载值为625,溢出时间为1s
	while(1)
	{
 //       IWDG_Feed();
       
        if(T10MSFlag)
        {
           T10MSFlag=0;
           Rx_CANBUS_Proc();          //CAN接收 
           Tx_CANBUS_Proc();          //CAN發送

           if( T100MSFlag)
           { 
              T100MSFlag=0;
              Text_Send_Msg();             
          } 
        }
     
	}
}




