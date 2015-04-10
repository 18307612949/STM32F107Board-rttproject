/***************************2012-2013, NJUT, Edu.******************************* 
FileName: usart3.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2013.07.10
Description:    usart3����  ����DMA��ʽ ���з��ͺͽ��� �����ͺͽ����ж�   
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
  *          |      USART3_TX              |        PC10                 |
  *          |      USART3_RX              |        PC11                 |
  *          +-----------------------------+-----------------------------+
*******************************************************************************/ 
#include "stm32f10x.h"
#include "finsh.h"
#include <stdint.h>
#include "stm32f10x.h"
#include "rtthread.h"
#include "string.h"

#define  UART_RX_LEN 512
#define  UART_TX_LEN 512

#define USART3_PORT				GPIOC

#define USART3_PORT_APB2Periph	RCC_APB2Periph_GPIOC
#define USART3_CLK_APB1Periph		RCC_APB1Periph_USART3

#define USART3_TX_PIN				GPIO_Pin_10
#define USART3_RX_PIN				GPIO_Pin_11	

uint8_t USART3_TX_DATA[UART_TX_LEN] = {0x00}; 
uint8_t USART3_RX_DATA[UART_RX_LEN] = {0x00}; 

uint8_t USART3_TX_Finish=1; // USART3������ɱ�־λ �������Ϊ1
uint8_t USART3_RX_Finish=0; // USART3����һ֡��ɱ�־λ �������Ϊ1
uint16_t rxlen = 0;

/*******************************************************************************
* Function Name  : Uart3Config
* Description    : USART2��ʼ��,115200,��У�飬8���ݣ�1ֹͣ���жϽ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Usart3Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(USART3_PORT_APB2Periph | RCC_APB2Periph_AFIO, ENABLE);
  
  /* Enable the USART2 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
  
  RCC_APB1PeriphClockCmd(USART3_CLK_APB1Periph, ENABLE);    
  
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin   = USART3_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(USART3_PORT, &GPIO_InitStructure);
  
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin   = USART3_RX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USART3_PORT, &GPIO_InitStructure); 
  
  /* USART configuration */
  USART_InitStructure.USART_BaudRate   = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits   = USART_StopBits_1;
  USART_InitStructure.USART_Parity     = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);
  
  /*�����ж�ʹ�� RXNE�����ж� TC��������ж� TXE�����ж� PE��ż�����ж� IDLE�����ж�*/
  USART_ITConfig(USART3, USART_IT_IDLE , ENABLE);
  
  /* Enable USART */
  USART_Cmd(USART3, ENABLE);	
  
  /*��֤���͵ĵ�һ���ֽڲ��ᶪʧ*/
  USART_ClearFlag(USART3,USART_FLAG_TC); 

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
  
}

/*******************************************************************************
* Function Name  : Usart2PutChar
* Description    : USART2����һ�ֽ�
* Input          : ch Ҫ���͵��ֽ�
* Output         : None
* Return         : None
*******************************************************************************/
void Usart3PutChar(uint8_t ch)
{
	USART_SendData(USART3, ch);  
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 	
}

/*******************************************************************************
* Function Name  : Usart2PutString
* Description    : USART2�����ַ���
* Input          : str Ҫ���͵��ַ���
* Output         : None
* Return         : None
*******************************************************************************/
void Usart3PutString(uint8_t *str)
{
  while(*str)
  {
    USART_SendData(USART3, *str++); 
    /*Loop until the end of transmission*/
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 	
  }
}

/*******************************************************************************
* Function Name  : Usart2DMAPutString
* Description    : USART2ͨ��DMA��ʽ�����ַ���
* Input          : str Ҫ���͵��ַ���
* Output         : None
* Return         : None
*******************************************************************************/
void DMA3PutString(uint8_t *str)
{
  uint16_t len;
  len = strlen((char const*)str);
  memset(USART3_TX_DATA,0x00,UART_TX_LEN);
  
  while(USART3_TX_Finish==0);//�ȴ���һ�����ݷ�����ɲ���һ��
  memcpy(USART3_TX_DATA,str,len);
  DMA_Cmd(DMA1_Channel2, DISABLE); //�ı�datasizeǰ��Ҫ��ֹͨ������
  DMA1_Channel2->CNDTR=len; //��װ���������
  USART3_TX_Finish = 0;//DMA���俪ʼ��־��
  DMA_Cmd(DMA1_Channel2, ENABLE);                        
}

/*******************************************************************************
* Function Name  : Usart2GetChar 
* Description    : ����2����һ���ַ�
* Input          : None
* Output         : None
* Return         : ���յ����ַ�
*******************************************************************************/
uint8_t Usart3GetChar(void)
{ 
    uint8_t ch;	
    while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);    
    ch = USART_ReceiveData(USART3);	   									 
    return ch;   
}

/*******************************************************************************
* Function Name  : USART2_DMA_Configuration
* Description    : ����DMAͨ�� USART3->DMA ����ͨ��DMA�ķ�ʽ�������� DMA1ͨ��3
* Input          : USART_ConvertedValue ��Ŵ��ڷ������ݵ�RAM��ַ
*                  USART_DR_Address ����1���ݼĴ�����ַ
*                : Len  DMAͨ��һ��DMA�����С
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_DMA_Configuration(uint32_t USART_DR_Address,uint8_t* USART_ConvertedValue,uint32_t Len)
{
  DMA_InitTypeDef DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMA1ʱ��
  DMA_DeInit(DMA1_Channel3);//��DMA1��ͨ��3�Ĵ�������Ϊȱʡֵ
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
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);//����DMA_InitStructure�еĲ�����ʼ��DMA1��ͨ��6�Ĵ���
      
  /*DMA��������жϿ� ��������жϿ�*/
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TC , ENABLE);
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TE , ENABLE);
  
  /* ʹ��USART3��DMA��������*/
  USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
  
  /* Enable DMA1 channel3 */
  DMA_Cmd(DMA1_Channel3, ENABLE);
  
  /* Configure the NVIC Preemption Priority Bits */  
  
  /*DMA�ж�����*/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
  
}

/*******************************************************************************
* Function Name  : USART3_DMA_Configuration
* Description    : ����DMAͨ�� DMA->USART3 ����ͨ��DMA�ķ�ʽ�������� DMA1ͨ��2
* Input          : USART_ConvertedValue ��Ŵ��ڷ������ݵ�RAM��ַ
*                  USART_DR_Address ����3���ݼĴ�����ַ 
*                : Len  DMAͨ��һ��DMA�����С
* Output         : None
* Return         : None
*******************************************************************************/
void DMA_USART3_Configuration(uint32_t USART_DR_Address,uint8_t* USART_ConvertedValue,uint32_t Len)
{
  DMA_InitTypeDef DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMA1ʱ��
  
  DMA_DeInit(DMA1_Channel2);//��DMA1��ͨ��12�Ĵ�������Ϊȱʡֵ
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
  DMA_Init(DMA1_Channel2, &DMA_InitStructure);//����DMA_InitStructure�еĲ�����ʼ��DMA1��ͨ��7�Ĵ���
  
  /*DMA��������жϿ� ��������жϿ�*/
  DMA_ITConfig(DMA1_Channel2, DMA_IT_TC , ENABLE);
  DMA_ITConfig(DMA1_Channel2, DMA_IT_TE , ENABLE);
  
  /* Enable the USART Tx DMA request */                
  USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
  
  /* Enable DMA1 channel7 */
  DMA_Cmd(DMA1_Channel2, DISABLE);

  /* Configure the NVIC Preemption Priority Bits */  
  
  /*DMA�ж�����*/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
}

/*******************************************************************************
* Function Name  : DMA1_Channel3_IRQHandler
* Description    : DMA �жϷ������ DMA ������ȫ��ɺ�����ж� ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
  DMA_ClearITPendingBit(DMA1_IT_TE3);  
  if (DMA_GetITStatus(DMA1_IT_TC3) != RESET) // �ж�Ϊ�����ж�
    {
      DMA_ClearITPendingBit(DMA1_IT_TC3);
      DMA_Cmd(DMA1_Channel3, DISABLE);//�ر�DMA,��ֹ�������������
      DMA_SetCurrDataCounter(DMA1_Channel3, (uint16_t)UART_RX_LEN); //��װ��
      DMA_Cmd(DMA1_Channel3, ENABLE);//������,�ؿ�DMA
    }
}

/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : DMA �жϷ������ DMA ������ȫ��ɺ�����ж� ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel2_IRQHandler(void)
{
  DMA_ClearITPendingBit(DMA1_IT_TE2);  
  if (DMA_GetITStatus(DMA1_IT_TC2) != RESET) // �ж�Ϊ �����ж�
    {
      DMA_ClearITPendingBit(DMA1_IT_TC2);
      DMA_Cmd(DMA1_Channel2, DISABLE);//�ر�DMA
      USART3_TX_Finish=1;//��DMA�������
    }
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
  uint16_t temp = 0;
  rxlen = 0;
   /*�����ж�-����USART��DMA���գ�ͬʱ�رս����жϣ�ʹ�ܿ����ж� ���þ�ΧĬ״̬��*/
    if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
      USART_ClearITPendingBit(USART3,USART_IT_RXNE);
      DMA_Cmd(DMA1_Channel3, ENABLE);
      USART_ReceiverWakeUpCmd(USART3,ENABLE);
      USART_ITConfig(USART3, USART_IT_RXNE , DISABLE);
      USART_ITConfig(USART3, USART_IT_IDLE , ENABLE);
   }

   /*����ж�-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����*/
    if(USART_GetFlagStatus(USART3,USART_FLAG_ORE) == SET)
    {
      temp = USART3->SR;
      temp = USART3->CR1;
    }

  /*��SR���DR���Idle �ȶ�SR��Ȼ���DR������� ��USART_ClearFlagע��*/
  /*һ֡���ս��� ����IDLE�����ж� ��������״̬�����IDLE�ж� */
  if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
  {
    temp = USART3->SR;
    temp = USART3->DR;
    DMA_Cmd(DMA1_Channel3, DISABLE);//�ر�DMA,��ֹ�������������
    rxlen = UART_RX_LEN - DMA_GetCurrDataCounter(DMA1_Channel3);  //��ȡ�������ݳ���
    USART3_RX_Finish = 1;//��λ������ɱ�־
    DMA_ClearFlag(DMA1_FLAG_GL3 | DMA1_FLAG_TC3 | DMA1_FLAG_TE3 | DMA1_FLAG_HT3);//���־
    DMA_SetCurrDataCounter(DMA1_Channel3, UART_RX_LEN); //��װ��
    DMA_Cmd(DMA1_Channel3, ENABLE);
  } 
}

/*******************************************************************************
* Function Name  : clear_buf_uart
* Description    : ��鴮�ڽ����ַ�����ӡ����������ջ���
* Input          : std - �����ַ��� index -��OK����λ��
* Output         : None
* Return         : TRUE or FALSE
*******************************************************************************/
void clear_buf_uart(void)
{
  int i;
	if(USART3_RX_Finish!=0)
      {
        rt_kprintf("\r\nUSART3 �������ݳ��ȣ�%d �ֽڡ����ݣ�",rxlen);
        for(i=0;i<rxlen;i++)
        rt_kprintf("%c",USART3_RX_DATA[i]);
        USART3_RX_Finish = 0;
        rxlen = 0;
        DMA3PutString(USART3_RX_DATA);
        memset(USART3_RX_DATA,0x00,UART_RX_LEN);
      }
}

/*******************************************************************************
* Function Name  : dmarevsend DMA���Ժ���
* Description    : usart2�������ݺ�usart1��ӡ����,���ڲ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void dmarevsend(void * paramater )
{
  Usart3Config();
  DMA_USART3_Configuration((uint32_t)(&USART3->DR),USART3_TX_DATA,UART_TX_LEN);
  USART3_DMA_Configuration((uint32_t)(&USART3->DR) ,USART3_RX_DATA,UART_RX_LEN);
  
  while(1)
  {
    clear_buf_uart();
    rt_thread_delay(100);
  }
}


int test_usart3()
{
    rt_thread_t init_thread;

    init_thread = rt_thread_create("dmarevsend",
                                   dmarevsend, RT_NULL,
                                   2048, 8, 20);

    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);

    return 0;
}

FINSH_FUNCTION_EXPORT(test_usart3, startup 3 receive send  e.g. test_usart3());
