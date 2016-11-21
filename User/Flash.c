//*****************文*件*信*息********************************************
// *文 件 名：Flash.c
// *版　  本：v1.0
// *创 建 人：孙娜
// *日　  期：：2013-12-17 16:12
// *描　  述：
// *
// *当前版本：v1.0
// *修 改 人：孙娜
// *日　  期：：2013-12-17 16:12
// *描　  述：
//************************************************************************
#define   _FLASH_C_
#include "_Include.h"
//-----------------------------------------------------------------------
#define     USER_ADDR    0x08007C00//用户参数118k start//每页2KB存储空间
#define     PASSWORD     0x1095 //
#define     DOTNUM       16   

int16u  CoderDot[DOTNUM];
//************************************************************************
// *功    能：保存用户参数
// *入    口：
// *出    口：
// *备    注：
// *函数性质：修改用户参数 保存 调用
//************************************************************************
void  Flash_SaveUserPara( void )
{
    int8u i;
    FLASH_Unlock() ;
    FLASH_ClearFlag( FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR ) ;
    FLASH_ErasePage( USER_ADDR ) ;
//------------------保存数据列表--------------------------------
    FLASH_ProgramHalfWord(USER_ADDR+0,DeviceFir_Number[0]);
    FLASH_ProgramHalfWord(USER_ADDR+2,DeviceFir_Number[1]);
//    FLASH_ProgramHalfWord(USER_ADDR+4,DeviceFir_Number[2]);
    
    FLASH_ProgramHalfWord(USER_ADDR+6,NODE_ID);
    FLASH_ProgramHalfWord(USER_ADDR+8,CO_BitRate);//
    FLASH_ProgramHalfWord(USER_ADDR+10,DeviceFir_Type);//    
//    FLASH_ProgramHalfWord(USER_ADDR+12,KxRange);//     
    
    FLASH_ProgramHalfWord(USER_ADDR+14,DeviceFir_Version);// MappingRange
//    FLASH_ProgramHalfWord(USER_ADDR+16,MappingRange);//
//    FLASH_ProgramHalfWord(USER_ADDR+18,TestMod);//

    FLASH_ProgramHalfWord(USER_ADDR+20,TempCalib[0]);//
    FLASH_ProgramHalfWord(USER_ADDR+22,TempCalib[1]);//
    FLASH_ProgramHalfWord(USER_ADDR+24,TempActual[0]);//
    FLASH_ProgramHalfWord(USER_ADDR+26,TempActual[1]);//
    FLASH_ProgramHalfWord(USER_ADDR+28,TempOffset);//

    
    
 /*   
	for(i=0;i<DOTNUM;i++)
	{
		FLASH_ProgramHalfWord(USER_ADDR+i*2,CoderDot[i]); 
	}*/
    FLASH_ProgramHalfWord( USER_ADDR+1020,PASSWORD); 	
    FLASH_Lock() ;	
}
//************************************************************************
// *功    能：读取用户参数
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//************************************************************************
void  Flash_ReadUserPara( void )
{
//	int8u i;
	if(*(int16u*)(USER_ADDR+1020)!= PASSWORD )
	{ 
        System_ParameterInit();  
		Flash_SaveUserPara();
	}
    else
    {
       DeviceFir_Number[0]=*(int16u*)(USER_ADDR+0) ;
       DeviceFir_Number[1]=*(int16u*)(USER_ADDR+2) ;
//       DeviceFir_Number[2]=*(int16u*)(USER_ADDR+4) ;
       
       NODE_ID         =*(int16u*)(USER_ADDR+6) ;
       CO_BitRate      =*(int16u*)(USER_ADDR+8) ;
       
       DeviceFir_Type  =*(int16u*)(USER_ADDR+10) ;
//       KxRange         =*(int16u*)(USER_ADDR+12) ;
       DeviceFir_Version=*(int16u*)(USER_ADDR+14) ;
//       MappingRange     =*(int16u*)(USER_ADDR+16) ;
//       TestMod          =*(int16u*)(USER_ADDR+18) ;
       TempCalib[0]     =*(int16u*)(USER_ADDR+20) ; 
       TempCalib[1]     =*(int16u*)(USER_ADDR+22) ;
       TempActual[0]    =*(int16u*)(USER_ADDR+24) ;
       TempActual[1]    =*(int16u*)(USER_ADDR+26) ;
       TempOffset       =*(int16u*)(USER_ADDR+28) ;

       
 //      AlldataBuf[0]=*(int16u*)(USER_ADDR+1020);
       
    }
/*	for(i=0;i<DOTNUM;i++)
	{
		CoderDot[i] = *(int16u*)(USER_ADDR+i*2) ;    	
	}
*/
}
//************************************************************************
//
//            E N D
//
//************************************************************************
