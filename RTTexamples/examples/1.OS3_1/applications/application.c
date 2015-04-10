/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2013, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <board.h>
#include <rtthread.h>
#include <led.h>
#include <key.h>


#ifdef RT_USING_DFS
#include <dfs_fs.h>
#include <dfs_init.h>
#include <dfs_elm.h>
#endif

#ifdef RT_USING_LWIP
#include <stm32_eth.h>
#include <netif/ethernetif.h>
extern int lwip_system_init(void);
#endif

#ifdef RT_USING_FINSH
#include <shell.h>
#include <finsh.h>
#endif

struct rt_thread thread1; 
char thread1_stack[512]; 


static void rt_thread_entry_led(void* parameter)
{
  LED_Init();
  Key_Init();
  Set_Keyint();
  while(1)
  {
    LEDTog(LED1);
    LEDTog(LED2);
    LEDTog(LED3);
    LEDTog(LED4);
    rt_thread_delay(RT_TICK_PER_SECOND/2);
  }
}
static int led_tog_init(void)
{
    rt_thread_t init_thread;

    init_thread = rt_thread_create("led_tog",
                                   rt_thread_entry_led, RT_NULL,
                                   0x100, 0x1f, 1000);

    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);
    
    return 0;

}

//��ڳ��� 
void thread1_entry(void* parameter) 
{ 
 int i,j; 
  char *ptr[20]; /* ���ڷ���20�������ڴ���ָ��*/ 
 /* ��ָ������*/ 
  for (i = 0; i < 20; i ++)  
    ptr[i] = RT_NULL; 
  for(j = 0; j <2; j ++ )
 { 
    for (i = 0; i <20; i++) 
   { 
   /* ÿ�η���(1 <<i)��С�ֽ������ڴ�ռ�*/ 
      ptr[i] = rt_malloc(1 <<i); 
     /* �������ɹ�*/ 
      if (ptr[i] != RT_NULL) 
     { 
    rt_kprintf("get memory: 0x%x\n", 
ptr[i]); 
    /* �ͷ��ڴ��*/ 
    rt_free(ptr[i]); 
    ptr[i] = RT_NULL; 
     } 
   } 
 } 
} 

void rt_init_thread_entry(void* parameter)
{
    {
        extern void rt_platform_init(void);
        rt_platform_init();
    }

    /* Filesystem Initialization */
#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
	/* initialize the device file system */
	dfs_init();

	/* initialize the elm chan FatFS file system*/
	elm_init();
    
    /* mount sd card fat partition 1 as root directory */
    if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
    {
        rt_kprintf("File System initialized!\n");
    }
    else
    {
        rt_kprintf("File System initialzation failed!\n");
    }
#endif /* RT_USING_DFS && RT_USING_DFS_ELMFAT */

#ifdef RT_USING_LWIP
	/* initialize lwip stack */
	/* register ethernetif device */
	eth_system_device_init();

	/* initialize lwip system */
	lwip_system_init();
	rt_kprintf("TCP/IP initialized!\n");
#endif

#ifdef RT_USING_FINSH
	/* initialize finsh */
	finsh_system_init();
	finsh_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
        led_tog_init();
}

int rt_application_init(void)
{
    rt_thread_t tid;
    rt_err_t result;
    

    
    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);
    if (tid != RT_NULL) rt_thread_startup(tid);

    
    /* ��ʼ���߳�1 */
    result = rt_thread_init(&thread1, "t1", /* �߳�����t1 */
        thread1_entry, RT_NULL, /* �̵߳������thread1_entry����ڲ�����RT_NULL*/
        &thread1_stack[0], sizeof(thread1_stack), /* �߳�ջ��thread1_stack */
        6, 10);
    if (result == RT_EOK) /* ���������ȷ�������߳�1 */
        rt_thread_startup(&thread1);
    
    return 0;
}



/*@}*/
