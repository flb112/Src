//*****************��*��*��*Ϣ********************************************
// *�� �� ����SPI_drive.c
// *�桡  ����v0.0
// *�� �� �ˣ�������
// *�ա�  �ڣ�2013-12-24 16:04
// *�衡  ����
// *
// *��ǰ�汾��v0.0
// *�� �� �ˣ�������
// *�ա�  �ڣ�2013-12-24 16:04
// *�衡  ����
//************************************************************************

#define __SPIDRIVE_C__
#include "_Include.h"

#include "stm32f10x_spi.h"
#include "stm32f10x.h"

#include "sys.h"
#include "usart.h"
#include "delay.h"


   struct WINSTR{
            	    int8u  arrpt;				//��������ָ��
            	    int8u  arrcnt;				//��������Ԫ�ظ���
                    int16s BufFilter[WINDOW];
                    int16s filterval;
                    int16s initval;
                    float  oldval;
        
	            };

static	struct 	WINSTR 	AccStr_x;//
static	struct 	WINSTR 	AccStr_y;//
static	struct 	WINSTR 	AccStr_z;//



#define KX122_SPI_CS   PBout(12)

#define NOP_WAIT do { \
    nop(); \
    nop(); \
    nop(); \
    nop(); \
    nop(); \
}while(0)

typedef enum  {KX_DRV_SUCCESS, KX_DRV_ERR} kx_drv_status_e;
typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
} kx122_acc_t;






#define SPI_WRRD_RETRY_NUM 200

//#define SAMPLE_BUF_FULL_INT
//#define SYNC_HW_INT_RD_ODR
//#define ASYNC_RD_ODR
//#define WUF_BUF_FIFO_MODE
#define WUF_WMI_FIFO_MODE

#define bufmax 1200//1800//600//




//ATH(counts) = Wake Up Threshold(g) x 16 (counts / g)
//WUFC(counts) = Wake Up Delay Time(sec) x OWUF(Hz)


//kx122 register definition
#define KX122_OUTX_L                            0x06
#define KX122_INT_REL                           0x17
#define KX122_CTRL_REG1                         0x18
#define KX122_CTRL_REG2                         0x19
#define KX122_CTRL_REG3                         0x1A
#define KX122_ODCNTL_REG                        0x1B
#define KX122_INT_CTRL_REG1                     0x1C
#define KX122_INT_CTRL_REG2                     0x1D
#define KX122_INT_CTRL_REG3                     0x1E
#define KX122_INT_CTRL_REG4                     0x1F
#define KX122_INT_CTRL_REG5                     0x20
#define KX122_INT_CTRL_REG6                     0x21
#define KX122_WAKEUP_TIMER_REG                  0x23
#define KX122_WAKEUP_THRESHOLD_REG              0x30
#define KX122_SELFTEST_REG                      0x60
#define KX122_INT_REL                           0x17
#define KX122_INT_SOURCE                        0x13
#define KX122_CHIP_ID_REG                       0x0F
#define KX122_COTR_REG                          0x0C
#define KX122_BUF_CNTL1                         0x3A
#define KX122_BUF_CNTL2                         0x3B
#define KX122_BUF_STATUS_1                      0x3C
#define KX122_BUF_STATUS_2                      0x3D
#define KX122_BUF_CLEAR                         0x3E
#define KX122_BUF_READ                          0x3F

//CNTL1
#define PC1       (1<<7)
#define RES      (1<<6)
#define GSE_MASK (3<<3)
#define GSE_2G   (0<<3)
#define GSE_4G   (1<<3)
#define GSE_8G   (2<<3)
#define WUFE     (1<<1)

//CNTL3
#define OWUF_MASK  (7<<0)
#define OWUF_0_781HZ    0
#define OWUF_1_563HZ    1
#define OWUF_3_125HZ    2
#define OWUF_6_25HZ    3
#define OWUF_12_5HZ    4
#define OWUF_25HZ    5
#define OWUF_50HZ    6
#define OWUF_100HZ    7


//INC1
#define PWSEL1_MASK  (3<<6)
#define PWSEL1_OSA_US50     (0<<6)
#define PWSEL1_OSA_1        (1<<6)
#define PWSEL1_OSA_2        (3<<6)
#define PWSEL1_OSA_4        (1<<6)

#define IEN1         (1<<5)
#define IEA1         (1<<4)
#define IEL1         (1<<3)

//INC2
#define AOI            (1<<6)

//INC4
#define BFI1         (1<<6)
#define WMI1         (1<<5)
#define DRDYI1       (1<<4)
#define WUFI1        (1<<1)

//INC5
#define PWSEL2_MASK  (3<<6)
#define PWSEL2_OSA_US50     (0<<6)
#define PWSEL2_OSA_1        (1<<6)
#define PWSEL2_OSA_2        (3<<6)
#define PWSEL2_OSA_4        (1<<6)

#define IEN2        (1<<5)
#define IEA2         (1<<4)
#define IEL2         (1<<3)



//INC6
#define BFI2         (1<<6)
#define WMI2         (1<<5)
#define DRDYI2       (1<<4)
#define WUFI2        (1<<1)

//ODCNTL
#define LPRO            (1 << 6)
#define OSA3_MASK   0x0F
#define OSA_12_5HZ  0
#define OSA_25HZ    1
#define OSA_50HZ    2
#define OSA_100HZ   3
#define OSA_200HZ    4
#define OSA_400HZ    5
#define OSA_800HZ    6
#define OSA_1600HZ   7
#define OSA_0_781HZ   8
#define OSA_1_563HZ   9
#define OSA_3_125HZ   10
#define OSA_6_25HZ    11
#define OSA_3200HZ    12
#define OSA_6400HZ    13
#define OSA_12800HZ   14
#define OSA_25600HZ   15


//BUF_CNTL2 
#define BUFE        (1<<7)
#define BRES        (1<<6)
#define BFIE        (1<<5)

typedef struct {
	uint8_t  res_bytes;//16bit,2 or 8bit,1
	uint8_t  wmi_buf_size;//maximux 339 @ BUF_RES=1, maximu 681 @ BUF_RES=0
	uint8_t  motion_timing;//200ms
	uint8_t  odr_osa; //0~15
}acc_kx122_state_t;



//volatile uint16_t can_data_buf_head =

static __IO u16 raw_data_buf_head = 0;
static __IO u16 raw_data_buf_tail = 0;
static __IO u16 raw_data_buf_fill = 0;
//static __IO uint16_t recv_cnt = 0;
uint16_t can_buf_head = 0;
uint16_t buf_flag_head = 0;



uint16_t recv_size;// = state->wmi_buf_size * state->res_bytes * 3;


uint8_t  moves_flag=0;

acc_kx122_state_t kx122_state;

kx_drv_status_e kx122_spi_read_reg(uint8_t reg, uint8_t *pdata, uint16_t num);
kx_drv_status_e kx122_spi_write_reg(uint8_t reg, uint8_t data);


void kx122_config(acc_kx122_state_t *state)
{
	state->res_bytes = 2;
	state->wmi_buf_size = 8;//0x80;
	state->odr_osa = OSA_12800HZ;
	state->motion_timing = 200;
	recv_size = state->wmi_buf_size * state->res_bytes * 3;
    raw_data_size = recv_size*7;
	flag_motion_det = 0;

}

void kx122_data_recv(void)
{
#if 0
	uint8_t b0, b1;
	uint16_t data_num;
	kx122_spi_read_reg(KX122_BUF_STATUS_1, &b0, 1);
	kx122_spi_read_reg(KX122_BUF_STATUS_2, &b1, 1);
	data_num = (((int)(b1 & 0x07) << 8) + b0);
//	xprintf("buffer number is %d\n", data_num);
#endif




	if (raw_data_buf_head >= (raw_data_size))//-recv_size
	{
//		xprintf("finish motion detection\n");
		flag_motion_det = 0;
		raw_data_buf_head = 0;
        RD_MOD=1;

	}
    else
    {
        kx122_spi_read_reg(KX122_BUF_READ, &raw_data_buf[raw_data_buf_head], recv_size);
        can_buf_head = raw_data_buf_head;

        raw_data_buf_head += recv_size;
        raw_data_buf_fill = raw_data_buf_head;    
        CAN_data_buf_head=raw_data_buf_head;
        
        
        
    }
	kx122_spi_write_reg(0x3e, 0x00);
}


void EXTIX_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������,GPIO_Mode_IPD,Ĭ������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //ʹ�ܸ��ù���ʱ��

//    GPIOA.7 �ж����Լ��жϳ�ʼ������
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);
    EXTI_InitStructure.EXTI_Line = EXTI_Line8;  //INT1
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);     //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;          //ʹ��PA7���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;///0x02;    //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;                   //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);       //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���



	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;  //INT2
	EXTI_Init(&EXTI_InitStructure);     //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;          //ʹ��PA15���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;    //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;                   //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);       //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

}


int8u Rd_SegKX122(void)
{ 
   int8u buffer[6*8]; u16 j=0;static u16 z=0;
   static int8u bufsta_ptr;
   static u16 i=0;
   static u16 index=0;
   static int8u data_buf[bufmax]; 
    
     if((RdStart==1)&&(bufsta_ptr==0))  //�д����������ɼ���ʼ
     {     
              bufsta_ptr=1;
/***********************************����¼��ǰ3000������*****************************************/
              if((index-bufmax)==0)  
              {
                    for(i=index-bufmax;i<bufmax;i++)
                    {
                        raw_data_buf[i]=data_buf[i];   
                    }
               }
               else
               { 
                     for(i=index;i<bufmax;i++)
                     {
                         raw_data_buf[j++]=data_buf[i];   
                     }
                     for(i=0;i<index;i++)
                     {
                        raw_data_buf[j++]=data_buf[i];   
                     }                                            
              }
              index=0;
/***********************************����¼��ǰ3000������*****************************************/              
              i=bufmax;//��3000�����ݿ�ʼ��¼�����������       
     }
    if(!RdReturn)  //�������ݽ׶β�������
    {  
         kx122_spi_read_reg(KX122_BUF_READ, &buffer[0], 6*kx122_state.wmi_buf_size);    
         if(bufsta_ptr)//�д��� ��bufmax�洢
         {

               for(j=0;j<6*kx122_state.wmi_buf_size;j++)
               {
                    if (i >= BUF_SIZE)
                    {    
                           i =0;
                           bufsta_ptr=0;
                           RdStart=0;
                          if(TestMod)
                          {
                             RdReturn=1;  //����CAN �ظ�����
                          }
                          break;
                    }
               
                    raw_data_buf[i++]=buffer[j]; 
                                
              }                                           
                             
         }
         else
         {                         
               for(j=0;j<6*kx122_state.wmi_buf_size;j++)
               {
                  if(index>bufmax-1)   
                  {
                      index=0;
                      break;
                                                         
                  } 
                  data_buf[index++]=buffer[j]; 

/*****************************************************************/                
/*                  if(++z==4)   //Z�����ݲ�Ҫ
                    {  z=0;
                       j+=2;                    
                    }*/
/****************************************************************/                     
                  Ang_data_buf[0]=buffer[0];
                  Ang_data_buf[1]=buffer[1];    
                  Ang_data_buf[2]=buffer[2];   
                  Ang_data_buf[3]=buffer[3];
                  Ang_data_buf[4]=buffer[4];    
                  Ang_data_buf[5]=buffer[5]; //����ƽʱ�Ƕȼ����ԭʼ���ٶ�����   
              }
          
         }        
        
    }

}
//�ⲿ�жϷ������
void EXTI9_5_IRQHandler(void)
{

   if (EXTI_GetITStatus(EXTI_Line8) != RESET)//�ж�ĳ�����ϵ��ж��Ƿ����ж��߼�
    {
        Rd_SegKX122();
        kx122_spi_write_reg(0x3e, 0x00);
        EXTI_ClearITPendingBit(EXTI_Line8);    //��� LINE7 �ϵ��жϱ�־λ
    }
}

//�ⲿ�жϷ������
void EXTI15_10_IRQHandler(void)
{
	uint8_t buf;
	if (EXTI_GetITStatus(EXTI_Line15) != RESET)//�ж�ĳ�����ϵ��ж��Ƿ����ж��߼�
	{
		kx122_spi_read_reg(KX122_INT_REL, &buf, 1);
        if((RdReturn==0)&&(RdStart==0))   //�������ݽ׶β�����
        {
           if(TestMod)
           {
             RdStart=1;
           }
        }

	    EXTI_ClearITPendingBit(EXTI_Line15);    //��� LINE15 �ϵ��жϱ�־λ
	}
}








//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ
//SPI�ڳ�ʼ��
//�������Ƕ�SPI2�ĳ�ʼ��


void SPI2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //PORTBʱ��ʹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,  ENABLE);  //SPI2ʱ��ʹ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //PB13/14/15�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB    
    
    
    
    GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15); //PB13/14/15����
    
    
    
    

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;      //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;    //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;      //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRCֵ����Ķ���ʽ
    SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

    SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����


}
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ
//SPI_BaudRatePrescaler_8   8��Ƶ
//SPI_BaudRatePrescaler_16  16��Ƶ
//SPI_BaudRatePrescaler_256 256��Ƶ

void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    SPI2->CR1 &= 0XFFC7;
    SPI2->CR1 |= SPI_BaudRatePrescaler; //����SPI2�ٶ�
    SPI_Cmd(SPI2, ENABLE);
}



u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
    {
        retry++;
        if (retry > SPI_WRRD_RETRY_NUM)
            return 0;
    }
    SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
    retry = 0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
    {
        retry++;
        if (retry > SPI_WRRD_RETRY_NUM)
            return 0;
    }
    return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����
}

kx_drv_status_e kx122_spi_write_reg(uint8_t reg, uint8_t data)
{
    KX122_SPI_CS = 0;
    NOP_WAIT;
    SPI2_ReadWriteByte(reg);
    SPI2_ReadWriteByte(data);
    NOP_WAIT;
    KX122_SPI_CS = 1;
    return KX_DRV_SUCCESS;
}

kx_drv_status_e kx122_spi_read_reg(uint8_t reg, uint8_t *pdata, uint16_t num)
{
    uint16_t i;
  
    uint8_t rd_reg = (reg | 0x80);
    KX122_SPI_CS = 0;
    NOP_WAIT;

    SPI2_ReadWriteByte(rd_reg);
    for (i = 0; i < num; i++)
    {
        *(pdata + i) = SPI2_ReadWriteByte(0xff);
    }
    NOP_WAIT;

    
    
    KX122_SPI_CS = 1;
    return KX_DRV_SUCCESS;
}

uint8_t kx122_init(void)
{
    static uint16_t i=0;
    uint8_t buf;
    do
    {
        kx122_spi_write_reg(0x18, 0x00);
        kx122_spi_write_reg(0x7F, 0x00);
        kx122_spi_write_reg(KX122_CTRL_REG2, 0x00);
        //Software RESET
        kx122_spi_write_reg(KX122_CTRL_REG2, 0x80);
        delay_ms(5);

        //delay_ms(50);
        //kx122_spi_write_reg(0x18, 0x80);
        //read who am i register
        kx122_spi_read_reg(KX122_CHIP_ID_REG, &buf, 1);
        if(++i>500)
        {
          i=0;
          return 0;
        
        }
        
    }
    while (buf != 0x1b);
    i=0;
    //read cotr register
    kx122_spi_read_reg(KX122_COTR_REG, &buf, 1);
    //self test
    kx122_spi_write_reg(KX122_SELFTEST_REG, 0xCA);
    delay_ms(50);
    kx122_spi_write_reg(KX122_SELFTEST_REG, 0x0);

#ifdef SAMPLE_BUF_FULL_INT
    //Sample Buffer-Full interrupt via physical hardware interrupt
    kx122_spi_write_reg(KX122_CTRL_REG1, 0x40);
    kx122_spi_write_reg(KX122_INT_CTRL_REG1, 0x38);
    kx122_spi_write_reg(KX122_INT_CTRL_REG4, 0x40);
    kx122_spi_write_reg(KX122_ODCNTL_REG, 0x0F);
    kx122_spi_write_reg(KX122_BUF_CNTL2, 0xE0);
    kx122_spi_write_reg(KX122_CTRL_REG1, 0xE0);
#endif


#ifdef 	WUF_WMI_FIFO_MODE
	//motion detection, Wake Up Function via latched physical hardware interrupt    
	kx122_spi_write_reg(KX122_CTRL_REG1, RES | WUFE);
	kx122_spi_write_reg(KX122_CTRL_REG3,OWUF_100HZ );//OWUF_50HZ
	kx122_spi_write_reg(KX122_INT_CTRL_REG2, 0x3F);//default value

	//WUFC(counts) = Desired Delay Time(sec) x OWUF(Hz)
	//WUFC(counts) = 0.1 x 50 = 5 counts
	kx122_spi_write_reg(KX122_WAKEUP_TIMER_REG, 0x00);

	//WAKEUP_THREHOLD(counts) = Desired Threshold(g) x 16 (counts / g)
	//WAKEUP_THREHOLD(counts) = 0.5 x 16 = 8 counts
    if(TestMod)
    {  
	    kx122_spi_write_reg(KX122_WAKEUP_THRESHOLD_REG, 0x08);//0x08  ��������
	    kx122_spi_write_reg(KX122_INT_CTRL_REG5, IEA2 | IEN2);
	//to set the WUF interrupt to be reported on physical interrupt pin INT2.
	    kx122_spi_write_reg(KX122_INT_CTRL_REG6, WUFI2);
     }

	//Sample Buffer-Full Interrupt via physical hardware interrupt 
	kx122_spi_write_reg(KX122_INT_CTRL_REG1, IEA1 | IEN1 | IEL1);
	kx122_spi_write_reg(KX122_INT_CTRL_REG4, WMI1);
	kx122_spi_write_reg(KX122_ODCNTL_REG,OSA_6400HZ );// OSA_50HZ OSA_12800HZ  
	kx122_spi_write_reg(KX122_BUF_CNTL1,kx122_state.wmi_buf_size);// 
	
    kx122_spi_write_reg(KX122_BUF_CNTL2, BUFE | BRES );

 //   KxRange=16;
	kx122_spi_write_reg(KX122_CTRL_REG1, PC1 | RES | WUFE|(int8u)KxRange);

	//Read Interrupt Release(INT_REL) register to unlatch(clear) the output interrupt
	//created by the motion detection function
	kx122_spi_read_reg(KX122_INT_REL, &buf, 1);
#endif


#ifdef SYNC_HW_INT_RD_ODR
    //Synchronous Hardware Interrupt Read Back Acceleration Data(Setting G-Range and ODR)
    kx122_spi_write_reg(KX122_CTRL_REG1, 0x60);
    kx122_spi_write_reg(KX122_INT_CTRL_REG1, 0x38);
    kx122_spi_write_reg(KX122_INT_CTRL_REG4, 0x10);
    kx122_spi_write_reg(KX122_ODCNTL_REG, 0x02);
    kx122_spi_write_reg(KX122_CTRL_REG1, 0xE0);
#endif
#ifdef ASYNC_RD_ODR
    //Asynchronous Read Back Acceleration Data (Setting G-Range and ODR    kx122_spi_write_reg(KX122_CTRL_REG1, 0x60);
    kx122_spi_write_reg(KX122_INT_CTRL_REG1, 0x40);
    kx122_spi_write_reg(KX122_ODCNTL_REG, 0x02);
    kx122_spi_write_reg(KX122_CTRL_REG1, 0xC0);
#endif

	//motion detection, Wake Up Function via latched physical hardware interrupt    
#ifdef WUF_BUF_FIFO_MODE
	kx122_spi_write_reg(KX122_CTRL_REG1, RES | WUFE);
	kx122_spi_write_reg(KX122_CTRL_REG3, OWUF_50HZ);
	kx122_spi_write_reg(KX122_INT_CTRL_REG2, 0x3F);//default value

	//WUFC(counts) = Desired Delay Time(sec) x OWUF(Hz)
	//WUFC(counts) = 0.1 x 50 = 5 counts
	kx122_spi_write_reg(KX122_WAKEUP_TIMER_REG, 0x05);

	//WAKEUP_THREHOLD(counts) = Desired Threshold(g) x 16 (counts / g)
	//WAKEUP_THREHOLD(counts) = 0.5 x 16 = 8 counts
	kx122_spi_write_reg(KX122_WAKEUP_THRESHOLD_REG, 0x08);

	kx122_spi_write_reg(KX122_INT_CTRL_REG5, IEA2 | IEN2);
	//to set the WUF interrupt to be reported on physical interrupt pin INT2.
	kx122_spi_write_reg(KX122_INT_CTRL_REG6, WUFI2);





	//Sample Buffer-Full Interrupt via physical hardware interrupt 
	kx122_spi_write_reg(KX122_INT_CTRL_REG1, IEA1 | IEN1 | IEL1);
	kx122_spi_write_reg(KX122_INT_CTRL_REG4, BFI1);
	kx122_spi_write_reg(KX122_ODCNTL_REG, OSA_50HZ);
	kx122_spi_write_reg(KX122_BUF_CNTL2, BUFE | BRES | BFIE);
	

	kx122_spi_write_reg(KX122_CTRL_REG1, PC1 | RES | WUFE);

	//Read Interrupt Release(INT_REL) register to unlatch(clear) the output interrupt
	//created by the motion detection function
	kx122_spi_read_reg(KX122_INT_REL, &buf, 1);
#endif
    return 1;
}




void hw1_init(void)
{
    // conifg kx122 CS
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRH &= 0XFFFFFFF0;
    GPIOA->CRH |= 0X00000003;
}



void SpiDrive_Init(void)
{
//    MY_NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	kx122_config(&kx122_state);
    hw1_init();
    EXTIX_Init();
    SPI2_Init();
    kx122_init();

}

int16u Offset_g_Range(void)
{
//      int16u range=0;
      if(KxRange==0)//2G
      {
          return (16384.0);
      }  
      else if(KxRange==8)//4G
      {
         
         return (16384.0/2);
      }
      else if(KxRange==16)  //8G           
      {

         return (16384.0/4);
      
      }    
      else
      {
         return (16384.0);  
      }
}    
float Ang_Calc(float ad,int8u bit)
{
    float ang;
    if(bit)
    {
      ad=-ad;
    }
   
    ang=ad/Offset_g_Range();       //�Ƕȱ궨��ĽǶ�����ֵ
    if(ang>=1)
        ang=9000;
    else if(ang<=-1)
        ang=-900;
    else
    {
        ang=asin(ang);
        ang=(ang/3.14)*18000;
        
    }
    if(bit)
    {
      ang=-ang;
    }    
    
    
    return ((uint16_t)ang);
}



//***********************************************************************/
// *��    �ܣ���ȡ�Ƕȵ�ADֵ�������˲��󣩣� ������Ƕ�ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void SnProc(void)
{

    unsigned char buffer[6];
    kx122_acc_t acc;
    u8 xbit;u8 ybit;u8 zbit;

/*    
    kx122_spi_read_reg(KX122_OUTX_L, buffer, 6);
 
    if (buffer[1] & 0x80)
    {   xbit=1;
        acc.x = (((int)buffer[1] << 8) + (buffer[0]) - 65536);
    }
    else
    {   xbit=0;
        acc.x = (((int)buffer[1] << 8) + buffer[0]);
    }
    if (buffer[3] & 0x80)
    {   ybit=1; 
        acc.y = (((int)buffer[3] << 8) + (buffer[2]) - 65536);
    }
    else
    {   ybit=0;
        acc.y = (((int)buffer[3] << 8) + (buffer[2]));
    }
    if (buffer[5] & 0x80)
    {   zbit=1;
        acc.z = (((int)buffer[5] << 8) + (buffer[4]) - 65536);
    }
    else
    {   zbit=0;
        acc.z = (((int)buffer[5] << 8) + (buffer[4]));
    }
*/      
    if (Ang_data_buf[1] & 0x80)
    {   xbit=1;
        acc.x = (((int)Ang_data_buf[1] << 8) + (Ang_data_buf[0]) - 65536);
    }
    else
    {   xbit=0;
        acc.x = (((int)Ang_data_buf[1] << 8) + Ang_data_buf[0]);
    }
    if (raw_data_buf[3] & 0x80)
    {   ybit=1; 
        acc.y = (((int)Ang_data_buf[3] << 8) + (Ang_data_buf[2]) - 65536);
    }
    else
    {   ybit=0;
        acc.y = (((int)Ang_data_buf[3] << 8) + (Ang_data_buf[2]));
    }
    if (Ang_data_buf[5] & 0x80)
    {   zbit=1;
        acc.z = (((int)Ang_data_buf[5] << 8) + (Ang_data_buf[4]) - 65536);
    }
    else
    {   zbit=0;
        acc.z = (((int)Ang_data_buf[5] << 8) + (Ang_data_buf[4]));
    }
    

    AccStr_x.filterval = SlideWinFilterf(&(AccStr_x.arrpt),&(AccStr_x.arrcnt),&AccStr_x.BufFilter[0],acc.x); //�����˲� //���ֵ�Ŵ�10�� ����һλС��λ
    AccStr_y.filterval = SlideWinFilterf(&(AccStr_y.arrpt),&(AccStr_y.arrcnt),&AccStr_y.BufFilter[0],acc.y); //�����˲� //���ֵ�Ŵ�10�� ����һλС��λ
    AccStr_z.filterval = SlideWinFilterf(&(AccStr_z.arrpt),&(AccStr_z.arrcnt),&AccStr_z.BufFilter[0],acc.z); //�����˲� //���ֵ�Ŵ�10�� ����һλС��λ              

    
     AlldataBuf[0]=Ang_Calc(AccStr_x.filterval,xbit); 
     AlldataBuf[1]=Ang_Calc(AccStr_y.filterval,ybit);
     AlldataBuf[2]=Ang_Calc(AccStr_z.filterval,zbit);

}

void  AccStr_Init( void )
{
    AccStr_x.arrpt=0;  //��������λ��ָ��
    AccStr_x.arrcnt=0; //Ԫ�ظ���Ϊ0 
    AccStr_x.filterval=0;
    AccStr_x.initval=0;     
    
    AccStr_y.arrpt=0;  //��������λ��ָ��
    AccStr_y.arrcnt=0; //Ԫ�ظ���Ϊ0 
    AccStr_y.filterval=0;
    AccStr_y.initval=0;      
    
    AccStr_z.arrpt=0;  //��������λ��ָ��
    AccStr_z.arrcnt=0; //Ԫ�ظ���Ϊ0 
    AccStr_z.filterval=0;
    AccStr_z.initval=0;           
}  
  





void AccTran_AngCalc(void)
{
    SnProc();               
}

int8u Select_g_Range(int8u gse)
{
//    int8u range=0;
    int8u buf=0;
    switch(gse)
    {
        case 2:
              KxRange=0;
              MappingRange=2;
        break;
      
        
        case 4:
              KxRange=8;
              MappingRange=4;
         
        break;
      
       case  8:
              KxRange=16;
              MappingRange=8;
        break;
        
      	default:
              return 0;               
			  break; //command specifier not valid
    
    }
    kx122_spi_write_reg(KX122_CTRL_REG1, PC1 | RES | WUFE|(int8u)KxRange);
	kx122_spi_read_reg(KX122_INT_REL, &buf, 1);
    return 1;
}

