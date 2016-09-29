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
#include "fxas21002_drv.h"










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
   spi_init();
   FXAS21002C_init();
   
    Temp_Calib();
    Can_BaudSetGet(&CO_BitRate);  
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
#if LPS25HB 
              PressureBuf=Hpa_Rd();   //��ѹֵ
#endif
            
#if HTS221             
             HumiBuf=Humi_Calcu();   //���    
             TempBuf=Temp_Calcu();   //�ض�
#endif
#if Kionx_Acc
             AccTran_AngCalc();             
             
#endif             
          } 
        }
     
	}
}



/*
������������棬
ϼӳ���������ꡣ
�������Ӱ��춹��
�޶˱�Ӱ���괦
������������Զ��
���Ž�����̨·��
·��С���ж��š�

�������δ�ˣ�������£��ֶ�������� 
侺�ϼ����������㲣�������Զ�����������Ҷ�� 
���ľ����ã�������������ࡣ 
�����󣬴���˫�������������������п��¡� 
*/




