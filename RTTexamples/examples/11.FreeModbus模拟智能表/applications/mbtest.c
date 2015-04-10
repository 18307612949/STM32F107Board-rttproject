/***************************2012-2013, NJUT, Edu.****************************** 
  FileName: mbtest.c 
  Author:  �ﶬ÷       Version :  1.0        Date: 2012.12.9
  Description:    freemodbus����  STM32F107���ӻ�
  Version:         1.0 
  Function List:    
    1. 
    2.
  History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/08/08     1.0     �ļ���ʽ�޶�   
******************************************************************************/ 
#include <rtthread.h>
#include "stm32f10x.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4

/* ----------------------- Static variables ---------------------------------*/

void rt_init_freemodbus_entry(void* parameter)
{
    eMBErrorCode    eStatus;

    eStatus = eMBInit( MB_RTU, 0x01, 2, 9600, MB_PAR_NONE );

    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );

    for( ;; )
    {
        ( void )eMBPoll(  );
        rt_thread_delay(1);

        /* . */

        
    }  
}
void test_modbus()
{
     rt_thread_t init_thread;

    init_thread = rt_thread_create("freemodbus",
                                   rt_init_freemodbus_entry, RT_NULL,
                                   0x400, 9, 20);

    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);
}

#ifdef  RT_USING_FINSH 
#include  <finsh.h> 
/*  ���testmodbus������finsh  shell�� */ 
FINSH_FUNCTION_EXPORT (test_modbus, freemodbus RTUģʽ �ӻ���ַΪ1 Uart2 9600  ��У�� e.g.:test_modbus()); 

#endif 
  
