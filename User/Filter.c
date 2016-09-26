//*****************��*��*��*Ϣ****************************************
// *�� �� ����Filter.c
// *�桡  ����v1.0
// *�� �� �ˣ�Liuliu
// *�ա�  �ڣ�2014/5/23 15:43:38
// *�衡  ����
// *
// *��ǰ�汾��v1.0
// *�� �� �ˣ�Liuliu
// *�ա�  �ڣ�2014/5/23 15:43:43
// *�衡  ����
//************************************************************************
#define   __FILTER_C__
#include  "_Include.h"
//***********************************************************************/
// *��    �ܣ��޷��˲�
// *��    �ڣ�old_val��ǰһ�ε�ֵ��new_val����ֵ��range����Χ
// *��    �ڣ�����ֵ�ڷ������·���1�����򷵻�0
// *��    ע������Ϊold_val-range<new_val<old_val+range
// *�������ʣ�
//***********************************************************************/.
int8u ValInRange(int32u old_val,int32u new_val,int32u range)
{
		if((new_val>(old_val+range))||((new_val+range)<old_val))
			return 0;
		else
			return 1;
}
//***********************************************************************/
// *��    �ܣ�ð������
// *��    �ڣ�buf������Ļ��棻num,�����Ԫ�ظ�����exchange����������
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
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
// *��    �ܣ�ð������
// *��    �ڣ�buf������Ļ��棻num,�����Ԫ�ظ�����exchange����������
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
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
// *��    �ܣ������������С��ֵ����DETA
// *��    �ڣ�buf�����棻num,Ԫ�ظ�����
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
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
// *��    �ܣ����������˲�
// *��    �ڣ�pbufpos,����λ��ָ��;pbufcnt,������Ԫ�ظ���ָ��;pbuf,����ָ��;
//						val,��ֵ;ftcnt�����ڳ��ȣ�Ԫ�ظ�����
// *��    �ڣ���������������֮�󷵻�ƽ��ֵ�����򷵻ش���ֵ
// *��    ע����֤ftcntС�ڵ��ڻ���ָ����ָ�������Ĵ�С
// *�������ʣ�
//***********************************************************************/
int16u SlideWinFilter (int8u *pbufpos, int8u *pbufcnt, int16u *pbuf, int16u val)
{
	int8u i;
	int32u data_ram=0;
	
	if(*(pbufpos)>=WINDOW)
	{
	   *pbufpos=0;			     	//20130619 hxl
	}	
	*(pbuf+*pbufpos)=val;    		//�滻��꣨λ�ã����ڵ�Ԫ��
	(*pbufpos)++;           		//������
	if((*pbufcnt)!=WINDOW)
    { 
      (*pbufcnt)++;   //��������δ��
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
// *��    �ܣ����������˲�
// *��    �ڣ�pbufpos,����λ��ָ��;pbufcnt,������Ԫ�ظ���ָ��;pbuf,����ָ��;
//						val,��ֵ;ftcnt�����ڳ��ȣ�Ԫ�ظ�����
// *��    �ڣ���������������֮�󷵻�ƽ��ֵ�����򷵻ش���ֵ
// *��    ע����֤ftcntС�ڵ��ڻ���ָ����ָ�������Ĵ�С
// *�������ʣ�
//***********************************************************************/
int16u SlideWinFilter16u(int8u *pbufpos, int8u *pbufcnt, int16u *pbuf, int16u val)
{
	int8u i;
	int32u data_ram=0;
	
	if(*(pbufpos)>=WINDOW)
	{
	   *pbufpos=0;			     	//20130619 hxl
	}	
	*(pbuf+*pbufpos)=val;    		//�滻��꣨λ�ã����ڵ�Ԫ��
	(*pbufpos)++;           		//������
	if((*pbufcnt)!=WINDOW)
		(*pbufcnt)++;				//��������δ��
	for(i=0; i<(*pbufcnt);i++)
	{	
		data_ram+=*(pbuf+i);
	}
	data_ram=data_ram/(*pbufcnt);
	return ((int16u)data_ram);			
}
//***********************************************************************/
// *��    �ܣ��������ڴ���
// *��    �ڣ�pbufpos,����λ��ָ��;pbufcnt,������Ԫ�ظ���ָ��;pbuf,����ָ��;
//						val,��ֵ;ftcnt�����ڳ��ȣ�Ԫ�ظ�����
// *��    �ڣ���������������֮�󷵻�ƽ��ֵ�����򷵻ش���ֵ
// *��    ע����֤ftcntС�ڵ��ڻ���ָ����ָ�������Ĵ�С
// *�������ʣ�
//***********************************************************************/
int32u  SlideWinDeta (int8u *pbufpos, int8u *pbufcnt, int32u *pbuf, int32u val)
{
	int32u temp=0;
	
	if(*(pbufpos)>=WINDOW)
	{
	   *pbufpos=0;			     	//20130619 hxl
	}	
	*(pbuf+*pbufpos)=val;    		//�滻��꣨λ�ã����ڵ�Ԫ��
	(*pbufpos)++;           		//������
	if((*pbufcnt)!=WINDOW)
    {	
      	(*pbufcnt)++;   //��������δ��
      	return 0;
	}
    else 
    {
     	temp=BufDeta(pbuf, WINDOW);
        return (temp);
	}
}
//***********************************************************************/
// *��    �ܣ�һ���˲�
// *��    �ڣ�old_val��ǰһ���˲�ֵ��new_val����ֵ��filtercnt���˲�ϵ��
// *��    �ڣ�һ���˲�������ֵ
// *��    ע��0<filtercnt<256
// *�������ʣ�
//***********************************************************************/

float Filter_1st (float old_val,int16u new_val, int8u filtercnt)
{	
	return ((old_val*(filtercnt-1)+new_val)/filtercnt);
}


//***********************************************************************/
// *��    �ܣ����������˲�
// *��    �ڣ�pbufpos,����λ��ָ��;pbufcnt,������Ԫ�ظ���ָ��;pbuf,����ָ��;
//						val,��ֵ;ftcnt�����ڳ��ȣ�Ԫ�ظ�����
// *��    �ڣ���������������֮�󷵻�ƽ��ֵ�����򷵻ش���ֵ
// *��    ע����֤ftcntС�ڵ��ڻ���ָ����ָ�������Ĵ�С
// *�������ʣ�
//***********************************************************************/
int16s SlideWinFilterf (int8u *pbufpos, int8u *pbufcnt, int16s *pbuf, int16s val)
{
	int8u i;
	int32s data_ram=0;
	
	if(*(pbufpos)>=WINDOW)
	{
	   *pbufpos=0;			     	//20130619 hxl
	}	
	*(pbuf+*pbufpos)=val;    		//�滻��꣨λ�ã����ڵ�Ԫ��
	(*pbufpos)++;           		//������
	if((*pbufcnt)!=WINDOW)
    { 
      (*pbufcnt)++;   //��������δ��
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
// *��    �ܣ��������˲�
// *��    �ڣ�
//						
// *��    �ڣ�
// *��    ע��
// *�������ʣ� Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵 R:����������
// R���󣬶�̬��Ӧ�����������ȶ��Ա��  
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
        p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
        kg=p_mid/(p_mid+R); //kgΪkalman filter��RΪ����
        x_now=x_mid+kg*(ResrcData-x_mid);//���Ƴ�������ֵ
               
        p_now=(1-kg)*p_mid;//����ֵ��Ӧ��covariance       

        p_last = p_now; //����covarianceֵ
        x_last = x_now; //����ϵͳ״ֵ̬

        return x_now;               
}

/*-------------------------------------------------------------------------------------------------------------*/