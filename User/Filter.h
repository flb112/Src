//*****************��*��*��*Ϣ********************************************
// *�� �� ����Delay.h
// *�桡  ����v1.0
// *�� �� �ˣ�Liuliu
// *�ա�  �ڣ�2014/5/7 10:15:56
// *�衡  ����
// *
// *��ǰ�汾��v1.0
// *�� �� �ˣ�Liuliu
// *�ա�  �ڣ�2014/5/7 10:16:00
// *�衡  ����
//*****************************************************************************
//***********************************************************************/
#ifndef __FILTER_H__
#define __FILTER_H__

	/*-----�궨��---------------------*/

	/*-----�ṹ���������Ͷ���---------*/


	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
	int8u ValInRange(int32u old_val,int32u new_val,int32u range);
	int16u SlideWinFilter (int8u *pbufpos, int8u *pbufcnt, int16u *pbuf, int16u val);
    float Filter_1st (float old_val,int16u new_val, int8u filtercnt);

    
    int16s SlideWinFilterf (int8u *pbufpos, int8u *pbufcnt, int16s *pbuf, int16s val);
	int32u  SlideWinDeta (int8u *pbufpos, int8u *pbufcnt, int32u *pbuf, int32u val);
    void Bubble(void *buf, int16u num);
    void BubbleFloat(void *buf, int16u num);    
    
#endif