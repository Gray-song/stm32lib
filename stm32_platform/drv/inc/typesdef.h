/**
  ******************************************************************************
  * @file    typesdef.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _TYPESDEF_H_
#define _TYPESDEF_H_

#ifndef NULL
#define NULL  ((void *)0)
#endif


typedef void (*voidfunc_t)(void);
typedef unsigned char bool;
typedef enum __IO_ERR_T__
{
    IO_ERR_OK = 0,
    IO_ERR_PARAMETER_ERROR,
    IO_ERR_BUSY,
    IO_ERR_RUN_ERROR,
    IO_ERR_TIMEOUT,
    IO_ERR_OTHER_FAILURE  
}io_err_t; 
#define TRUE 1
#define FALSE 0
#define ENABLE_DEF     TRUE
#define DISABLE_DEF    FALSE
#endif
