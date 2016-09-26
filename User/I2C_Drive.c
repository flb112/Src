/*****************************************************
* 文 件 名 : I2C_Drive.c
* 版　  本 : v1.0
* 创 建 人 : huangxinglong
* 日　  期 : 2012-05-23
*
* 当前版本 : v1.0
* 修 改 人 : 
* 日　  期 : 
*******************************************************/
#include  "_Include.h"
#include  "I2C_Drive.h"
#include  "_Type.h"
/******************************************************
  * @brief： I2C通信初始化
  * @param[in]： 
  * @retval：
  * None.
*******************************************************/
void  _I2C_Init(void)
{
  SCL_INIT_OUT();
  SDA_INIT_OUT();
  SCL_HIGH();
  SDA_HIGH();
}

/******************************************************
  * @brief： I2C器件复位
  * @param[in]： 
  * @retval：
  * None.
*******************************************************/
void  _I2C_Reset(void)
{
  
}

/******************************************************
  * @brief： I2C通信开始
  * @param[in]： 
  * @retval：
  * None.
*******************************************************/
void  _I2C_Start(void)
{
  SCL_HIGH();
  SDA_HIGH();
  DLY();
  SDA_LOW();
  DLY();
  SCL_LOW();
  DLY();
}

/******************************************************
  * @brief： I2C通信停止
  * @param[in]： 
  * @retval：
  * None.
*******************************************************/
void  _I2C_Stop(void)
{
  SDA_LOW();
  DLY();
  SCL_HIGH();
  DLY();
  SDA_HIGH();
  DLY();
}

/******************************************************
  * @brief： I2C发送应答信号
  * @param[in]： 
  * @retval：
  * None.
*******************************************************/
void  _I2C_Ack(void)
{
  SDA_LOW();
  DLY();
  SCL_HIGH();
  DLY();
  SCL_LOW();
  nop(); nop();
  SDA_HIGH();  
  DLY();
}

/******************************************************
  * @brief： I2C发送非应答信号
  * @param[in]： 
  * @retval：
  * None.
*******************************************************/
void  _I2C_nAck(void)
{
  SDA_HIGH();
  DLY();
  SCL_HIGH();
  DLY();
  SCL_LOW();
  nop(); nop();
  SDA_LOW();  
  DLY();
}

/******************************************************
  * @brief： I2C查询应答信号
  * @param[in]： 
  * @retval：SUCCESS(1),ERROR(0)
  * None.1--有应答信号，0--无应答信号
*******************************************************/
bool  _I2C_CheckAck(void)
{
  ErrorStatus I2c_AckBit;
  
  SCL_HIGH();
  nop(); nop();
  SDA_INIT_IN();
  DLY();
  I2c_AckBit=(ErrorStatus)GPIO_ReadInputPin(I2C_PORT,P_SDA); //读应答信号
  SCL_LOW();
  nop(); nop();
  SDA_INIT_OUT();  
  DLY();
  if(I2c_AckBit)
    return FALSE;
  else
    return TRUE;
}

/******************************************************
  * @brief： I2C发送数据位1
  * @param[in]： 
  * @retval：
  * None.
*******************************************************/
void  _I2C_WriteBit1(void)
{
  SDA_HIGH();
  DLY();
  SCL_HIGH();
  DLY();
  SCL_LOW();
  DLY();
}

/******************************************************
  * @brief： I2C发送数据位0
  * @param[in]： 
  * @retval：
  * None.
*******************************************************/
void  _I2C_WriteBit0(void)
{
  SDA_LOW();
  DLY();
  SCL_HIGH();
  DLY();
  SCL_LOW();
  DLY();
}

/******************************************************
  * @brief： I2C写入1byte数据
  * @param[in]：WrData,写入的数据 
  * @retval：
  * None.
*******************************************************/
void  _I2C_Wr1Byte(u8 WrData)
{
  u8  i;
  for(i=8;i>0;i--)
  {
    if(WrData&0x80)
      _I2C_WriteBit1();
    else
      _I2C_WriteBit0();
    WrData<<=1;     //取下一位
  }
}

/******************************************************
  * @brief： I2C写入nbyte数据
  * @param[in]：WrDataBuf,写入的数据缓存 
  *             WrDataNum,写入数据的字节数
  * @retval：Ture(写入成功),false(写入失败)
  * None.
*******************************************************/
bool  _I2C_WrnByte(u8 *WrDataBuf ,const u8 WrDataNum)
{
  u8  i;
  for(i=0;i<WrDataNum;i++)
  {
    _I2C_Wr1Byte(WrDataBuf[i]);
    if(!(_I2C_CheckAck()))
       return(FALSE);  
  }
//  _I2C_Stop();        //完成
  return(TRUE);  
}

/******************************************************
  * @brief： I2C读1byte数据
  * @param[in]：
  * @retval：ReadData，读出的数据
  * None.
*******************************************************/
u8  _I2C_Rd1Byte(void)
{
  u8  i,RdData=0;
 
   SDA_INIT_IN();
   DLY();
  for(i=8;i>0;i--)
  {
    SCL_HIGH();
    DLY();
    RdData<<=1;
    if(GPIO_ReadInputPin(I2C_PORT,P_SDA))
      RdData |=0x01;
    SCL_LOW();
    DLY();
  }
  SDA_INIT_OUT();
  return  (RdData);
}

/******************************************************
  * @brief： I2C读nbyte数据
  * @param[in]：RdDataNum,读出数据的字节数
  *             RdDataBuf,读出数据缓存
  * @retval：
  * None.
*******************************************************/
void _I2C_RdnByte(u8 *RdDataBuf,const u8 RdDataNum)
{
    u8 i;
    for(i=0;i<RdDataNum;i++)
    {
      RdDataBuf[i]=_I2C_Rd1Byte();
      if(i<(RdDataNum-1))
        _I2C_Ack();
      else
        _I2C_nAck();    //最后一个字节，发非应答信号
    }
 //   _I2C_Stop();
}

/******************************************************
  * @brief： 写nbyte数据到器件
  * @param[in]：DataAddr,写入的子地址
  *             W_DataBuf,写入的数据缓存
  *             W_DataNum,写入的字节数
  * @retval：True写入成功,False写入失败
  * None.
*******************************************************/
bool  _I2C_WrToEep(u8 DataAddr,u8* W_DataBuf,const u8 W_DataNum)
{
  bool  W_Status=FALSE;
  
  _I2C_Start();
  _I2C_Wr1Byte(DEVICE_ADDR);  //写器件地址
  if(_I2C_CheckAck())
  {
    _I2C_Wr1Byte(DataAddr); //有应答，写子地址
    if(_I2C_CheckAck())
    {
       //-------------------有应答，写数据
      if(_I2C_WrnByte(W_DataBuf,W_DataNum))
        W_Status=TRUE;    //写入成功
    }
  }
  _I2C_Stop();
  if(W_Status)
    return TRUE;
  else
    return FALSE;
}

/******************************************************
  * @brief： 从器件读nbyte数据
  * @param[in]：DataAddr,写入的子地址
  *             R_DataBuf,读出的数据缓存
  *             R_DataNum,需要读出的字节数
  * @retval：True读出成功,False读出失败
  * None.
*******************************************************/
bool  _I2C_RdFromEep(u8 DataAddr,u8* R_DataBuf,const u8 R_DataNum)
{
  bool  R_Status=FALSE;
  
  _I2C_Start();
  _I2C_Wr1Byte(DEVICE_ADDR);  //写器件地址
  if(_I2C_CheckAck())
  {
    _I2C_Wr1Byte(DataAddr); //有应答，写子地址
    if(_I2C_CheckAck())
    {
       _I2C_Start();      //有应答，重启
       _I2C_Wr1Byte(DEVICE_ADDR|0X01);  //写入读状态
      if(_I2C_CheckAck())
      {
        R_Status=TRUE;    //写入成功
        _I2C_RdnByte(R_DataBuf,R_DataNum);
      }
    }
  }
  _I2C_Stop();
  if(R_Status)
    return TRUE;
  else
    return FALSE;
}