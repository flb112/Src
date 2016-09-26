//*****************文*件*信*息********************************************
// *文 件 名：Key.h
// *版　  本：v1.0
// *创 建 人：Liuliu
// *日　  期：2014/5/7 10:15:56
// *描　  述：
// *
// *当前版本：v1.0
// *修 改 人：Liuliu
// *日　  期：2014/5/7 10:16:00
// *描　  述：
//************************************************************************
#ifndef __KEY_H__
	#define __KEY_H__
	/*-----宏定义---------------------*/
	#define	KEYNUM	  7	
	#define DOWN      4                  //30
	#define HOLD      100				//S		
	#define RATE      4                 //20ms
	
	#define KEY_NUL      	0
	#define	KEY_DOWN		1		//按键第一次按下
	#define	KEY_HOLD		2		//
	#define	KEY_RATE		3
	#define	KEY_UP 			4		//无按键	
	
	/*-----结构体数据类型定义---------*/
	typedef struct 
	{
		//int8u press;	//按键按下
		int8u state; 
		int16u time;
		int8u flag;			//按键功能执行
		
	}Keystruct ;
	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
	void Key_GPIO_Init(void);
	void Key_Scan(void);
	void Key_Proc(void);
	void ExtSwitch_Input(void);
	void Bell_Handle(void);
#endif
//***********************************************************************/ 
#ifdef   __KEY_C__
#define  KEY_EXT
#else
#define  KEY_EXT  extern
#endif
/*-------------变量声明--------------*/
KEY_EXT int8u KeyFlag;
KEY_EXT Keystruct Key[KEYNUM];			//key0:Open,1:Close,2:Stop,3:input,4:up,5down,
//***********************************************************************/
