/******************************2012-2013, NJUT, Edu.*************************** 
FileName: adc_temprature.h 
Author:  �ﶬ÷       Version :  1.0        Date: 2013.03.30
Description:    ʹ��Ƭ�ڵ��¶ȴ����������¶�,��ͨ����������¶�ֵ    
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/03/30     1.0     �ļ�����   
*******************************************************************************/ 
#ifndef __ADC_TEMPERATURE
#define __ADC_TEMPERATURE

  #include "stm32f10x.h"
  #include "stm32f10x_adc.h"
  #include "stm32f10x_dma.h"
  #include <rtthread.h>
  
  void TemperatureConfig(void);
  int GetTemperature(void) ;

#endif 
