/****************************2012-2013, NJUT, Edu.****************************** 
FileName: i2ctest.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2013.07.30
Description:    I2C.C �������Գ��� 
              1.д��I2Cһ�����ݲ���ӡ
              2.����I2Cһ�����ݲ���ӡ
              3. ���ݱȽϲ���ӡ�ȽϽ��
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/07/30     1.0     �ļ�����   
********************************************************************************/ 

#include <rtthread.h>
#include "I2C.h"
#include "key.h"
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
void test_i2c (void)
{
  unsigned char  i;  
  
  rt_I2C_init();
  
  rt_kprintf("\r\nI2C����FLASH��24C02�� ok\n");
  
  rt_kprintf("\r\nд�������   \n");
  for(i=0;i<64;i++) 
  {   
        rt_I2C_write(0x00+i,i);
        rt_kprintf("0x%02X  ",i);
  }
  
  rt_kprintf("\r\n��ȡ������   \n");
  for(i=0;i<64;i++)
  {  
      rt_kprintf("0x%02X  ",rt_I2C_read(i));
  }   
}

FINSH_FUNCTION_EXPORT(test_i2c,  startup flash test e.g: test_i2c()); 

