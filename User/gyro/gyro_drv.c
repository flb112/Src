/*
 * gyro.c
 *
 * Copyright (c) 2016-08-01 zxsf-lfb
 *
 * ST6轴传感器LSM6DSL配置
 */
 
 
/* 
 * Includes
 */
#include "gyro_drv.h"
#include "string.h"
#include "math.h"
#include "stm32_spi.h"
#include "delay.h"
#include "Timer.h"
#include "can_drive.h"
#include "acc_drv.h"
#include "filter.h"



/* 
 * Macros
 */
// register addresses LSM6DSL_H_
#define RW_FUNC_CFG_ACCESS             0x01
#define RW_SNR_SYNC_TIME_FRAME         0x04
#define RW_SNR_SYNC_RES_RATIO          0x05
#define RW_FIFO_CTRL1                  0x06    
#define RW_FIFO_CTRL2                  0x07 
#define RW_FIFO_CTRL3                  0x08 
#define RW_FIFO_CTRL4                  0x09 
#define RW_FIFO_CTRL5                  0x0A
#define RW_DRDY_PULSE_CFG_G            0x0B
#define RW_INT1_CTRL                   0x0D
#define RW_INT2_CTRL                   0x0E
#define R_WHO_AM_I                     0x0F
#define RW_CTRL1_XL                    0x10
#define RW_CTRL2_G                     0x11
#define RW_CTRL3_C                     0x12
#define RW_CTRL4_C                     0x13
#define RW_CTRL5_C                     0x14
#define RW_CTRL6_C                     0x15
#define RW_CTRL7_G                     0x16
#define RW_CTRL8_XL                    0x17
#define RW_CTRL9_XL                    0x18
#define RW_CTRL10_C                    0x19
#define R_OUT_TEMP_L       	           0x20 
#define R_OUTX_L_G                     0x22
#define R_OUTX_L_XL                    0x28
#define RW_TAP_CFG                     0x58
#define RW_TAP_THS_6D                  0x59
#define RW_INT_DUR2                    0x5A
#define RW_WAKE_UP_THS                 0x5B
#define RW_WAKE_UP_DUR                 0x5C
#define RW_FREE_FALL                   0x5D
#define RW_MD1_CFG                     0x5E
#define RW_MD2_CFG                     0x5F

/* 
 * Constants
 */


/* 
 * struct
 */
typedef enum gyro_odr{
	G_PWR_DOWN = 0, // 
	GODR_12_5HZ,// 12.5 Hz, etc.
	GODR_26HZ,
	GODR_52HZ,
	GODR_104HZ,
	GODR_208HZ, 
	GODR_416HZ,
	GODR_833HZ,
    GODR_1660HZ,
	GODR_3330HZ,
	GODR_6660HZ
}g_odr_t;
// Set initial input parameters
enum gyro_fsr{
  GFS_245DPS = 0,
  GFS_500DPS,
  GFS_1000DPS,
  GFS_2000DPS
};

typedef enum acc_odr{
	A_PWR_DOWN = 0, // 
	AODR_12_5HZ,// 12.5 Hz, etc.
	AODR_26HZ,
	AODR_52HZ,
	AODR_104HZ,
	AODR_208HZ, 
	AODR_416HZ,
	AODR_833HZ,
    AODR_1660HZ,
	AODR_3330HZ,
	AODR_6660HZ,
    AODR_1_6HZ
}a_odr_t;
// Set initial input parameters
enum acc_fsr{
  AFS_2G = 0,
  AFS_16G,
  AFS_8G,
  AFS_4G
};

/* 
 * Global Variables
 */


/* 
 * Local Variables
 */
//static device_addr;
//static sraw_t gyro_data; // RAW acceleration sensor data

// Sensor configuration
static uint8 acc_data[6];
static float ang_x,ang_y; // scale resolutions per LSB for the sensors
static int16 ang_x_buf[64] = {0};
static int16 ang_y_buf[64] = {0};
static maf_t ft_x,ft_y;

/* 
 * Local Functions
 */
//static void EXTIX_init(void);
static void reg_write(uint8 reg, uint8 value);
static void regs_read(uint8 start, uint8 count, uint8 *buf);
static uint8 gyro_reset();
/*
 * 写入一个字节数据
 * 成功返回1，不成功返回0
 */
static void reg_write(uint8 reg, uint8 value)
{
    GYRO_SPI_ENABLE();
    spi_reg_write(SPI2,reg,value);
    GYRO_SPI_DISABLE();
}

/*
 * 读出n个字节数据
 */
static void regs_read(uint8 reg, uint8 count, uint8* dest)
{
    GYRO_SPI_ENABLE();
    spi_regs_read(SPI2,reg,count,dest);
    GYRO_SPI_DISABLE();
}

/*
 *
 */
static uint8 gyro_reset() 
{
    uint8 tmp,cnt=0;
    //reg_write(RW_FUNC_CFG_ACCESS,0x80);  // Enable access to the embedded functions configuration registers
    reg_write(RW_CTRL3_C,0x01); //reset
    //reg_write(RW_FUNC_CFG_ACCESS,0x00);
	delay_us(100);
	while(cnt<200)  
	{ 
		regs_read(R_WHO_AM_I,1,&tmp);
        if(0x6a == tmp)
        {
            return 1;
        }
        cnt++;
	}
    return 0;
}

/*
 * init
 */
void gyro_ang_init()
{
    maf_init(&ft_x,ang_x_buf,(uint8)(sizeof(ang_x_buf)/sizeof(ang_x_buf[0])));
    maf_init(&ft_y,ang_y_buf,(uint8)(sizeof(ang_y_buf)/sizeof(ang_y_buf[0])));
    gyro_sensor_init();
}
void gyro_sensor_init()
{
    uint8 tmp;
    uint8 odr,fsr;
    //odr = GODR_3330HZ; 
	//fsr = GFS_2000DPS;
#if 0
    GPIO_InitTypeDef GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif    
    gyro_reset();
    
    //reg_write(RW_FUNC_CFG_ACCESS,0x80);  // Enable access to the embedded functions configuration registers
   
    odr = AODR_6660HZ; 
	fsr = AFS_16G;
    tmp = (odr<<4)|(fsr<<2)|0x02;   //lpf
    reg_write(RW_CTRL1_XL,tmp);
    tmp = 0x05<<5;
    reg_write(RW_CTRL8_XL,tmp); //BW=ODR/100
    
    odr = GODR_6660HZ;
	fsr = GFS_2000DPS;
    tmp = (odr<<4)|(fsr<<2);
    reg_write(RW_CTRL2_G,tmp);
    regs_read(RW_CTRL2_G,1,&tmp);
    reg_write(RW_CTRL4_C,0x02); //lpf
    regs_read(RW_CTRL6_C,1,&tmp);
    tmp |= 0x00;
    reg_write(RW_CTRL6_C,tmp);  //BW=351hz  
    
    regs_read(RW_CTRL3_C,1,&tmp);
    tmp |= 0x04;    //0x44,BDU=1
    reg_write(RW_CTRL3_C,tmp);

    
    //reg_write(RW_FUNC_CFG_ACCESS,0x00);
    
    timer_set_hook(3,gyro_read);
}

static uint8 st=0;
// Read the gyroscope data
void gyro_read()
{
	uint8 raw_data[8];  // x/y/z gyro register data stored here
    int16 ang_tmp;
	regs_read(R_OUTX_L_G, 6, &raw_data[2]);  // Read the six raw data registers into data array

    regs_read(R_OUTX_L_XL, 6, &raw_data[0]);
    /*regs_read(R_OUTX_L_XL, 6, &acc_data[0]);
    
    ang_tmp = (int16)ang_x;
    raw_data[0] = ang_tmp & 0xff;
    raw_data[1] = (ang_tmp >> 8) & 0xff;
    ang_tmp = (int16)ang_y;
    raw_data[2] = ang_tmp & 0xff;
    raw_data[3] = (ang_tmp >> 8) & 0xff;*/

    //can_send(raw_data,sizeof(raw_data));
    can_frame_send(raw_data);
#if 0
    if(1==st)
    {
        st=0;
        GPIO_ResetBits(GPIOA,GPIO_Pin_2);
    }
    else
    {
        st=1;
        GPIO_SetBits(GPIOA,GPIO_Pin_2);
    }
#endif
}

/*
 *
 */
void ang_calc()
{
    uint8 ang_ad[6];
    int16 x,y,z;
    float tmp,acc_a;
    
    memcpy(ang_ad,acc_data,6);
    x = ang_ad[1];
    x <<= 8;
    x += ang_ad[0];
    y = ang_ad[3];
    y <<= 8;
    y += ang_ad[2];
    z = ang_ad[5];
    z <<= 8;
    z += ang_ad[4];
    
    acc_a = sqrt(x*x+y*y+z*z);
    if(0 == acc_a)
    {
        x = 0;
        y = 0;
        acc_a = 1;
    }
    
    tmp = asin(x/acc_a)*180/3.14159;
    x = maf_filter(&ft_x,(int16)(tmp*100));
    ang_x = filter_1st(ang_x,x,8); //64->32

    tmp = asin(y/acc_a)*180/3.14159;
    y = maf_filter(&ft_y,(int16)(tmp*100));
    ang_y = filter_1st(ang_y,y,8); //64->32

}