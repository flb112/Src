//*****************文*件*信*息****************************************
// *项 目 名：
// *文 件 名：SPI.c
// *版　  本：v1.0
// *创 建 人：huangxinglong
// *日　  期：2014/5/23 10:15:56
// *描　  述：
// *				
// *当前版本：v1.0
// *修 改 人：huangxinglong
// *日　  期：2014/5/23 10:16:00
// *描　  述：
//************************************************************************

#include  "_Include.h"

//***********************************************************************/
// *功    能：SPI初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/

void SPI2_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                  //初始化GPIOB

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);//PB13/14/15上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		    //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		        //串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
	SPI2_ReadWriteByte(0xff);//启动传输		 
 

}   

//***********************************************************************/
// *功    能：SPI速度设置函数
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
//***********************************************************************/
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI2,ENABLE); 

} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
//***********************************************************************/
// *功    能：SPI初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据					    
}


//***********************************************************************/
// *功    能： 从机片选
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Set_SlaveCs(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}

void Reset_SlaveCs(void)
{
  
   GPIO_SetBits(GPIOA,GPIO_Pin_8);
}



//***********************************************************************/
// *功    能： 读x轴的值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Read_X_Axis(void)
{		
	Set_SlaveCs();    
    X_Axis_Buf=SPI2_ReadWriteByte(0x11);    
	Reset_SlaveCs();			    
}

//***********************************************************************/
// *功    能： 读y轴的值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Read_y_Axis(void)
{		
	Set_SlaveCs();    
    Y_Axis_Buf=SPI2_ReadWriteByte(0x11);    
	Reset_SlaveCs();			    
}

//***********************************************************************/
// *功    能： 读y轴的值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Read_z_Axis(void)
{		
	Set_SlaveCs();    
    Z_Axis_Buf=SPI2_ReadWriteByte(0x11);    
	Reset_SlaveCs();			    
}


















