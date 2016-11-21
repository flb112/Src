/*
 * acc_drv.c
 *
 * Copyright (c) 2016-08-01 zxsf-lfb
 *
 * AIS3624加速度配置
 */
 
 
/* 
 * Includes
 */
#include "acc_drv.h"
#include "stm32_spi.h"
#include "string.h"
#include "delay.h"

#include "can_drive.h"



/* 
 * Macros
 */
// register addresses AIS3624
#define R_WHO_AM_I              0x0F
#define RW_CTRL_REG1            0x20
#define RW_CTRL_REG2            0x21
#define RW_CTRL_REG3            0x22
#define RW_CTRL_REG4            0x23
#define RW_CTRL_REG5            0x24
#define R_HP_FILTER_RESET       0x25
#define RW_REFERENCE            0x26
#define R_STATUS_REG            0x27
#define R_OUT_X_L               0x28
#define RW_INT1_CFG             0x30
#define R_INT1_SORRCE           0x31
#define RW_INT1_THS             0x32
#define RW_INT1_DUR             0x33
#define RW_INT2_CFG             0x34
#define R_INT2_SORRCE           0x35
#define RW_INT2_THS             0x36
#define RW_INT2_DUR             0x37
     
#define PWR_NORMAL  0x01
     
/* 
 * Constants
 */


/* 
 * struct
 */
typedef enum acc_odr{
    AODR_50HZ = 0,
    AODR_100HZ,
    AODR_400HZ,
    AODR_1000HZ,
}a_odr_t;

enum acc_fsr{
  AFS_6G = 0,
  AFS_12G,
  AFS_24G = 3,
};


/* 
 * Global Variables
 */


/* 
 * Local Variables
 */
uint8 acc_data[6];
uint8 odr,fsr;

/* 
 * Local Functions
 */
static void  reg_write(uint8_t reg, uint8_t value);
static void regs_read(uint8 reg, uint8 count, uint8* dest);

/*
 * write 
 */
static void  reg_write(uint8_t reg, uint8_t value)
{
    ACC_SPI_ENABLE();
    spi_reg_write(SPI1,reg,value);
    ACC_SPI_DISABLE();
}

/*
 * read
 */
static void regs_read(uint8 reg, uint8 count, uint8* dest)
{
    ACC_SPI_ENABLE();
    spi_regs_read(SPI1,reg,count,dest);
    ACC_SPI_DISABLE();
}

/*
 * kx122 init
 */
void acc_sensor_init(void)
{
    uint8 tmp,cnt=0;
    
    odr = AODR_1000HZ;
    fsr = AFS_24G;
    
    while(cnt < 10)
    {
        regs_read(R_WHO_AM_I,1,&tmp);
        if(0x32 == tmp)
        {
            break;
        }
        cnt++;
    }
    
    regs_read(RW_CTRL_REG1,1,&tmp);
    tmp |= PWR_NORMAL<<5;
    tmp |= odr<<3;
    reg_write(RW_CTRL_REG1,tmp);    //
    
    tmp = 0x80 | (fsr<<4);
    reg_write(RW_CTRL_REG4,tmp);
    
}

/*
 * read acc data
 */
void acc_data_read(uint8 *acc, uint8 len)
{
    regs_read(R_OUT_X_L|0x40,6,acc_data);   //auto inc address
    if(len > 6)
    {
        len = 6;
    }
    memcpy(acc,acc_data,len);
}

#if 0
float Ang_Calc(float ad,int8u bit)
{
    float ang;
    if(bit)
    {
      ad=-ad;
    }
   
    ang=ad/Offset_g_Range();       //角度标定后的角度正弦值
    if(ang>=1)
        ang=9000;
    else if(ang<=-1)
        ang=-900;
    else
    {
        ang=asin(ang);
        ang=(ang/3.14)*18000;
        
    }
    if(bit)
    {
      ang=-ang;
    }    
    
    
    return ((uint16_t)ang);
}



//***********************************************************************/
// *功    能：获取角度的AD值（窗口滤波后）， ，算出角度值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void SnProc(void)
{

    unsigned char buffer[6];
    kx122_acc_t acc;
    u8 xbit;u8 ybit;u8 zbit;

/*    
    kx122_spi_read_reg(KX122_OUTX_L, buffer, 6);
 
    if (buffer[1] & 0x80)
    {   xbit=1;
        acc.x = (((int)buffer[1] << 8) + (buffer[0]) - 65536);
    }
    else
    {   xbit=0;
        acc.x = (((int)buffer[1] << 8) + buffer[0]);
    }
    if (buffer[3] & 0x80)
    {   ybit=1; 
        acc.y = (((int)buffer[3] << 8) + (buffer[2]) - 65536);
    }
    else
    {   ybit=0;
        acc.y = (((int)buffer[3] << 8) + (buffer[2]));
    }
    if (buffer[5] & 0x80)
    {   zbit=1;
        acc.z = (((int)buffer[5] << 8) + (buffer[4]) - 65536);
    }
    else
    {   zbit=0;
        acc.z = (((int)buffer[5] << 8) + (buffer[4]));
    }
*/      
    if (Ang_data_buf[1] & 0x80)
    {   xbit=1;
        acc.x = (((int)Ang_data_buf[1] << 8) + (Ang_data_buf[0]) - 65536);
    }
    else
    {   xbit=0;
        acc.x = (((int)Ang_data_buf[1] << 8) + Ang_data_buf[0]);
    }
    if (raw_data_buf[3] & 0x80)
    {   ybit=1; 
        acc.y = (((int)Ang_data_buf[3] << 8) + (Ang_data_buf[2]) - 65536);
    }
    else
    {   ybit=0;
        acc.y = (((int)Ang_data_buf[3] << 8) + (Ang_data_buf[2]));
    }
    if (Ang_data_buf[5] & 0x80)
    {   zbit=1;
        acc.z = (((int)Ang_data_buf[5] << 8) + (Ang_data_buf[4]) - 65536);
    }
    else
    {   zbit=0;
        acc.z = (((int)Ang_data_buf[5] << 8) + (Ang_data_buf[4]));
    }
    

    AccStr_x.filterval = SlideWinFilterf(&(AccStr_x.arrpt),&(AccStr_x.arrcnt),&AccStr_x.BufFilter[0],acc.x); //窗口滤波 //结果值放大10倍 保持一位小数位
    AccStr_y.filterval = SlideWinFilterf(&(AccStr_y.arrpt),&(AccStr_y.arrcnt),&AccStr_y.BufFilter[0],acc.y); //窗口滤波 //结果值放大10倍 保持一位小数位
    AccStr_z.filterval = SlideWinFilterf(&(AccStr_z.arrpt),&(AccStr_z.arrcnt),&AccStr_z.BufFilter[0],acc.z); //窗口滤波 //结果值放大10倍 保持一位小数位              

    
     AlldataBuf[0]=Ang_Calc(AccStr_x.filterval,xbit); 
     AlldataBuf[1]=Ang_Calc(AccStr_y.filterval,ybit);
     AlldataBuf[2]=Ang_Calc(AccStr_z.filterval,zbit);

}

void  AccStr_Init( void )
{
    AccStr_x.arrpt=0;  //缓存数组位置指针
    AccStr_x.arrcnt=0; //元素个数为0 
    AccStr_x.filterval=0;
    AccStr_x.initval=0;     
    
    AccStr_y.arrpt=0;  //缓存数组位置指针
    AccStr_y.arrcnt=0; //元素个数为0 
    AccStr_y.filterval=0;
    AccStr_y.initval=0;      
    
    AccStr_z.arrpt=0;  //缓存数组位置指针
    AccStr_z.arrcnt=0; //元素个数为0 
    AccStr_z.filterval=0;
    AccStr_z.initval=0;           
}  
  





void AccTran_AngCalc(void)
{
    SnProc();               
}
#endif

