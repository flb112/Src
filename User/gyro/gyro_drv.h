/*
 * gyro.h
 *
 * Copyright (c) 2016-08-01 zxsf-lfb
 *
 * ST6Öá´«¸ÐÆ÷LSM6DSLÅäÖÃ
 *
 */
 
#ifndef  _GYRO_DRV_H_
#define  _GYRO_DRV_H_

/* 
 * Includes
 */
#include <stm32f10x.h>
#include "my_type.h"


/* 
 * Macros
 */
#define GYRO_SPI_ENABLE()       do{GPIO_ResetBits(GPIOB,GPIO_Pin_12);}while(0)
#define GYRO_SPI_DISABLE()      GPIO_SetBits(GPIOB,GPIO_Pin_12)
	

/* 
 * Constants
 */

/* 
 * Struct
 */
typedef struct{
	    int16_t	x;
	    int16_t	y;
	    int16_t	z;
}sraw_t;
	
	
/* 
 * Global Variables
 */



/* 
 * Functions
 */
void gyro_ang_init();
void gyro_sensor_init();

// Read the gyroscope data
void gyro_read();

void ang_calc();



#endif /* _GYRO_DRV_H_ */

