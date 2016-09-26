/**************************************************************************************************
  Filename:     my_type.h
  Versions:		v1.0		  
  Description:  type redefine
**************************************************************************************************/

#ifndef _MY_TYPE_H_
#define _MY_TYPE_H_

/* ------------------------------------------------------------------------------------------------
 *                                               Types
 * ------------------------------------------------------------------------------------------------
 */
typedef signed   char   int8;
typedef unsigned char   uint8;

//typedef signed   long   int16;
//typedef unsigned long   uint16;

//typedef signed   long long   int32;
//typedef unsigned long long   uint32;

typedef signed   short  int16;
typedef unsigned short  uint16;

typedef signed   long   int32;
typedef unsigned long   uint32;

/* ------------------------------------------------------------------------------------------------
 *                                        Standard Defines
 * ------------------------------------------------------------------------------------------------
 */
#ifndef NULL
#define NULL 0
#endif

#ifndef BOOL
typedef enum
{
  FALSE = 0,
  TRUE = !FALSE
}BOOL;
#endif

/*
typedef enum {
  DISABLE = 0,
  ENABLE = !DISABLE
}func_state;



typedef enum {
  ERROR = 0,
  SUCCESS = !ERROR
}err_status;
*/

/* ------------------------------------------------------------------------------------------------
 *                                       Memory Attributes
 * ------------------------------------------------------------------------------------------------
 */

/* ----------- IAR Compiler ----------- */
#ifdef __IAR_SYSTEMS_ICC__
#define  CODE   __code
#define  XDATA  __xdata

/* ----------- GNU Compiler ----------- */
#elif defined __KEIL__
#define  CODE   code
#define  XDATA  xdata
#define  IDATA	idata

/* ----------- Unrecognized Compiler ----------- */
#else
//#error "ERROR: Unknown compiler."
#endif

#endif