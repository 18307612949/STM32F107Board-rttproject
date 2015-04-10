/***************************2012-2013, NJUT, Edu.******************************* 
FileName: usart2.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2013.07.10
Description:    usart2���� (����485����) ����DMA��ʽ ���з��ͺͽ��� �����ͺͽ����ж�   
ע�⣺���� DMAͨ����ͻ��remove i2c_ee_dma.c�ļ� 
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/07/10     1.0     �ļ�����   
  *          STM32Board Key Pin assignment
  *          =============================
  *          +-----------------------------------------------------------+
  *          |                     Pin assignment                        |
  *          +-----------------------------+-----------------------------+
  *          |      FunctionPin            |     Port & Pin              |
  *          +-----------------------------+-----------------------------+
  *          |      USART2_TX              |        PD5                  |
  *          |      USART2_RX              |        PD6                  |
  *          +-----------------------------+-----------------------------+
*******************************************************************************/ 
#include "stm32f10x.h"
#include "finsh.h"
#include "rs485.h"
#include <stdint.h>
#include "rtthread.h"
#include "string.h"

#define  UART_RX_LEN 512
#define  UART_TX_LEN 512

#define USART2_PORT				GPIOD

#define USART2_PORT_APB2Periph	RCC_APB2Periph_GPIOD
#define USART2_CLK_APB1Periph		RCC_APB1Periph_USART2

#define USART2_TX_PIN				GPIO_Pin_5
#define USART2_RX_PIN				GPIO_Pin_6	

uint8_t USART2_TX_DATA[UART_TX_LEN] = {0x01}; 
uint8_t USART2_RX_DATA[UART_RX_LEN] = {0x00}; 
uint8_t USART2_TX_Finish=1; // USART2������ɱ�־��


/*******************************************************************************
* Function Name  : Uart2Config
* Description    : USART2��ʼ��,115200,��У�飬8���ݣ�1ֹͣ���жϽ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Usart2Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(USART2_PORT_APB2Periph | RCC_APB2Periph_AFIO, ENABLE);
  
  /* Enable the USART2 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
  
  RCC_APB1PeriphClockCmd(USART2_CLK_APB1Periph, ENABLE);    
  
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin   = USART2_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(USART2_PORT, &GPIO_InitStructure);
  
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin   = USART2_RX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USART2_PORT, &GPIO_InitStructure); 
  
  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
  
  /* USART configuration */
  USART_InitStructure.USART_BaudRate   = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits   = USART_StopBits_1;
  USART_InitStructure.USART_Parity     = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
  /*�����ж�ʹ�� RXNE�����ж� TC��������ж� TXE�����ж� PE��ż�����ж� IDLE�����ж�*/
  USART_ITConfig(USART2, USART_IT_IDLE , ENABLE);
  
  /* Enable USART */
  USART_Cmd(USART2, ENABLE);	
  
  /*��֤���͵ĵ�һ���ֽڲ��ᶪʧ*/
  USART_ClearFlag(USART2, USART_FLAG_TC);
}

/*******************************************************************************
* Function Name  : Usart2PutChar
* Description    : USART2����һ�ֽ�
* Input          : ch Ҫ���͵��ֽ�
* Output         : None
* Return         : None
*******************************************************************************/
void Usart2PutChar(uint8_t ch)
{
	USART_SendData(USART2, ch);  
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 	
}

/*******************************************************************************
* Function Name  : Usart2PutString
* Description    : USART2�����ַ���
* Input          : str Ҫ���͵��ַ���
* Output         : None
* Return         : None
*******************************************************************************/
void Usart2PutString(uint8_t *str)
{
	while(*str)
	{
		USART_SendData(USART2, *str++); 
		/*Loop until the end of transmission*/
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 	
	}
}

/*******************************************************************************
* Function Name  : Usart2GetChar 
* Description    : ����2����һ���ַ�
* Input          : None
* Output         : None
* Return         : ���յ����ַ�
*******************************************************************************/
uint8_t Usart2GetChar(void)
{ 
    uint8_t ch;	
    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);    
    ch = USART_ReceiveData(USART2);	   									 
    return ch;   
}

/*******************************************************************************
* Function Name  : USART2_DMA_Configuration
* Description    : ����DMAͨ�� USART2->DMA ����ͨ��DMA�ķ�ʽ��������
* Input          : USART_ConvertedValue ��Ŵ��ڷ������ݵ�RAM��ַ
*                  USART_DR_Address ����1���ݼĴ�����ַ
*                : Len  DMAͨ��һ��DMA�����С
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_DMA_Configuration(uint32_t USART_DR_Address,uint8_t* USART_ConvertedValue,uint32_t Len)
{
  DMA_InitTypeDef DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMA1ʱ��
  DMA_DeInit(DMA1_Channel6);//��DMA1��ͨ��16�Ĵ�������Ϊȱʡֵ
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_Address;//�ò������Զ����������ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART_ConvertedValue;//�ò������Զ����ڴ����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//����������Ϊ���ݴ������Դ
  DMA_InitStructure.DMA_BufferSize = Len;//����DMAͨ��һ��DMA�����С����λΪ�������ݴ�С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�����ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�趨�������ݿ��Ϊ8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;//�趨�ڴ����ݿ��Ϊ8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA����ģʽΪNormal�����ΪCircular,����ѭ������
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//ָ��ͨDMAͨ��һӵ�и����ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//��DMAͨ��һ�����ó��ڴ浽�ڴ�ģʽ
  DMA_Init(DMA1_Channel6, &DMA_InitStructure);//����DMA_InitStructure�еĲ�����ʼ��DMA1��ͨ��6�Ĵ���
      
  /*DMA��������жϿ� ��������жϿ�*/
  DMA_ITConfig(DMA1_Channel6, DMA_IT_TC , ENABLE);
  DMA_ITConfig(DMA1_Channel6, DMA_IT_TE , ENABLE);
  
  /* ʹ��USART1��DMA��������*/
  USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
  
  /* Enable DMA1 channel6 */
  DMA_Cmd(DMA1_Channel6, ENABLE);
  
  /* Configure the NVIC Preemption Priority Bits */  
  
  /*DMA�ж�����*/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
  
}

/*******************************************************************************
* Function Name  : USART2_DMA_Configuration
* Description    : ����DMAͨ�� DMA->USART2 ����ͨ��DMA�ķ�ʽ��������
* Input          : USART_ConvertedValue ��Ŵ��ڷ������ݵ�RAM��ַ
*                  USART_DR_Address ����1���ݼĴ�����ַ 
*                : Len  DMAͨ��һ��DMA�����С
* Output         : None
* Return         : None
*******************************************************************************/
void DMA_USART2_Configuration(uint32_t USART_DR_Address,uint8_t* USART_ConvertedValue,uint32_t Len)
{
  DMA_InitTypeDef DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMA1ʱ��
  
  DMA_DeInit(DMA1_Channel7);//��DMA1��ͨ��17�Ĵ�������Ϊȱʡֵ
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_Address;//�ò������Զ����������ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART_ConvertedValue;//�ò������Զ����ڴ����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//����������Ϊ���ݴ����Ŀ��
  DMA_InitStructure.DMA_BufferSize = Len;//����DMAͨ��һ��DMA�����С����λΪ�������ݴ�С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�����ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�趨�������ݿ��Ϊ8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;//�趨�ڴ����ݿ��Ϊ8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;////DMA����ģʽΪNormal�����ΪCircular,
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//ָ��ͨDMAͨ��һӵ�и����ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//��DMAͨ��һ�����ó��ڴ浽�ڴ�ģʽ
  DMA_Init(DMA1_Channel7, &DMA_InitStructure);//����DMA_InitStructure�еĲ�����ʼ��DMA1��ͨ��7�Ĵ���
  
  /*DMA��������жϿ� ��������жϿ�*/
  DMA_ITConfig(DMA1_Channel7, DMA_IT_TC , ENABLE);
  DMA_ITConfig(DMA1_Channel7, DMA_IT_TE , ENABLE);
  
  /* Enable the USART Tx DMA request */                
  USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
  
  /* Enable DMA1 channel7 */
  DMA_Cmd(DMA1_Channel7, DISABLE);

  /* Configure the NVIC Preemption Priority Bits */  
  
  /*DMA�ж�����*/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
}

/*******************************************************************************
* Function Name  : DMA1_Channel6_IRQHandler
* Description    : DMA �жϷ������ DMA ������ȫ��ɺ�����ж� ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel6_IRQHandler(void)
{
  DMA_ClearITPendingBit(DMA1_IT_TE6);  
  if (DMA_GetITStatus(DMA1_IT_TC6) != RESET) // �ж�Ϊ�����ж�
    {
      DMA_ClearITPendingBit(DMA1_IT_TC6);
      DMA_Cmd(DMA1_Channel6, DISABLE);//�ر�DMA,��ֹ�������������
      DMA_SetCurrDataCounter(DMA1_Channel6, (uint16_t)UART_RX_LEN); //��װ��
      DMA_Cmd(DMA1_Channel6, ENABLE);//������,�ؿ�DMA
    }
}

/*******************************************************************************
* Function Name  : DMA1_Channel7_IRQHandler
* Description    : DMA �жϷ������ DMA ������ȫ��ɺ�����ж� ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel7_IRQHandler(void)
{
  int delaytime;
	DMA_ClearITPendingBit(DMA1_IT_TE7);  
  if (DMA_GetITStatus(DMA1_IT_TC7) != RESET) // �ж�Ϊ �����ж�
    {
      DMA_ClearITPendingBit(DMA1_IT_TC7);
      DMA_Cmd(DMA1_Channel7, DISABLE);//�ر�DMA
      USART2_TX_Finish=1;//��DMA�������
      
      for(delaytime = 0;delaytime<5000;delaytime++);
      RS485Read();//��Ϊ��״̬ �����������豸����Ϊ��֮ǰ��Ҫ��ʱ�������ݷ���
    }
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
  uint32_t DATA_LEN = 0;
  uint16_t i = 0;
  /*��SR���DR���Idle �ȶ�SR��Ȼ���DR������� ��USART_ClearFlagע��*/
  /*һ֡���ս��� ����IDLE�����ж� ��������״̬�����IDLE�ж� */
  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
  {
    i = USART2->SR;
    i = USART2->DR;
    DMA_Cmd(DMA1_Channel6, DISABLE);//�ر�DMA,��ֹ�������������
    
    DATA_LEN = UART_RX_LEN - DMA_GetCurrDataCounter(DMA1_Channel6); 
    
    if(DATA_LEN > 0)
    {                        
      while(USART2_TX_Finish==0);//�ȴ���һ�����ݷ�����ɲ���һ��
      /*������2DMA�յ������� �͵� ����2�ķ���DMA�洢��ַ*/
      for(i=0;i<DATA_LEN;i++)
      {
        USART2_TX_DATA[i]=USART2_RX_DATA[i];
      }
      //USART��DMA���������ѯ��ʽ���ͣ��˷��������ȼ��ж϶�������֡����
      DMA_Cmd(DMA1_Channel7, DISABLE); //�ı�datasizeǰ��Ҫ��ֹͨ������
      DMA1_Channel7->CNDTR=DATA_LEN; //��װ���������
      USART2_TX_Finish=0;//DMA���俪ʼ��־��
      RS485Write();//��Ϊд״̬
      DMA_Cmd(DMA1_Channel7, ENABLE);                        
    }
    DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6);//���־
    DMA_SetCurrDataCounter(DMA1_Channel6, UART_RX_LEN); //��װ��
    DMA_Cmd(DMA1_Channel6, ENABLE);//������,�ؿ�DMA
  } 
}

/*******************************************************************************
* Function Name  : testdma
* Description    : ʵ�ִ����յ����ٴ��Լ��Ĵ��ڷ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void test_usart2(void )
{
  RS485GPIOConfig();
  Usart2Config();
  
  RS485Read() ;
  DMA_USART2_Configuration((uint32_t)(&USART2->DR),USART2_TX_DATA,UART_TX_LEN);
  USART2_DMA_Configuration((uint32_t)(&USART2->DR) ,USART2_RX_DATA,UART_RX_LEN);
}


FINSH_FUNCTION_EXPORT(test_usart2, startup usart2 receive send e.g. test_usart2());
