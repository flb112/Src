//*****************��*��*��*Ϣ****************************************
// *�� Ŀ ����CAN ST-Stm32103x8
// *�� �� ����Main.c
// *�桡  ����v1.0
// *�� �� �ˣ�huangxinglong
// *�ա�  �ڣ�2014/5/23 10:15:56
// *�衡  ����
// *				
// *��ǰ�汾��v1.0
// *�� �� �ˣ�huangxinglong
// *�ա�  �ڣ�2014/5/23 10:16:00
// *�衡  ����CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ
//************************************************************************
#define   __MAIN_C__
#include  "_Include.h"
#include "stm32_spi.h"
#include "gyro_drv.h"


//************************************************************************
// *��    �ܣ�������
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
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
 //   IWDG_Init(4,322);    //���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s
	while(1)
	{
 //       IWDG_Feed();
       
        if(T10MSFlag)
        {
           T10MSFlag=0;
           Rx_CANBUS_Proc();          //CAN���� 
           Tx_CANBUS_Proc();          //CAN�l��

           if( T100MSFlag)
           { 
              T100MSFlag=0;
              Text_Send_Msg();             
          } 
        }
     
	}
}




