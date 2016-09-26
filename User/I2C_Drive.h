/*****************************************************
* 文 件 名 : I2C_Drive.h
* 版　  本 : v1.0
* 创 建 人 : lfb
* 日　  期 : 2012-05-23
*
* 当前版本 : v1.0
* 修 改 人 : 
* 日　  期 : 
*******************************************************/
#ifndef _I2C_DRIVE_H_
#define _I2C_DRIVE_H_

//  #include  "stm8s.h"
//  #include  "Timer.h"

/***************i2c device address***************/
  #define DEVICE_ADDR 0xAE  //AT24C02的设备地址

/****************pin Define********************/
  #define I2C_PORT GPIOB
  #define P_SCL GPIO_PIN_1
  #define P_SDA GPIO_PIN_0

  #define DLY() DLY_NCLK(5)
/**********************SCL  Input****************************/




/*  #define SCL_INIT_IN()  do{                              \
                            
                                  GPIO_Init(I2C_PORT,P_SCL,\
                                  GPIO_MODE_IN_PU_NO_IT); \
                        }while(0)*/
                          
   #define SCL_INIT_IN()  do{                              \
                            
 	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin  = P_SCL;      //open
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//
                        }while(0)                         
                          
                          
                          
//void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
/**********************SCL  Output****************************/
  #define SCL_INIT_OUT()  do{                             \
                            GPIO_Init(I2C_PORT,P_SCL,        \
                                  GPIO_MODE_OUT_PP_HIGH_SLOW); \
                          }while(0)

/**********************SDA  Input****************************/
  #define SDA_INIT_IN()  do{                              \
                            GPIO_Init(I2C_PORT,P_SDA,       \
                                  GPIO_MODE_IN_PU_NO_IT); \
                         }while(0)

/**********************SDA  Output****************************/
  #define SDA_INIT_OUT()  do{                             \
                                  GPIO_Init(I2C_PORT,P_SDA,         \
                                  GPIO_MODE_OUT_PP_HIGH_SLOW); \
                         }while(0)

                           
                           
/**********************SCL  Output High****************************/
  #define SCL_HIGH()  do{                                  \
                         GPIO_WriteHigh(I2C_PORT,P_SCL);      \
                     }while(0)

/**********************SCL  Output Low****************************/                     
  #define SCL_LOW()  do{                                  \
                        GPIO_WriteLow(I2C_PORT,P_SCL);     \
                     }while(0)

/**********************SDA  Output High****************************/                     
  #define SDA_HIGH()  do{                                  \
                        GPIO_WriteHigh(I2C_PORT,P_SDA);       \
                      }while(0)

/**********************SDA  Output Low****************************/
  #define SDA_LOW()  do{                                  \
                        GPIO_WriteLow(I2C_PORT,P_SDA);       \
                     }while(0)

/*************************function Declara***********************/
  void  _I2C_Init(void);
  void  _I2C_Start(void);
  void  _I2C_Stop(void);
  void  _I2C_Ack(void);
  void  _I2C_nAck(void);
  bool  _I2C_CheckAck(void);
  
  void  _I2C_Wr1Byte(u8 WrData);
  bool  _I2C_WrnByte(u8 *WrDataBuf , u8 WrDataNum);
  bool  _I2C_WrToEep(u8 DataAddr,u8* W_DataBuf,u8 W_DataNum);
  
  u8   _I2C_Rd1Byte(void);
  void _I2C_RdnByte(u8 *RdDataBuf,u8 RdDataNum);
  bool _I2C_RdFromEep(u8 DataAddr,u8* R_DataBuf,u8 R_DataNum);


#endif