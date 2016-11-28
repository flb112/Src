/***********************************************************************
* �ļ�����	canbus.C
* �桡����	v1.0
* ��  ����	canbusЭ��ջ
* �����ˣ�	huangxinglong
* �������ڣ�2010-8-19 10:59

***********************************************************************/
/***********************************************************************/
#define		__CANBUS_C__
#include 	"_Include.h"
/***********************************************************************/
#define ATTR_RW 1


#define UM_ENTRY(MainCommands,SlaveCommands,Attribute,Variable) {MainCommands,SlaveCommands,Attribute,(int16u*)&Variable}
//**********************************Ӧ�ò�����**************************************/
const UM_TABLE um_Param1[]={
	UM_ENTRY(0x02,0x01,2,DeviceFir_Version),		//�豸�̼��汾��
	UM_ENTRY(0x02,0x02,4,DeviceFir_Number[0]),		//�豸SN��
    UM_ENTRY(0x02,0x03,2,Acqu_Cycle),		        //�������ɼ�����
    UM_ENTRY(0x02,0x04,2,NODE_ID),		            //�豸��ַ �ڵ�ID 
    UM_ENTRY(0x02,0x05,2,CO_BitRate),		        //CAN������
    UM_ENTRY(0x02,0x06,2,DeviceFir_Type),		    //�豸����
//    UM_ENTRY(0x02,0x07,2,RdStart),		            //������
//    UM_ENTRY(0x02,0x08,2,MappingRange),		        //��������    
    UM_ENTRY(0x02,0x09,4,time_buf[0]),		        //У׼ʱ���  
//    UM_ENTRY(0x02,0x0a,2,TestMod),                  //����ģʽ    
    
};






//0001: ��ȡX������  1
//0010: ��ȡY������  2
//0011: ��ȡZ������  3
//0100: ��ȡ�������� 4
//0101: ��ȡ��ѹ���� 5
//0110: ��ȡ�¶����� 6
//0111: ��ȡʪ������ 7

//������ 1 �������б�
const UM_TABLE um_Param2[]={
	UM_ENTRY(0x01,0x01,2,AlldataBuf[0]),		//x
	UM_ENTRY(0x01,0x02,2,AlldataBuf[1]),		//y
    UM_ENTRY(0x01,0x03,2,AlldataBuf[2]),		//z
    UM_ENTRY(0x01,0x04,6,AlldataBuf[0]),		//all     
};

const	int16u	umParamNum1 = sizeof(um_Param1) / sizeof(um_Param1[0]);		//��������
const	int16u	umParamNum2 = sizeof(um_Param2) / sizeof(um_Param2[0]);		//��������




/***********************************************************************/
// *���ܣ�
// *��ڣ�
// *���ڣ�
// *��ע��
// *�������ʣ�
/***********************************************************************/
void Sub_ChangeStore (int8u* des , int8u length) //reentrant
{
	int8u i,j;
	int8u temp[8];


	j=0;
	for(i=length; i>0; i--)
	{
		*(temp+j)=*(des+i-1);
		j++;
	}
	for(i=0; i<length; i++)
	{
		*(des+i) = *(temp+i);
	}
//	memcpy(des, (void*)temp,length);
}
/***********************************************************************/
// *���ܣ�������� �ظ�֡
// *��ڣ�
// *���ڣ�
// *��ע��
// *�������ʣ�
/***********************************************************************/
void Co_DataWrite_Return(int16u *databuf,int16u *index,int8u DataType,int16u StdId,int8u num)
{          int8u i,j,k;
           int8u buf[5];
  
            i = *index;
		     do
		    {
			     if(++(*index)> (CO_RXCAN_NUM_MSGS-1)) 
                 {   *index = 0;
                    i = *index;
                 }
			    if(Co_TxCan[*index].NewMsg==0) break;
		    }
            while(*index != i); 
            
            j=num; 
            k=0;
            buf[0]=0;
            buf[1]=0;
            buf[2]=0;
            buf[3]=0;
            buf[4]=0;            
            while(j!=0)
            {  j--;
               buf[k++]=*databuf; 
               if(j==0)
               {
                  break;
               }
               buf[k++]=(*databuf>>8);
               databuf++; 
               j--;
            }
            
            
            Co_TxCan[*index].StdId=StdId;        
            Co_TxCan[*index].Data[HEADMSG_FRAMSEQUENC]=0;          
            Co_TxCan[*index].NewMsg=1;//�������ͱ�־    //     ��֡                    �ɹ�       ������� 
            Co_TxCan[*index].Data[HEADMSG_FRAMSEQUENC]= (HEADMSG_FRAMSEQUENCE_SINGLE|SETSUCCESS|DataType)   ; //��Ϣ�^ 
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATALENGTH_L]=   num ;    //���ݳ���L
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATA3]=    0  ;    //���ݳ���H
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATA4]=   buf[0];    //
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATA5]=   buf[1];    //      
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATA6]=   buf[2] ;    //
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATA7]=   buf[3] ;    //
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATA8]=   buf[4];// 

}
/***********************************************************************/
// *���ܣ���Ч������������
// *��ڣ�
// *���ڣ�
//                  
// *��ע��
//                  
//                  
// *�������ʣ�
/***********************************************************************/
void Co_DataPacket_Error(int16u *index,int8u DataType,int16u StdId)
{

            Co_TxCan[*index].StdId=StdId;        
            Co_TxCan[*index].Data[HEADMSG_FRAMSEQUENC]=0;          
            Co_TxCan[*index].NewMsg=1;//�������ͱ�־    //     ��֡                    ʧ��       ������� 
            Co_TxCan[*index].Data[HEADMSG_FRAMSEQUENC]= (HEADMSG_FRAMSEQUENCE_SINGLE|SETFAIL|DataType)   ; //��Ϣ�^ 
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATALENGTH_L]=   5 ;    //���ݳ���L
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATA3]=   0  ;    //���ݳ���H
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATA4]=   0xff;    //
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATA5]=   0xff;    //      
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATA6]=   0 ;    //
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATA7]=   0 ;    //
            Co_TxCan[*index].Data[SLAVECOMMANDS_DATA8]=   0;// 

}


void Co_DataWrite_Proc(int16u *databuf,int16u *txc_index,int8u DataType,int16u StdId,uint8_t* Data)
{
     int16u temp=0;
     float temp1=0;
        switch(DataType)
        {
          
           case 0:break;
           case 1:
              temp=0;
              temp=(temp|Data[3])<<8;
              temp=(temp|Data[2]);   
              *databuf = temp;           
              Co_DataWrite_Return((databuf),txc_index,DataType,StdId,2);             
              Flash_SaveUserPara();  
              break;
              
           case 2: 
              *databuf = (*databuf & 0x0000);
              
               *databuf = (*databuf | Data[3])<<8;    //�豸SN��
               *databuf = (*databuf | Data[2]);
               databuf++;
               *databuf = (*databuf & 0x0000);
               
               *databuf = (*databuf | Data[5])<<8;    //�豸SN��
               *databuf = (*databuf | Data[4]);             
                databuf--;
              
              Co_DataWrite_Return((databuf),txc_index,DataType,StdId,4);             
              Flash_SaveUserPara();
              break;
           case 3:                                //��������������
              
              
              Co_DataPacket_Error(txc_index,DataType,StdId);
              break;
              
           case 4: 
              temp=0;
              temp=(temp|Data[3])<<8;
              temp=(temp|Data[2]);
              if(temp==0 || temp>127 )
              {
                   Co_DataPacket_Error(txc_index,DataType,StdId); 
                   break;
              }
              *databuf = temp;                  //�豸��ַ �ڵ�ID 
              Co_DataWrite_Return(databuf,txc_index,DataType,StdId,2);                
              Flash_SaveUserPara();
              break;
                
           case 5:           
              temp=0;
              temp=(temp|Data[3])<<8;
              temp=(temp|Data[2]);
              if(temp<1 || temp>3 )                            //CAN������ 
              {
                   Co_DataPacket_Error(txc_index,DataType,StdId); 
                   break;
              }
              *databuf = temp;

              Co_DataWrite_Return(databuf,txc_index,DataType,StdId,2);  
              Can_BaudSetGet(databuf);
              Flash_SaveUserPara();break;
              
              case 6:  //�豸����
              temp=0;
              temp=(temp|Data[3])<<8;
              temp=(temp|Data[2]);
              if(temp<1 || temp>2 )
              {
                   Co_DataPacket_Error(txc_index,DataType,StdId); 
                   break;
              }
              *databuf = temp;           
              Co_DataWrite_Return((databuf),txc_index,DataType,StdId,2);             
              Flash_SaveUserPara();               
              break;
              case 7:   //��������

              temp=0;
              temp=(temp|Data[3])<<8;
              temp=(temp|Data[2]);
              if(temp!=1)
              {
                   Co_DataPacket_Error(txc_index,DataType,StdId); 
                   break;
              }
              *databuf = temp;                

              Co_DataWrite_Return((databuf),txc_index,DataType,StdId,2);                            
              break;
              
              case 8: //��������

              /*if(Select_g_Range(Data[2]))
              {
                   Co_DataWrite_Return((databuf),txc_index,DataType,StdId,2);                
              }
              else
              {

                   Co_DataPacket_Error(txc_index,DataType,StdId); 
                   break; 
              
              }
 /*             *databuf = (*databuf & 0x0000);  
              *databuf = (*databuf | Data[3])<<8;
              *databuf = (*databuf | Data[2]); */                         
              Flash_SaveUserPara();               
              break;              
              case 9: //ʱ���У׼

              *databuf = (*databuf & 0x0000);
              *databuf = (*databuf | Data[3])<<8;
              *databuf = (*databuf | Data[2]); 
              timestamp=0;
              timestamp=*databuf;
              
              databuf++;
              *databuf = (*databuf & 0x0000);
              *databuf = (*databuf | Data[5])<<8;
              *databuf = (*databuf | Data[4]);                
              
              timestamp=timestamp|((int32u)*databuf)<<16; 
              databuf--;
              Co_DataWrite_Return((databuf),txc_index,DataType,StdId,4);              
              break;   
              
           case 0x0a:           
              temp=0;
              temp=(temp|Data[3])<<8;
              temp=(temp|Data[2]);
              if(temp>1 || temp<0 )                            //����ģʽ 
              {
                   Co_DataPacket_Error(txc_index,DataType,StdId); 
                   break;
              }
              *databuf = temp;
              //kx122_init();
              Co_DataWrite_Return(databuf,txc_index,DataType,StdId,2);  
              break;              
        
             default:
                Co_DataPacket_Error(txc_index,DataType,StdId); 
              break;


    }   
}


/***********************************************************************/
// *���ܣ�CANBUS ���ݰ�����������֡����
// *��ڣ�databuf �ش��Ļ��� , txc_index���ݰ��洢��λ��ָ��    DataType�������   StdId ��׼֡ID  timerenbʱ���ʹ��
// *���ڣ�
//                  
// *��ע��
//                  
//                  
// *�������ʣ�
/***********************************************************************/
void Co_DataPacket_Single(int16u *databuf,int16u *txc_index,int8u DataType,int16u StdId,int8u timerenb,int8u bsum)
{
            int8u k=0;
            int8u buf[8]={0};


                   Co_TxCan[*txc_index].StdId=StdId;
        
                   Co_TxCan[*txc_index].Data[HEADMSG_FRAMSEQUENC]=0;          
                   Co_TxCan[*txc_index].NewMsg=1;//�������ͱ�־    //     ��һ֡                            ʱ���          ������� 
//                   Co_TxCan[*txc_index].Data[HEADMSG_FRAMSEQUENC]= (HEADMSG_FRAMSEQUENCE_FIRST|SUBCOMMANDS_WORD_TIMERSTAMP|DataType)   ; //��Ϣ�^ 
                   Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATALENGTH_L]=   12 ;    //���ݳ���L
                   Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATALENGTH_H]=   0  ;    //���ݳ���H
                   if(timerenb)
                   { 
                     Co_TxCan[*txc_index].Data[HEADMSG_FRAMSEQUENC]= (HEADMSG_FRAMSEQUENCE_FIRST|SUBCOMMANDS_WORD_TIMERSTAMP|DataType)   ; //��Ϣ�^                      
                     Co_TxCan[*txc_index].Data[SLAVECOMMANDS_TIMERSTAMP_L]=   time_buf[0] ;    //ʱ���L
                     Co_TxCan[*txc_index].Data[SLAVECOMMANDS_TIMERSTAMP_L1]=  time_buf[1] ;    //ʱ���L0            
                     Co_TxCan[*txc_index].Data[SLAVECOMMANDS_TIMERSTAMP_H]=   time_buf[2] ;    //ʱ���H
                     Co_TxCan[*txc_index].Data[SLAVECOMMANDS_TIMERSTAMP_H1]=  time_buf[3] ;    //ʱ���H0
                   }
                   else
                   { 
                     Co_TxCan[*txc_index].Data[HEADMSG_FRAMSEQUENC]= (HEADMSG_FRAMSEQUENCE_FIRST|DataType)   ; //��Ϣ�^ 
                     Co_TxCan[*txc_index].Data[SLAVECOMMANDS_TIMERSTAMP_L]=   0 ;    //ʱ���L
                     Co_TxCan[*txc_index].Data[SLAVECOMMANDS_TIMERSTAMP_L1]=  0 ;    //ʱ���L0            
                     Co_TxCan[*txc_index].Data[SLAVECOMMANDS_TIMERSTAMP_H]=   0 ;    //ʱ���H
                     Co_TxCan[*txc_index].Data[SLAVECOMMANDS_TIMERSTAMP_H1]=  0 ;    //ʱ���H0                  
                   
                   }
                   Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA1]       =   0 ;//ʱ���H0 
                   
                   (*txc_index)++;
                   Co_TxCan[*txc_index].StdId=StdId;
                   Co_TxCan[*txc_index].Data[HEADMSG_FRAMSEQUENC]=0;          
                   Co_TxCan[*txc_index].NewMsg=1;//�������ͱ�־    //     ���һ֡             �������             
                   Co_TxCan[*txc_index].Data[HEADMSG_FRAMSEQUENC]= (HEADMSG_FRAMSEQUENCE_LAST|DataType)   ; //��Ϣ�^               
                   buf[0]=0;buf[1]=0;buf[2]=0;buf[3]=0;buf[4]=0;buf[5]=0;buf[6]=0;buf[7]=0;
            
                   while(bsum!=0)
                  {  bsum--;
                     buf[k++]=*databuf; 
                     if(bsum==0)
                     {
                        break;
                     }
                     buf[k++]=(*databuf>>8);
                     databuf++; 
                     bsum--;
                 }
                 Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA2]=  buf[0];    //
                 Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA3]=  buf[1];    //
                 Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA4]=  buf[2];    //
                 Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA5]=  buf[3];    //      
                 Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA6]=  buf[4];    //
                 Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA7]=  buf[5];    //
                 Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA8]=  buf[6];//                   
                          
}




/***********************************************************************/
// *���ܣ�CANBUS ���ݰ����������֡����
// *��ڣ�databuf �ش��Ļ��� , txc_index���ݰ��洢��λ��ָ��    DataType�������   StdId ��׼֡ID  timerenbʱ���ʹ��
// *���ڣ�
//                  
// *��ע��
//                  
//                  
// *�������ʣ�
/***********************************************************************/
void Co_DataPacket_Many(int16u *databuf,int16u *txc_index,int8u DataType,int16u StdId,int8u timerenb)

{         int16u i;
           
            Co_TxCan[*txc_index].StdId=StdId;  //1154
        
            Co_TxCan[*txc_index].Data[HEADMSG_FRAMSEQUENC]=0;          
            Co_TxCan[*txc_index].NewMsg=1;//�������ͱ�־    //     ��һ֡                            ʱ���          ������� 
            Co_TxCan[*txc_index].Data[HEADMSG_FRAMSEQUENC]= (HEADMSG_FRAMSEQUENCE_FIRST|SUBCOMMANDS_WORD_TIMERSTAMP|DataType)   ; //��Ϣ�^ 
            Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATALENGTH_L]=   0x81 ;    //���ݳ���L
            Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATALENGTH_H]=   0x41  ;    //���ݳ���H   2309������   5376+5
            Co_TxCan[*txc_index].Data[SLAVECOMMANDS_TIMERSTAMP_L]=   time_buf[0] ;    //ʱ���L
            Co_TxCan[*txc_index].Data[SLAVECOMMANDS_TIMERSTAMP_L1]=  time_buf[1] ;    //ʱ���L0            
            Co_TxCan[*txc_index].Data[SLAVECOMMANDS_TIMERSTAMP_H]=   time_buf[2] ;    //ʱ���H
            Co_TxCan[*txc_index].Data[SLAVECOMMANDS_TIMERSTAMP_H1]=  time_buf[3] ;    //ʱ���H0
            Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA1]       =   0;//����1L   
            
//            RdReturn=1;
            
   /*                      
            (*txc_index)++;        
          for((*txc_index);(*txc_index)<(raw_data_size/7);(*txc_index)++)  //2560/768=3*768=2304             5376/7=768
            {                 
                
                Co_TxCan[*txc_index].StdId=StdId;
                Co_TxCan[*txc_index].Data[HEADMSG_FRAMSEQUENC]=0;          
                Co_TxCan[*txc_index].NewMsg=1;//�������ͱ�־    //     �м�֡            �������             
                Co_TxCan[*txc_index].Data[HEADMSG_FRAMSEQUENC]= (HEADMSG_FRAMSEQUENCE_MIDDLE|DataType)   ; //��Ϣ�^ 
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA2]=   raw_data_buf[i++];    //����1L
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA3]=   raw_data_buf[i++];    //����1H
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA4]=   raw_data_buf[i++];    //����2L
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA5]=   raw_data_buf[i++];    //����2H            
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA6]=   raw_data_buf[i++];    //����3L
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA7]=   raw_data_buf[i++];    //����3H
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA8]=   raw_data_buf[i++];    //����4L    2303               
                
            }          
 //             (*txc_index)++;
                Co_TxCan[*txc_index].StdId=StdId;
                Co_TxCan[*txc_index].Data[HEADMSG_FRAMSEQUENC]=0;          
                Co_TxCan[*txc_index].NewMsg=1;//�������ͱ�־    //     ���һ֡             �������             
                Co_TxCan[*txc_index].Data[HEADMSG_FRAMSEQUENC]= (HEADMSG_FRAMSEQUENCE_LAST|DataType)   ; //��Ϣ�^ 
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA2]=   raw_data_buf[i++];    //����    2304
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA3]=   raw_data_buf[i++];    //����2L
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA4]=   raw_data_buf[i++];    //����2H
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA5]=   raw_data_buf[i++] ;    //����3L            
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA6]=   raw_data_buf[i++] ;    //����3H
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA7]=   raw_data_buf[i++] ;    //����4L
                Co_TxCan[*txc_index].Data[SLAVECOMMANDS_DATA8]=   raw_data_buf[i++] ;    //����4H               

*/      
}










/***********************************************************************/
// *���ܣ�CANBUS ���ݰ���֡����
// *��ڣ�databuf �ش��Ļ��� , txc_index���ݰ��洢��λ��ָ��    DataType�������   StdId ��׼֡ID  timerenbʱ���ʹ��
// *���ڣ�
//                  
// *��ע��//��Ϣ�^  BIT 7:6--->11��   ��֡        01��  ��һ֡     10��    �м�֡   11�� ���֡
//                  BIT 5:4--->00��   ��ʱ���    01��  ��ʱ���  
//                  BIT 3:0--->0001�� X��       0010��  Y��        0011��  Z��
// *�������ʣ�
/***********************************************************************/
void Co_DataPacket_Proc(int16u *databuf,int16u *txc_index,int8u DataType,int16u StdId,int8u timerenb,int8u bsum)
{
            int16u i;
//        int16u  p_data;
    
           i = *txc_index;
		   do
		   { 
			  if(++(*txc_index)> (CO_RXCAN_NUM_MSGS-10)) 
              {   *txc_index = 0;
                i = *txc_index;
              }
			  if(Co_TxCan[*txc_index].NewMsg==0) break;
		    }
            while(*txc_index != i);                          
            switch(DataType)
            {
            
                case 0:
                     break;  
            
                case 1:  //0001: ��ȡX��Ƕ�  1

                case 2:  // 0010: ��ȡY��Ƕ� 2
                     
                case 3:  //0011: ��ȡZ��Ƕ�  3

                case 4:  //0100: ��ȡ����Ƕ� 4
                     
                case 5:  //0101: ��ȡ��ѹ���� 5
                                  
                case 6:  //0110: ��ȡ�¶����� 6

                case 7:  //0111: ��ȡʪ������ 7
                      Co_DataPacket_Single(databuf,txc_index,DataType,StdId,timerenb,bsum);                     
                     break;
                     
                case 8:   //1000  ��ȡ�ֺ�բ�������� 8
                     //Co_DataPacket_Error(txc_index,DataType,StdId); 
                     Co_DataPacket_Many(databuf,txc_index,DataType,StdId,timerenb);
                     break; 

				 default:
                     
                     Co_DataPacket_Error(txc_index,DataType,StdId); 
					 break; 
                     
            
             }
}            


/***********************************************************************/
// *���ܣ�CANBUS ���ݰ���֡����
// *��ڣ�databuf �ش��Ļ��� , txc_index���ݰ��洢��λ��ָ��    DataType�������   StdId ��׼֡ID  timerenbʱ���ʹ��
// *���ڣ�
//                  
// *��ע��//��Ϣ�^  BIT 7:6--->11��   ��֡        01��  ��һ֡     10��    �м�֡   11�� ���֡
//                  BIT 5:4--->00��   ��ʱ���    01��  ��ʱ���  
//                  BIT 3:0--->0001�� X��       0010��  Y��        0011��  Z��
// *�������ʣ�
/***********************************************************************/
void Co_DataPacket_Proc1(int16u *databuf,int16u *txc_index,int8u DataType,int16u StdId,int8u timerenb,int8u bsum)
{
            int16u i;
    
           i = *txc_index;
		   do
		   { 
			  if(++(*txc_index)> (CO_RXCAN_NUM_MSGS-10)) 
              {   *txc_index = 0;
                i = *txc_index;
              }
			  if(Co_TxCan[*txc_index].NewMsg==0) break;
		    }
            while(*txc_index != i);                          
            switch(DataType)
            {
            
                case 0:
                     break;  
            
                case 1:  //0001: 

                case 2:  //0010: 
                     
                case 3:  //0011: 

                case 4:  //0100: 
                     
                case 5:  //0101: 
                                  
                case 6:  //0110: 

                case 7:  //0111: 
                case 8:  
                case 9:  
                      Co_DataPacket_Single(databuf,txc_index,DataType,StdId,timerenb,bsum);                     
                     break;

				 default:
                     
                     Co_DataPacket_Error(txc_index,DataType,StdId); 
					 break; 
                     
            
             }
}            
























/***********************************************************************/
// *���ܣ�ʱ����ж�
// *��ڣ����ջ���ͨ��
// *���ڣ�
//                  
// *��ע��
// *�������ʣ�
/***********************************************************************/
int8u TimerStamp_Start(int8u channel)
{
    if((Co_RxCan[channel].Data[SUBCOMMANDS_BUFPOSI]&0x10)==SUBCOMMANDS_WORD_TIMERSTAMP)// ʱ���
    {
       return 1;
    }
    else
    {
       return 0;
    }
}
/***********************************************************************/
// *���ܣ�CANBUS ����Э��ջ����
// *��ڣ�
// *���ڣ�
//                  
// *��ע��
// *�������ʣ�
/***********************************************************************/

void Rx_CANBUS_Proc (void)
{
	int8u i,j;

	static	int8u SDOserverChannel = 0;


	{
        i = SDOserverChannel;
		do
		{
			if(++SDOserverChannel == CO_RXCAN_NUM_MSGS) SDOserverChannel = 0;
			if(Co_RxCan[SDOserverChannel].NewMsg==1) break;
		}
		while(SDOserverChannel != i);

        if((Co_RxCan[SDOserverChannel].NewMsg==1))                          //��ѯ���ջ�����
		{


          
 			switch(Co_RxCan[SDOserverChannel].Data[MAINCOMMANDS_BUFPOSI])//���������
 			{
				
                   case MAINCOMMANDS_RD:           //��ȡ��̬����
                   for(j=0;j<umParamNum2;j++)
                   {
                      if(um_Param2[j].slavecommands==(Co_RxCan[SDOserverChannel].Data[SUBCOMMANDS_BUFPOSI]&0x0f))    //�������ֽ���
                      {
                         
                         Co_DataPacket_Proc(um_Param2[j].addr,&TXC_INDEX,um_Param2[j].slavecommands,Co_RxCan[SDOserverChannel].StdId,TimerStamp_Start(SDOserverChannel),um_Param2[j].attribute);
                                            //������ַ       �����ͻ���λ��   ��      ��������������  ��   ֡ID
                         break;
                      }
                   
                   }
                   
                   break;                
                
                  case MAINCOMMANDS_MACHINEPARAMETER://02�豸������д
                      
                   for(j=0;j<umParamNum1;j++)
                   {
                      if((Co_RxCan[SDOserverChannel].Data[SUBCOMMANDS_BUFPOSI]&0x80))//д������------->����
                      {
                         if(um_Param1[j].slavecommands==(Co_RxCan[SDOserverChannel].Data[SUBCOMMANDS_BUFPOSI]&0x7f))//�������ֽ���
                         {
                            //Co_DataWrite_Proc(um_Param1[j].addr,&TXC_INDEX,um_Param1[j].slavecommands,Co_RxCan[SDOserverChannel].StdId); //�������ò�������
                           Co_DataWrite_Proc(um_Param1[j].addr,&TXC_INDEX,um_Param1[j].slavecommands,Co_RxCan[SDOserverChannel].StdId,Co_RxCan[SDOserverChannel].Data);
                                            //������ַ       �����ͻ���λ��   ��      ��������������  ��   ֡ID
                            break;
                         }                                                                                
                      
                         
                      }
                      else                                                                                          //д������------->�xȡ
                      {
                         if(um_Param1[j].slavecommands==(Co_RxCan[SDOserverChannel].Data[SUBCOMMANDS_BUFPOSI]&0x7f))//�������ֽ���
                         {
                            Co_DataPacket_Proc1(um_Param1[j].addr,&TXC_INDEX,um_Param1[j].slavecommands,Co_RxCan[SDOserverChannel].StdId,TimerStamp_Start(SDOserverChannel),um_Param1[j].attribute); //��ȡ��������
                                            //������ַ       �����ͻ���λ��   ��      ��������������  ��   ֡ID
                            break;
                         }
                      }
                   }
                   break;
                   
                   case MAINCOMMANDS_RST:    //ϵͳ��λ
                  
                         System_Reset();
                         break;
                
                  
                     
				 default:
						break; //command specifier not valid
			}
			Co_RxCan[SDOserverChannel].NewMsg = 0;//release
		}
	}

}



/***********************************************************************/
// *���ܣ�CANBUS ����
// *��ڣ�
// *���ڣ�
//                  
// *��ע��
// *�������ʣ�
/***********************************************************************/

void Tx_CANBUS_Proc (void)
{
        u16 i=0;
        static u16 TXChannel=0;
        static u16 Channel=0; 
        i = TXChannel;
		do
		{
			if(++TXChannel == CO_TXCAN_NUM_MSGS) TXChannel = 0;
			if(Co_TxCan[TXChannel].NewMsg==1) break;
		}
		while(TXChannel != i);
        {
            if(Co_TxCan[TXChannel].NewMsg==1)
            {  

               if(!Can_Send_Msg(&Co_TxCan[TXChannel]))
               {
                  Co_TxCan[TXChannel].NewMsg=0;                          //�ɹ����� 
                  Co_TxCan[TXChannel].StdId=0;
                  Co_TxCan[TXChannel].SendCount=0;
                  for(i=0;i<8;i++)
                  {
                      Co_TxCan[TXChannel].Data[i]=0;                    //�巢�ͻ���
                  }  
                  Channel++;
               }
               else
               {
                  if(++Co_TxCan[TXChannel].SendCount>SENDCNT)           //�ظ����͵Ĵ���  
                  {
                     Co_TxCan[TXChannel].SendCount=0;
                     Co_TxCan[TXChannel].NewMsg=0;                        
                  }
               }
            }
        }
                            
}





/***********************************************************************/
// *���ܣ������ڵ�ID����CANͨ�Ų�����
// *��ڣ�
// *���ڣ�
// *��ע��
// *�������ʣ�
/***********************************************************************/
void Co_Rd_NodeId_BitRate(void)
{	int8u temp;

/*	
	temp = 0;
	if(NODID1 == 1)	temp |= 0x01;
	if(NODID2 == 1)	temp |= 0x02;
	if(NODID3 == 1)	temp |= 0x04;
	if(NODID4 == 1)	temp |= 0x08;
	if(NODID5 == 1)	temp |= 0x10;
	if(NODID6 == 1)	temp |= 0x20;

	CO_NodeID = temp;    //range 1 to 127
//	CO_NodeID = 3;    //range 1 to 127
	
	temp = 0;
	if(BITRATE1 == 1)	temp |= 0x01;
	if(BITRATE2 == 1)	temp |= 0x02;
//	CO_BitRate = 1;  //range 0 to 7
		
	CO_BitRate = temp;  //range 0 to 7
                                    // 0 = 20 kbps    1 = 50 kbps
                                    // 2 = 100 kbps    3 = 125 kbps
                                    // 4 = 250 kbps   5 = 500 kbps
                                    // 6 = 800 kbps   7 = 1000 kbps

   if(CO_NodeID==0 || CO_NodeID>127 || CO_BitRate > 3)	//�ڵ�ID�벨���������Ƿ���ȷ
   {
      ErrorReport(ERROR_WrongNodeIDorBitRate, (CO_NodeID<<8)|CO_BitRate);
      CO_NodeID = 1;
      if(CO_BitRate > 3) CO_BitRate = 1;
      if(CO_NodeID ==0 ) CO_NodeID = 8;
   }*/
}

/***********************************************************************/
// *���ܣ����CAN��Ϣ������
// *��ڣ�
// *���ڣ�
// *��ע��
// *�������ʣ�
/***********************************************************************/
void Co_ClrCan (void)
{	int16u	i;


	for(i=0; i<sizeof(Co_RxCan); i++)
	{
		*(((int8u*)Co_RxCan)+i) = 0;		//���CAN��Ϣ���մ洢��
	}
	for(i=0; i<sizeof(Co_TxCan); i++)
	{
		*(((int8u*)Co_TxCan)+i) = 0;		//���CAN��Ϣ���ʹ洢��
	}
}



















