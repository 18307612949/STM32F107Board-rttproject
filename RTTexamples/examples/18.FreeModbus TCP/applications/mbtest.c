/***************************2012-2013, NJUT, Edu.****************************** 
  FileName: mbtest.c 
  Author:  �ﶬ÷       Version :  1.0        Date: 2014.10.30
  Description:    freemodbus TCP����  STM32F107���ӻ�
  Version:         1.0 
  Function List:    
    1. 
    2.
  History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/10/30     1.0     �ļ���ʽ�޶�   
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

    eStatus = eMBTCPInit( MB_TCP_PORT_USE_DEFAULT );

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
                                   0x800, 9, 20);

    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);
}

#ifdef  RT_USING_FINSH 
#include  <finsh.h> 
/*  ���testmodbus������finsh  shell�� */ 
FINSH_FUNCTION_EXPORT (test_modbus, freemodbus TCPģʽ �ӻ���ַ:192.168.1.30:502 e.g.:test_modbus()); 

#endif 
  
