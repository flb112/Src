//*****************文*件*信*息****************************************
// *文 件 名：Key.c
// *版　  本：v1.0
// *创 建 人：huangxinglong
// *日　  期：2014/5/7 10:15:56
// *描　  述：
// *
// *当前版本：v1.0
// *修 改 人：huangxinglong
// *日　  期：2014/5/7 10:16:00
// *描　  述：
//************************************************************************
#define   __KEY_C__
#include  "_Include.h"

//***********************************************************************/
// *功    能：按键status handle
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Key_StatusHandle(void)
{
	int8u i;	
	for(i=0;i<KEYNUM;i++)
	{
		if(Key[i].state!=KEY_UP)
		{
			if(Key[i].time==(HOLD+RATE))
			{	
				Key[i].flag=1;
				Key[i].state=KEY_RATE;		
			}
			else if(Key[i].time==HOLD)
			{
				Key[i].flag=1;
				Key[i].state=KEY_HOLD;	
			}
			else if(Key[i].time==DOWN)
			{	
				Key[i].flag=1;				
				Key[i].state=KEY_DOWN;
			}
		}	
	}
}

//***********************************************************************/
// *功    能：按键处理
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void KeyStop_Handle(Keystruct* keystop)
{
	
	if(keystop->flag==1)
	{		
		keystop->flag=0;	
		switch(keystop->state)
		{
			case KEY_UP:
				keystop->time=0;					
				keystop->state=KEY_NUL;	
				break;
			case KEY_DOWN:
//				O_STOPLED=0;
//				Key_StopDown();
				break;
			case KEY_HOLD:		
				break;
			case KEY_RATE:	
				keystop->time-=RATE;
				break;
		}  
	}
}

//***********************************************************************/
// *功    能：按键初始化
// *入    口：
// *出    口：
// *备    注：//8 个按键open/close/stop, menu 5个
// *函数性质：
//***********************************************************************/
void Key_GPIO_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;      //open
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//
        
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;	//stop
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //设置成上拉输入  
	GPIO_Init(GPIOB, &GPIO_InitStructure);      

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4;   //PC0,PC1  Mode0,mode1 //close
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;             //设置成上拉输入  
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
        
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;   //PD9,PD11,PD12,PD13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;             //设置成上拉输入  
	GPIO_Init(GPIOD, &GPIO_InitStructure);               
}
//***********************************************************************/
// *功    能：按键scan
// *入    口：10ms intervals
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Key_Scan(void)
{	
	int8u val=0;
	int8u i,temp;

	val=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
	val|=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)<<1;
	val|=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)<<2;   
	//menu
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)==0)         //key[3] KEY1
		val|=1<<3;
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)==0)			//key[4] KEY2
		val|=1<<4;
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)==0)			//key[5] KEY3
		val|=1<<5;
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)==0)			//key[6] KEY5
		val|=1<<6;
	//---------------------------------------------------------------------------------
    temp=0x01;
 	for(i=0;i<KEYNUM;i++)
 	{
 		if(val&temp)
 		{
 			Key[i].time++;
            break;
		}
		else if(Key[i].time>0)
		{	
			Key[i].state=KEY_UP;
			Key[i].flag=1;
		}	
		temp<<=1;
 	}
 	Key_StatusHandle();
       
}
//***********************************************************************/
// *功    能：按键处理
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Key_Proc(void)
{
   ;// 	KeyStop_Handle(&Key[0]);                    
}
