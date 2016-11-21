//*****************文*件*信*息********************************************
// *文 件 名：Timer.h
// *版　  本：v1.0
// *创 建 人：Liuliu
// *日　  期：2014/5/23 15:53:59
// *描　  述：
// *
// *当前版本：v1.0
// *修 改 人：Liuliu
// *日　  期：2014/5/23 15:54:02
// *描　  述：
//************************************************************************
#ifndef __TIMER_H__
	#define __TIMER_H__
#include "_Type.h"
	/*-----宏定义---------------------*/
#define TIM3_TICK   313 //us
	/*-----结构体数据类型定义---------*/
typedef void (*hk_tm_proc)(void);

//extern hk_timer_proc timer3_hook;
	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
    void TIMInit(void);
	void Timer2_Init();//TIM2初始化
	void TIM2_IRQHandler(void); //TIM2中断处理函数
	void Time_Proc(void);
    
	void Time3_Init( void);
	void TIM3_IRQHandler(void); //TIM3中断处理函数	
    void Timer1_Init(void)  ;
    
    int8u timer_set_hook(int8u timer_num,hk_tm_proc fn);
    
#endif
//***********************************************************************/
#ifdef   __TIMER_C__
#define  TIEMR_EXT
#else
#define  TIEMR_EXT  extern
#endif
/*-------------变量声明--------------*/
TIEMR_EXT int16u   Time_Delay;
TIEMR_EXT uint32_t timestamp;
TIEMR_EXT uint8_t  time_buf[4];
//***********************************************************************/




