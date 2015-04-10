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
/*
 * �����嵥����Ϣ��������
 *
 * �������ᴴ��3����̬�̣߳�һ���̻߳����Ϣ��������ȡ��Ϣ��һ���̻߳ᶨʱ����
 * Ϣ���з�����Ϣ��һ���̻߳ᶨʱ����Ϣ���з��ͽ�����Ϣ��
 */

#include <stdio.h>
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

#define MSG_VIP "over"
/* ��Ϣ���п��ƿ� */
static struct rt_messagequeue mq;
/* ��Ϣ�������õ��ķ�����Ϣ���ڴ�� */
static char msg_pool[2048];

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
struct rt_thread thread1;


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

/* �߳�1��ں��� */
static void thread1_entry(void* parameter)
{
    char buf[128];

    while (1)
    {
        rt_memset(&buf[0], 0, sizeof(buf));

        /* ����Ϣ�����н�����Ϣ */
        if (rt_mq_recv(&mq, &buf[0], sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("thread1: recv msg from msg queue, the content:%s\n", buf);
            
            /* ����Ƿ��յ��˽�����Ϣ */
            if (strcmp(buf, MSG_VIP) == 0)
                break;
        }

        /* ��ʱ1s */
        rt_thread_delay(RT_TICK_PER_SECOND);
    }

    rt_kprintf("thread1: got an urgent message, leave\n");
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
struct rt_thread thread2;
/* �߳�2��� */
static void thread2_entry(void* parameter)
{
    int i, result;
    char buf[128];

    i = 0;
    while (1)    
    {
        rt_snprintf(buf, sizeof(buf), "this is message No.%d", i);

        /* ������Ϣ����Ϣ������ */
        result = rt_mq_send(&mq, &buf[0], sizeof(buf));
        if ( result == -RT_EFULL)
            break;

        rt_kprintf("thread2: send message - %s\n", buf);

        i++;
    }

    rt_kprintf("message queue full, thread2 leave\n");
}

ALIGN(RT_ALIGN_SIZE)
static char thread3_stack[1024];
struct rt_thread thread3;
/* �߳�3��ں��� */
static void thread3_entry(void* parameter)
{
    char msg[] = MSG_VIP;
    int result;

    rt_thread_delay(RT_TICK_PER_SECOND * 5);
    rt_kprintf("thread3: send an urgent message <%s> \n", msg);

    /* ���ͽ�����Ϣ����Ϣ������ */
    do {
        result = rt_mq_urgent(&mq, &msg[0], sizeof(msg));
        
        if (result != RT_EOK)
            rt_thread_delay(20);
    } while (result != RT_EOK);

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
    
    /* ��ʼ����Ϣ���� */
    result = rt_mq_init(&mq, "mqt", 
        &msg_pool[0], /* �ڴ��ָ��msg_pool */ 
        128 - sizeof(void*), /* ÿ����Ϣ�Ĵ�С�� 128 - void* */
        sizeof(msg_pool), /* �ڴ�صĴ�С��msg_pool�Ĵ�С */
        RT_IPC_FLAG_FIFO);/* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */
    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }
    
    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);
    if (tid != RT_NULL) rt_thread_startup(tid);

    rt_thread_init(&thread1,
                   "thread1",
                   thread1_entry,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack),10,50);
    rt_thread_startup(&thread1);


    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),10,50);
    rt_thread_startup(&thread2);


    rt_thread_init(&thread3,
                   "thread3",
                   thread3_entry,
                   RT_NULL,
                   &thread3_stack[0],
                   sizeof(thread3_stack),10,50);
    rt_thread_startup(&thread3);	
    return 0;
}



/*@}*/
