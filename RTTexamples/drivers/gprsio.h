/************************************************* 
  Copyright (C), 2012, NJUT
  File name:      gprsio.c
  Author:  sundm     Version:  1.0      Date: 2013.1.3 
  Description:    sundm GPRSģ�������ӿں��� 
  Others:        ���õײ���������亯��  
  Function List:  
*************************************************/ 

#ifndef _GPRSIO
#define _GPRSIO

#include "rtthread.h"
#include "stm32f10x.h"



#define GPRS_PORT GPIOD

#define GPRS_LDO_PIN GPIO_Pin_3
#define GPRS_RST_PIN GPIO_Pin_4
#define GPRS_TREMON_PIN GPIO_Pin_2	


void GPRSPortConfig(void); /*TermOn �˿ڼ�LED��Ʒ��ʼ��*/
void ComConfig(void);/*��������*/
void ModuleReset(void);/*GPRSģ�鸴λ*/
void ModuleStart(void);/*GPRSģ�鿪��*/
void ModuleOff(void);/*GPRSģ��term on�ػ�*/
void ModulePwrOff(void);/*GPRSģ��ϵ�*/
void ComPutString(char *str);/*��������ַ���*/

#endif



