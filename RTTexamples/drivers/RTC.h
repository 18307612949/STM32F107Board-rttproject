/***************************2012-2013, NJUT, Edu.****************************** 
* File Name          : calendar.c     Version :  2.0      Date: 2013.07.30
* Author             : �ﶬ÷ת�ԣ�www.armjishu.com Team
* Description        : RTCConfig��ʼ����Time_Display("")������غ�����õ�ǰʱ�䲢��ʾ
*History:         
*      <author>  <time>   <version >   <desc> 
*      Sundm    12/09/30     1.0     �ļ�����   
*******************************************************************************/
#ifndef __RTC_H
#define __RTC_H

//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

void RTCConfig(char* timestring, char flag);//  flag: Ϊ0��ʾ��ϵͳ�����Ƿ��޸�ʱ�䣬Ϊ1��ʾǿ���޸�ʱ��
void Time_Display( char *timestr);//��ʾ��ǰʱ�䲢����


#endif
