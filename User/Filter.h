/*
 * filter.h
 *
 * Copyright (c) 2016-07-05 lfb
 *
 * V1.0——包含一阶和二阶卡尔曼，移动平均值
 *
 */
 
#ifndef _FILTER_H_
#define _FILTER_H_

#ifdef __cplusplus
extern "C" {
#endif
    
#include "my_type.h"
/* 
 * NOTES: n Dimension means the state is n dimension, 
 * measurement always 1 dimension 
 */

/* 1 Dimension */
typedef struct
{

    float x;  /* state */
    float A;  /* x(n)=A*x(n-1)+u(n),u(n)~N(0,q) */
    float H;  /* z(n)=H*x(n)+w(n),w(n)~N(0,r)   */
    float q;  /* process(predict) noise convariance */
    float r;  /* measure noise convariance */
    float p;  /* estimated error convariance */
    float gain;
}kalman1;

void kal1_init(kalman1 *kal,float init_x, float init_p);

float kal1_filter(kalman1 *kal,float new_data);

/* 2 Dimension */
typedef struct 
{
    float x[2];     /* state: [0]-angle [1]-diffrence of angle, 2x1 */
    float A[2][2];  /* X(n)=A*X(n-1)+U(n),U(n)~N(0,q), 2x2 */
    float H[2];     /* Z(n)=H*X(n)+W(n),W(n)~N(0,r), 1x2   */
    float q[2];     /* process(predict) noise convariance,2x1 [q0,0; 0,q1] */
    float r;        /* measure noise convariance */
    float p[2][2];  /* estimated error convariance,2x2 [p0 p1; p2 p3] */
    float gain[2];  /* 2x1 */
}kalman2; 
void kal2_init(kalman2 *kal,float *init_x, float (*init_p)[2]);
float kal2_filter(kalman2 *kal,float new_data);
	
	
/*
 * moving average filter  
 */     
typedef struct
{
	uint8 len;
	int16 *buf;
	uint8 cnt;
	uint8 pos;
}maf_t;

void maf_init(maf_t *ft, int16 *buf, uint8 len);
int16 maf_filter(maf_t *ft, int16 new_data);
void maf_clear(maf_t *ft);

float filter_1st (float old_val,int16 new_val, uint8 ft_cnt);

#ifdef __cplusplus
}
#endif
    
#endif  /*_FILTER_H*/

