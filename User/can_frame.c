


#include "can_frame.h"
//#include <stdlib.h>

/*
#define first_frame 0x40				//第一帧标志
#define inbetweens 0x80				//中间帧标志
#define last_frame 0xC0				//最后帧标志
*/
enum frame { 
             first_frame=0x40,
             inbetweens=0x80,
             last_frame=0xC0
            };

/*
#define X_axis 0x01					//位3~0,01为获取X轴数据,位4为0,无需时间戳
#define Y_axis 0x02					//位3~0,01为获取X轴数据,位4为1,需要时间戳
#define Z_axis 0x04					//位3~0,10为获取Y轴数据,位4为0,无需时间戳
#define XYZ_axis 0x07
#define X_time 0x11					//位3~0,10为获取Y轴数据,位4为1,需要时间戳
#define Y_time 0x12					//位3~0,11为获取Z轴数据,位4为0,无需时间戳
#define Z_time 0x14					//位3~0,11为获取Z轴数据,位4为1,需要时间戳
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



char frame_RXD_flag=0;						//判断是否需要发头帧跟获取需要发送的信息
char frame_TXD_flag=0;						//判断数据完整性
char frame_TXD_head=0;						//
char TXD_head_buf[9]="0";
int buf_length=0;

/********************************************************
函数名：int can_frame(int frame_flag ,char* buf)
函数功能：判断并调用CAN收发函数
变量功能：
	形参：
		char* buf：一帧数据帧，64位
		int frame_flag：判断主程序调用功能标志位，值非0为发，值0为收
	全局变量：
		frame_RXD_flag：用于判断是否为头帧，值1为头帧
	局部变量：
		value_flag：保存调用函数返回值
函数返回值功能：
	-1：函数运行错误
	0：缺省命令
	1：读取X轴
	2：读取X轴加时间戳
	3：读取Y轴
	4：读取Y轴加时间戳
	5：读取Z轴
	6：读取Z轴加时间戳
修改日期：2016.03.25
作者：沈意钊
**********************************************************/
int can_frame(int frame_flag ,int* buf)
{
	int value_flag=0;
	
	if(frame_flag)							//判断主程序调用功能，非0为调用发，0为调用收
	{
		value_flag = can_frame_TXD(length,tx_t_buf);		//调用发函数并获取返回值
		return value_flag;
	}
	else
	{
		value_flag = can_frame_RXD(rx_buf);		//调用收函数并获取返回值
		frame_RXD_flag = value_flag;			//获取标志位
		return value_flag;
	}
	
	return -1;								//程序运行不正常，返回-1
}



/********************************************************
函数名：int can_frame_TXD(int buf_length ,char* buf)
函数功能：自动构建数据帧，包括中间帧，尾帧
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
函数名：int can_frame_TXD(int buf_length)
函数功能：自动构建数据帧，头帧
变量功能：
	形参：
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
		default: return 0;					//返回值0：缺省命令	
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
函数名：int can_frame_RXD(char* buf)
函数功能：接收主机Can命令并返回相应的值
变量功能：
	形参：
		char* buf：通常为接收主机发送的帧，64位
	局部变量：
		can_temp_buf[9]：提取8位子命令数据的字符串
		RXD_flag：用作while循环内变量
函数返回值功能：
	0：缺省命令
	1：读取X轴
	2：读取X轴加时间戳
	3：读取Y轴
	4：读取Y轴加时间戳
	5：读取Z轴
	6：读取Z轴加时间戳
修改日期：2016.03.24
作者：沈意钊
**********************************************************/
int can_frame_RXD(char* buf)
{
	char can_temp_buf[9]="00000000";			//保证数组最后一位为"\0"
	char RXD_flag=0;
	
	while( (can_temp_buf[RXD_flag] != '\0') && (RXD_flag < 9) )		//判断字符数组当前位址是否为末尾.不是,则继续循环
	{
		can_temp_buf[RXD_flag] = *(buf+RXD_flag);
										//偏移RXD_flag位获取命令字符数据
		RXD_flag++;						//自增,跳到下一位
	}
	
	if( can_temp_buf[0] == 0x01 )				//判断主机命令：01为读取传感器数据。
	{
		switch( can_temp_buf[1] )				//
		{
			case X_axis: return 1;	break;	
			case X_time: return 2;	break;	
			case Y_axis: return 3;	break;	
			case Y_time: return 4;	break;	
			case Z_axis: return 5;	break;	
			case Z_time: return 6;	break;	
			default: return 0;				//返回值0：缺省命令
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
			default: return 0;				//返回值0：缺省命令
		}
	}

        return 0;								//返回值0：缺省命令
}


