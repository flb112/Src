#include "sys.h"

#include "delay.h"

#include "micro_const.h"

#include "i2c_mems.h"

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
//-------------------------------------------------------//
void main(void) 
{
    MY_NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    delay_init(72); 
    delay_ms(500);
    I2C_MEMS_Init();
    delay_ms(20);
 //初始化LPS25HB
/*   I2C_BufferRead(&WHO_AM_I1, LPS25HB_ADDRESS, 0x0f, 1);
   delay_ms(20);
    for(i=0; i<(sizeof(LPS25HB_IniArray)/2); i++) {
    I2C_ByteWrite(&LPS25HB_IniArray[i][1], LPS25HB_ADDRESS, LPS25HB_IniArray[i][0]);
    }
*/     delay_ms(20);
 // 初始化HTS221   
      I2C_BufferRead(&WHO_AM_I2, HTS221_ADDRESS, 0x0f, 1); 
     delay_ms(20);
     	for(i=0; i<(sizeof(HTS221_IniArray)/2); i++) {
	I2C_ByteWrite(&HTS221_IniArray[i][1], HTS221_ADDRESS, HTS221_IniArray[i][0]);
	}
      delay_ms(20);  
 //HTS221读取温湿度和校准   
    /* 读取温度校准值 */
	I2C_BufferRead(buffer, HTS221_ADDRESS, REG_3Ch, 2);
	hts221_temp_cal_data.t0_out = (signed short)((buffer[1]<< 8) | buffer[0]);

	I2C_BufferRead(buffer, HTS221_ADDRESS, REG_3Eh, 2);
	hts221_temp_cal_data.t1_out = (signed short)((buffer[1]<< 8) | buffer[0]);

	I2C_BufferRead(buffer, HTS221_ADDRESS, REG_2Ah, 2);
	hts221_temp_cal_data.t_out = (signed short)((buffer[1]<< 8) | buffer[0]);

	I2C_BufferRead(buffer, HTS221_ADDRESS, REG_32h, 1);
	I2C_BufferRead(buffer+1, HTS221_ADDRESS, REG_35h, 1);
	buffer[1] &= 0x03;
	hts221_temp_cal_data.t0_degc = (signed short)((buffer[1]<< 8) | buffer[0]);

	I2C_BufferRead(buffer, HTS221_ADDRESS, REG_33h, 1);
	I2C_BufferRead(buffer+1, HTS221_ADDRESS, REG_35h, 1);
	buffer[1] &= 0x0C;
	hts221_temp_cal_data.t1_degc = (signed short)((buffer[1]<< 6) | buffer[0]);
	
	/* 读取湿度校准值 */
      I2C_BufferRead(buffer, HTS221_ADDRESS, REG_36h, 2);
      hts221_humidity_cal_data.h0_t0_out = (signed short)((buffer[1]<< 8) | buffer[0]);
    
      I2C_BufferRead(buffer, HTS221_ADDRESS, REG_3Ah, 2);
      hts221_humidity_cal_data.h1_t0_out = (signed short)((buffer[1]<< 8) | buffer[0]);
    
      I2C_BufferRead(buffer, HTS221_ADDRESS, REG_28h, 2);
      hts221_humidity_cal_data.h_out = (signed short)((buffer[1]<< 8) | buffer[0]);

      I2C_BufferRead(buffer, HTS221_ADDRESS, REG_30h, 1);
      hts221_humidity_cal_data.h0_rh = buffer[0];
    
      I2C_BufferRead(buffer, HTS221_ADDRESS, REG_31h, 1);
      hts221_humidity_cal_data.h1_rh = buffer[0];    
      delay_ms(50);
 //以上数据是初始化，配置传感器，读取校准参数，GPIO配置等
	while(1) {
 
         
  //读取气压值
  /* 
    I2C_BufferRead(buffer, LPS25HB_ADDRESS, 0x28, 5);
    pressure = (signed int)((((buffer[2]<<8) | buffer[1])<<8) | buffer[0]);
    pressure_temp = (double)pressure/4096.0f; // 气压转换bar(hPa)----此数据是有用的 
    temp = (signed int)(buffer[4]*8+buffer[3]);//温度转换---芯片内部温度
     delay_ms(20);
 */          
//读取温湿度

 	
	       // Read samples in polling mode (no int)
	      I2C_BufferRead(buffer, HTS221_ADDRESS, REG_2Ah, 2);//读温度值
                hts221_temp_cal_data.t_out = (signed short)((buffer[1]<< 8) | buffer[0]);
                delay_ms(1);
                I2C_BufferRead(buffer, HTS221_ADDRESS, REG_28h, 2);//读湿度值
                hts221_humidity_cal_data.h_out = (signed short)((buffer[1]<< 8) | buffer[0]);
                delay_ms(1);
		//计算温度 
		temperature = (float)((hts221_temp_cal_data.t1_degc - hts221_temp_cal_data.t0_degc) * hts221_temp_cal_data.t_out + (hts221_temp_cal_data.t1_out*hts221_temp_cal_data.t0_degc) - (hts221_temp_cal_data.t0_out * hts221_temp_cal_data.t1_degc)) / (hts221_temp_cal_data.t1_out - hts221_temp_cal_data.t0_out);
		temperature /= 8; //获得温度值

		//计算湿度 
		m = (float)(hts221_humidity_cal_data.h1_rh - hts221_humidity_cal_data.h0_rh);
		m /= (float)(hts221_humidity_cal_data.h1_t0_out - hts221_humidity_cal_data.h0_t0_out);
		humidity = (float)((hts221_humidity_cal_data.h_out - hts221_humidity_cal_data.h0_t0_out) * m + hts221_humidity_cal_data.h0_rh);
		if(humidity > 200)
			humidity = 200;
		if(humidity < 0)
			humidity = 0;
		humidity /= 2; //获得湿度值
                 delay_ms(100);
	
    }
}
