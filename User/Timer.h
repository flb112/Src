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
	/*-----�궨��---------------------*/
	/*-----�ṹ���������Ͷ���---------*/
	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
	void Timer2_Init();//TIM2��ʼ��
	void TIM2_IRQHandler(void); //TIM2�жϴ�����
	void Time_Proc(void);
	int8u  Time4_Init( int16u  timerb50us);
	void Timer4Enable( int32u timercount ) ;
	void Timer4Disable( void ); //�ر�ʱ��;
	void TIM4_IRQHandler(void); //TIM3�жϴ�����	
    void Timer1_Init(void)  ;
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




