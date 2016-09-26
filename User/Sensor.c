//*****************文*件*信*息********************************************
// *文 件 名：Sensor.c
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
#define   _SENSOR_C_
#include "_Include.h"
#include "delay.h"
#include "i2c_mems.h"



   struct WINSTR{
            	    int8u  arrpt;				//缓存数组指针
            	    int8u  arrcnt;				//存入数组元素个数
                    int16u BufFilter[WINDOW];
                    int16u filterval;
                    int16u initval;
                    float  oldval;
        
	            };

static	struct 	WINSTR 	TempStr;//温度
static	struct 	WINSTR 	HumiStr;//湿度
static	struct 	WINSTR 	HpaStr;//气压

//static     int16u      TempBufFilter[WINDOW] ;                  //温度滤波数组
//static     int16u      HumiBufFilter[WINDOW] ;                  //湿度滤波数组
//static     int16u      HpaBufFilter[WINDOW] ;                  //压力滤波数组






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
// *功    能：LPS25HB初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/  

#if LPS25HB  
int8u Lps25Hb_Init(void)
{
    int8u ret=0;
 //初始化LPS25HB
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
// *功    能：HTS221初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/ 
#if HTS221  
int8u HTS221_Init(void)
{
    int8u ret=0;
 // 初始化HTS221   
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
// *功    能：读取温湿度校准数据
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/ 
 
int8u RdCalib_Data(void)
{
    int8u ret=0;

      
 //HTS221读取温湿度和校准   
    /* 读取温度校准值 */
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
	
	/* 读取湿度校准值 */
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
// *功    能：读取气压值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/ 
#if LPS25HB 
float Hpa_Rd(void)
 {   
  //读取气压值 
    float pressure_buf=0;
    int32u temp=0;int8u ret=0;
    ret =  I2C_BufferRead(buffer, LPS25HB_ADDRESS, 0x28, 5);
    if(ret != 0)
    {
	   return 0;
    }
   
     pressure_buf = (signed int)((((buffer[2]<<8) | buffer[1])<<8) | buffer[0]);
     pressure_buf = (double)pressure_buf/4096.0f; // 气压转换bar(hPa)----此数据是有用的 
     temp = (signed int)(buffer[4]*8+buffer[3]);  //温度转换---芯片内部温度
     delay_ms(20); 
    
  	 HpaStr.filterval = SlideWinFilter(&(HpaStr.arrpt),&(HpaStr.arrcnt),
                                   &HpaStr.BufFilter[0],((int16u)(pressure_buf))); //窗口滤波  
/***************************************一阶滤波****************************************************/
     temp=Filter_1st (HpaStr.oldval,HpaStr.filterval, 20);     
     HpaStr.oldval=HpaStr.filterval;
     return (temp);     
    
//     return (HpaStr.filterval);    
    
 } 
#endif


           

//***********************************************************************/
// *功    能：温度校准
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/ 
void Temp_Calib(void)
{
  TempCalib_K=(float)(TempCalib[1]-TempCalib[0])/(TempActual[1] -TempActual[0]);
  TempCalib_A=TempCalib[1]-TempCalib_K*TempActual[1];

}
//***********************************************************************/
// *功    能：温度校准初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/ 
void TempCalib_Init(void)
{
   TempCalib[1]=300;//校准温度值
   TempCalib[0]=200;;//校准温度值
   TempActual[1]=360;;//当前的温度值
   TempActual[0]=260;;//当前的温度值
//   Temp_Calib();
}

//***********************************************************************/
// *功    能：计算温度
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/ 
int16u Temp_Calcu(void)
{      int16u temp,ret;
       float offset;
	       // Read samples in polling mode (no int)
	   ret =  I2C_BufferRead(buffer, HTS221_ADDRESS, REG_2Ah, 2);//读温度值
	   if(ret != 0)
       {
	       return 0;
       }       
   
       hts221_temp_cal_data.t_out = (signed short)((buffer[1]<< 8) | buffer[0]);
       delay_ms(1);

		//计算温度 
		temperature = (float)((hts221_temp_cal_data.t1_degc - hts221_temp_cal_data.t0_degc) * hts221_temp_cal_data.t_out + (hts221_temp_cal_data.t1_out*hts221_temp_cal_data.t0_degc) - (hts221_temp_cal_data.t0_out * hts221_temp_cal_data.t1_degc)) / (hts221_temp_cal_data.t1_out - hts221_temp_cal_data.t0_out);
		temperature /= 8; //获得温度值
                
                
//                TempStr.initval=(int16u)(temperature);


  	    TempStr.filterval = SlideWinFilter(&(TempStr.arrpt),&(TempStr.arrcnt),
                                   &TempStr.BufFilter[0],((int16u)(temperature*10))); //窗口滤波   //结果值放大10倍 保持一位小数位

/***************************************一阶滤波****************************************************/
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
        temp=TempCalib_K*temp+TempCalib_A+offset;   //校准后       
        return (temp);  
            
//        return (TempStr.filterval);
}










//***********************************************************************/
// *功    能：计算湿度
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/ 
int16u Humi_Calcu(void)
{
       int16u ret;
	   ret =  I2C_BufferRead(buffer, HTS221_ADDRESS, REG_28h, 2);//读温度值
	   if(ret != 0)
       {
	       return 0;
       }       
        hts221_humidity_cal_data.h_out = (signed short)((buffer[1]<< 8) | buffer[0]);
        delay_ms(1);

		//计算湿度 
		m = (float)(hts221_humidity_cal_data.h1_rh - hts221_humidity_cal_data.h0_rh);
		m /= (float)(hts221_humidity_cal_data.h1_t0_out - hts221_humidity_cal_data.h0_t0_out);
		humidity = (float)((hts221_humidity_cal_data.h_out - hts221_humidity_cal_data.h0_t0_out) * m + hts221_humidity_cal_data.h0_rh);
		if(humidity > 200)
			humidity = 200;
		if(humidity < 0)
			humidity = 0;
		humidity /= 2; //获得湿度值
                
  	    HumiStr.filterval = SlideWinFilter(&(HumiStr.arrpt),&(HumiStr.arrcnt),&HumiStr.BufFilter[0],((int16u)(humidity*10))); //窗口滤波 //结果值放大10倍 保持一位小数位
        return (HumiStr.filterval);                

}





void  Str_Init( void )
{
    TempStr.arrpt=0;  //缓存数组位置指针
    TempStr.arrcnt=0; //元素个数为0 
    TempStr.filterval=0;
    TempStr.initval=0;     
    
    HumiStr.arrpt=0;  //缓存数组位置指针
    HumiStr.arrcnt=0; //元素个数为0 
    HumiStr.filterval=0;
    HumiStr.initval=0;      
    
    HpaStr.arrpt=0;  //缓存数组位置指针
    HpaStr.arrcnt=0; //元素个数为0 
    HpaStr.filterval=0;
    HpaStr.initval=0;       
    
}




