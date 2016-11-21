//*****************��*��*��*Ϣ********************************************
// *�� �� ����Timer.h
// *�桡  ����v1.0
// *�� �� �ˣ�Liuliu
// *�ա�  �ڣ�2014/5/23 15:53:59
// *�衡  ����
// *
// *��ǰ�汾��v1.0
// *�� �� �ˣ�Liuliu
// *�ա�  �ڣ�2014/5/23 15:54:02
// *�衡  ����
//************************************************************************
#ifndef __TIMER_H__
	#define __TIMER_H__
#include "_Type.h"
	/*-----�궨��---------------------*/
#define TIM3_TICK   300 //us
	/*-----�ṹ���������Ͷ���---------*/
typedef void (*hk_tm_proc)(void);

//extern hk_timer_proc timer3_hook;
	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
    void TIMInit(void);
	void Timer2_Init();//TIM2��ʼ��
	void TIM2_IRQHandler(void); //TIM2�жϴ�����
	void Time_Proc(void);
    
	void Time3_Init( void);
	void TIM3_IRQHandler(void); //TIM3�жϴ�����	
    void Timer1_Init(void)  ;
    
    int8u timer_set_hook(int8u timer_num,hk_tm_proc fn);
    
#endif
//***********************************************************************/
#ifdef   __TIMER_C__
#define  TIEMR_EXT
#else
#define  TIEMR_EXT  extern
#endif
/*-------------��������--------------*/
TIEMR_EXT int16u   Time_Delay;
TIEMR_EXT uint32_t timestamp;
TIEMR_EXT uint8_t  time_buf[4];
//***********************************************************************/




