/****************************2012-2013, NJUT, Edu.****************************** 
FileName: rtctest.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2014.07.30
Description:    rtc.C �������Գ��� 
              1.д��ʱ�䣬����ʱ������
              2.��ʾ��ǰʱ��
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/07/30     1.0     �ļ�����   
********************************************************************************/ 

#include "rtc.h"
/*RTC���Ժ���*/
void test_rtc(char* timestring)
{
  char time[100]={0x00};
  RTCConfig(timestring,1);
  Time_Display(time);
}


#include "finsh.h"
FINSH_FUNCTION_EXPORT (test_rtc ,  startup RTCConfig and Display time e.g. test_rtc("2013073012030502")); 
FINSH_FUNCTION_EXPORT (RTCConfig ,  startup RTCConfig and Display time e.g. RTCConfig("2013073012030502",0)); 
FINSH_FUNCTION_EXPORT (Time_Display ,  startup Display time e.g. Time_Display("")); 
