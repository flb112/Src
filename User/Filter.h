//*****************文*件*信*息********************************************
// *文 件 名：Delay.h
// *版　  本：v1.0
// *创 建 人：Liuliu
// *日　  期：2014/5/7 10:15:56
// *描　  述：
// *
// *当前版本：v1.0
// *修 改 人：Liuliu
// *日　  期：2014/5/7 10:16:00
// *描　  述：
//*****************************************************************************
//***********************************************************************/
#ifndef __FILTER_H__
#define __FILTER_H__

	/*-----宏定义---------------------*/

	/*-----结构体数据类型定义---------*/


	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
	int8u ValInRange(int32u old_val,int32u new_val,int32u range);
	int16u SlideWinFilter (int8u *pbufpos, int8u *pbufcnt, int16u *pbuf, int16u val);
    float Filter_1st (float old_val,int16u new_val, int8u filtercnt);

    
    int16s SlideWinFilterf (int8u *pbufpos, int8u *pbufcnt, int16s *pbuf, int16s val);
	int32u  SlideWinDeta (int8u *pbufpos, int8u *pbufcnt, int32u *pbuf, int32u val);
    void Bubble(void *buf, int16u num);
    void BubbleFloat(void *buf, int16u num);    
    
#endif