/****************************2012-2013, NJUT, Edu.****************************** 
FileName: cantest.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2014.07.30
Description:    can���Գ��� 

Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/07/30     1.0     �ļ�����   
********************************************************************************/ 
#include "can.h"
#include "finsh.h"

void test_can()
{
  canconfig();
  can1send("123",3); 
  rt_thread_delay(RT_TICK_PER_SECOND);
  can2send("abcd",4); 
}

/*----------------------------------���²��Ժ���------------------------------*/
#ifdef  RT_USING_FINSH 
#include "finsh.h"
FINSH_FUNCTION_EXPORT (can1send,  can1send("123",3)); 
FINSH_FUNCTION_EXPORT (can2send,  can2send("abcd",4)); 
FINSH_FUNCTION_EXPORT(test_can, can system test e.g. test_can());
#endif

