/*********************2012-2013, NJUT, Edu.************************************ 
FileName: flashtest.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2012.12.30
Description:    �����ֽ�flash��д����      
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/08/30     1.0     �ļ���ʽ����   
  *          STM32Board Key Pin assignment
******************************************************************************/ 
#include <stm32f10x.h>
#include  <rtthread.h >
#include  <finsh.h> 

#define FLASH_ADR 0x08038000  //Ҫд�����ݵĵ�ַ
#define FLASH_DATA 0xa5a5a5a5  //Ҫд�������
void test_flash(void)
{
 u32 tmp;
 
 //�жϴ�FLASH�Ƿ�Ϊ�հ�
 tmp=*(vu32*)(FLASH_ADR);
 if(tmp==0xffffffff)
 {
  FLASH_Unlock(); //����flash��д����������
  FLASH_ProgramWord(FLASH_ADR,FLASH_DATA);//��ָ���ĵ�ַдһ����
  FLASH_Lock();   //����flash��д����������
  rt_kprintf("�Ѿ�д������\r\n");
 }
 else if(tmp==FLASH_DATA)
 {
  rt_kprintf("����������ȷ\r\n");
 }
 else
 {
  rt_kprintf("�������ݴ���\r\n");
  FLASH_Unlock();//����flash��д����������
  FLASH_ErasePage(FLASH_ADR);//����flash��һ��ҳ��
  FLASH_Lock();   //����flash��д����������
  rt_kprintf("has clear error address!\r\n");
 }
}

/*  ���flashtestbyte ������finsh  shell�� 
���̣���ȡ�õ�ַ���ݣ�
����	��Ϊ������������棬��д�����ݣ���ӡ"�Ѿ�д������"��
����	����ΪҪд������ݣ�����������ݣ���ӡ"�������ݴ���"
����	��Ϊ�����ݣ��򷵻أ���ӡ"����������ȷ"��
*/ 
FINSH_FUNCTION_EXPORT (test_flash,  test flash write&read e.g.test_flash()); 

