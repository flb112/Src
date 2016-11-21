/*
 * acc_drv.h
 *
 * Copyright (c) 2016-08-01 zxsf-lfb
 *
 * AIS3624º”ÀŸ∂»≈‰÷√
 *
 */
 
#ifndef  _ACC_DRV_H_
#define  _ACC_DRV_H_

/* 
 * Includes
 */
#include <stm32f10x.h>
#include "my_type.h"


/* 
 * Macros
 */
#define ACC_SPI_ENABLE()       do{GPIO_ResetBits(GPIOA,GPIO_Pin_4);}while(0)
#define ACC_SPI_DISABLE()      GPIO_SetBits(GPIOA,GPIO_Pin_4)
	
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
}ang_t;
	
	
/* 
 * Global Variables
 */



/* 
 * Functions
 */
void acc_sensor_init(void);
void acc_data_read(uint8 *acc, uint8 len);


#endif /* _ACC_DRV_H_ */

