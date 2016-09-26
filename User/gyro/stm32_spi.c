/*
 * stm32_spi.c
 *
 * Copyright (c) 2016-08-01 zxsf-lfb
 *
 * STM32 spi 配置
 */
 
 
/* 
 * Includes
 */
#include "stm32_spi.h"



/* 
 * Macros
 */
#define SPI_WR_RETRY_NUM 200

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


/* 
 * Local Functions
 */
static void spi_clk_init(void);
static void spi_gpio_init(void);
static void spi_nvic_config(void);


void spi_init(void)
{
	SPI_InitTypeDef SPI_InitStructure;

	spi_clk_init();
	spi_gpio_init();
	//spi_nvic_config();
	
#ifdef _SPI1_ 
	/* SPI_MASTER configuration ------------------------------------------------*/
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  	SPI_InitStructure.SPI_CRCPolynomial = 7;
  	SPI_Init(SPI1, &SPI_InitStructure);
  	/* Enable SPI_MASTER TXE interrupt */
  	//SPI_I2S_ITConfig(SPI_MASTER, SPI_I2S_IT_TXE, ENABLE);
  	/* Enable SPI_MASTER */
  	SPI_Cmd(SPI1, ENABLE);
#endif
#ifdef _SPI2_
  	/* SPI_SLAVE configuration -------------------------------------------------*/
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  	SPI_InitStructure.SPI_CRCPolynomial = 7;
  	SPI_Init(SPI2, &SPI_InitStructure);
  	/* Enable SPI_SLAVE RXNE interrupt */
  	SPI_I2S_ITConfig(SPI_SLAVE, SPI_I2S_IT_RXNE, ENABLE);
  	/* Enable SPI_SLAVE */
  	SPI_Cmd(SPI2, ENABLE);
#endif
}


/*
 * 写入一个字节数据
 * 成功返回1，不成功返回0
 */
uint8 spi_write_byte(uint8 tx_data)
{
	u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI_MASTER, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
        retry++;
        if(retry > SPI_WR_RETRY_NUM)
            return 0;
    }
    SPI_I2S_SendData(SPI_MASTER, tx_data); //通过外设SPIx发送一个数据
    return 1;
}

/*
 * 读出一个字节数据
 * 成功返回1，不成功返回0
 */
uint8 spi_read_byte(uint8 *rx_data)
{
	u8 retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI_MASTER, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
    {
        retry++;
        if(retry > SPI_WR_RETRY_NUM)
            return 0;
    }
    *rx_data = SPI_I2S_ReceiveData(SPI_MASTER); //返回通过SPIx最近接收的数据
    return 1;
}


/*
 * 硬件初始化
 */
static void spi_clk_init(void)
{
	uint32 apb_clk;
	
	apb_clk = 0;
#if _SPI1_
	apb_clk |= SPI1_CLK;
#endif
#if _SPI2_
	apb_clk |= SPI2_CLK;
#endif
	apb_clk |= RCC_APB2Periph_AFIO;
	RCC_APB2PeriphClockCmd(apb_clk, ENABLE);
	
#if _SPI1_
	RCC_APB1PeriphClockCmd(SPI1_CLK, ENABLE); 
#endif
#if _SPI2_
	RCC_APB1PeriphClockCmd(SPI2_CLK, ENABLE);
#endif

}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void spi_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

#ifdef _SPI3_
	/* Enable SPI3 Pins Software Remapping */
  	GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);
#endif
#if _SPI1_
	/* Configure SPI1 MASTER pins: SCK and MISO*/
  	/* Configure SCK and MOSI pins as Alternate Function Push Pull */
  	GPIO_InitStructure.GPIO_Pin = SPI1_PIN_SCK | SPI1_PIN_MOSI;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
#endif
#if _SPI2_  
	/* Configure SPI2 SLAVE pins: SCK and MISO*/
  	/* Configure SCK and MOSI pins as Input Floating */
  	GPIO_InitStructure.GPIO_Pin = SPI2_PIN_SCK ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);
  	/* Configure MISO pin as Alternate Function Push Pull */
  	GPIO_InitStructure.GPIO_Pin = SPI2_PIN_MISO;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);
#endif
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
static void spi_nvic_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

#ifdef _SPI1_
	/* Configure and enable SPI_MASTER interrupt -------------------------------*/
  	NVIC_InitStructure.NVIC_IRQChannel = SPI_MASTER_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
#ifdef _SPI2_
	/* Configure and enable SPI_SLAVE interrupt --------------------------------*/
  	NVIC_InitStructure.NVIC_IRQChannel = SPI_SLAVE_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_Init(&NVIC_InitStructure);
#endif
}


/**
  * @brief  This function handles SPI1 or SPI3 global interrupt request.
  * @param  None
  * @retval None
  */
#ifndef  _SPI1_
 void SPI1_IRQHandler(void)
#else
 void SPI3_IRQHandler(void)
#endif
{
  if (SPI_I2S_GetITStatus(SPI_MASTER, SPI_I2S_IT_TXE) != RESET)
  {
    /* Send SPI_MASTER data */
   // SPI_I2S_SendData(SPI_MASTER, SPI_MASTER_Buffer_Tx[TxIdx++]);

    /* Disable SPI_MASTER TXE interrupt */
//    if (TxIdx == BufferSize)
//    {
//      SPI_I2S_ITConfig(SPI_MASTER, SPI_I2S_IT_TXE, DISABLE);
//    }
  }
}

/**
  * @brief  This function handles SPI2 global interrupt request.
  * @param  None
  * @retval None
  */
void SPI2_IRQHandler(void)
{
  /* Store SPI_SLAVE received data */
  //SPI_SLAVE_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI_SLAVE);
}
