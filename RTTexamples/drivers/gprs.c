/***************************2012-2014, NJUT, Edu.******************************* 
FileName: gprs.c 
Author:  �ﶬ÷       Version :  1.4        Date: 2014.07.26
Description:   ��ΪGPRSģ��ͨ�ţ������豸�������ڣ���ɿ��ػ������룯�˳�͸�� Զ��������/��  ������/�� 
Version:         1.0
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/07/10    1.0       �ļ�����  
      Sundm    13/08/6     1.2       ������� ��/������  
      Sundm    14/04/14    1.3       ���LDO����  
      Sundm    14/04/15    1.4       ��Ӷ����շ�������ʱ�Ѿ��ϵ紦�� 
      Sundm    14/07/15    1.4       �޸ĳ��豸,�¼����� 

Description���豸��ʼ��������GPRS�����豸�򿪣�ע��ص���������ʼ�����ڽ����¼���
�����̣߳����Ӵ������ݡ�
����ATָ��ʱ���رմ��ڼ����̡߳�
�����յ����ݺ��������ݵ���ص�����,�����¼���gprs_send_data_package��
gprs_send_data_package�н������ݣ���������ݡ�

  Others:          
  Function List:  
   1. sysconfig() ����GPRS�����豸���򿪣�ע��ص���������ʼ�����ڽ����¼�
   2. poweron(); term on ����
   3. poweroff(); ����ػ�
   4. gprsinit(); ATָ��鿨�����ź�ǿ��
   5. gprsconnect(); ��Զ����������
   6. closeconnect(); �ر�Զ����������
   7. gprssend("abc"); �������ݷ���
   8. gprsread();  �������ݽ���
   9. gprstp());  ��͸��
   10.gprstpoff(); �ر�͸��
   11.msgreaddata(); ��ȡ����
   12.msgsend("abc"); ���Ͷ���
*******************************************************************************/ #include  <rtthread.h >
#include "gprs.h"

#define AT_CMD "AT\x00D\x00A"               // AT��ѯ
#define ATE0_CMD "ATE0\x00D\x00A"           // ��ֹ����
#define SIMCARD_CMD "AT+CPIN?\x00D\x00A"    // SIM  ����λ��  PIN1  ��״̬��ѯ
#define CSQ_CMD "AT+CSQ\x00D\x00A"          // ��ѯ����ע�����
#define MSG_FMT2_CMD "AT+CNMI=1,1,0,0,0\x00D\x00A"      //����Ϣ֪ͨ��ʽΪ�洢�����ʹ洢λ��

// MG323 TP
#define MG323_TP_NETTYPE_CMD "at^sics=0,conType,GPRS0\x00D\x00A"
#define MG323_TP_APNTYPE_CMD "AT^SICS=0,apn,cmnet\x00D\x00A"
#define MG323_TP_CONID_CMD "AT^SISS=0,conId,0\x00D\x00A"
#define MG323_TP_SERTYPE_CMD "AT^SISS=0,srvType,Socket\x00D\x00A"
#define MG323_TP_CONNECT_CMD "at^siss=0,address,\"socktcp://218.94.84.250:8999\"\x00D\x00A"
#define MG323_TP_NETOPEN_CMD "AT^SISO=0\x00D\x00A"      //����Զ�̷�����
#define MG323_TP_TRANS_CMD "AT^IPENTRANS=0\x00D\x00A"   //��͸��ģʽ 

#define MG323_TP_TRANSOFF_CMD "+++"                 //�˳�͸��ģʽ 

#define MG323_TP_NETCLOSE_CMD "AT^SISC=0\x00D\x00A" //�ر�����
#define MG323_SEND_CMD "AT^SISW=0,4\x00D\x00A"      //�����ֽ�����
#define MG323_READ_CMD "AT^SISR=0,10\x00D\x00A"      //�����ֽ�����


#define MSG_FMT_CMD "AT+CMGF=1\x00D\x00A"      //����Ϣ����Ϊtextģʽ
#define MSG_READ_CMD "AT+CMGR=1\x00D\x00A"      //����һ��λ�ö���
#define MSG_DEL_CMD "AT+CMGD=1,4\x00D\x00A"      //�h������

#define MG323_MSGSETFMT_CMD "AT+CMGF=1\x00D\x00A"      //�O�ø�ʽ
#define MG323_MSGSETNUM_CMD "AT+CMGS=\"13813820420\"\x00D\x00A"      //�O��̖�a
#define MG323_MSGSEND_CMD "\x01A\x00D\x00A"      //�l������

#define CLOSE_CMD "AT^SMSO\x00D\x00A"               // ����ϵͳ�ػ�����

/***************************GPRS���ڽ��Ք����¼�******************************/
#define REV_DATA      0x01
#define REV_WATCH      0x02
#define REV_STOPWATCH      0x04

#define REV_MASK      ( REV_DATA | REV_WATCH | REV_STOPWATCH )
static struct rt_event rev_event;
static rt_device_t gprs_device;


/* gprsio.c��ʹ��*/
void Delay_10ms(uint16_t ms)
{
  rt_thread_delay( ms );
}

/* ����GPRS�����߳����*/
void gprswatch_entry(void* parameter)
{
  rt_err_t result = RT_EOK;
  rt_uint32_t event;
  char gprs_rx_buffer[512]={0x00};
  
  while(1)
  {
      result = rt_event_recv(&rev_event, 
         REV_MASK, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, 
         RT_WAITING_FOREVER, &event);
      if (result == RT_EOK)
      {
        if (event & REV_DATA)
        {
          rt_memset(gprs_rx_buffer,0x00,sizeof(gprs_rx_buffer));
          rt_thread_delay(RT_TICK_PER_SECOND*2);
          rt_device_read(gprs_device, 0, gprs_rx_buffer, 512);
          rt_kprintf(gprs_rx_buffer);
        }
        if (event & REV_STOPWATCH)
        {
          return;
        }
      }
    }
}

void gprswatch(void)
{
  /* ����gprswatch�߳�*/
  rt_thread_t thread = rt_thread_create("gprswatch",
  gprswatch_entry, RT_NULL,
  1024, 25, 7);
  
  /* �����ɹ��������߳�*/
  if (thread != RT_NULL)
    rt_thread_startup(thread);
}
void stopwatch(void)
{
  rt_event_send(&rev_event, REV_STOPWATCH);
}

/*���ݵ���ص�����,�����¼���gprs_send_data_package*/
rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
  rt_event_send(&rev_event, REV_DATA);
  return RT_EOK;
}

/*GPRS���ڷ��ͺͽ���*/
rt_bool_t gprs_send_data_package(char *cmd,char *ack,uint16_t waittime, uint8_t retrytime)
{
  rt_bool_t res = RT_FALSE; 
  rt_err_t result = RT_EOK;
  rt_uint32_t event;
  char gprs_rx_buffer[512]={0x00};
  rt_thread_t thread;
  
  thread = rt_thread_find("gprswatch");
  if( thread != RT_NULL)
    rt_thread_delete(thread);
  
  do 
  {
    rt_device_write(gprs_device, 0, cmd, rt_strlen(cmd));   
    result = rt_event_recv(&rev_event, 
       REV_MASK, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, 
       waittime*RT_TICK_PER_SECOND, &event);
    if (result == RT_EOK)
    {
      if (event & REV_DATA)
      {
        rt_memset(gprs_rx_buffer,0x00,sizeof(gprs_rx_buffer));
        rt_thread_delay(RT_TICK_PER_SECOND*2);
        rt_device_read(gprs_device, 0, gprs_rx_buffer, 512);
        rt_kprintf(gprs_rx_buffer);
        if((rt_strstr(gprs_rx_buffer,ack))||(rt_strstr(gprs_rx_buffer,"OK")))
          res = RT_TRUE;
        else
          res = RT_FALSE;
      }
    }
    retrytime--;
  }while((!res)&&(retrytime>=1));
  gprswatch();
  return res;
} 


/*GPRS�˿ڳ�ʼ�������豸��ע��ص�����*/
rt_bool_t sysconfig(void)
{
  gprs_device = rt_device_find("uart3");
  
  if (gprs_device != RT_NULL)    
  {
    rt_kprintf("\r\n GPRS port initialized!\r\n");
    GPRSPortConfig();
    /* ���ûص����������豸*/
    rt_device_set_rx_indicate(gprs_device, uart_input);
    rt_device_open(gprs_device, RT_DEVICE_OFLAG_RDWR);  
  }
  else
  {
    rt_kprintf("\r\n GPRS port not find !\r\n");
    return RT_FALSE;
  }
  /*GPRS���ڴ򿪺󣬳�ʼ�����ڽ����¼�*/
  rt_event_init(&rev_event, "rev_ev", RT_IPC_FLAG_FIFO);
  return RT_TRUE;
}

/*��������*/
rt_bool_t poweron(void)
{
  rt_kprintf("\r\n Start power on! Display information from module:\r\n");
  ModuleStart();
  if(gprs_send_data_package("","^NWTIME",10,2))
  {
    rt_kprintf("\r\n GPRS power on OK !\r\n");
  }
  else
    return RT_FALSE; 
  return RT_TRUE;
}

/*�ػ�����*/
rt_bool_t poweroff(void)
{
  rt_kprintf("\r\n Start power off! Display information from module:\r\n");
  if(gprs_send_data_package(CLOSE_CMD,"^SHUTDOWN",5,1))
  {
    rt_kprintf("\r\n GPRS software power off OK !\r\n");
  }
  else
  {
    ModuleStart();
    if(gprs_send_data_package("","^SHUTDOWN",3,1))
    {
      ModuleOff();
      rt_kprintf("\r\n GPRS Tremon off OK !\r\n");
    }
    else
    return RT_FALSE; 
  }
  return RT_TRUE;
}

/*���Ͷ���Ϣ*/
rt_bool_t msgsend(char * str)
{
  rt_kprintf("\r\n Send MSG ! Display information from module:\r\n");
  if(gprs_send_data_package(MG323_MSGSETFMT_CMD,"OK",2,1))
  {
  }
  else 
    return RT_FALSE;
  
  if(gprs_send_data_package(MG323_MSGSETNUM_CMD,">",2,1))
  {
  }
  else 
    return RT_FALSE;
  
  gprs_send_data_package(str,str,5,1);
  gprs_send_data_package(MG323_MSGSEND_CMD,".",2,1);//����ظ�1A 0D 0D 
  if(gprs_send_data_package("","CMGS",10,2))
  {
    rt_kprintf("\r\n Send MSG OK !\r\n");
  }
  else 
    return RT_FALSE;
  
  return RT_TRUE;
}

/*���ն���Ϣ*/
rt_bool_t msgreaddata(void)
{
  rt_kprintf("\r\n Read MSG! Display information from module:\r\n");
  if(gprs_send_data_package(MSG_FMT_CMD,"OK",2,1))
  {
  }
  else 
    return RT_FALSE;
  
  if(gprs_send_data_package(MSG_READ_CMD,"OK",2,1))
  {
  }
  else 
    return RT_FALSE;

  if(gprs_send_data_package(MSG_DEL_CMD,"OK",2,1))
  {
    rt_kprintf("\r\n Read MSG OK !\r\n");
  }
  else 
    return RT_FALSE;
  return RT_TRUE;
}

/*GPRS�������� ��ʼ������*/
rt_bool_t gprsinit(void)
{
  rt_kprintf("\r\nGPRS init ! Display information from module:\r\n");
  if(gprs_send_data_package(AT_CMD,"OK",2,3))
  {
  }
  else
    return RT_FALSE; 
  
  if(gprs_send_data_package(SIMCARD_CMD,"OK",2,1))
  {
  }
  else
    return RT_FALSE; 

  if(gprs_send_data_package(CSQ_CMD,"OK",2,1))
  {
  }
  else
    return RT_FALSE; 

  /*if(gprs_send_data_package(MSG_FMT2_CMD,"OK",2,1))
  {
  }
  else
    return RT_FALSE; */
  
  return RT_TRUE; 
}


/*GPRS������������*/
rt_bool_t gprsconnect(void)
{
  rt_kprintf("\r\n Connect net! Display information from module:\r\n");
  if(gprs_send_data_package(MG323_TP_NETTYPE_CMD,"OK",2,1))
  {
  }
  else
    return RT_FALSE; 
  
  if(gprs_send_data_package(MG323_TP_APNTYPE_CMD,"OK",2,1))
  {
  }
  else
    return RT_FALSE; 
  
 /* if(gprs_send_data_package(MG323_TP_CONID_CMD,"OK",2,1))
  {
  }
  else
    return RT_FALSE; */
  
  if(gprs_send_data_package(MG323_TP_SERTYPE_CMD,"OK",2,1))
  {
  }
  else
    return RT_FALSE; 
  
  if(gprs_send_data_package(MG323_TP_CONNECT_CMD,"OK",2,1))
  {
  }
  else
    return RT_FALSE; 
  
  
  if(gprs_send_data_package(MG323_TP_NETOPEN_CMD,"OK",10,3))
  {
    rt_kprintf("\r\n Connect net OK !\r\n");
  }
  else
    return RT_FALSE; 

  return RT_TRUE;
}

/*GPRS�ر���������*/
rt_bool_t closeconnect(void)
{
  rt_kprintf("\r\n Close net! Display information from module:\r\n");
  if(gprs_send_data_package(MG323_TP_NETCLOSE_CMD,"OK",2,1))
  {
    rt_kprintf("\r\n Close net OK !\r\n");
  }
  else
    return RT_FALSE; 

  return RT_TRUE;
}


/*GPRS͸��������*/
rt_bool_t gprstp(void)
{
  rt_kprintf("\r\nEnable trans net! Display information from module:\r\n");
  if(gprs_send_data_package(MG323_TP_TRANS_CMD,"OK",2,1))
  {
    rt_kprintf("\r\n Trans net OK !\r\n");
  }
  else
    return RT_FALSE; 

  return RT_TRUE;
}
                  
/*GPRS͸���ر�����*/
rt_bool_t gprstpoff(void)
{
  rt_kprintf("\r\n Trans off ! Display information from module:\r\n");
  if(gprs_send_data_package(MG323_TP_TRANSOFF_CMD,"OK",2,1))
  {
    rt_kprintf("\r\n Trans net off OK !\r\n");
  }
  else
    return RT_FALSE; 

  return RT_TRUE;
}

/*GPRS������������*/
rt_bool_t gprssend(char* str)
{
  char strtemp[50] = {0x00};  
  int len;
  
  rt_kprintf("\r\n Send data! Display information from module:\r\n");
  len = rt_strlen(str);
  rt_sprintf(strtemp,"AT^SISW=0,%d\x00D\x00A",len);
  if(gprs_send_data_package(strtemp,"SISW",2,1))
  {
  }
  else
    return RT_FALSE; 

  if(gprs_send_data_package(str,"OK",2,1))
  {
    rt_kprintf("\r\n Send data OK !\r\n");
  }
  else
    return RT_FALSE; 
  
  return RT_TRUE;
}

/*GPRS��ȡ��������*/
rt_bool_t gprsread(void)
{
  rt_kprintf("\r\n Read data! Display information from module:\r\n");

  if(gprs_send_data_package(MG323_READ_CMD,"SISR",2,1))
  {
    rt_kprintf("\r\n Read data OK !\r\n");
  }
  else
    return RT_FALSE; 

  return RT_TRUE;
}
#include "finsh.h"
FINSH_FUNCTION_EXPORT(sysconfig, gprs module test e.g. sysconfig());
FINSH_FUNCTION_EXPORT(poweron, gprs module test e.g. poweron());
FINSH_FUNCTION_EXPORT(gprsinit, gprs module test e.g. gprsinit());
FINSH_FUNCTION_EXPORT(msgsend, gprs module test e.g. msgsend("123"));
FINSH_FUNCTION_EXPORT(msgreaddata, gprs module test e.g. msgreaddata());
FINSH_FUNCTION_EXPORT(gprsconnect, gprs module test e.g. gprsconnect());
FINSH_FUNCTION_EXPORT(gprssend, gprs module test e.g. gprssend("abc"));
FINSH_FUNCTION_EXPORT(gprsread, gprs module test e.g. gprsread());
FINSH_FUNCTION_EXPORT(gprstp, gprs module test e.g. gprstp());
FINSH_FUNCTION_EXPORT(gprstpoff, gprs module test e.g. gprstpoff());
FINSH_FUNCTION_EXPORT(closeconnect, gprs module test e.g. closeconnect());
FINSH_FUNCTION_EXPORT(poweroff, gprs module test e.g. poweroff());
