/************************************************* 
  Copyright (C), 2012, NJUT
  File name:      gprsio.c
  Author:  sundm     Version:  1.0      Date: 2013.1.3 
  Description:    sundm GPRSģ�������ӿں��� 
  Others:     ���еĺ�������Ҫ�޸�     
  Function List:  ����GPRSPortConfig(void);//�˿ڳ�ʼ��
                  ����ModuleStart(void);//GPRSģ�鿪��
                  ����ModuleReset(void);//GPRSģ�鸴λ
                  ����ComPutString(uint8_t *str);//��������ַ���
*************************************************/ 
#include "GPRSIO.h"

extern void Delay_10ms(uint16_t ms);
/*******************************************************************************
* Function Name  : GPRSPortConfig
* Description    : TermOn  and RST Port  Config
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPRSPortConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 	
  
  GPIO_InitStructure.GPIO_Pin   = GPRS_TREMON_PIN | GPRS_RST_PIN| GPRS_LDO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPRS_PORT, &GPIO_InitStructure); 
}

/*******************************************************************************
* Function Name  : TermonOn
* Description    : TermOn Port On �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TermonOn(void)
{
  GPIO_SetBits(GPRS_PORT,GPRS_TREMON_PIN);
}

/*******************************************************************************
* Function Name  : TermonOff
* Description    : TermOn Port Off �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TermonOff(void)
{
  GPIO_ResetBits(GPRS_PORT,GPRS_TREMON_PIN);
}

/*******************************************************************************
* Function Name  : ModuleReset
* Description    : ģ�鸴λ RESET �õ�50ms 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ModuleReset(void)
{
  GPIO_SetBits(GPRS_PORT,GPRS_RST_PIN);
  Delay_10ms(50);
  GPIO_ResetBits(GPRS_PORT,GPRS_RST_PIN);
}

/*******************************************************************************
* Function Name  : ModuleStart
* Description    : GPRSģ�鿪�� ��λ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ModuleStart(void)
{
  GPIO_SetBits(GPRS_PORT,GPRS_LDO_PIN);
  Delay_10ms(100);
  GPIO_SetBits(GPRS_PORT,GPRS_TREMON_PIN);
  Delay_10ms(150);
  GPIO_ResetBits(GPRS_PORT,GPRS_TREMON_PIN);
  Delay_10ms(100);
}

/*******************************************************************************
* Function Name  : ModuleOff
* Description    : GPRSģ�鿪�� ��λ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ModuleOff(void)
{
  GPIO_SetBits(GPRS_PORT,GPRS_TREMON_PIN);
  Delay_10ms(100);
  GPIO_ResetBits(GPRS_PORT,GPRS_TREMON_PIN);
}

/*******************************************************************************
* Function Name  : ModulePwrOff
* Description    : GPRSģ��ϵ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ModulePwrOff(void)
{
  GPIO_ResetBits(GPRS_PORT,GPRS_LDO_PIN);
  Delay_10ms(60);
}



