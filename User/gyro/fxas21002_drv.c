/*
 * fxas21002.c
 *
 * Copyright (c) 2016-08-01 zxsf-lfb
 *
 * NXP����������
 */
 
 
/* 
 * Includes
 */
#include "fxas21002.h"
#include "stm32_spi.h"



/* 
 * Macros
 */
#define BUF_LEN		32
#define	DEV_ADDR	0

/* 
 * Constants
 */


/* 
 * struct
 */


/* 
 * Global Variables
 */


/* 
 * Local Variables
 */
static device_addr;
static sraw_t gyro_data; // RAW acceleration sensor data
static int8 temp; // RAW temperature data

// Sensor configuration
static uint8 fsr,odr;
float g_res, g_bias[3] = {0, 0, 0}; // scale resolutions per LSB for the sensors

/* 
 * Local Functions
 */
static void reg_write_byte(uint8 reg, uint8 value);
static uint8 reg_read_byte(uint8 reg);
static void regs_read(uint8 start, uint8 count, uint8 *buf);


void fxas21002_init(uint8 addr)
{
	device_addr = addr;
	odr = GODR_800HZ; 
	fsr = GFS_2000DPS;
}


/*
 * д��һ���ֽ�����
 * �ɹ�����1�����ɹ�����0
 */
static void reg_write(uint8 reg, uint8 value)
{
	spi_write_byte(reg);
	spi_write_byte(value);
}

/*
 * ����һ���ֽ�����
 * �ɹ�����1�����ɹ�����0
 */
static uint8 reg_read(uint8 reg)
{
	uint8 value;

	reg |= 0x80;	//read
	spi_write_byte(reg);
	spi_read_byte(&value);
	return value;
}

void regs_read(uint8 reg, uint8 count, uint8* dest)
{
	uint8 cnt = 0;

	reg |= 0x80;	//read
	spi_write_byte(reg);

	while (cnt<count) 
	{
		dest[cnt]=spi_read_byte(reg+cnt);   // Put read results in the Rx buffer
		cnt++;
	}
}

// Read the temperature data
static uint8 FXAS21002C_temp_read()
{
	uint8 temp;
	temp = reg_read(FXAS21002C_H_TEMP);
	return temp;
}

// Put the FXAS21002C into standby mode.
// It must be in standby for modifying most registers
void FXAS21002C_standby()
{
	uint8 tmp = reg_read(FXAS21002C_H_CTRL_REG1);
	reg_write(FXAS21002C_H_CTRL_REG1, tmp & ~(0x03));// Clear bits 0 and 1; standby mode
}


// Sets the FXAS21000 to active mode.
// Needs to be in this mode to output data
void FXAS21002C_ready()
{
  uint8 tmp = reg_read(FXAS21002C_H_CTRL_REG1);
  reg_write(FXAS21002C_H_CTRL_REG1, tmp & ~(0x03));  // Clear bits 0 and 1; standby mode
  reg_write(FXAS21002C_H_CTRL_REG1, tmp |   0x01);   // Set bit 0 to 1, ready mode; no data acquisition yet
}

// Put the FXAS21002C into active mode.
// Needs to be in this mode to output data.
void FXAS21002C_active()
{
	uint8 tmp = reg_read(FXAS21002C_H_CTRL_REG1);
	reg_write(FXAS21002C_H_CTRL_REG1, tmp & ~(0x03));  // Clear bits 0 and 1; standby mode
  	reg_write(FXAS21002C_H_CTRL_REG1, tmp |   0x02);   // Set bit 1 to 1, active mode; data acquisition enabled
}

void FXAS21002C_init()
{
	FXAS21002C_standby();  // Must be in standby to change registers

	// Set up the full scale range to 250, 500, 1000, or 2000 deg/s.
	reg_write(FXAS21002C_H_CTRL_REG0, fsr); 
	 // Setup the 3 data rate bits, 4:2
	if (odr < 8) 
	{
		reg_write(FXAS21002C_H_CTRL_REG1, odr << 2);    
	}  

	// Disable FIFO, route FIFO and rate threshold interrupts to INT2, enable data ready interrupt, route to INT1
  	// Active HIGH, push-pull output driver on interrupts
  	writeReg(FXAS21002C_H_CTRL_REG2, 0x0E);

  	 // Set up rate threshold detection; at max rate threshold = FSR; rate threshold = THS*FSR/128
  	//writeReg(FXAS21002C_H_RT_CFG, 0x07);         // enable rate threshold detection on all axes
  	//writeReg(FXAS21002C_H_RT_THS, 0x00 | 0x0D);  // unsigned 7-bit THS, set to one-tenth FSR; set clearing debounce counter
  	//writeReg(FXAS21002C_H_RT_COUNT, 0x04);       // set to 4 (can set up to 255)         
	// Configure interrupts 1 and 2
	writeReg(CTRL_REG3, 0x01)); // set FS_DOUBLE
	//writeReg(CTRL_REG3, readReg(CTRL_REG3) |  (0x02)); // select ACTIVE HIGH, push-pull interrupts    
	//writeReg(CTRL_REG4, readReg(CTRL_REG4) & ~(0x1D)); // clear bits 0, 3, and 4
	//writeReg(CTRL_REG4, readReg(CTRL_REG4) |  (0x1D)); // DRDY, Freefall/Motion, P/L and tap ints enabled  
	//writeReg(CTRL_REG5, 0x01);  // DRDY on INT1, P/L and taps on INT2

	FXAS21002C_active();  // Set to active to start reading
}

// Read the gyroscope data
void FXAS21002C_gyro_read()
{
	uint8 raw_data[6];  // x/y/z gyro register data stored here
	regs_read(FXAS21002C_H_OUT_X_MSB, 6, &raw_data[0]);  // Read the six raw data registers into data array
	gyro_data.x = ((int16) (raw_data[0] << 8 | raw_data[1]));
	gyro_data.y = ((int16) (raw_data[2] << 8 | raw_data[3]));
	gyro_data.z = ((int16) (raw_data[4] << 8 | raw_data[5]))
}

// Get accelerometer resolution
float FXAS21002C_gres_get(void)
{
	switch (fsr)
	{
		// Possible gyro scales (and their register bit settings) are:
  // 250 DPS (11), 500 DPS (10), 1000 DPS (01), and 2000 DPS  (00). 
    case GFS_2000DPS:
          g_res = 1600.0/8192.0;
          break;
    case GFS_1000DPS:
          g_res = 800.0/8192.0;
          break;
    case GFS_500DPS:
          g_res = 400.0/8192.0;
          break;           
    case GFS_250DPS:
          g_res = 200.0/8192.0;
	}
}

void FXAS21002C_calibrate(float * gb)
{
  int32 gyro_bias[3] = {0, 0, 0};
  uint16 tmp, fcount;
  int16 temp[3];
  uint8 raw_data[6];
  
  // Clear all interrupts by reading the data output and STATUS registers
  //readGyroData(temp);
  reg_read(FXAS21002C_H_STATUS);
  
  standby();  // Must be in standby to change registers

  reg_write(FXAS21002C_H_CTRL_REG1, 0x08);   // select 50 Hz ODR
  fcount = 50;                                     // sample for 1 second
  reg_write(FXAS21002C_H_CTRL_REG0, 0x03);   // select 200 deg/s full scale
  uint16 gyrosensitivity = 41;                   // 40.96 LSB/deg/s

  active();  // Set to active to start collecting data
   
  uint8_t rawData[6];  // x/y/z FIFO accel data stored here
  for(tmp = 0; tmp < fcount; tmp++)   // construct count sums for each axis
  {
	  regs_read(FXAS21002C_H_OUT_X_MSB, 6, &raw_data[0]);  // Read the FIFO data registers into data array
	  temp[0] = ((int16) (raw_data[0] << 8 | raw_data[1]));
	  temp[1] = ((int16) (raw_data[2] << 8 | raw_data[3]));
	  temp[2] = ((int16) (raw_data[4] << 8 | raw_data[5]));
	  
	  gyro_bias[0] += (int32_t) temp[0];
	  gyro_bias[1] += (int32_t) temp[1];
	  gyro_bias[2] += (int32_t) temp[2];
	  
	  delay(25); // wait for next data sample at 50 Hz rate
  }
 
  gyro_bias[0] /= (int32_t) fcount; // get average values
  gyro_bias[1] /= (int32_t) fcount;
  gyro_bias[2] /= (int32_t) fcount;
  
  g_bias[0] = (float)gyro_bias[0]/(float) gyrosensitivity; // get average values
  g_bias[1] = (float)gyro_bias[1]/(float) gyrosensitivity; // get average values
  g_bias[2] = (float)gyro_bias[2]/(float) gyrosensitivity; // get average values

  ready();  // Set to ready
}

void FXAS21002C::reset() 
{
	reg_write(FXAS21002C_H_CTRL_REG1, 0x20); // set reset bit to 1 to assert software reset to zero at end of boot process
	delay(100);
	while(!(reg_read(FXAS21002C_H_INT_SRC_FLAG) & 0x08))  
	{ 
		;// wait for boot end flag to be set
	}

}