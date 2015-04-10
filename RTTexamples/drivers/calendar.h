/***************************2012-2013, NJUT, Edu.****************************** 
* File Name          : calendar.h     Version :  2.0      Date: 2013.07.30
* Author             : �ﶬ÷ת�ԣ�www.armjishu.com Team
* Description        : ��ǿ��������֧��ũ����24�����������������Ĺ���
*                          ����ʱ����1970��ΪԪ�꣬��32bit��ʱ��Ĵ���������
*                          �е�2100������
*History:         
*      <author>  <time>   <version >   <desc> 
*      Sundm    12/09/30     1.0     �ļ�����   
*******************************************************************************/

#ifndef __CALENDAR_H
#define __CALENDAR_H
#include "stm32f10x.h"

u8 GetChinaCalendar(u16  year,u8 month,u8 day,u8 *p);
void GetChinaCalendarStr(u16 year,u8 month,u8 day,u8 *str);
u8 GetJieQiStr(u16 year,u8 month,u8 day,u8 *str);

#endif 
