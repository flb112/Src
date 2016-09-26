//*****************��*��*��*Ϣ********************************************
// *�� �� ����Sensor.c
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
#define   _SENSOR_C_
#include "_Include.h"
#include "delay.h"
#include "i2c_mems.h"



   struct WINSTR{
            	    int8u  arrpt;				//��������ָ��
            	    int8u  arrcnt;				//��������Ԫ�ظ���
                    int16u BufFilter[WINDOW];
                    int16u filterval;
                    int16u initval;
                    float  oldval;
        
	            };

static	struct 	WINSTR 	TempStr;//�¶�
static	struct 	WINSTR 	HumiStr;//ʪ��
static	struct 	WINSTR 	HpaStr;//��ѹ

//static     int16u      TempBufFilter[WINDOW] ;                  //�¶��˲�����
//static     int16u      HumiBufFilter[WINDOW] ;                  //ʪ���˲�����
//static     int16u      HpaBufFilter[WINDOW] ;                  //ѹ���˲�����






//--------------HTS221-------------------------------//
#define REG_28h     0x28 //0x16
#define REG_29h     0x29 //0X50
#define REG_2Ah     0x2A //0xCC
#define REG_2Bh     0x2B //0xFE
#define	REG_30h		0x30   //0x3D
#define	REG_31h		0x31   //0x87
#define	REG_32h		0x32   //0xBE
#define	REG_33h		0x33   //0xE0
#define	REG_34h		0x34   //0x00
#define	REG_35h		0x35   //0x04
#define	REG_36h		0x36   //0x91
#define	REG_37h		0x37   //0xFF
#define	REG_38h		0x38   //0x1D
#define	REG_39h		0x39   //0x0E
#define	REG_3Ah		0x3A   //0x03
#define	REG_3Bh		0x3B   //0x6C
#define	REG_3Ch		0x3C   //0x96
#define	REG_3Dh		0x3D   //0xFD
#define	REG_3Eh		0x3E   //0xCC
#define	REG_3Fh		0x3F   //0x0C
struct temperature_coeff
{
 signed short t0_out;
 signed short t1_out;
 signed short t_out;
 
 unsigned short t0_degc;
 unsigned short t1_degc;
} hts221_temp_cal_data; 

struct humidity_coeff
{
 signed short h0_t0_out;
 signed short h1_t0_out;
 signed short h_out;
 
 unsigned short h0_rh;
 unsigned short h1_rh; 
} hts221_humidity_cal_data; 
float temperature;
float humidity;
float m;

uint8_t WHO_AM_I2 = 0;
uint8_t HTS221_ADDRESS = 0xBF;
uint8_t i;
uint8_t buffer[8];
uint8_t HTS221_IniArray[][2] = {
	0x10,0x3F,
	0x20,0x81,
};

//-----------------------LPS25HB---------------------------------------//


 u32 pressure_temp;
 u16 temp;
 u32 pressure;
 u8 LPS25HB_ADDRESS = 0xB8;//ba

  u8 WHO_AM_I1  ;
  uint8_t i;
  uint8_t buffer[8];
  uint8_t LPS25HB_IniArray[][2] = {
    0x10, 0x0F,
    0x20, 0xA4,
    0x21, 0x40,
    0x22, 0x00,
    0x23, 0x00,
    0x24, 0x00,
    0x2E, 0xDF,
  };
//***********************************************************************/
// *��    �ܣ�LPS25HB��ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/  

#if LPS25HB  
int8u Lps25Hb_Init(void)
{
    int8u ret=0;
 //��ʼ��LPS25HB
   ret = I2C_BufferRead(&WHO_AM_I1, LPS25HB_ADDRESS, 0x0f, 1);
   if(ret != 0)
   {
	   return 0;
   }
   delay_ms(20);
   for(i=0; i<(sizeof(LPS25HB_IniArray)/2); i++)
   {
       ret = I2C_ByteWrite(&LPS25HB_IniArray[i][1], LPS25HB_ADDRESS, LPS25HB_IniArray[i][0]);
       if(ret != 0)
       {
	        return 0;
       }
   }   
     delay_ms(20);
   return 1;
}
#endif 

//***********************************************************************/
// *��    �ܣ�HTS221��ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/ 
#if HTS221  
int8u HTS221_Init(void)
{
    int8u ret=0;
 // ��ʼ��HTS221   
   ret =   I2C_BufferRead(&WHO_AM_I2, HTS221_ADDRESS, 0x0f, 1); 
   if(ret != 0)
   {
	   return 0;
   }
   delay_ms(20);
   for(i=0; i<(sizeof(HTS221_IniArray)/2); i++)
   {
       ret = I2C_ByteWrite(&HTS221_IniArray[i][1], HTS221_ADDRESS, HTS221_IniArray[i][0]);
       if(ret != 0)
       {
	      return 0;
        }
   }  
}
#endif 

//***********************************************************************/
// *��    �ܣ���ȡ��ʪ��У׼����
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/ 
 
int8u RdCalib_Data(void)
{
    int8u ret=0;

      
 //HTS221��ȡ��ʪ�Ⱥ�У׼   
    /* ��ȡ�¶�У׼ֵ */
	ret = I2C_BufferRead(buffer, HTS221_ADDRESS, REG_3Ch, 2);
	if(ret != 0)
   {
	   return 0;
   }
	hts221_temp_cal_data.t0_out = (signed short)((buffer[1]<< 8) | buffer[0]);

	ret = I2C_BufferRead(buffer, HTS221_ADDRESS, REG_3Eh, 2);
	if(ret != 0)
   {
	   return 0 ;
   }
	hts221_temp_cal_data.t1_out = (signed short)((buffer[1]<< 8) | buffer[0]);

	ret = I2C_BufferRead(buffer, HTS221_ADDRESS, REG_2Ah, 2);
	if(ret != 0)
   {
	   return 0;
   }
   	hts221_temp_cal_data.t_out = (signed short)((buffer[1]<< 8) | buffer[0]);

	ret = I2C_BufferRead(buffer, HTS221_ADDRESS, REG_32h, 1);
	if(ret != 0)
   {
	   return 0;
   }
	ret = I2C_BufferRead(buffer+1, HTS221_ADDRESS, REG_35h, 1);
	if(ret != 0)
   {
	   return 0;
   }
   	buffer[1] &= 0x03;
	hts221_temp_cal_data.t0_degc = (signed short)((buffer[1]<< 8) | buffer[0]);

	ret = I2C_BufferRead(buffer, HTS221_ADDRESS, REG_33h, 1);
	if(ret != 0)
   {
	   return 0;
   }
	ret = I2C_BufferRead(buffer+1, HTS221_ADDRESS, REG_35h, 1);
	if(ret != 0)
   {
	   return 0;
   }
   buffer[1] &= 0x0C;
   hts221_temp_cal_data.t1_degc = (signed short)((buffer[1]<< 6) | buffer[0]);
	
	/* ��ȡʪ��У׼ֵ */
   ret = I2C_BufferRead(buffer, HTS221_ADDRESS, REG_36h, 2);
   if(ret != 0)
   {
	   return 0;
   }
   hts221_humidity_cal_data.h0_t0_out = (signed short)((buffer[1]<< 8) | buffer[0]);
    
   ret = I2C_BufferRead(buffer, HTS221_ADDRESS, REG_3Ah, 2);
   if(ret != 0)
   {
	   return 0;
   }
   hts221_humidity_cal_data.h1_t0_out = (signed short)((buffer[1]<< 8) | buffer[0]);
    
   ret = I2C_BufferRead(buffer, HTS221_ADDRESS, REG_28h, 2);
   if(ret != 0)
   {
	   return 0;
   }      
   hts221_humidity_cal_data.h_out = (signed short)((buffer[1]<< 8) | buffer[0]);

   ret =  I2C_BufferRead(buffer, HTS221_ADDRESS, REG_30h, 1);
   if(ret != 0)
   {
	   return 0;
   }
   hts221_humidity_cal_data.h0_rh = buffer[0];
    
   ret = I2C_BufferRead(buffer, HTS221_ADDRESS, REG_31h, 1);
   if(ret != 0)
   {
	   return 0;
   }
   hts221_humidity_cal_data.h1_rh = buffer[0];    
   delay_ms(50);
   return 1;   

}
//***********************************************************************/
// *��    �ܣ���ȡ��ѹֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/ 
#if LPS25HB 
float Hpa_Rd(void)
 {   
  //��ȡ��ѹֵ 
    float pressure_buf=0;
    int32u temp=0;int8u ret=0;
    ret =  I2C_BufferRead(buffer, LPS25HB_ADDRESS, 0x28, 5);
    if(ret != 0)
    {
	   return 0;
    }
   
     pressure_buf = (signed int)((((buffer[2]<<8) | buffer[1])<<8) | buffer[0]);
     pressure_buf = (double)pressure_buf/4096.0f; // ��ѹת��bar(hPa)----�����������õ� 
     temp = (signed int)(buffer[4]*8+buffer[3]);  //�¶�ת��---оƬ�ڲ��¶�
     delay_ms(20); 
    
  	 HpaStr.filterval = SlideWinFilter(&(HpaStr.arrpt),&(HpaStr.arrcnt),
                                   &HpaStr.BufFilter[0],((int16u)(pressure_buf))); //�����˲�  
/***************************************һ���˲�****************************************************/
     temp=Filter_1st (HpaStr.oldval,HpaStr.filterval, 20);     
     HpaStr.oldval=HpaStr.filterval;
     return (temp);     
    
//     return (HpaStr.filterval);    
    
 } 
#endif


           

//***********************************************************************/
// *��    �ܣ��¶�У׼
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/ 
void Temp_Calib(void)
{
  TempCalib_K=(float)(TempCalib[1]-TempCalib[0])/(TempActual[1] -TempActual[0]);
  TempCalib_A=TempCalib[1]-TempCalib_K*TempActual[1];

}
//***********************************************************************/
// *��    �ܣ��¶�У׼��ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/ 
void TempCalib_Init(void)
{
   TempCalib[1]=300;//У׼�¶�ֵ
   TempCalib[0]=200;;//У׼�¶�ֵ
   TempActual[1]=360;;//��ǰ���¶�ֵ
   TempActual[0]=260;;//��ǰ���¶�ֵ
//   Temp_Calib();
}

//***********************************************************************/
// *��    �ܣ������¶�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/ 
int16u Temp_Calcu(void)
{      int16u temp,ret;
       float offset;
	       // Read samples in polling mode (no int)
	   ret =  I2C_BufferRead(buffer, HTS221_ADDRESS, REG_2Ah, 2);//���¶�ֵ
	   if(ret != 0)
       {
	       return 0;
       }       
   
       hts221_temp_cal_data.t_out = (signed short)((buffer[1]<< 8) | buffer[0]);
       delay_ms(1);

		//�����¶� 
		temperature = (float)((hts221_temp_cal_data.t1_degc - hts221_temp_cal_data.t0_degc) * hts221_temp_cal_data.t_out + (hts221_temp_cal_data.t1_out*hts221_temp_cal_data.t0_degc) - (hts221_temp_cal_data.t0_out * hts221_temp_cal_data.t1_degc)) / (hts221_temp_cal_data.t1_out - hts221_temp_cal_data.t0_out);
		temperature /= 8; //����¶�ֵ
                
                
//                TempStr.initval=(int16u)(temperature);


  	    TempStr.filterval = SlideWinFilter(&(TempStr.arrpt),&(TempStr.arrcnt),
                                   &TempStr.BufFilter[0],((int16u)(temperature*10))); //�����˲�   //���ֵ�Ŵ�10�� ����һλС��λ

/***************************************һ���˲�****************************************************/
        temp=Filter_1st(TempStr.oldval,TempStr.filterval,20);     
        TempStr.oldval=TempStr.filterval;
        if (TempOffset & 0x8000)
        {
          offset=TempOffset-65536;
        } 
        else
        {
           offset=TempOffset;        
        }
        temp=TempCalib_K*temp+TempCalib_A+offset;   //У׼��       
        return (temp);  
            
//        return (TempStr.filterval);
}










//***********************************************************************/
// *��    �ܣ�����ʪ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/ 
int16u Humi_Calcu(void)
{
       int16u ret;
	   ret =  I2C_BufferRead(buffer, HTS221_ADDRESS, REG_28h, 2);//���¶�ֵ
	   if(ret != 0)
       {
	       return 0;
       }       
        hts221_humidity_cal_data.h_out = (signed short)((buffer[1]<< 8) | buffer[0]);
        delay_ms(1);

		//����ʪ�� 
		m = (float)(hts221_humidity_cal_data.h1_rh - hts221_humidity_cal_data.h0_rh);
		m /= (float)(hts221_humidity_cal_data.h1_t0_out - hts221_humidity_cal_data.h0_t0_out);
		humidity = (float)((hts221_humidity_cal_data.h_out - hts221_humidity_cal_data.h0_t0_out) * m + hts221_humidity_cal_data.h0_rh);
		if(humidity > 200)
			humidity = 200;
		if(humidity < 0)
			humidity = 0;
		humidity /= 2; //���ʪ��ֵ
                
  	    HumiStr.filterval = SlideWinFilter(&(HumiStr.arrpt),&(HumiStr.arrcnt),&HumiStr.BufFilter[0],((int16u)(humidity*10))); //�����˲� //���ֵ�Ŵ�10�� ����һλС��λ
        return (HumiStr.filterval);                

}





void  Str_Init( void )
{
    TempStr.arrpt=0;  //��������λ��ָ��
    TempStr.arrcnt=0; //Ԫ�ظ���Ϊ0 
    TempStr.filterval=0;
    TempStr.initval=0;     
    
    HumiStr.arrpt=0;  //��������λ��ָ��
    HumiStr.arrcnt=0; //Ԫ�ظ���Ϊ0 
    HumiStr.filterval=0;
    HumiStr.initval=0;      
    
    HpaStr.arrpt=0;  //��������λ��ָ��
    HpaStr.arrcnt=0; //Ԫ�ظ���Ϊ0 
    HpaStr.filterval=0;
    HpaStr.initval=0;       
    
}




