/*****************************************************
* �� �� �� : I2C_Drive.c
* �桡  �� : v1.0
* �� �� �� : huangxinglong
* �ա�  �� : 2012-05-23
*
* ��ǰ�汾 : v1.0
* �� �� �� : 
* �ա�  �� : 
*******************************************************/
#include  "_Include.h"
#include  "I2C_Drive.h"
#include  "_Type.h"
/******************************************************
  * @brief�� I2Cͨ�ų�ʼ��
  * @param[in]�� 
  * @retval��
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
  * @brief�� I2C������λ
  * @param[in]�� 
  * @retval��
  * None.
*******************************************************/
void  _I2C_Reset(void)
{
  
}

/******************************************************
  * @brief�� I2Cͨ�ſ�ʼ
  * @param[in]�� 
  * @retval��
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
  * @brief�� I2Cͨ��ֹͣ
  * @param[in]�� 
  * @retval��
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
  * @brief�� I2C����Ӧ���ź�
  * @param[in]�� 
  * @retval��
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
  * @brief�� I2C���ͷ�Ӧ���ź�
  * @param[in]�� 
  * @retval��
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
  * @brief�� I2C��ѯӦ���ź�
  * @param[in]�� 
  * @retval��SUCCESS(1),ERROR(0)
  * None.1--��Ӧ���źţ�0--��Ӧ���ź�
*******************************************************/
bool  _I2C_CheckAck(void)
{
  ErrorStatus I2c_AckBit;
  
  SCL_HIGH();
  nop(); nop();
  SDA_INIT_IN();
  DLY();
  I2c_AckBit=(ErrorStatus)GPIO_ReadInputPin(I2C_PORT,P_SDA); //��Ӧ���ź�
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
  * @brief�� I2C��������λ1
  * @param[in]�� 
  * @retval��
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
  * @brief�� I2C��������λ0
  * @param[in]�� 
  * @retval��
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
  * @brief�� I2Cд��1byte����
  * @param[in]��WrData,д������� 
  * @retval��
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
    WrData<<=1;     //ȡ��һλ
  }
}

/******************************************************
  * @brief�� I2Cд��nbyte����
  * @param[in]��WrDataBuf,д������ݻ��� 
  *             WrDataNum,д�����ݵ��ֽ���
  * @retval��Ture(д��ɹ�),false(д��ʧ��)
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
//  _I2C_Stop();        //���
  return(TRUE);  
}

/******************************************************
  * @brief�� I2C��1byte����
  * @param[in]��
  * @retval��ReadData������������
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
  * @brief�� I2C��nbyte����
  * @param[in]��RdDataNum,�������ݵ��ֽ���
  *             RdDataBuf,�������ݻ���
  * @retval��
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
        _I2C_nAck();    //���һ���ֽڣ�����Ӧ���ź�
    }
 //   _I2C_Stop();
}

/******************************************************
  * @brief�� дnbyte���ݵ�����
  * @param[in]��DataAddr,д����ӵ�ַ
  *             W_DataBuf,д������ݻ���
  *             W_DataNum,д����ֽ���
  * @retval��Trueд��ɹ�,Falseд��ʧ��
  * None.
*******************************************************/
bool  _I2C_WrToEep(u8 DataAddr,u8* W_DataBuf,const u8 W_DataNum)
{
  bool  W_Status=FALSE;
  
  _I2C_Start();
  _I2C_Wr1Byte(DEVICE_ADDR);  //д������ַ
  if(_I2C_CheckAck())
  {
    _I2C_Wr1Byte(DataAddr); //��Ӧ��д�ӵ�ַ
    if(_I2C_CheckAck())
    {
       //-------------------��Ӧ��д����
      if(_I2C_WrnByte(W_DataBuf,W_DataNum))
        W_Status=TRUE;    //д��ɹ�
    }
  }
  _I2C_Stop();
  if(W_Status)
    return TRUE;
  else
    return FALSE;
}

/******************************************************
  * @brief�� ��������nbyte����
  * @param[in]��DataAddr,д����ӵ�ַ
  *             R_DataBuf,���������ݻ���
  *             R_DataNum,��Ҫ�������ֽ���
  * @retval��True�����ɹ�,False����ʧ��
  * None.
*******************************************************/
bool  _I2C_RdFromEep(u8 DataAddr,u8* R_DataBuf,const u8 R_DataNum)
{
  bool  R_Status=FALSE;
  
  _I2C_Start();
  _I2C_Wr1Byte(DEVICE_ADDR);  //д������ַ
  if(_I2C_CheckAck())
  {
    _I2C_Wr1Byte(DataAddr); //��Ӧ��д�ӵ�ַ
    if(_I2C_CheckAck())
    {
       _I2C_Start();      //��Ӧ������
       _I2C_Wr1Byte(DEVICE_ADDR|0X01);  //д���״̬
      if(_I2C_CheckAck())
      {
        R_Status=TRUE;    //д��ɹ�
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