/******************************2012-2013, NJUT, Edu.*************************** 
FileName: can.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2013.08.20
Description:    can����      
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/08/20     1.0     �ļ�����   
  *          STM32Board Key Pin assignment
  *          =============================
  *          +-----------------------------------------------------------+
  *          |                     Pin assignment                        |
  *          +-----------------------------+-----------------------------+
  *          |      can                    |     Port & Pin              |
  *          +-----------------------------+-----------------------------+
  *          |      CAN1_TX                |        D1                   |
  *          |      CAN1_RX                |        D0                   |
  *          |      CAN2_TX                |        B6                   |
  *          |      CAN2_RX                |        B5                   |
  *          +-----------------------------+-----------------------------+
******************************************************************************/ 
#include "can.h"


CanRxMsg RxMessage;
CanTxMsg TxMessage;

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : CAN1 �� CAN2 GPIO��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB, ENABLE);
  /* Configure CAN1 RX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
   
  /* Configure CAN2 RX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure CAN1 TX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Configure CAN2 TX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Remap CAN1 and CAN2 GPIOs */
  GPIO_PinRemapConfig(GPIO_Remap2_CAN1 , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_CAN2, ENABLE);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : CAN1 �� CAN2 NVIC��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
 
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : CAN_Config
* Description    : CAN1 �� CAN2 �ṹ����˲�����ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void CAN_Config(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_DeInit(CAN2);
  CAN_StructInit(&CAN_InitStructure);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE); 
  /* CAN1 cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler = 4;
  CAN_Init(CAN1, &CAN_InitStructure);
  CAN_Init(CAN2, &CAN_InitStructure);

  /* CAN1 filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;//0-27
  
  /*can1���ղ�����*/
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;//������
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;//������

  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* CAN2 filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
  /*CAN2��������4����ʶ��*/
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;

  CAN_FilterInitStructure.CAN_FilterIdHigh = (0x7cb<<5);
  CAN_FilterInitStructure.CAN_FilterIdLow = (0x4ab<<5);
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x7ab<<5);
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0x40b<<5);
  
  CAN_FilterInit(&CAN_FilterInitStructure);
  
}

/*******************************************************************************
* Function Name  : Init_RxMes
* Description    : ��ʼ�� a Rx Message
* Input          : CanRxMsg *RxMessage
* Output         : None
* Return         : None
*******************************************************************************/
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t i = 0;

  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (i = 0; i < 8; i++)
  {
    RxMessage->Data[i] = 0x00;
  }
}
/*******************************************************************************
* Function Name  : Init_TxMes
* Description    : ��ʼ�� a Tx Message
* Input          : CanTxMsg *TxMessage
* Output         : None
* Return         : None
*******************************************************************************/
void Init_TxMes(CanTxMsg *TxMessage)
{

  TxMessage->StdId = 0x321;
  TxMessage->ExtId = 0x01;
  TxMessage->RTR = CAN_RTR_DATA;//����֡
  TxMessage->IDE = CAN_ID_STD;//��׼��ʶ�� 11λ
  TxMessage->DLC = 1;//����Ϊ1
  TxMessage->Data[0] = 0xcc;
}

/*******************************************************************************
* Function Name  : CAN1_RX0_IRQHandler
* Description    : CAN1 �����жϴ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
  
	int i;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
  if ((RxMessage.StdId == 0x321)&&(RxMessage.IDE == CAN_ID_STD))
  {
    rt_kprintf("\r\ncan1 receive:");
    for(i=0;i<RxMessage.DLC;i++)
    {
      rt_kprintf(" 0x%02X ",RxMessage.Data[i]);
    }
    rt_kprintf("\r\n");
  }
}

/*******************************************************************************
* Function Name  : CAN2_RX0_IRQHandler
* Description    : CAN2 �����жϴ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN2_RX0_IRQHandler(void)
{
  
	int i;
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
  //if ((RxMessage.StdId == 0x7cb)&&(RxMessage.IDE == CAN_ID_STD))
  {
    rt_kprintf("\r\ncan2 receive:");
    for(i=0;i<RxMessage.DLC;i++)
    {
      rt_kprintf(" 0x%02X ",RxMessage.Data[i]);
    }
    rt_kprintf("\r\n");
  }
}

/*******************************************************************************
* Function Name  : canconfig
* Description    : ��ʼ�� can ȫ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void canconfig(void)
{
  GPIO_Configuration();
  NVIC_Configuration();
  CAN_Config();
  /* IT Configuration for CAN1 */  
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
  /* IT Configuration for CAN2 */  
  CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}

/*******************************************************************************
* Function Name  : can1send
* Description    : can1�����ַ�data
* Input          : data ���� number���ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void can1send(uint8_t* data,uint8_t number)
{
  int i;
	TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;//����֡
  TxMessage.IDE = CAN_ID_STD;//��׼��ʶ�� 11λ
  TxMessage.DLC = number;//����
  for( i=0;i<number;i++)
  {
    TxMessage.Data[i] = data[i];
  }
  CAN_Transmit(CAN1, &TxMessage);
}
/*******************************************************************************
* Function Name  : can2send
* Description    : can2�����ַ�data
* Input          :  data ���� number���ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void can2send(uint8_t* data,uint8_t number)
{
  int i;
	TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;//����֡
  TxMessage.IDE = CAN_ID_STD;//��׼��ʶ�� 11λ
  TxMessage.DLC = number;//����
  for(i=0;i<number;i++)
  {
    TxMessage.Data[i] = data[i];
  }
  CAN_Transmit(CAN2, &TxMessage);
}


