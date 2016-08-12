/****************************2012-2013, NJUT, Edu.****************************** 
FileName: i2ctest.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2013.07.30
Description:    I2C_ee.C �������Գ��� ����Ӳ��I2C��ѯ��ʽ
              1.д��I2Cһ�����ݲ���ӡ
              2.����I2Cһ�����ݲ���ӡ
              
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/07/30     1.0     �ļ�����   
********************************************************************************/ 

#include <rtthread.h>
#include "I2C_ee.h"
#include  <finsh.h> 

/*******************************************************************************
* Function Name  : i2ctest
* Description    : I2C.C �������Ժ���
*              1.д��I2Cһ�����ݲ���ӡ
*              2.����I2Cһ�����ݲ���ӡ
*              3. ���ݱȽϲ���ӡ�ȽϽ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void test_i2c_ee (void)
{
  uint16_t  i;  
  uint8_t WriteBuffer[256] = {0x00};
  uint8_t ReadBuffer[256] = {0x00};
  
  IIC_Configuration();
  
  rt_kprintf("\r\nI2C����FLASH��24C02�� ok\n");
  
  for(i = 0;i<256; i++)
  {
      WriteBuffer[i] = 0xFF;
  }
  for(i = 0;i<32; i++)
  {
      IIC_EE_WriteOnePage(WriteBuffer,i*8,8);
      rt_thread_delay(2);
  }


  for(i = 0;i<256; i++)
  {
      WriteBuffer[i] = i;
  }

  IIC_Buffer_Write(WriteBuffer, 5, 128);
	

  
  IIC_Buffer_Read( ReadBuffer, 0,256);
	
  rt_kprintf ("\r\n ������ \n");
  for(i = 0; i<256; i++)
  {
    rt_kprintf ("%x ",ReadBuffer[i]);
  }	

}

FINSH_FUNCTION_EXPORT(test_i2c_ee,  startup flash test e.g: test_i2c_ee()); 
