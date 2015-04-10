/************************************************* 
  Copyright (C), 2012, NJUT
  File name:      gprs.h
  Author:  sundm     Version:  1.0      Date: 2013.1.3 
  Description:    GPRSģ��api�ӿں��� 
  Others:        
  Function List:  
*************************************************/ 

#ifndef _GPRS
#define _GPRS

#include "rtthread.h"
#include "stm32f10x.h"
#include "gprsio.h"

rt_bool_t  sysconfig(void);// ����GPRS�����豸���򿪣�ע��ص���������ʼ�����ڽ����¼�
rt_bool_t  poweron(void); // ����
rt_bool_t  poweroff(void);// �ػ�
rt_bool_t  gprsinit(void);// ATָ��鿨�����ź�ǿ��
rt_bool_t  gprsconnect(void);// ��Զ����������
rt_bool_t  closeconnect(void);// �ر�Զ����������
rt_bool_t  gprssend(char * str);// �������ݷ���
rt_bool_t  gprsread(void);//  �������ݽ���
rt_bool_t  gprstp(void);//  ��͸��
rt_bool_t  gprstpoff(void);// �ر�͸��
rt_bool_t  msgreaddata(void);// ��ȡ����
rt_bool_t  msgsend(char * str);// ���Ͷ���

#endif



