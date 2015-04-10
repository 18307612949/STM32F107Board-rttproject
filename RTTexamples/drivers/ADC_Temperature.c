/******************************2012-2013, NJUT, Edu.*************************** 
FileName: adc_temprature.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2013.03.30
Description:    ʹ��Ƭ�ڵ��¶ȴ����������¶�,��ͨ����������¶�ֵ    
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/03/30     1.0     �ļ�����   
*******************************************************************************/ 

#include "ADC_Temperature.h"

uint16_t ADCCov[16]; 
volatile uint8_t ADC_TX_Finish = 0; 
static DMA_InitTypeDef DMA_InitStructure; 

/*******************************************************************************
* Function Name  : ADCTEMP_Config
* Description    : ADC �ڲ��¶ȴ��������� �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADCTEMP_Config(void) 
{ 
  ADC_InitTypeDef ADC_InitStructure; 

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); 
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;      //����ģʽ 
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;       //��ͨ��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;      //����ɨ�� 
  ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_None;//�������ת�� 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;     //�����Ҷ��� 
  ADC_InitStructure.ADC_NbrOfChannel = 1;         //1��ͨ�� 
  ADC_Init(ADC1, &ADC_InitStructure); 
  
  /* ����ͨ��16�Ĳ����ٶ�,������Ϊ�ǲ���,����Ҫ�ܿ�,����Ϊ����*/  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5); 
  
  /* ʹ���ڲ��¶ȴ��������ڲ��Ĳο���ѹ */  
  ADC_TempSensorVrefintCmd(ENABLE);  
  
  /* ����ADC1��DMAģʽ */ 
  ADC_DMACmd(ADC1, ENABLE); 
  
  /* ����ADC1*/ 
  ADC_Cmd(ADC1, ENABLE); 
  
  /*����У׼�Ĵ��� */    
  ADC_ResetCalibration(ADC1); 
  while(ADC_GetResetCalibrationStatus(ADC1)); 
  
  /*��ʼУ׼״̬*/ 
  ADC_StartCalibration(ADC1); 
  while(ADC_GetCalibrationStatus(ADC1)); 
  
  /* �˹���ADCת��.*/  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
} 
 
/*******************************************************************************
* Function Name  : DMA_Config
* Description    : ADC DMA ���� �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA_Config(void) 
{ 
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 

  DMA_DeInit(DMA1_Channel1); 
  
  DMA_InitStructure.DMA_PeripheralBaseAddr  =(u32)( &(ADC1->DR));  //ADC1���ݼĴ��� 
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADCCov;     //��ȡADC������ 
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;     //Ƭ��������Դͷ 
  DMA_InitStructure.DMA_BufferSize = 16;        //ÿ��DMA16������ 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ������ 
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    // �ڴ��ַ���� 
  DMA_InitStructure.DMA_PeripheralDataSize =  DMA_PeripheralDataSize_HalfWord; //���� 
  DMA_InitStructure.DMA_MemoryDataSize =  DMA_MemoryDataSize_HalfWord;   //���� 
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;        //��ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;       //�����ȼ� 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;        //���ڴ浽�ڴ� 
  DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
  
  /*DMA��������жϿ�*/
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);       
  
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
    ADC_TX_Finish = 1; 
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
  uint16_t tmp; 
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
* Function Name  : getadctemp
* Description    : ʹ��Ƭ�ڵ��¶ȴ����������¶�,��ͨ����������¶�ֵ 
* Input          : None
* Output         : None
* Return         : �˲�������
*******************************************************************************/
int GetTemperature(void) 
{ 
 uint16_t adcvalue = 0; 
 DMAReConfig();//��������DMA 
   
  if( ADC_TX_Finish == 1) 
  { 
    ADC_TX_Finish = 0; 
    
    adcvalue = DigitFilter(ADCCov,16);  
    adcvalue = (1430 - adcvalue * 3300/4096)/4.3 + 25;//ת��Ϊ�¶�ֵ 
  }
  rt_kprintf("\r\n ��ǰCPU�¶�Ϊ����%d�� \n\r",adcvalue);
  return adcvalue;
} 
 
/*******************************************************************************
* Function Name  : TemperatureConfig
* Description    : Ƭ�ڵ��¶ȴ�������������
* Input          : None
* Output         : None
* Return         : �˲�������
*******************************************************************************/
void TemperatureConfig(void)
{
  NVIC_Configuration();
  ADCTEMP_Config();
  DMA_Config();
}

