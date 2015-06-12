/******************************2014-2015, NJTECH, Edu.************************** 
FileName: rfspi.h
Author:  �ﶬ÷       Version :  1.0        Date: 2015.05.30
Description:    ��Ƶģ��SPI�����ӿ�ͷ�ļ�     
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    15/05/30     1.0     �ļ�����  
1.SPI�˿ڳ�ʼ����SPI1: SDO SDI SCK SPI3:SDO SDI SCK
2.�����ٶȵ�SPI����
3.��ʼ��SPI�˿ڣ�����RF����
*******************************************************************************/
#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"

#define GPIO_SPI3     GPIOC
#define RCC_SPI3      RCC_APB2Periph_GPIOC
#define SPI3_SCK      GPIO_Pin_10
#define SPI3_MISO      GPIO_Pin_11
#define SPI3_MOSI      GPIO_Pin_12

#define GPIO_SPI1     GPIOA
#define RCC_SPI1      RCC_APB2Periph_GPIOA
#define SPI1_SCK      GPIO_Pin_5
#define SPI1_MISO      GPIO_Pin_6
#define SPI1_MOSI      GPIO_Pin_7

#define SPI_SPEED_2         0 
#define SPI_SPEED_8         1 
#define SPI_SPEED_16        2 
#define SPI_SPEED_256       3 
																					  
void SPI3_Init(void);			 //��ʼ��SPI�� 
u8 SPI3_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
void SPI3_SetSpeed(u8 SpeedSet); //����SPI�ٶ�

void SPI1_Init(void);
void SPI1_SetSpeed(u8 SpeedSet); //����SPI�ٶ�
u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�		 
#endif

