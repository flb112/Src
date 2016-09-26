/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : i2c_mems.h
* Author             : MSH Application Team
* Version            : V1.1
* Date               : 04/05/2010
* Description        : Descriptor Header for i2c_mems file
* HISTORY:
* Date          | Modification                              | Author
* 04/05/2010    | Initial Revision                          | Andrea Labombarda
* 09/11/2010    | new readBuffer Signature                  | Andrea Labombarda
* 28/05/2011    | I2C SA0 CS pin support                    | Andrea Labombarda

********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
*
*******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __I2C_MEMS_H
#define __I2C_MEMS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Defines for the GPIO pins used for the I2C communication */
#define I2C_MEMS            I2C2
#define I2C_MEMS_CLK        RCC_APB1Periph_I2C1
#define I2C_MEMS_GPIO       GPIOB
#define I2C_MEMS_GPIO_CLK   RCC_APB2Periph_GPIOB
#define I2C_MEMS_SCL        GPIO_Pin_8
#define I2C_MEMS_SDA        GPIO_Pin_9


#define CSON CS_GPIO_PORT->BSRR = CS_GPIO_PIN
#define CSOFF CS_GPIO_PORT->BRR = CS_GPIO_PIN
#define CSTOGGLE CS_GPIO_PORT->ODR ^= CS_GPIO_PIN

#define SA0ON SA0_GPIO_PORT->BSRR = SA0_GPIO_PIN
#define SA0OFF SA0_GPIO_PORT->BRR = SA0_GPIO_PIN
#define SA0TOGGLE SA0_GPIO_PORT->ODR ^= SA0_GPIO_PIN

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void I2C_MEMS_Init(void);
void I2C_MEMS_DeInit(void);
uint8_t I2C_BufferRead(uint8_t* pBuffer, uint8_t deviceAddr, uint8_t readAddr, uint16_t numByteToRead);
uint8_t I2C_ByteWrite(uint8_t* pBuffer, uint8_t deviceAddress, uint8_t WriteAddr);
//uint16_t I2C_ByteWrite(uint8_t* pBuffer, uint8_t deviceAddress, uint8_t WriteAddr);
uint8_t I2C_Master_BufferRead1Byte(uint8_t deviceAddr, uint8_t readAddr);
uint16_t I2Cx_Write(uint8_t* pBuffer, uint8_t slave_address7, uint8_t subaddress, uint16_t WriteNumbr);
uint16_t I2Cx_Read(uint8_t* pBuffer, uint8_t slave_address7, uint8_t subaddress, uint16_t ReadNumbr);

#endif /* __I2C_MEMS_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

