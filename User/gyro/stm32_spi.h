/*
 * stm32_spi.h
 *
 * Copyright (c) 2016-08-01 zxsf-lfb
 *
 * STM32 spi 配置
 *
 */
 
#ifndef  _STM32_SPI_H_
#define  _STM32_SPI_H_

/* 
 * Includes
 */
#include <stm32f10x.h>
#include "my_type.h"


/* 
 * Macros
 */
 #define	_SPI1_	1
// #define	_SPI2_	1
	#define SPI_MASTER			SPI1
  	#define SPI_MASTER_IRQn		SPI1_IRQn
// 	#define SPI_SLAVE           SPI2
//	#define SPI_SLAVE_IRQn      SPI2_IRQn 

 #if _SPI1_
  #define SPI1_CLK				RCC_APB2Periph_SPI1
  #define SPI1_GPIO             GPIOA
  #define SPI1_GPIO_CLK         RCC_APB2Periph_GPIOA 
  #define SPI1_GPIO_NSS		 	GPIO_Pin_4
  #define SPI1_PIN_SCK          GPIO_Pin_5
  #define SPI1_PIN_MISO         GPIO_Pin_6
  #define SPI1_PIN_MOSI         GPIO_Pin_7
  #define SPI_MASTER_IRQn       SPI1_IRQn
#endif
#if  _SPI2_
  #define SPI2_CLK				RCC_APB1Periph_SPI2
  #define SPI2_GPIO             GPIOB
  #define SPI2_GPIO_CLK         RCC_APB2Periph_GPIOB 
  #define SPI2_GPIO_NSS		 	GPIO_Pin_12
  #define SPI2_PIN_SCK          GPIO_Pin_13
  #define SPI2_PIN_MISO         GPIO_Pin_14
  #define SPI2_PIN_MOSI         GPIO_Pin_15  
#endif
	
/* 
 * Constants
 */

/* 
 * Struct
 */

	
/* 
 * Global Variables
 */



/* 
 * Functions
 */
void spi_init(void);

/*
 * 写入一个字节数据
 * 成功返回1，不成功返回0
 */
uint8 spi_write_byte(uint8 tx_data);

/*
 * 读出一个字节数据
 * 成功返回1，不成功返回0
 */
uint8 spi_read_byte(uint8 *rx_data);



#endif /* _STM32_SPI_H_ */

