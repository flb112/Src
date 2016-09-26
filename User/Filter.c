//*****************文*件*信*息****************************************
// *文 件 名：Filter.c
// *版　  本：v1.0
// *创 建 人：Liuliu
// *日　  期：2014/5/23 15:43:38
// *描　  述：
// *
// *当前版本：v1.0
// *修 改 人：Liuliu
// *日　  期：2014/5/23 15:43:43
// *描　  述：
//************************************************************************
#define   __FILTER_C__
#include  "_Include.h"
//***********************************************************************/
// *功    能：限幅滤波
// *入    口：old_val，前一次的值；new_val，新值；range，范围
// *出    口：当新值在幅度内事返回1，否则返回0
// *备    注：幅度为old_val-range<new_val<old_val+range
// *函数性质：
//***********************************************************************/.
int8u ValInRange(int32u old_val,int32u new_val,int32u range)
{
		if((new_val>(old_val+range))||((new_val+range)<old_val))
			return 0;
		else
			return 1;
}
//***********************************************************************/
// *功    能：冒泡排序
// *入    口：buf，排序的缓存；num,排序的元素个数；exchange，交换函数
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void Bubble(void *buf, int16u num)
{
  int16u tmp,*pbuf=(int16u*)buf;
  int8u i,j;
  for(i=0;i<num;i++)
  {
    for(j=i;j<num;j++)
	{
    	if(pbuf[i]<pbuf[j])
	    {
		    tmp=pbuf[i];
		    pbuf[i]=pbuf[j];
		    pbuf[j]=tmp;
	    }
	}
  }
}



//***********************************************************************/
// *功    能：冒泡排序
// *入    口：buf，排序的缓存；num,排序的元素个数；exchange，交换函数
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void BubbleFloat(void *buf, int16u num)
{
  float tmp,*pbuf=(float*)buf;
  int8u i,j;
  for(i=0;i<num;i++)
  {
    for(j=i;j<num;j++)
	{
    	if(pbuf[i]<pbuf[j])
	    {
		    tmp=pbuf[i];
		    pbuf[i]=pbuf[j];
		    pbuf[j]=tmp;
	    }
	}
  }
}

//***********************************************************************/
// *功    能：数组最大与最小差值，即DETA
// *入    口：buf，缓存；num,元素个数；
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int32u BufDeta(void *buf, int16u num)
{
  int32u max,min,temp,*pbuf=(int32u*)buf;
  int8u i;

  max=pbuf[0];
  min=pbuf[0];  
  for(i=1;i<num;i++)
  {
	if(max<pbuf[i])
		max=pbuf[i];
	if(min>pbuf[i])
		min=pbuf[i];
  }
  temp=max-min;
  return temp;
}
//***********************************************************************/
// *功    能：滑动窗口滤波
// *入    口：pbufpos,缓存位置指针;pbufcnt,缓存中元素个数指针;pbuf,缓存指针;
//						val,新值;ftcnt，窗口长度（元素个数）
// *出    口：当窗口数据填满之后返回平均值，否则返回传入值
// *备    注：保证ftcnt小于等于缓存指针所指向的数组的大小
// *函数性质：
//***********************************************************************/
int16u SlideWinFilter (int8u *pbufpos, int8u *pbufcnt, int16u *pbuf, int16u val)
{
	int8u i;
	int32u data_ram=0;
	
	if(*(pbufpos)>=WINDOW)
	{
	   *pbufpos=0;			     	//20130619 hxl
	}	
	*(pbuf+*pbufpos)=val;    		//替换光标（位置）所在的元素
	(*pbufpos)++;           		//光标后移
	if((*pbufcnt)!=WINDOW)
    { 
      (*pbufcnt)++;   //窗口数据未满
      return val;
    }
    else 
    {
      Bubble(pbuf, WINDOW);
      for(i=(WINDOW/2-2);(i<WINDOW/2+1);i++)
      { 
        data_ram+=*(pbuf+i);
      }
      data_ram=data_ram/3;
      return ((int16u)data_ram);      
    }		
}
//***********************************************************************/
// *功    能：滑动窗口滤波
// *入    口：pbufpos,缓存位置指针;pbufcnt,缓存中元素个数指针;pbuf,缓存指针;
//						val,新值;ftcnt，窗口长度（元素个数）
// *出    口：当窗口数据填满之后返回平均值，否则返回传入值
// *备    注：保证ftcnt小于等于缓存指针所指向的数组的大小
// *函数性质：
//***********************************************************************/
int16u SlideWinFilter16u(int8u *pbufpos, int8u *pbufcnt, int16u *pbuf, int16u val)
{
	int8u i;
	int32u data_ram=0;
	
	if(*(pbufpos)>=WINDOW)
	{
	   *pbufpos=0;			     	//20130619 hxl
	}	
	*(pbuf+*pbufpos)=val;    		//替换光标（位置）所在的元素
	(*pbufpos)++;           		//光标后移
	if((*pbufcnt)!=WINDOW)
		(*pbufcnt)++;				//窗口数据未满
	for(i=0; i<(*pbufcnt);i++)
	{	
		data_ram+=*(pbuf+i);
	}
	data_ram=data_ram/(*pbufcnt);
	return ((int16u)data_ram);			
}
//***********************************************************************/
// *功    能：滑动窗口存数
// *入    口：pbufpos,缓存位置指针;pbufcnt,缓存中元素个数指针;pbuf,缓存指针;
//						val,新值;ftcnt，窗口长度（元素个数）
// *出    口：当窗口数据填满之后返回平均值，否则返回传入值
// *备    注：保证ftcnt小于等于缓存指针所指向的数组的大小
// *函数性质：
//***********************************************************************/
int32u  SlideWinDeta (int8u *pbufpos, int8u *pbufcnt, int32u *pbuf, int32u val)
{
	int32u temp=0;
	
	if(*(pbufpos)>=WINDOW)
	{
	   *pbufpos=0;			     	//20130619 hxl
	}	
	*(pbuf+*pbufpos)=val;    		//替换光标（位置）所在的元素
	(*pbufpos)++;           		//光标后移
	if((*pbufcnt)!=WINDOW)
    {	
      	(*pbufcnt)++;   //窗口数据未满
      	return 0;
	}
    else 
    {
     	temp=BufDeta(pbuf, WINDOW);
        return (temp);
	}
}
//***********************************************************************/
// *功    能：一阶滤波
// *入    口：old_val，前一次滤波值；new_val，新值；filtercnt，滤波系数
// *出    口：一阶滤波计算后的值
// *备    注：0<filtercnt<256
// *函数性质：
//***********************************************************************/

float Filter_1st (float old_val,int16u new_val, int8u filtercnt)
{	
	return ((old_val*(filtercnt-1)+new_val)/filtercnt);
}


//***********************************************************************/
// *功    能：滑动窗口滤波
// *入    口：pbufpos,缓存位置指针;pbufcnt,缓存中元素个数指针;pbuf,缓存指针;
//						val,新值;ftcnt，窗口长度（元素个数）
// *出    口：当窗口数据填满之后返回平均值，否则返回传入值
// *备    注：保证ftcnt小于等于缓存指针所指向的数组的大小
// *函数性质：
//***********************************************************************/
int16s SlideWinFilterf (int8u *pbufpos, int8u *pbufcnt, int16s *pbuf, int16s val)
{
	int8u i;
	int32s data_ram=0;
	
	if(*(pbufpos)>=WINDOW)
	{
	   *pbufpos=0;			     	//20130619 hxl
	}	
	*(pbuf+*pbufpos)=val;    		//替换光标（位置）所在的元素
	(*pbufpos)++;           		//光标后移
	if((*pbufcnt)!=WINDOW)
    { 
      (*pbufcnt)++;   //窗口数据未满
      return val;
    }
    else 
    {
      Bubble(pbuf, WINDOW);
      for(i=(WINDOW/2-2);(i<WINDOW/2+1);i++)
      { 
        data_ram+=*(pbuf+i);
      }
      data_ram=data_ram/3;
      return (data_ram);      
    }		
}

/*-------------------------------------------------------------------------------------------------------------*/
/*       
       
             
*/
//***********************************************************************/
// *功    能：卡尔曼滤波
// *入    口：
//						
// *出    口：
// *备    注：
// *函数性质： Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏 R:测量噪声，
// R增大，动态响应变慢，收敛稳定性变好  
//***********************************************************************/
double KalmanFilter(const double ResrcData,
                                        double ProcessNiose_Q,double MeasureNoise_R)
{
        double R = MeasureNoise_R;
        double Q = ProcessNiose_Q;

        static double x_last;

        double x_mid = x_last;
        double x_now;

        static        double p_last;

        double p_mid ;
        double p_now;
        double kg;       

        x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
        p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
        kg=p_mid/(p_mid+R); //kg为kalman filter，R为噪声
        x_now=x_mid+kg*(ResrcData-x_mid);//估计出的最优值
               
        p_now=(1-kg)*p_mid;//最优值对应的covariance       

        p_last = p_now; //更新covariance值
        x_last = x_now; //更新系统状态值

        return x_now;               
}

/*-------------------------------------------------------------------------------------------------------------*/