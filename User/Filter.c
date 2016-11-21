/*
 * filter.c
 *
 * Copyright (c) 2016-07-05 lfb
 *
 * V1.0——实现一阶和二阶卡尔曼，移动平均值
 *
 */
//#include "stdafx.h"
#include "filter.h"

/*
 * @brief   
 *   Init fields of structure @kalman1_state.
 *   I make some defaults in this init function:
 *     A = 1;
 *     H = 1; 
 *   and @q,@r are valued after prior tests.
 *
 *   NOTES: Please change A,H,q,r according to your application.
 *
 * @inputs  
 *   state - Klaman filter structure
 *   init_x - initial x state value   
 *   init_p - initial estimated error convariance
 * @outputs 
 * @retval  
 */
void kal1_init(kalman1 *kal,float init_x, float init_p)
{
	kal->x = init_x;
    kal->p = init_p;
    kal->A = 1;
    kal->H = 1;
	kal->q = 20e-4;//10e-6;  /* predict noise convariance */
	kal->r = 0.1;//10e-5;  /* measure error convariance */
}

/*
 * @brief   
 *   1 Dimension Kalman filter
 * @inputs  
 *   state - Klaman filter structure
 *   z_measure - Measure value
 * @outputs 
 * @retval  
 *   Estimated result
 */
float kal1_filter(kalman1 *kal,float new_data)
{
    /* Predict */
    float x,p;
    x = kal->A * kal->x;
    p = kal->A * kal->A * kal->p + kal->q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */

    /* Measurement */
    kal->gain = p * kal->H / (p * kal->H * kal->H + kal->r);
    x = x + kal->gain * (new_data - kal->H * x);
    p = (1 - kal->gain * kal->H) * p;
    kal->x = x;
    kal->p = p;

    return x;
}

/*
 * @brief   
 *   Init fields of structure @kalman1_state.
 *   I make some defaults in this init function:
 *     A = {{1, 0.1}, {0, 1}};
 *     H = {1,0}; 
 *   and @q,@r are valued after prior tests. 
 *
 *   NOTES: Please change A,H,q,r according to your application.
 *
 * @inputs  
 * @outputs 
 * @retval  
 */
void kal2_init(kalman2 *kal,float *init_x, float (*init_p)[2])
{
    kal->x[0]    = init_x[0];
    kal->x[1]    = init_x[1];
    kal->p[0][0] = init_p[0][0];
    kal->p[0][1] = init_p[0][1];
    kal->p[1][0] = init_p[1][0];
    kal->p[1][1] = init_p[1][1];
    //A       = {{1, 0.1}, {0, 1}};
    kal->A[0][0] = 1;
    kal->A[0][1] = 0.1;
    kal->A[1][0] = 0;
    kal->A[1][1] = 1;
    //H       = {1,0};
    kal->H[0]    = 1;
    kal->H[1]    = 0;
    //q       = {{10e-6,0}, {0,10e-6}};  /* measure noise convariance */
    kal->q[0]    = 1e-5;
    kal->q[1]    = 1e-5;
    kal->r       = 10;  /* estimated error convariance */
}

/*
 * @brief   
 *   2 Dimension kalman filter
 * @inputs  
 *   state - Klaman filter structure
 *   z_measure - Measure value
 * @outputs 
 *   x[0] - Updated state value, Such as angle,velocity
 *   x[1] - Updated state value, Such as diffrence angle, acceleration
 *   p    - Updated estimated error convatiance matrix
 * @retval  
 *   Return value is equals to x[0], so maybe angle or velocity.
 */
float kal2_filter(kalman2 *kal,float new_data)
{
    float temp0 = 0.0f;
    float temp1 = 0.0f;
    float temp = 0.0f;

	float x[2],p[2][2];
    /* Step1: Predict */
    x[0] = kal->A[0][0] * kal->x[0] + kal->A[0][1] * kal->x[1];
    x[1] = kal->A[1][0] * kal->x[0] + kal->A[1][1] * kal->x[1];
    /* p(n|n-1)=A^2*p(n-1|n-1)+q */
    p[0][0] = kal->A[0][0] * kal->p[0][0] + kal->A[0][1] * kal->p[1][0] + kal->q[0];
    p[0][1] = kal->A[0][0] * kal->p[0][1] + kal->A[1][1] * kal->p[1][1];
    p[1][0] = kal->A[1][0] * kal->p[0][0] + kal->A[0][1] * kal->p[1][0];
    p[1][1] = kal->A[1][0] * kal->p[0][1] + kal->A[1][1] * kal->p[1][1] + kal->q[1];

    /* Step2: Measurement */
    /* gain = p * H^T * [r + H * p * H^T]^(-1), H^T means transpose. */
    temp0 = p[0][0] * kal->H[0] + p[0][1] * kal->H[1];
    temp1 = p[1][0] * kal->H[0] + p[1][1] * kal->H[1];
    temp  = kal->r + kal->H[0] * temp0 + kal->H[1] * temp1;
    kal->gain[0] = temp0 / temp;
    kal->gain[1] = temp1 / temp;
    /* x(n|n) = x(n|n-1) + gain(n) * [z_measure - H(n)*x(n|n-1)]*/
    temp = kal->H[0] * x[0] + kal->H[1] * x[1];
    x[0] = x[0] + kal->gain[0] * (new_data - temp); 
    x[1] = x[1] + kal->gain[1] * (new_data - temp);

    /* Update @p: p(n|n) = [I - gain * H] * p(n|n-1) */
    p[0][0] = (1 - kal->gain[0] * kal->H[0]) * p[0][0];
    p[0][1] = (1 - kal->gain[0] * kal->H[1]) * p[0][1];
    p[1][0] = (1 - kal->gain[1] * kal->H[0]) * p[1][0];
    p[1][1] = (1 - kal->gain[1] * kal->H[1]) * p[1][1];

	kal->x[0] = x[0];
	kal->x[1] = x[1];
	kal->p[0][0] = p[0][0];
	kal->p[0][1] = p[0][1];
	kal->p[1][0] = p[1][0];
	kal->p[1][1] = p[1][1];

    return x[0];
}


/*
*滑动窗口滤波
*/
void maf_init(maf_t *ft, int16 *buf, uint8 len)
{
	ft->len = len;
	ft->cnt = 0;
	ft->pos = 0;
	ft->buf = buf;
}

int16 maf_filter(maf_t *ft, int16 new_data)
{
	uint8  i;
	int32 sum = 0;
	int16 ft_val;
	if(ft->pos >= ft->len)
	{
		ft->pos = 0;
	}
	ft->buf[ft->pos] = new_data;
	ft->pos++;
	if(ft->cnt < ft->len)
	{
		ft->cnt++;
	}
	else
	{
		ft->cnt = ft->len;
	}
	for(i=0;i<ft->cnt;i++)
	{
		sum += ft->buf[i];
	}
	ft_val = sum/ft->cnt;
	return ft_val;
}

void maf_clear(maf_t *ft)
{
	ft->pos = 0;
	ft->cnt = 0;
}

//***********************************************************************/
// *功    能：一阶滤波
// *入    口：old_val，前一次滤波值；new_val，新值；filtercnt，滤波系数
// *出    口：一阶滤波计算后的值
// *备    注：0<filtercnt<256
// *函数性质：
//***********************************************************************/

float filter_1st (float old_val,int16 new_val, uint8 ft_cnt)
{	
    float sum;
    sum = old_val*(ft_cnt-1)+new_val;
	return (sum/ft_cnt);
}
    
/*
 * just for test
 */
#if _DEBUG_
void test(void)
{
	const float td[] = {1,5,12,20,16,8,23,11,30,25};
	maf_t ft1;
	int len = sizeof(td)/sizeof(td[0]);
	maf_init(&ft1);
	maf_len_set(len);
	for( int i=0;i<len;i++)
	{
		float val = maf_filter(ft1,
		printf("%5.2f",&val);
	}
	printf("\r\n");
}
#endif
                               