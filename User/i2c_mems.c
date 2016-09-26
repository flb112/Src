/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : i2c_mems.c
* Author             : MSH Application Team
* Author             : andrea labombarda
* Version            : V1.1
* Date               : 04/05/2010
* Description        : This file provides a set of functions needed to manage the
*                     communication between I2C peripheral and sensor
* HISTORY:
* Date        | Modification                              | Author
* 04/05/2010    | Initial Revision                          | Andrea Labombarda
* 09/11/2010    | New ReadBuffer function                   | Andrea Labombarda
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

/* Includes ------------------------------------------------------------------*/
#include "i2c_mems.h"

#include "stdio.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define I2C_Speed              100000
//#define I2C_Speed              100000 //the real freq is 200KHz! (????)
//#define I2C_Speed              200000 //the real freq is KHz! (????)
#define I2C_Speed             50000//400000
#define I2C_SLAVE_ADDRESS7     0xA0
#define I2C_TIMEOUT             1000


/* I2C STOP mask */
#define CR1_STOP_Set            ((uint16_t)0x0200)
#define CR1_STOP_Reset          ((uint16_t)0xFDFF)

/* I2C ACK mask */
#define CR1_ACK_Set             ((uint16_t)0x0400)
#define CR1_ACK_Reset           ((uint16_t)0xFBFF)

/* I2C POS mask */
#define CR1_POS_Set             ((uint16_t)0x0800)
#define CR1_POS_Reset           ((uint16_t)0xF7FF)


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void GPIO_Configuration(void);
void I2C_Configuration(void);

/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : I2C GPIOs Configuration
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);//使能I2C2+I2C1
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);//使能I2C2+I2C1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; //复用开路输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB,&GPIO_InitStructure);


  
  //GPIO_InitTypeDef  GPIO_InitStructure; 
  
  /* Configure I2C_EE pins: SCL and SDA */
 // GPIO_InitStructure.GPIO_Pin =  I2C_MEMS_SCL | I2C_MEMS_SDA; 
 // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
 // GPIO_Init(I2C_MEMS_GPIO, &GPIO_InitStructure);  
  
  // Enable the GPIO_CS Clock
  //RCC_APB2PeriphClockCmd(CS_GPIO_CLK, ENABLE);

  /* Configure the GPIO_CS pin */
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

//  GPIO_Init(CS_GPIO_PORT, &GPIO_InitStructure);
  
  // Enable the GPIO_SA0 Clock
 // RCC_APB2PeriphClockCmd(SA0_GPIO_CLK, ENABLE);

  /* Configure the GPIO_SA0 pin */
 // GPIO_InitStructure.GPIO_Pin = SA0_GPIO_PIN;
 // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
 // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

 // GPIO_Init(SA0_GPIO_PORT, &GPIO_InitStructure);
  
}

/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    : initializes I2C Parameter
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 
  
  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
  /* I2C Peripheral Enable */
  I2C_Cmd(I2C_MEMS, ENABLE);
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2C_MEMS, &I2C_InitStructure);
}

/*******************************************************************************
* Function Name  : I2C_MEMS_Init
* Description    : I2C Initialization
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_MEMS_Init(void)
{  
  
  /* I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(I2C_MEMS_CLK, ENABLE);   
  
  /* GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(I2C_MEMS_GPIO_CLK, ENABLE);    
  
  /* GPIO configuration */
  GPIO_Configuration();

  /* I2C configuration */
  I2C_Configuration();
  
//  SA0ON;
 // CSON;
  
}


/*******************************************************************************
* Function Name  : I2C_MEMS_DeInit
* Description    : I2C DeInitialization
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_MEMS_DeInit()
{
  GPIO_InitTypeDef  GPIO_InitStructure;  
    
  /* UnConfigure I2C_MEMS pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin = I2C_MEMS_SCL | I2C_MEMS_SDA; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(I2C_MEMS_GPIO, &GPIO_InitStructure);  
  
}

/*******************************************************************************
* Function Name  : I2C_BufferRead
* Description    : read a numByteToRead bytes from I2C Bus
* Input          : deviceAddr is the I2C address of the device
*                  readAddr is the register address you want to read from
*                  numByteToRead is the number of bytes to read
* Output         : pBuffer is the buffer that contains bytes read
* Return         : None
*******************************************************************************/


uint8_t I2C_BufferRead(uint8_t* pBuffer, uint8_t deviceAddr, uint8_t readAddr, uint16_t numByteToRead) {
    uint16_t Timeout = 0;
    __IO uint32_t temp = 0;
    
    if(numByteToRead > 1 && deviceAddr != 0xD6) {
      readAddr |= 0x80;
    }
   // /* While the bus is busy * /
  //while(I2C_GetFlagStatus(I2C_MEMS, I2C_FLAG_BUSY));
    Timeout = 2*I2C_TIMEOUT; /* Initialize timeout value */
    while(I2C_GetFlagStatus(I2C_MEMS, I2C_FLAG_BUSY)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) 
      goto timeout_err;
    } 
  
  I2C_AcknowledgeConfig(I2C_MEMS, ENABLE);
  // * Send START condition * /
  I2C_GenerateSTART(I2C_MEMS, ENABLE);

  // / * Test on EV5 and clear it * /
  //while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_MODE_SELECT));
    Timeout = I2C_TIMEOUT; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_MODE_SELECT)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0)
      goto timeout_err;
    }     

  // / * Send EEPROM address for write  * /
  I2C_Send7bitAddress(I2C_MEMS, deviceAddr, I2C_Direction_Transmitter);

  // / * Test on EV6 and clear it * /
  //  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) ;
    Timeout = I2C_TIMEOUT; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
    }

  // / * Send the EEPROM's internal address to read from: Only one byte address  * /
  I2C_SendData(I2C_MEMS, readAddr);  

  /// * Test on EV8 and clear it * /
  // while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    Timeout = I2C_TIMEOUT; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
    }
    
  /// * Send STRAT condition a second time * /  
  I2C_GenerateSTART(I2C_MEMS, ENABLE);

  /// * Test on EV5 and clear it * /
 // while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_MODE_SELECT));
    Timeout = I2C_TIMEOUT; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_MODE_SELECT)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
    } 
    // * Send EEPROM address for read * /
  I2C_Send7bitAddress(I2C_MEMS, deviceAddr, I2C_Direction_Receiver);  

  if (numByteToRead == 1)  {
    /* Wait until ADDR is set */
        Timeout = 2*I2C_TIMEOUT; /* Initialize timeout value */

    while ((I2C_MEMS->SR1&0x0002) != 0x0002){
        if (Timeout-- == 0) goto timeout_err;
    }

   /* Clear ACK bit */
    I2C_MEMS->CR1 &= CR1_ACK_Reset;
    /* Disable all active IRQs around ADDR clearing and STOP programming because the EV6_3
    software sequence must complete before the current byte end of transfer */
    __disable_irq();
    /* Clear ADDR flag */
    temp = I2C_MEMS->SR2;
    /* Program the STOP */
   I2C_GenerateSTOP(I2C_MEMS, ENABLE);
    /* Re-enable IRQs */
    __enable_irq();
    /* Wait until a data is received in DR register (RXNE = 1) EV7 */
        Timeout = 2*I2C_TIMEOUT; /* Initialize timeout value */
    
     while ((I2C_MEMS->SR1 & 0x00040) != 0x000040){
	         if (Timeout-- == 0) goto timeout_err;
     }
     
    /* Read the data */
      *pBuffer = I2C_MEMS->DR;
  
  }
  else if (numByteToRead == 2) {
                  
    /* Set POS bit */
    I2C_MEMS->CR1 |= CR1_POS_Set;
    /* Wait until ADDR is set: EV6 */
    Timeout = 2*I2C_TIMEOUT; /* Initialize timeout value */
    while ((I2C_MEMS->SR1&0x0002) != 0x0002)
    {
        if (Timeout-- == 0) goto timeout_err;
    }
    /* EV6_1: The acknowledge disable should be done just after EV6,
    that is after ADDR is cleared, so disable all active IRQs around ADDR clearing and 
    ACK clearing */
    __disable_irq();
    /* Clear ADDR by reading SR2 register  */
    temp = I2C_MEMS->SR2;
    /* Clear ACK */
    I2C_MEMS->CR1 &= CR1_ACK_Reset;
    /*Re-enable IRQs */
    __enable_irq();
    /* Wait until BTF is set */
        Timeout = 2*I2C_TIMEOUT; /* Initialize timeout value */

    while ((I2C_MEMS->SR1 & 0x00004) != 0x000004){
        if (Timeout-- == 0) goto timeout_err;
    }
    /* Disable IRQs around STOP programming and data reading */
    __disable_irq();
    /* Program the STOP */
    I2C_GenerateSTOP(I2C_MEMS, ENABLE);
    /* Read first data */
    *pBuffer = I2C_MEMS->DR;
    /* Re-enable IRQs */
    __enable_irq();
    /**/
    pBuffer++;
    /* Read second data */
    *pBuffer = I2C_MEMS->DR;
    /* Clear POS bit */
    I2C_MEMS->CR1  &= CR1_POS_Reset;
  }
  

  else { //numByteToRead > 2 
    // * Test on EV6 and clear it * /
   // while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
     Timeout = I2C_TIMEOUT; /* Initialize timeout value */
     while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
    }  
    
    // * While there is data to be read * /
    while(numByteToRead)   {
      /* Receive bytes from first byte until byte N-3 */
      if (numByteToRead != 3) {
        /* Poll on BTF to receive data because in polling mode we can not guarantee the
        EV7 software sequence is managed before the current byte transfer completes */
        Timeout = 2*I2C_TIMEOUT; /* Initialize timeout value */
        while ((I2C_MEMS->SR1 & 0x00004) != 0x000004){
            if (Timeout-- == 0) goto timeout_err;
        }
        /* Read data */
        *pBuffer = I2C_MEMS->DR;
         pBuffer++;
        /* Decrement the read bytes counter */
        numByteToRead--;
      }
      
      /* it remains to read three data: data N-2, data N-1, Data N */
      if (numByteToRead == 3) {
        /* Wait until BTF is set: Data N-2 in DR and data N -1 in shift register */
                Timeout = 2*I2C_TIMEOUT; /* Initialize timeout value */

        while ((I2C_MEMS->SR1 & 0x00004) != 0x000004){
            if (Timeout-- == 0) goto timeout_err;
        }
        /* Clear ACK */
        I2C_MEMS->CR1 &= CR1_ACK_Reset;
    
        /* Disable IRQs around data reading and STOP programming */
        __disable_irq();
        /* Read Data N-2 */
        *pBuffer = I2C_MEMS->DR;
        /* Increment */
        pBuffer++;
        /* Program the STOP */
        I2C_MEMS->CR1 |= CR1_STOP_Set;
        /* Read DataN-1 */
        *pBuffer = I2C_MEMS->DR;
        /* Re-enable IRQs */
        __enable_irq();
        /* Increment */
        pBuffer++;
        /* Wait until RXNE is set (DR contains the last data) */
        Timeout = 2*I2C_TIMEOUT; /* Initialize timeout value */
        
        while ((I2C_MEMS->SR1 & 0x00040) != 0x000040){
            if (Timeout-- == 0) goto timeout_err;
        }
        /* Read DataN */
        *pBuffer = I2C_MEMS->DR;
        /* Reset the number of bytes to be read by master */
        numByteToRead = 0;
      }
    }
  }
         Timeout = 2*I2C_TIMEOUT; /* Initialize timeout value */

  /* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
  while ((I2C_MEMS->CR1&0x200) == 0x200){
	            if (Timeout-- == 0) goto timeout_err;
  
  }

  // * Enable Acknowledgement to be ready for another reception * /
  I2C_AcknowledgeConfig(I2C_MEMS, ENABLE);
  
  return 0;
timeout_err: 
  return 1;
}


/*******************************************************************************
* Function Name  : I2C_ByteWrite
* Description    : write a Byte to I2C Bus
* Input          : deviceAddr is the I2C address of the device
*                  WriteAddr is the register address you want to write to
*                  pBuffer contains bytes to write
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t I2C_ByteWrite(uint8_t* pBuffer, uint8_t deviceAddress, uint8_t WriteAddr) {    
/*
  //Send STRAT condition 
  I2C_GenerateSTART(I2C_MEMS, ENABLE);

  // Test on EV5 and clear it 
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_MODE_SELECT));  

  // Send EEPROM address for write 
  I2C_Send7bitAddress(I2C_MEMS, deviceAddress, I2C_Direction_Transmitter);
  
  //Test on EV6 and clear it 
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  
  //Send the EEPROM's internal address to write to : only one byte Address 
  I2C_SendData(I2C_MEMS, WriteAddr); 
  
  // Test on EV8 and clear it 
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  //Send the byte to be written 
  I2C_SendData(I2C_MEMS, *pBuffer); 
   
  // Test on EV8 and clear it 
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  //Send STOP condition 
  I2C_GenerateSTOP(I2C_MEMS, ENABLE);
  */
   uint16_t Timeout = 0;
  I2C_GenerateSTART(I2C_MEMS, ENABLE);
  
  Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_MODE_SELECT)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
  }
  // Send device address for write 
  I2C_Send7bitAddress(I2C_MEMS, deviceAddress, I2C_Direction_Transmitter);
  
  //Test on EV6 and clear it 
  Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
  }
  
  //Send the reg's internal address to write to : only one byte Address 
  I2C_SendData(I2C_MEMS, WriteAddr); 
  
  // Test on EV8 and clear it 
  
    Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
  }
  

  //Send the byte to be written 
  I2C_SendData(I2C_MEMS, *pBuffer); 
   
  // Test on EV8 and clear it 
   Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
   if (Timeout-- == 0) goto timeout_err;
 }
  
  //Send STOP condition 
  I2C_GenerateSTOP(I2C_MEMS, ENABLE);
    return 0;

//err:
timeout_err:  
  return 1;
}


/*
 * Following routines have following characteristic:
 *
 *    1. Check for timeout
 *    2. Multi-byte (but don't set 0x80 in subaddr)
 */
/*******************************************************************************
* Function Name  : I2C_Write
* Description    : Write data into slave device.
* Input          : I2C1 or I2C2, slave_address7, subaddress, Write Number
* Output         : None
* Return         : number of bytes transmitted
*******************************************************************************/
uint16_t I2Cx_Write(uint8_t* pBuffer, uint8_t slave_address7, uint8_t subaddress, uint16_t WriteNumbr)
//uint16_t I2C_ByteWrite(uint8_t* pBuffer, uint8_t deviceAddress, uint8_t WriteAddr)
{
  uint16_t Tx_Idx = 0;
  uint16_t Timeout = 0;

  if (pBuffer == NULL)
    goto err;



  /* I2C Reset Flags */
  I2C_MEMS->SR1=0;
  I2C_MEMS->SR2=0;

  /* Acknowledge Enable */
  I2C_AcknowledgeConfig(I2C_MEMS, ENABLE);

  /* Send I2Cx START */
  I2C_GenerateSTART(I2C_MEMS, ENABLE);

  /* Test on I2C1 EV5 and clear it */
  Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_MODE_SELECT)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
  }

  /* Send Slave Address + write */
//  I2C_Send7bitAddress(I2C_MEMS, deviceAddress, I2C_Direction_Transmitter);

  /* Test on I2C1 EV6 and clear it */
  Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
  }

  /* Send SubAddress */
 // I2C_SendData(I2C_MEMS, subaddress);
 // I2C_SendData(I2C_MEMS, WriteAddr); 
  /* Test on I2C1 EV8 and clear it */
  Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
  }

  /* Write data */
 
    /* Send the byte to be written */
    I2C_SendData(I2C_MEMS, *pBuffer); 
    
  Timeout = I2C_TIMEOUT; /* Initialize timeout value */
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
    if (Timeout-- == 0) goto timeout_err;
    }

   
      I2C_GenerateSTOP(I2C_MEMS, ENABLE);

  /* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
  Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while ((I2C_MEMS->CR1 & 0x200) == 0x200){
    if (Timeout-- == 0) goto timeout_err;
  }
err:
timeout_err:  
  return Tx_Idx;
}

/*******************************************************************************
* Function Name  : I2C_Read
* Description    : Write data into slave device.
* Input          : I2C1 or I2C2, slave_address7, subaddress, Write Number
* Output         : None
* Return         : number of bytes transmitted
*******************************************************************************/
uint16_t I2Cx_Read(uint8_t* pBuffer, uint8_t slave_address7, uint8_t subaddress, uint16_t ReadNumbr)
{
  uint16_t Rx_Idx = 0;
  uint16_t Timeout = 0;

  if (pBuffer == NULL)
    goto err;

  if (ReadNumbr < 1) ReadNumbr = 1;

  /* I2C Reset Flags */
  I2C_MEMS->SR1=0;
  I2C_MEMS->SR2=0;

  /* Acknowledge Enable */
  I2C_AcknowledgeConfig(I2C_MEMS, ENABLE);

  /* Send I2Cx START */
  I2C_GenerateSTART(I2C_MEMS, ENABLE);

  /* Test on I2C1 EV5 and clear it */
  Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_MODE_SELECT)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
  }

  /* Send Slave Address + write */
  I2C_Send7bitAddress(I2C_MEMS, slave_address7, I2C_Direction_Transmitter);

  /* Test on I2C1 EV6 and clear it */
  Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
  }

  /* Send SubAddress */
  I2C_SendData(I2C_MEMS, subaddress);

  /* Test on I2C1 EV8 and clear it */
  Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
  }

  /* Send I2Cx RESTART */
  I2C_GenerateSTART(I2C_MEMS, ENABLE);

  /* Test on I2C1 EV5 and clear it */
  Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_MODE_SELECT)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
  }

  /* Send Slave Address + Read */
  I2C_Send7bitAddress(I2C_MEMS, (slave_address7 + 0x01), I2C_Direction_Receiver);

  /* Test on I2C1 EV6 and clear it */
  Timeout = I2C_TIMEOUT; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
    /* If the timeout delay is exeeded, exit with error code */
    if (Timeout-- == 0) goto timeout_err;
  }

  /* Read data */
  while (Rx_Idx < ReadNumbr) {
    if ((Rx_Idx + 1) == ReadNumbr)
      I2C_AcknowledgeConfig(I2C_MEMS, DISABLE); // FOR NOT ACK Generation

    /* Wait for EV7 */
    while(!I2C_CheckEvent(I2C_MEMS, I2C_EVENT_MASTER_BYTE_RECEIVED));
    pBuffer[Rx_Idx] = I2C_ReceiveData(I2C_MEMS);
    Rx_Idx++;
  }

  I2C_GenerateSTOP(I2C_MEMS, ENABLE);

  /* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
  while ((I2C_MEMS->CR1 & 0x200) == 0x200);

err:
timeout_err:  
  return Rx_Idx;
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

