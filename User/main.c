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
    Str_Init();
    AccStr_Init();    
    delay_init(72); 
    Flash_ReadUserPara(); 
    I2C_MEMS_Init();
#if LPS25HB 
    Lps25Hb_Init();
#endif
#if HTS221
   HTS221_Init();  
   RdCalib_Data();  
#endif    


#if Kionx_Acc
   SpiDrive_Init();
#endif 
    
   
    Temp_Calib();
    Can_BaudSetGet(&CO_BitRate);  
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
#if LPS25HB 
              PressureBuf=Hpa_Rd();   //气压值
#endif
            
#if HTS221             
             HumiBuf=Humi_Calcu();   //濕度    
             TempBuf=Temp_Calcu();   //溫度
#endif
#if Kionx_Acc
             AccTran_AngCalc();             
             
#endif             
          } 
        }
     
	}
}



/*
春来日语菊幽奇，
霞映接天蒙烟雨。
玲珑骰子安红豆，
无端抱影销魂处
玉容熙醉移人远。
敲门借问天台路，
路过小桥有断桥。

长恨相从未了，而今何事，又对西风离别。 
渚寒霞淡，玉容蒙悴，棹移人远，缥缈行舟如叶。 
想文君望久，倚竹愁生步罗袜。 
归来后，翠尊双饮，下了珠帘，玲珑闲看月。 
*/




