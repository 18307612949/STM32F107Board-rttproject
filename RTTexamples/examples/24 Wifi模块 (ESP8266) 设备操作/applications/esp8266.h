/************************************************* 
  Copyright (C), 2012, NJUT
  File name:      gprsio.c
  Author:  sundm     Version:  1.0      Date: 2013.1.3 
  Description:    sundm GPRSģ�������ӿں��� 
  Others:        ���õײ���������亯��  
  Function List:  
*************************************************/ 

#ifndef _ESP8266
#define _ESP8266

#include "rtthread.h"
#include "stm32f10x.h"

rt_bool_t wificonfig();// ����wifi�����豸���򿪣�ע��ص���������ʼ�����ڽ����¼�
rt_bool_t wifiinit(); //wifi�����ȵ�
rt_bool_t wifijap() ;
rt_bool_t wificonnect(); //��Զ����������
rt_bool_t wifisend(char * str); //�������ݷ���
rt_bool_t wificloseconnect();// �ر�Զ����������
rt_bool_t wifiexit(); // wifi�˳��ȵ�

#endif



