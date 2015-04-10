/*****************************2012-2013, NJUT, Edu.***************************** 
FileName: RS485.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2013.06.10
Description:    RS485 ����      
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/06/10     1.0     �ļ��޶�   
*******************************************************************************/ 
#include "rs485.h"
/*******************************************************************************
* Function Name  : RS485GPIOConfig
* Description    : 485����GPIO��ʼ��
*485���ƿ����ú���	
          *=============================
          *        RE   |   DE    
          *----------------------------
          *       PD7       PD7
          *=============================
*���ܣ�     1.�ضϣ� RE=1  DE=0
	   2.���գ� RE=0  DE=0
	   3.���ͣ� RE=1  DE=1* 
            PD7 Ϊ0 ���� Ϊ1����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RS485GPIOConfig(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* GPIOD Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 	
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);	  
        
}

/*******************************************************************************
* Function Name  : RS485Read
* Description    : ��ͨ�� ��ȡ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RS485Read( void ) 
{
  GPIO_ResetBits(GPIOD,GPIO_Pin_7);
}

/*******************************************************************************
* Function Name  : RS485Write
* Description    : ��ͨ�� д��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RS485Write( void ) 
{
  GPIO_SetBits(GPIOD,GPIO_Pin_7);
}

