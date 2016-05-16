/******************************2012-2013, NJUT, Edu.*************************** 
FileName: adc.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2016.02.10
Description:    1.adc����,�������˲�  
                2.B0 B1 C0 ʹ��ADC1 ͨ��8��9��10 
Version:         2.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/03/30     1.0     �ļ�����   
      Sundm    15/10/30     1.0     ��������˲�����Filter   
  *          STM32Board Key Pin assignment
  *          =============================
  *          +-----------------------------------------------------------+
  *          |                     Pin assignment                        |
  *          +-----------------------------+-----------------------------+
  *          |      Keys                   |     Port & Pin              |
  *          +-----------------------------+-----------------------------+
  *          |       A1IN                  |         B0                  |
  *          |       A2IN                  |         B1                  |
  *          |       AIN                   |         C0                  |
  *          +-----------------------------+-----------------------------+
*******************************************************************************/ 

#include "adc1.h"
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include <stdio.h>
#include <rtthread.h>

#define ADC1_DR_Address    ((uint32_t)0x4001244C)  //DR_Address�Ĵ�������ַ
#define FilterLen 128
#define Sample_Num 128
#define Channel_Num 3
uint16_t ADCConvertedValue[Sample_Num][Channel_Num];
rt_device_t device2;

/*******************************************************************************
* Function Name  : DMA_Config
* Description    : ADC DMA ���� �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA_Config(void) 
{ 
  DMA_InitTypeDef DMA_InitStructure;     
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;    //DMA��Ӧ���������ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCConvertedValue;   //�ڴ�洢����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA��ת��ģʽΪSRCģʽ����������Ƶ��ڴ�
  DMA_InitStructure.DMA_BufferSize = Sample_Num*Channel_Num;		   //DMA�����С����λΪDMA_MemoryDataSize
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//����һ�����ݺ��豸��ַ��ֹ����
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//�رս���һ�����ݺ�Ŀ���ڴ��ַ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //�����������ݿ��Ϊ16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA�����ݳߴ磬HalfWord����Ϊ16λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //ת��ģʽ��ѭ����
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA���ȼ���
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2Mģʽ����
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);   
  
  /*DMA��������жϿ�*/
  //DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); 
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
} 

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : DMA NVIC���� �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable DMA channel6 IRQ Channel */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : DMAReConfig
* Description    : ADC DMA ��������  �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAReConfig(void) 
{ 
  DMA_InitTypeDef DMA_InitStructure; 
  DMA_DeInit(DMA1_Channel1); 
  DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); 
  DMA_Cmd(DMA1_Channel1, ENABLE); 
} 

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : DMA1 ͨ��1 �жϷ������ DMA ������ȫ��ɺ�����ж�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void) 
{ 
  if(DMA_GetITStatus(DMA1_IT_TC1))//ͨ��1��������ж� 
  { 
    DMA_ClearITPendingBit(DMA1_IT_GL1); //���ȫ���жϱ�־ 
  } 
} 
/*******************************************************************************
* Function Name  : DigitFilter �ڲ�����
* Description    : ����˲� ȡNO��2/5��Ϊͷβ����ֵ,ע��NҪ����5,���򲻻�ȥͷβ 
* Input          : buf �����׵�ַ len ���ݳ���
* Output         : None
* Return         : �˲�������
*******************************************************************************/
uint16_t DigitFilter(uint16_t * buf,uint8_t len) 
{ 
  uint8_t i,j; 
  uint32_t tmp; 
  uint8_t cut_no=0; 
  /*���򣬽�buf[0]��buf[no-1]�Ӵ�С���� */
  for(i=0;i<len;i++) 
  { 
    for(j=0;j<len-i-1;j++) 
    {  
      if(buf[j]>buf[j+1]) 
      {  
        tmp=buf[j]; 
        buf[j]=buf[j+1]; 
        buf[j+1]=tmp; 
      } 
    } 
  } 
  /*lenΪ���Σ��˴��ǽ�len��ǰ2/5���� */
  if(len>5)
  { 
    cut_no=len/5; 
  } 
  /*��ƽ��*/ 
  tmp=0; 
  for(i=cut_no;i<len-cut_no;i++) //��ƽ�� 
  tmp+=buf[i]; 
  return(tmp/(len-2*cut_no)); 
} 
 
/*******************************************************************************
* Function Name  : ADC_Config
* Description    : ADC ����DMA��ʽ����ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;      
 
  /*NVIC Config*/
  NVIC_Configuration();
  
  /* Configure PC.00 (ADC Channel10) as analog input*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure PB0 (ADC Channel8) as analog input*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure PB1 (ADC Channel9) as analog input*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  /* Enable ADC1  */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
  
  /* ADC1 configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //������ת��ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  //����ɨ��ģʽ ��ͨ��
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   //��������ת��ģʽ
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC�ⲿ���أ��ر�״̬,�������ת�� 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //���뷽ʽ,ADCΪ12λ�У��Ҷ��뷽ʽ
  ADC_InitStructure.ADC_NbrOfChannel = Channel_Num;	 //����ͨ������Channel_Num��
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel 8 configuration ADCͨ���飬 ��8��ͨ�� ����˳��1��ת��ʱ�� ����ʱ��Ϊ55.5 ����  */ 
  /* PB0��ΪAI ��ADC_Channel_8*/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);
  
  /* ADC1 regular channel 9 configuration ADCͨ���飬 ��9��ͨ�� ����˳��2��ת��ʱ�� ����ʱ��Ϊ55.5 ���� */ 
  /* PB1��ΪAI ��ADC_Channel_9*/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);

  /*PC10 ADC1 regular channel 10configuration ADCͨ���飬 ��10��ͨ�� ����˳��3��ת��ʱ�� ����ʱ��Ϊ55.5 ���� */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 3, ADC_SampleTime_55Cycles5);
  
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);	  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);  
  
  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);	  
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));  
  /* Start ADC1 calibaration */
  /*��ʼУ׼״̬*/ 
  ADC_StartCalibration(ADC1);		
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));	   
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  
  /* DMA Config */
  DMA_Config();  
  
  /* ����ϵͳ�еĴ���2�豸 */
  device2 = rt_device_find("uart2");
  if (device2!= RT_NULL)
  {
    rt_device_open(device2, RT_DEVICE_OFLAG_RDWR);
  }
  
}

/*******************************************************************************
* Function Name  : getadcvalue
* Description    : ��ȡ��ǰADCֵ
* Input          : None
* Output         : advalue
* Return         : None
*******************************************************************************/
void getadcvalue(uint16_t* adcvalue)
{
  uint16_t Precent[3] = {0};
  uint32_t Voltage[3] = {0};
  uint16_t adcvaluetemp[Sample_Num] = {0};
  uint32_t i,j;
  uint8_t uartbuf[8] = {0};
  
  uartbuf[0]=0x12;
  uartbuf[1]=0x34;
  
  for(i=0;i<Channel_Num;i++ )
  {
    for(j=0;j<Sample_Num;j++)
    {
      adcvaluetemp[j] = ADCConvertedValue[j][i];
    }
    adcvalue[i] = DigitFilter(adcvaluetemp, Sample_Num);
    Precent[i] = (adcvalue[i]*100/0x1000);	
    Voltage[i] = adcvalue[i] * 3300/0x1000;		
    /*������ʾ����ֵ */      
    rt_kprintf("\r\n ��ǰAD[%d]ת�����Ϊ��0x%X, �ٷֱ�Ϊ��%d% ����ѹֵ��%d.%dV \n\r", 
          i, adcvalue[i], Precent[i], Voltage[i]/1000, Voltage[i]%1000);
    uartbuf[i*2+2]=adcvalue[i]>>8;
    uartbuf[i*2+3]=adcvalue[i];
  }
    rt_device_write(device2, 0,uartbuf,8);

}

