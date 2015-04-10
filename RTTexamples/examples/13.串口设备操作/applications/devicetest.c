/***************************2012-2014, NJUT, Edu.******************************* 
FileName: devicetest.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2014.05.10
Description:    usart2,3 �����豸������
            * ������һ��devt�̣߳�Ȼ��򿪴���2��3
            * ������2��3������ʱ������ȡ���е���������Ȼ��д�뵽
            * ����3��2�豸�С�
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/05/10     1.0     �ļ�����   
  *          STM32Board Key Pin assignment
  *          =============================
  *          +-----------------------------------------------------------+
  *          |                     Pin assignment                        |
  *          +-----------------------------+-----------------------------+
  *          |      FunctionPin            |     Port & Pin              |
  *          +-----------------------------+-----------------------------+
  *          |      USART2_TX              |        PD5                  |
  *          |      USART2_RX              |        PD6                  |
  *          +-----------------------------+-----------------------------+
  *          |      USART3_TX              |        PC10                  |
  *          |      USART3_RX              |        PC11                  |
  *          +-----------------------------+-----------------------------+
*******************************************************************************/ 
#include <rtthread.h>
#include "rs485.h"

/* UART������Ϣ�ṹ*/
struct rx_msg
{
  rt_device_t dev;
  rt_size_t size;
};


/* ���ڽ�����Ϣ����Ϣ����*/
static struct rt_messagequeue rx_mq;

/* �����̵߳Ľ��ջ�����*/
/*ע����serial.h�У�����UART_RX_BUFFER_SIZE���� */
static char uart_rx_buffer[1024];

/* ���ݵ���ص�����*/
rt_err_t uartdevice_input(rt_device_t dev, rt_size_t size)
{
  struct rx_msg msg;
  msg.dev = dev;
  msg.size = size;
  /* ������Ϣ����Ϣ������*/
  rt_mq_send(&rx_mq, &msg, sizeof(struct rx_msg));
  return RT_EOK;
}

void device_thread_entry(void* parameter)
{

  struct rx_msg msg;
  int count = 0;
  rt_device_t device2, device3;
  rt_err_t result = RT_EOK;
  rt_uint32_t rx_length;
  
  RS485GPIOConfig();
  //485ת��оƬ��PD5PD6�Ľ�����Ӱ��
  RS485Write();//uart2����sp3485,��ֹ�����PD5PD6���ţ�����ǰ����д״̬
  
  result = rt_mq_init(&rx_mq, "rx_mq", &msg, sizeof(struct rx_msg), sizeof(struct rx_msg)*20, RT_IPC_FLAG_FIFO);
  if (result != RT_EOK) 
  { 
      rt_kprintf("init message queue failed.\n"); 
  } 

  /* ����ϵͳ�еĴ���2�豸 */
  device2 = rt_device_find("uart2");
  if (device2!= RT_NULL)
  {
    /* ���ûص����������豸*/
    rt_device_set_rx_indicate(device2, uartdevice_input);
    rt_device_open(device2, RT_DEVICE_OFLAG_RDWR);
  }
  
  /* ����ϵͳ�еĴ���3�豸 */
  device3 = rt_device_find("uart3");
  if (device3 != RT_NULL)
  {
    /* ���ûص����������豸*/
    rt_device_set_rx_indicate(device3, uartdevice_input);
    rt_device_open(device3, RT_DEVICE_OFLAG_RDWR);
  }

  while (1)
  {
    /* ����Ϣ�����ж�ȡ��Ϣ*/
    result = rt_mq_recv(&rx_mq, &msg, sizeof(struct rx_msg), 1000);
    if (result == -RT_ETIMEOUT)
    {
      /* ���ճ�ʱ*/
      rt_kprintf("timeout count:%d\n", ++count);
    }
    /* �ɹ��յ���Ϣ*/
    if (result == RT_EOK)
    {
      rx_length = (sizeof(uart_rx_buffer) - 1) > msg.size ?
      msg.size : sizeof(uart_rx_buffer) - 1;
      /* ��ȡ��Ϣ*/
      rx_length = rt_device_read(msg.dev, 0, &uart_rx_buffer[0], rx_length);
      uart_rx_buffer[rx_length] = '\0';
      /* д��д�豸��*/
      rt_device_write(device2, 0, &uart_rx_buffer[0], rx_length);
      rt_device_write(device3, 0, &uart_rx_buffer[0], rx_length);
    }
  }
}

void test_device()
{
  /* ����devt�߳�*/
  rt_thread_t thread = rt_thread_create("devt",
  device_thread_entry, RT_NULL,
  1024, 25, 7);
  
  /* �����ɹ��������߳�*/
  if (thread != RT_NULL)
    rt_thread_startup(thread);
}


#ifdef  RT_USING_FINSH 
 #include  <finsh.h> 
 /*  ���������finsh  shell�� */ 
FINSH_FUNCTION_EXPORT(test_device, device test e.g.test_device());
#endif