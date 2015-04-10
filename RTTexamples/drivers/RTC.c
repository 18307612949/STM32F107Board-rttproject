/***************************2012-2013, NJUT, Edu.****************************** 
* File Name          : calendar.c     Version :  2.0      Date: 2013.07.30
* Author             : �ﶬ÷ת�ԣ�www.armjishu.com Team
* Description        : RTCConfig��ʼ����Time_Display("")������غ�����õ�ǰʱ�䲢��ʾ
*History:         
*      <author>  <time>   <version >   <desc> 
*      Sundm    12/09/30     1.0     �ļ�����   
*******************************************************************************/
#include "stm32f10x.h"
#include <rtthread.h>
#include <stdio.h>
#include <string.h>
#include "RTC.h"
#include "date.h"
#include "calendar.h"
#include <stdlib.h>

/* Private variables ---------------------------------------------------------*/
static struct rtc_time systemtime;
static u8 const *WEEK_STR[] = {"��", "һ", "��", "��", "��", "��", "��"};

/* Private function prototypes -----------------------------------------------*/
static void RTC_Configuration(void);
static void NVIC_Configuration(void);
uint32_t Time_Regulate(char* timestring);
void Time_Adjust(char* timestring);
uint8_t USART_Scanf(uint32_t value);

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : NVIC���� �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the RTC Interrupt ʹ��RTC�ж� */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : RTC���� �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  /* PWRʱ�ӣ���Դ���ƣ���BKPʱ�ӣ�RTC�󱸼Ĵ�����ʹ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  /*ʹ��RTC�ͺ󱸼Ĵ������� */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  /* ������BKP��ȫ���Ĵ�������Ϊȱʡֵ */
  BKP_DeInit();

  /* Enable LSE */
  /* ʹ��LSE���ⲿ32.768KHz���پ���*/
  RCC_LSEConfig(RCC_LSE_ON);
  
  /* Wait till LSE is ready */
  /* �ȴ��ⲿ�������ȶ���� */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  /*ʹ���ⲿ32.768KHz������ΪRTCʱ�� */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  /* ʹ�� RTC ��ʱ�ӹ��� */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  /*�ȴ�RTC�Ĵ���ͬ�� */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  /* ʹ��RTC�����ж� */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
  RTC_WaitForLastTask();
  
  /* Set RTC prescaler: set RTC period to 1sec */
  /* 32.768KHz����Ԥ��Ƶֵ��32767,����Ծ���Ҫ��ܸ߿����޸Ĵ˷�Ƶֵ��У׼���� */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
  RTC_WaitForLastTask();
}

/*******************************************************************************
* Function Name  : mid
* Description    : ���ַ������м��ȡn�ַ� �ڲ�����
* Input          : dstΪĿ���ַ��� src ΪԴ�ַ��� nΪ���ȣ�mΪλ��
* Output         : None
* Return         : None
*******************************************************************************/
char* mid(char *dst,char *src, int n,int m) /**/
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len-m;    /*�ӵ�m�������*/
    if(m<0) m=0;    /*�ӵ�һ����ʼ*/
    p += m;
    while(n--) *(q++) = *(p++);
    *(q++)='\0'; /*�б�Ҫ�𣿺��б�Ҫ*/
    return dst;
}

/*******************************************************************************
* Function Name  : Time_Regulate
* Description    : ������ʱ���ַ���ת���ɱ�׼tm��ʽʱ�� �ڲ�����
* Input          : timestringΪԴ�ַ��� 
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t Time_Regulate(char* timestring)
{
  char *ch;  
  uint32_t k;
  struct rtc_time *tm;
  ch = rt_malloc(4); 
  tm = rt_malloc(sizeof(struct rtc_time));
  tm->tm_year = atoi(mid(ch,timestring,4,0));
  tm->tm_mon = atoi(mid(ch,timestring,2,4));
  tm->tm_mday = atoi(mid(ch,timestring,2,6));
  tm->tm_hour = atoi(mid(ch,timestring,2,8));
  tm->tm_min = atoi(mid(ch,timestring,2,10));
  tm->tm_sec = atoi(mid(ch,timestring,2,12));
  k = mktimev(tm);
  return k;
}

/*******************************************************************************
* Function Name  : Time_Adjust
* Description    : ��ʱ��ת��ΪRTC����ֵд��RTC�Ĵ��� �ڲ�����
* Input          : timestringΪԴ�ַ��� 
* Output         : None
* Return         : None
*******************************************************************************/
void Time_Adjust(char* timestring)
{
  /* Wait until last write operation on RTC registers has finished */
  /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
  RTC_WaitForLastTask();
  
  /* Change the current time */
  /* ��ʱ��ת��ΪRTC����ֵд��RTC�Ĵ��� */
  RTC_SetCounter(Time_Regulate(timestring));
  
  /* Wait until last write operation on RTC registers has finished */
  /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
  RTC_WaitForLastTask();
}

/*******************************************************************************
* Function Name  : Time_Display
* Description    : ��ʾ��ǰʱ�估ũ���������� �ⲿ����
* Input          :  None
* Output         : timestr ���ء�20130730081200����ʽ�ַ���
* Return         : None
*******************************************************************************/
void Time_Display( char *timestr)
{
  char  time[40] = {0x00};
  char  str[40] = {0x00};
  
  to_tm(RTC_GetCounter(), &systemtime);
  
  sprintf(time, "%04d%02d%02d%02d%02d%02d", systemtime.tm_year,systemtime.tm_mon, 
          systemtime.tm_mday, systemtime.tm_hour, systemtime.tm_min, systemtime.tm_sec);
  /*���ص�ǰʱ���ַ���*/
  strcpy(timestr,time); 
  
  /*ũ����ʾ*/
  {
      /* ����ũ�� ����ӡ��ʾ*/
      GetChinaCalendar((u16)systemtime.tm_year, (u8)systemtime.tm_mon, (u8)systemtime.tm_mday, (u8*)str);
      rt_kprintf("\n\r\n\r  ����ũ����%d%d,%d,%d", str[0], str[1], str[2],  str[3]);
      /* ����ũ����� ����ӡ��ʾ*/
      GetChinaCalendarStr((u16)systemtime.tm_year,(u8)systemtime.tm_mon,(u8)systemtime.tm_mday,(u8*)str);
      rt_kprintf("  %s", str);
      /* ����ũ������ */
     if(GetJieQiStr((u16)systemtime.tm_year, (u8)systemtime.tm_mon, (u8)systemtime.tm_mday, (u8*)str))
       rt_kprintf("  %s\n\r", str);

  }
  /* �������ʱ�� */
  rt_kprintf("\r  ��ǰʱ��Ϊ: %d�� %d�� %d�� (����%s)  %d:%d:%d \r\n", 
                    systemtime.tm_year, systemtime.tm_mon, systemtime.tm_mday, 
                    WEEK_STR[systemtime.tm_wday], systemtime.tm_hour, 
                    systemtime.tm_min, systemtime.tm_sec);
}

/*******************************************************************************
* Function Name  : RTCConfig
* Description    : ʱ������ �ⲿ����
* Input          : flag: Ϊ0��ʾ��ϵͳ�����Ƿ��޸�ʱ�䣬Ϊ1��ʾǿ���޸�ʱ��
*                   timestring Ҫд���ʱ��
* Output         : timestr ���ء�20130730081200����ʽ�ַ���
* Return         : None
*******************************************************************************/
void RTCConfig(char* timestring, char flag)
{
	NVIC_Configuration();//ʹ��RTC�ж� 
  
  /* ����if...else.... if�ж�ϵͳʱ���Ƿ��Ѿ����ã��ж�RTC�󱸼Ĵ���1��ֵ
     �Ƿ�Ϊ����д���0XA5A5��������ǣ���˵��RTC�ǵ�һ���ϵ磬��Ҫ����RTC��
     ��ʾ�û�ͨ�����ڸ���ϵͳʱ�䣬��ʵ��ʱ��ת��ΪRTC����ֵд��RTC�Ĵ���,
     ���޸ĺ󱸼Ĵ���1��ֵΪ0XA5A5��
     else��ʾ�Ѿ�������ϵͳʱ�䣬��ӡ�ϴ�ϵͳ��λ��ԭ�򣬲�ʹ��RTC���ж�
        flag: Ϊ0��ʾ��ϵͳ�����Ƿ��޸�ʱ�䣬Ϊ1��ʾǿ���޸�ʱ��
  */
  if ((BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) || (flag==1))
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */
    rt_kprintf("\r\n\n RTC not yet configured....");

    /* RTC Configuration */
    RTC_Configuration();

    rt_kprintf("\r\n RTC configured....");

    /* Adjust time by values entred by the user on the hyperterminal */
    Time_Adjust(timestring);
    /* �޸ĺ󱸼Ĵ���1��ֵΪ0XA5A5 */
    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      rt_kprintf("\r\n\n Power On Reset occurred....");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      rt_kprintf("\r\n\n External Reset occurred....");
    }

    rt_kprintf("\r\n No need to configure RTC....");
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }

#ifdef RTCClockOutput_Enable
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
      
  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Disable the Tamper Pin */
  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
                                 functionality must be disabled */

  /* Enable RTC Clock Output on Tamper Pin */
  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
#endif

  /* Clear reset flags */
  RCC_ClearFlag();
}

/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : RTC���жϺ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
  /*  �ж�RTC�Ƿ��������жϣ�Ҳ�п�����������������ж�) */
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    /* ������жϱ�־ */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Enable time update */
    /* ���ʱ���Ѿ����úã����ӡʱ�� */
//    if(TimeSeted) Time_Display(RTC_GetCounter());      

    /* Wait until last write operation on RTC registers has finished */
    /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
    RTC_WaitForLastTask();
    
    /* Reset RTC Counter when Time is 23:59:59 */
    /* ���ʱ��ﵽ23:59:59����һ��ʱ��Ϊ00:00:00 */
    if (RTC_GetCounter() == 0x00015180)
    {
      RTC_SetCounter(0x0);
      
      /* Wait until last write operation on RTC registers has finished */
      /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
      RTC_WaitForLastTask();
    }
  }
}


