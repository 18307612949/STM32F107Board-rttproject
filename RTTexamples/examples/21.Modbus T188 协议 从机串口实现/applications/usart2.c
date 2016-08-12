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

#define  UART_RX_LEN 64
#define  UART_TX_LEN 64

#define USART2_PORT				GPIOD

#define USART2_PORT_APB2Periph	RCC_APB2Periph_GPIOD
#define USART2_CLK_APB1Periph		RCC_APB1Periph_USART2

#define USART2_TX_PIN				GPIO_Pin_5
#define USART2_RX_PIN				GPIO_Pin_6	

/*����Э��״̬�ַ�*/
typedef enum
{
  LISTENING, DATAIN, FINISHED,
}TYPEDEF_STA;

/*
 *ʹ��usart2
 *ʹ��timer5
 */
struct rs485struct
{   
  uint8_t recvBuf[UART_RX_LEN] ;
  uint16_t rxlength;
  TYPEDEF_STA state;
}  rs485 ; 

/*******************************************************************************
* Function Name  : Timer5Config
* Description    : Timer5��ʱ�����ú���,��ʱ���ڣ�30m ��Ƶ��1/7200 ���ϼ���300
*                  72M * 1/7200 *300 = 30mS
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Timer5Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;  	
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

  TIM_TimeBaseStructure.TIM_Period = (300 - 1);		 	
  TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);	
  TIM_TimeBaseStructure.TIM_ClockDivision = 0 ;			
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);		
  
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);		
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);		  	
  TIM_Cmd(TIM5, DISABLE);				       

  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
  NVIC_Init(&NVIC_InitStructure);	                       
}	

/*******************************************************************************
* Function Name  : TIMEnable
* Description    : ��ʱ����
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
static void TIMEnable(void)
{
    TIM_SetCounter(TIM5,0x0000);
    TIM_Cmd(TIM5, ENABLE);				            
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);	//��� TIM5���жϴ�����λ	
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);		//ʹ��ָ���� TIM 5�ж�  	
}

/*******************************************************************************
* Function Name  : TIMDisable
* Description    : ��ʱ����
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
static void TIMDisable(void)
{
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);		//��� TIM5���жϴ�����λ	
    TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);		//ʧ��ָ���� TIM 5�ж�  		
    TIM_Cmd(TIM5, DISABLE);				//ʧ��TIM4���� 
}
 
/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : ��ʱ��5�жϷ�����
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    
    rs485.state = FINISHED; //�˿��յ����ݽ���
    TIMDisable();        
  }
}

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
  USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);
  
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
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
  uint8_t rxdata;
   //�ض�ʱ��
  TIMDisable();
  rs485.state = DATAIN;//�˿��յ�����
  if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET) 
  {
    /* Read one byte from the receive data register */
    rxdata = USART_ReceiveData(USART2);
    rs485.recvBuf[rs485.rxlength] = rxdata;
    rs485.rxlength++;
    if(rs485.rxlength >= UART_RX_LEN) 
    {
      rs485.rxlength = 0;
    }
  }
  //����ʱ��TIM15
  TIMEnable();
}


TYPEDEF_STA getrs485state(void)
{
  return rs485.state;
}


void printrs485data(void)
{
  rt_kprintf("\r\n\r\n��������: ");
  for(int i=0; i<rs485.rxlength;i++)
  {
    rt_kprintf("%02x ",(char const *)rs485.recvBuf[i]);
  }
  rt_kprintf("\r\n");
  
}

void clrrs485state(void)
{
  rs485.state = LISTENING;//�˿ڴ��ڼ���״̬
  memset(&rs485.recvBuf[0],0x00,UART_RX_LEN);
  rs485.rxlength = 0;
}

void statechange(TYPEDEF_STA newstate)
{
  rs485.state = newstate;
}

/*******************************************************************************
* Function Name  : test_usart2
* Description    : ʵ�ִ����յ����ٴ��Լ��Ĵ��ڷ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void rt_T188_thread_entry(void* parameter)
{
    while(1)
    {
        rt_thread_delay(10);
        if(getrs485state()==FINISHED)
        {
          printrs485data();
           RS485Write() ;
          
          extern uint8_t T188Handle(unsigned char* RevBuf,uint32_t RevLen);
          T188Handle(rs485.recvBuf,rs485.rxlength);
          
          clrrs485state();
          RS485Read() ;
        }

    }
}

/*******************************************************************************
* Function Name  : test_usart2
* Description    : ʵ�ִ����յ����ٴ��Լ��Ĵ��ڷ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void test_T188(void )
{
  rt_thread_t T188_thread;
  
  RS485GPIOConfig();
  Usart2Config();  
  RS485Read() ;
  clrrs485state();
  Timer5Config();
  TIMDisable();
  T188_thread = rt_thread_create("testT188", rt_T188_thread_entry, RT_NULL, 0x400,
                                0x10, 20);
  if(T188_thread != RT_NULL)
      rt_thread_startup(T188_thread);
}



/*******************************************************************************
* Function Name  : test_usart2
* Description    : ʵ�ִ����յ����ٴ��Լ��Ĵ��ڷ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void rt_Modbus_thread_entry(void* parameter)
{
    while(1)
    {
        rt_thread_delay(10);
        if(getrs485state()==FINISHED)
        {
          printrs485data();
           RS485Write() ;
          
          extern uint8_t ModbusHandle(unsigned char* RevBuf,uint32_t RevLen);
          ModbusHandle(rs485.recvBuf,rs485.rxlength);
          
          clrrs485state();
          RS485Read() ;
        }

    }
}

/*******************************************************************************
* Function Name  : test_usart2
* Description    : ʵ�ִ����յ����ٴ��Լ��Ĵ��ڷ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void test_Modbus(void )
{
  rt_thread_t T188_thread;
  
  RS485GPIOConfig();
  Usart2Config();  
  RS485Read() ;
  clrrs485state();
  Timer5Config();
  TIMDisable();
  T188_thread = rt_thread_create("testT188", rt_Modbus_thread_entry, RT_NULL, 0x400,
                                0x10, 20);
  if(T188_thread != RT_NULL)
      rt_thread_startup(T188_thread);
}

#ifdef  RT_USING_FINSH 
  #include  <finsh.h> 
  FINSH_FUNCTION_EXPORT(test_T188, startup usart2 receive send e.g. test_T188());
  FINSH_FUNCTION_EXPORT(test_Modbus, startup usart2 receive send e.g. test_Modbus());
#endif
