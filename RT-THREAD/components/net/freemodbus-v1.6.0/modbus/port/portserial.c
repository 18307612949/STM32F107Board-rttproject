/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */
#include <rtthread.h>
#include "port.h"
#include "rs485.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "stm32f10x.h"

#define USART2_PORT				GPIOD

#define USART2_PORT_APB2Periph	RCC_APB2Periph_GPIOD
#define USART2_CLK_APB1Periph		RCC_APB1Periph_USART2

#define USART2_TX_PIN				GPIO_Pin_5
#define USART2_RX_PIN				GPIO_Pin_6	


/*******************************************************************************
* Function Name  : vMBPortClose
* Description    : ���ƴ��ڵĹر�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void vMBPortClose(void)//���ڹر�
{
	USART_ITConfig(USART2, USART_IT_TXE | USART_IT_RXNE, DISABLE);
	USART_Cmd(USART2, DISABLE);
}

/*******************************************************************************
* Function Name  : vMBPortSerialEnable
* Description    : ���ƴ��ڵ��շ��жϺ��շ�ʹ��
* Input          : xRxEnable ����ʹ��,  xTxEnable ����ʹ�� 1Ϊʹ�� 0Ϊʧ��
* Output         : None
* Return         : None
*******************************************************************************/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable ) 
{
	int delaytime =10000;
        if(TRUE==xRxEnable)
	{
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
                while(delaytime--);//ת���ɽ���״̬Ҫ���еȴ��������ݷ�����
                RS485Read() ; //����״̬

	}
	else
	{
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);	
	}

	if(TRUE==xTxEnable)
	{
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
                RS485Write() ; 
	}
	else
	{
	   USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
           RS485Read() ; 
	}
}

/*******************************************************************************
* Function Name  : xMBPortSerialInit
* Description    : ���ڳ�ʼ������ Ŀǰ�����޸Ķ˿ں�
* Input          : ucPORT���ں� ulBaudRate ������ ucDataBits����λ���� eParity��żλ
* Output         : None
* Return         : None
*******************************************************************************/
BOOL 
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RS485GPIOConfig();
  
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
  
  /* USART configuration */
  USART_InitStructure.USART_BaudRate   = ulBaudRate;
  USART_InitStructure.USART_StopBits   = USART_StopBits_1;
  switch ( eParity )
  {
    case MB_PAR_NONE:                   /*!< No parity. */
      USART_InitStructure.USART_WordLength = USART_WordLength_8b;
      USART_InitStructure.USART_Parity     = USART_Parity_No;
      break;
      case MB_PAR_ODD:                  /*!< Odd parity. */
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        USART_InitStructure.USART_Parity     = USART_Parity_Odd;
        break;
      case MB_PAR_EVEN:      /*!< Even parity. */
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        USART_InitStructure.USART_Parity     = USART_Parity_Even;
        break;
      default:
        break;
  }
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
  /* Enable USART */
  USART_Cmd(USART2, ENABLE);	
  
  /*��֤���͵ĵ�һ���ֽڲ��ᶪʧ*/
  USART_ClearFlag(USART2, USART_FLAG_TC);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
		
  return TRUE;
}

/*******************************************************************************
* Function Name  : xMBPortSerialPutByte
* Description    : ���ڷ���һ���ֽ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
  USART_SendData(USART2, ucByte);
  return TRUE;
}

/*******************************************************************************
* Function Name  : xMBPortSerialPutByte
* Description    : ���ڽ���һ���ֽ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
  *pucByte = USART_ReceiveData(USART2);
  return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
 void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR( void )
{
     pxMBFrameCBByteReceived(  );
}

void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  {
    prvvUARTRxISR();//�����ж�
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);		
  }
  
  if(USART_GetITStatus(USART2, USART_IT_TXE) == SET)
  {
    prvvUARTTxReadyISR();//��������ж�
    USART_ClearITPendingBit(USART2, USART_IT_TC);
  }
  if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
  {
    USART_ClearFlag(USART2,USART_FLAG_ORE);//��SR
    USART_ReceiveData(USART2);		//��DR
  }
}

