/****************************2012-2013, NJUT, Edu.****************************** 
FileName: adctest.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2014.07.30
Description:    adc.C ADC_Temperature.c �������Գ��� 
              1.������·ADCֵ������ӡ
              2.����CPU�¶�ֵ������ӡ 
              3.����3·ADCֵ������ӡ �����е�һ·�ǵ�·ADC��ֵ
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/07/30     1.0     �ļ�����   
********************************************************************************/ 

#include <rtthread.h>
#include "adc.h"
#include  <finsh.h> 

/*******************************************************************************
* Function Name  : test_adc
* Description    : adc.C  �������Ժ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void test_adc (void)
{
  ADC_Config();
  getadcvalue();
  
  ADCTEMP_Config();
  gettemperature(); 
  
  ADC_Mul_Config();
  getmuladcvalue(); 
  
}


#ifdef  RT_USING_FINSH 
  #include  <finsh.h> 
  FINSH_FUNCTION_EXPORT(test_adc, startup adc convert e.g. test_adc());
  FINSH_FUNCTION_EXPORT(getadcvalue, startup adc convert e.g. getadcvalue());
  FINSH_FUNCTION_EXPORT(gettemperature, startup temperature convert e.g. gettemperature());
  FINSH_FUNCTION_EXPORT(getmuladcvalue, startup temperature convert e.g. getmuladcvalue());
#endif