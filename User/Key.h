//*****************��*��*��*Ϣ********************************************
// *�� �� ����Key.h
// *�桡  ����v1.0
// *�� �� �ˣ�Liuliu
// *�ա�  �ڣ�2014/5/7 10:15:56
// *�衡  ����
// *
// *��ǰ�汾��v1.0
// *�� �� �ˣ�Liuliu
// *�ա�  �ڣ�2014/5/7 10:16:00
// *�衡  ����
//************************************************************************
#ifndef __KEY_H__
	#define __KEY_H__
	/*-----�궨��---------------------*/
	#define	KEYNUM	  7	
	#define DOWN      4                  //30
	#define HOLD      100				//S		
	#define RATE      4                 //20ms
	
	#define KEY_NUL      	0
	#define	KEY_DOWN		1		//������һ�ΰ���
	#define	KEY_HOLD		2		//
	#define	KEY_RATE		3
	#define	KEY_UP 			4		//�ް���	
	
	/*-----�ṹ���������Ͷ���---------*/
	typedef struct 
	{
		//int8u press;	//��������
		int8u state; 
		int16u time;
		int8u flag;			//��������ִ��
		
	}Keystruct ;
	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
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
/*-------------��������--------------*/
KEY_EXT int8u KeyFlag;
KEY_EXT Keystruct Key[KEYNUM];			//key0:Open,1:Close,2:Stop,3:input,4:up,5down,
//***********************************************************************/
