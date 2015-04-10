/****************************2012-2013, NJUT, Edu.****************************** 
FileName: cantest.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2014.12.15
Description:    can�豸�������Գ��� 

Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/12/15     1.0     �ļ�����   
********************************************************************************/ 

#include <rtthread.h>
#include <candevice.h>

/* can������Ϣ�ṹ*/
struct rx_msg
{
  rt_device_t dev;
  rt_size_t size;
};


/* ���ڽ�����Ϣ����Ϣ����*/
static struct rt_messagequeue rx_mq;

/* �����̵߳Ľ��ջ�����*/
/*ע����candevice.h�У����� CAN_RX_LEN ���� */
static CAN_msg_t can_msg;

/* ���ݵ���ص�����*/
rt_err_t candevice_input(rt_device_t dev, rt_size_t size)
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
  rt_device_t device1;
  rt_err_t result = RT_EOK;
  rt_uint32_t rx_length;
  
  result = rt_mq_init(&rx_mq, "rx_mq", &msg, sizeof(struct rx_msg), sizeof(struct rx_msg)*20, RT_IPC_FLAG_FIFO);
  if (result != RT_EOK) 
  { 
      rt_kprintf("init message queue failed.\n"); 
  } 

  /* ����ϵͳ�е� can1 �豸 */
  device1 = rt_device_find("can1");
  if (device1 != RT_NULL)
  {
    /* ���ûص����������豸*/
    rt_device_set_rx_indicate(device1, candevice_input);
    rt_device_open(device1, RT_DEVICE_OFLAG_RDWR);
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
      rx_length = msg.size ;
      /* ��ȡ��Ϣ*/
      rx_length = rt_device_read(msg.dev, 0, &can_msg, rx_length);
      rt_device_write(device1, 0, &can_msg, 1);
    }
  }
}

void test_candevice()
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
FINSH_FUNCTION_EXPORT(test_candevice, device test e.g.test_candevice());
#endif
