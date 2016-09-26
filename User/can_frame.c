


#include "can_frame.h"
//#include <stdlib.h>

/*
#define first_frame 0x40				//��һ֡��־
#define inbetweens 0x80				//�м�֡��־
#define last_frame 0xC0				//���֡��־
*/
enum frame { 
             first_frame=0x40,
             inbetweens=0x80,
             last_frame=0xC0
            };

/*
#define X_axis 0x01					//λ3~0,01Ϊ��ȡX������,λ4Ϊ0,����ʱ���
#define Y_axis 0x02					//λ3~0,01Ϊ��ȡX������,λ4Ϊ1,��Ҫʱ���
#define Z_axis 0x04					//λ3~0,10Ϊ��ȡY������,λ4Ϊ0,����ʱ���
#define XYZ_axis 0x07
#define X_time 0x11					//λ3~0,10Ϊ��ȡY������,λ4Ϊ1,��Ҫʱ���
#define Y_time 0x12					//λ3~0,11Ϊ��ȡZ������,λ4Ϊ0,����ʱ���
#define Z_time 0x14					//λ3~0,11Ϊ��ȡZ������,λ4Ϊ1,��Ҫʱ���
#define XYZ_time 0x17
*/
enum axis {
            X_axis=0x01,
            Y_axis=0x02,
            Z_axis=0x04,
            XYZ_axis=0x07,
            X_time=0x11,
            Y_time=0x12,
            Z_time=0x14,
            XYZ_time=0x17
           };
 
enum parameter {
                  FR=1,
                  SN,
                  C_time,
                  D_add,
                  C_bps
                };



char frame_RXD_flag=0;						//�ж��Ƿ���Ҫ��ͷ֡����ȡ��Ҫ���͵���Ϣ
char frame_TXD_flag=0;						//�ж�����������
char frame_TXD_head=0;						//
char TXD_head_buf[9]="0";
int buf_length=0;

/********************************************************
��������int can_frame(int frame_flag ,char* buf)
�������ܣ��жϲ�����CAN�շ�����
�������ܣ�
	�βΣ�
		char* buf��һ֡����֡��64λ
		int frame_flag���ж���������ù��ܱ�־λ��ֵ��0Ϊ����ֵ0Ϊ��
	ȫ�ֱ�����
		frame_RXD_flag�������ж��Ƿ�Ϊͷ֡��ֵ1Ϊͷ֡
	�ֲ�������
		value_flag��������ú�������ֵ
��������ֵ���ܣ�
	-1���������д���
	0��ȱʡ����
	1����ȡX��
	2����ȡX���ʱ���
	3����ȡY��
	4����ȡY���ʱ���
	5����ȡZ��
	6����ȡZ���ʱ���
�޸����ڣ�2016.03.25
���ߣ�������
**********************************************************/
int can_frame(int frame_flag ,int* buf)
{
	int value_flag=0;
	
	if(frame_flag)							//�ж���������ù��ܣ���0Ϊ���÷���0Ϊ������
	{
		value_flag = can_frame_TXD(length,tx_t_buf);		//���÷���������ȡ����ֵ
		return value_flag;
	}
	else
	{
		value_flag = can_frame_RXD(rx_buf);		//�����պ�������ȡ����ֵ
		frame_RXD_flag = value_flag;			//��ȡ��־λ
		return value_flag;
	}
	
	return -1;								//�������в�����������-1
}



/********************************************************
��������int can_frame_TXD(int buf_length ,char* buf)
�������ܣ��Զ���������֡�������м�֡��β֡
********************************************************/
int can_frame_TXD(int buf_length ,int time)
{
	int temp_flag=0;
    int temp_TXD_flag=0;
	int temp_length=buf_length;
	char TXD_temp_buf[65]="0";
	char temp_buf[9]="0";
	
	if(frame_RXD_flag)
	{
		temp_flag = can_head_TXD(buf_length,time);
		
		switch(temp_flag)
		{
			case -1: return -1; break;
			case  0: return   0; break;
			case  1: temp_length -= 1; break;
			default: return -1;
		}
	}
	
	while(temp_TXD_flag<=(temp_length-7))
	{
		temp_flag = 1;
		
		frame_TXD_head &= 0x3F;
		frame_TXD_head |= inbetweens;
		
		TXD_head_buf[0] = frame_TXD_head;
		while(temp_flag <7)
		{
            if(temp_flag == 1)
            {
              TXD_head_buf[temp_flag++] = data_buf[temp_TXD_flag++>>8];
              continue;
            }
			TXD_head_buf[temp_flag] = data_buf[temp_TXD_flag];
            TXD_head_buf[temp_flag+1] = (data_buf[temp_TXD_flag++]>>8);
			temp_flag += 2;
		}
		
		temp_flag = Can_Send_Msg(TXD_head_buf,8);
		if(temp_flag==0)
		{
			return -1;
		}
//		temp_length -= 7;
	}
	
	if((temp_length-temp_TXD_flag)>0)
	{
		frame_TXD_head &= 0x3F;
		frame_TXD_head |= last_frame;
		
		TXD_head_buf[0] = frame_TXD_head;
		for(temp_flag=1;(temp_length-temp_TXD_flag)>0;)
		{
			if(temp_flag == 1)
            {
              TXD_head_buf[temp_flag++] = data_buf[temp_TXD_flag++>>8];
              continue;
            }
			TXD_head_buf[temp_flag] = data_buf[temp_TXD_flag];
            TXD_head_buf[temp_flag+1] = (data_buf[temp_TXD_flag++]>>8);
			temp_flag += 2;
		}
		while(temp_flag<8)
		{
			TXD_head_buf[temp_flag] = '0';
			temp_flag++;
		}
		
		temp_flag = Can_Send_Msg(TXD_head_buf,8);
		if(temp_flag==0)
		{
			return -1;
		}
	}
	
	return 1;
}

/********************************************************
��������int can_frame_TXD(int buf_length)
�������ܣ��Զ���������֡��ͷ֡
�������ܣ�
	�βΣ�
********************************************************/
int can_head_TXD(int buf_length,int time)
{
	int temp_length=buf_length+4;
    char* temp_time=(char*)&time;
    char temp_head=0;
	
	if(buf_length > 1)
	{
		temp_head |= first_frame;
	}
	else if(buf_length <= 1)
	{
		temp_head |= last_frame;
	}
	
	switch(frame_RXD_flag)
	{
		case 1: temp_head |= X_axis; break;
		case 2: temp_head |= X_time; break;
		case 3: temp_head |= Y_axis; break;
		case 4: temp_head |= Y_time; break;
		case 5: temp_head |= Z_axis; break;
		case 6: temp_head |= Z_time; break;
		default: return 0;					//����ֵ0��ȱʡ����	
	}
	
	frame_RXD_flag = 0;
	frame_TXD_head = temp_head;
	TXD_head_buf[0] = temp_head;
	
	TXD_head_buf[1] |= temp_length;
	TXD_head_buf[2] |= (temp_length>>8);
	
	TXD_head_buf[3] |= *(temp_time+3);
	TXD_head_buf[4] |= *(temp_time+2);
	TXD_head_buf[5] |= *(temp_time+1);
	TXD_head_buf[6] |= *(temp_time+0);
	
	TXD_head_buf[7] |= (char)data_buf[0];
	
	temp_length = Can_Send_Msg(TXD_head_buf,8);
	if(temp_length)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
	return -1;
}
	
	
/********************************************************
��������int can_frame_RXD(char* buf)
�������ܣ���������Can���������Ӧ��ֵ
�������ܣ�
	�βΣ�
		char* buf��ͨ��Ϊ�����������͵�֡��64λ
	�ֲ�������
		can_temp_buf[9]����ȡ8λ���������ݵ��ַ���
		RXD_flag������whileѭ���ڱ���
��������ֵ���ܣ�
	0��ȱʡ����
	1����ȡX��
	2����ȡX���ʱ���
	3����ȡY��
	4����ȡY���ʱ���
	5����ȡZ��
	6����ȡZ���ʱ���
�޸����ڣ�2016.03.24
���ߣ�������
**********************************************************/
int can_frame_RXD(char* buf)
{
	char can_temp_buf[9]="00000000";			//��֤�������һλΪ"\0"
	char RXD_flag=0;
	
	while( (can_temp_buf[RXD_flag] != '\0') && (RXD_flag < 9) )		//�ж��ַ����鵱ǰλַ�Ƿ�Ϊĩβ.����,�����ѭ��
	{
		can_temp_buf[RXD_flag] = *(buf+RXD_flag);
										//ƫ��RXD_flagλ��ȡ�����ַ�����
		RXD_flag++;						//����,������һλ
	}
	
	if( can_temp_buf[0] == 0x01 )				//�ж��������01Ϊ��ȡ���������ݡ�
	{
		switch( can_temp_buf[1] )				//
		{
			case X_axis: return 1;	break;	
			case X_time: return 2;	break;	
			case Y_axis: return 3;	break;	
			case Y_time: return 4;	break;	
			case Z_axis: return 5;	break;	
			case Z_time: return 6;	break;	
			default: return 0;				//����ֵ0��ȱʡ����
		}
	}
	else if( can_temp_buf[0] == 0x02 )
	{
		switch( can_temp_buf[1] )				//
		{
			case X_axis: return 1;	break;	
			case X_time: return 2;	break;	
			case Y_axis: return 3;	break;	
			case Y_time: return 4;	break;	
			case Z_axis: return 5;	break;	
			case Z_time: return 6;	break;	
			default: return 0;				//����ֵ0��ȱʡ����
		}
	}

        return 0;								//����ֵ0��ȱʡ����
}


