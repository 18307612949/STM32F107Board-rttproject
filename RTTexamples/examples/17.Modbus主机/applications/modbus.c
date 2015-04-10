/*********************2011-2012, NJUT, Edu. 
  FileName: modbus.c 
  Author:  �ﶬ÷       Version :  1.0        Date: 2012.11.25
  Description:     modbusЭ�����ڵ㷢�ͣ����ս��� ����407      
  Version:         1.0 
  Function List:    
    1. MODBUS 03���ܺţ�������Ĵ��� 
    2.TIM5����t1.5���
  History:         
      <author>  <time>   <version >   <desc> 
      Sundm    12/11/25     1.0     �ļ�����   
      Sundm    13/06/10     2.0     RS485�����ļ�   
      Sundm    13/10/17     2.1     ��������ʱ�䣬֡������ModbusDelay(100,Inverter.Comm); 1Ϊ֡�䣩��
                                    ֡���ֽڼ�� ��TIM_TimeBaseStructure.TIM_Period = (1000 - 1);1000Ϊ�ֽڼ䣩  
***********************************************************/ 
#include "modbus.h"
#include "CommonString.h"

/*����Э��״̬�ַ�*/
typedef enum
{
	LISTENING,FINISHED
}TYPEDEF_COMM;

/*����MODBUSЭ��˿ڽṹ��*/
struct inverte
{   
  uint8_t SendBuf[16] ;   
  uint8_t RecvBuf[128];
  uint8_t Length;
  TYPEDEF_InverterERR Error;
  TYPEDEF_COMM Comm;
}  Inverter; 

/*******************************************************************************
* Function Name  : delay_ms
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void delay_ms(uint32_t ms)    
{ 
	uint32_t i,j; 
	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 1092; j++ )
                {
                  
                }
	}
} 

/*******************************************************************************
* Function Name  : CalCRC
* Description    : CRCУ�麯��������CRC�ַ�
* Input          : Messageԭ�� DataLenth ԭ�볤�� 
* Output         : CRCAddr ��������CRC��ַ
* Return         : None
*******************************************************************************/
void CalCRC(uint8_t *Message, uint16_t DataLenth, uint16_t *CRCAddr)    
{    
  /* CRC ��λ�ֽ�ֵ�� */    
    u8 CRC_TableHigh[256] = {  
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,    
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,    
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,    
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,    
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,    
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,    
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,    
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,    
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,    
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,    
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,    
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,    
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,    
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,    
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,    
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,    
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,    
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,    
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,    
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,    
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,    
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,    
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,    
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,    
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,    
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40  
    };    
      
    u8 CRC_TableLow[256] = {  
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,    
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,    
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,    
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,    
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,    
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,    
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,    
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,    
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,    
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,    
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,    
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,    
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,    
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,    
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,    
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,    
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,    
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,    
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,    
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,    
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,    
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,    
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,    
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,    
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,    
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40  
    };  
      
    u8 CRC_InitHigh = 0xFF ; /* ��CRC�ֽڳ�ʼ�� */    
    u8 CRC_InitLow = 0xFF ; /* ��CRC �ֽڳ�ʼ�� */    
    u8 CRC_Index = 0x00; /* CRCѭ���е����� */    
      
    while(DataLenth--) /* ������Ϣ������ */    
    {    
        CRC_Index = CRC_InitHigh ^ *Message++ ; /* ����CRC */    
        CRC_InitHigh = CRC_InitLow ^ CRC_TableHigh[CRC_Index] ;    
        CRC_InitLow = CRC_TableLow[CRC_Index] ;    
    } 
    *CRCAddr = ((u16) CRC_InitLow << 8 | CRC_InitHigh);
}  

/*******************************************************************************
* Function Name  : ModbusDelay
* Description    : MODBUS����ʱ������COMM�ı���˳�
* Input          : time��ʱʱ��  Comm��ǰ״̬ 
* Output         : None
* Return         : None
*******************************************************************************/
void InverterDelay ( uint16_t time,TYPEDEF_COMM  Comm) /**/
{
  uint16_t t=0;
  while((Inverter.Comm == Comm) &&( t < time ))
  {
    rt_thread_delay(1);//��ʱ10ms
    t++;
  } 
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
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);		//��� TIM5���жϴ�����λ	
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
* Function Name  : ReadInverter
* Description    : MODBUS��03���ܺţ�������Ĵ���  ��ֵ���� value��
* Input          : �豸��ַ���豸��Ҫ��ȡ�Ĵ����ĵ�ַ���������ݸ����������ʣ�
                  У��λ(0-�� 1-�� 2-ż 3-��2��ֹͣλ),�������ݴ�ŵĵ�ַ 
* Output         : value ���ֵ��ַ
* Return         : None
*******************************************************************************/
TYPEDEF_InverterERR ReadInverter(uint8_t InverterID, uint16_t Addr,
                  uint8_t N,uint16_t BandRate, uint8_t channelparity, uint8_t *value) 
{
     uint16_t  CRC16[1];
     uint8_t i;
     /* �ض�ʱ��*/
     TIMDisable();
      Inverter.Length=0;
     /*�ڷ��ͻ�������������*/
     memset(Inverter.SendBuf,0,16);
     Inverter.SendBuf[0] = InverterID;
     Inverter.SendBuf[1] = 0x03;
     Inverter.SendBuf[2] = Addr>>8;
     Inverter.SendBuf[3] = Addr&0XFF;
     Inverter.SendBuf[4] = 0;
     Inverter.SendBuf[5] = N;
     /*����CRC16�����뷢�ͻ���β*/
      CalCRC(Inverter.SendBuf,6,(uint16_t *)Inverter.SendBuf+3);
    /*ѡ��ͨ�������ò����ʣ������ж�ʹ�ܣ���RS4851����״̬*/
     Usart2Config( BandRate,channelparity);
     delay_ms(10);
     RS485Write();
     delay_ms(10);

    /*�������ݻ���*/
     for (i=0;i<8;i++)
     {
       Usart2PutChar(Inverter.SendBuf[i]);
     }
    /*��RS485����״̬*/
     RS485Read();   
    /*����ջ���*/
     memset(Inverter.RecvBuf,0,128);
     /*����״̬*/
     Inverter.Comm=LISTENING;
    /*�ȶ���ʱ1000ms����Inverter.Comm��ֵ�仯���˳�*/
     InverterDelay(100,Inverter.Comm); 
     
    /*ȫ���ر�*/
      //RS485OFF();
      
     Inverter.Error = InverterOK;
     /*���ջ��������ݳ���Ϊ0�����ʾ��ʱ��Ӧ��*/
     if(Inverter.Length==0)
     { 
         Inverter.Error=TimeOut;   rt_kprintf(" �Ǳ�Ӧ��ʱ"); /*("�Ǳ�Ӧ��ʱ");*/
          return TimeOut ;
     }
      Inverter.Comm=FINISHED;
      /*����У�� ������ԣ����ˮ��ר�Ŵ���*/
     if(Inverter.RecvBuf[2]!=Inverter.Length-5)
     {
        Inverter.Error=FrameErr;rt_kprintf(" ���ݳ��ȴ���"); 
        rt_kprintf(" \r\n");
        for(i=0;i<Inverter.Length;i++)
        {
          rt_kprintf(" %02X",Inverter.RecvBuf[i]);
        }
        rt_kprintf(" \r\n");
        i = 0;
        for(i=0;i<Inverter.Length;i++)
        {
          if(Inverter.RecvBuf[i]==InverterID)
            break;
        }
        if (i==Inverter.Length) 
         return FrameErr ; /*("���ݳ��ȴ���");*/
        DeleteString(Inverter.RecvBuf, Inverter.Length, 0, i);
        Inverter.Length = Inverter.RecvBuf[2]+5;
     }
     CalCRC(Inverter.RecvBuf,Inverter.Length-2,CRC16);
     if(*(uint16_t *)(CRC16)!=*(uint16_t *)(Inverter.RecvBuf+Inverter.Length-2))
     {
        Inverter.Error=CheckErr;rt_kprintf(" У�����"); 
        return CheckErr; /*("У�����");*/
     }
      /*У��һ���������Ǳ�����ɹ�*/
     else if(Inverter.RecvBuf[0]==InverterID)
     {
         rt_kprintf(" ��������ɹ�,�ֽ�����%d \n\r",Inverter.RecvBuf[2]); //�洢�ֽڳ���
         Inverter.Error=InverterOK; /*("��������ɹ���");*/
         for(i=0;i<Inverter.Length;i++)
         {
           rt_kprintf(" 0x%02X",Inverter.RecvBuf[i]); /*�洢�ֽ�*/
           *value =Inverter.RecvBuf[i];value++;
         }
           rt_kprintf("\n\r");
       /*У��һ���������Ǳ�����ɹ�*/
        return InverterOK;
      }
     return InverterOK;
 }

/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : ��ʱ�жϣ���ʱû�ж������ݣ�֡�������
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
 void TIM5_IRQHandler(void)
 {
     if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
     {
         TIM_ClearITPendingBit(TIM5, TIM_IT_Update);//��� TIMx ���жϴ�����λ
         TIM_ClearFlag(TIM5, TIM_IT_Update);//��� TIM5���жϴ�����λ
         Inverter.Comm=FINISHED; //�˿�״̬����
         TIMDisable();
     }
 }

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : ����1�����жϣ������ջ�����������ʱ�������1.5t�ж�ʱ��
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
 {
    unsigned char temp; 
   //�ض�ʱ��
     TIMDisable();

    //����жϱ�־λ
    if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET) 
    { 
        Inverter.RecvBuf[Inverter.Length]=USART_ReceiveData(USART2);;
        Inverter.Length++;
    }
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
    { 
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
    //����ʱ��TIM5
     TIMEnable();
 } 

/*******************************************************************************
* Function Name  : Timer5Config
* Description    : Timer5��ʱ�����ú��� 
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void Timer5Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;  	
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//ʹ��ʱ��TIM5

    //��ʱ��ʱ��Ƶ����Ϊ��72M/7200 = 10000
    //��ʱʱ�䣺1/10000 = 0.1mS	 1000*0.1mS=100mS
    TIM_TimeBaseStructure.TIM_Period = (1000 - 1);		//100ms��ʱ 1200bit/s,3.5�ֽ�ʱ��28λ��23ms��	40msҲ�����õ�ˮ����
    TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);	//Ԥ��Ƶ��  7200	
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;		//��ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)����Ƶ��֮��ķ�Ƶ���� 	
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ��� 	
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);		//��ʼ��TIM5
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
    //�жϿ���������
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
    NVIC_Init(&NVIC_InitStructure);	                     
    
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);		//��� TIM5���жϴ�����λ	
    TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);		//ʹ��ָ���� TIM 5�ж�  	
    TIM_Cmd(TIM5, DISABLE);				//ʹ��TIM5 ���� 
}	
   
/*******************************************************************************
* Function Name  : ModbusInit
* Description    : Modbus���ó�ʼ������ 
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void Bus485Init(void)
{
  Timer5Config();
  RS485GPIOConfig();
  //RS485OFF();
}

/*******************************************************************************
* Modbus����վ���Գ���
* 1.���ö�ʱ����1.5t��֡���ֽڼ����ʱ
* 2.����ReadInverter��������ȡ��վĳ����ַ����
* 3.������void ReadInverter(BandRate,Parity,InverterID,Addr,N)
* ����������豸�ţ��豸��Ҫ��ȡ�Ĵ����ĵ�ַ���������ݸ���
*     �����ʣ��������ݴ�ŵĵ�ַ
* �����modbus �������
********************************************************************************/
static void test_modbus(uint16_t BandRate,uint8_t parity,uint8_t InverterID, 
                       uint16_t Addr, uint8_t N)
{
	uint8_t Value[128];
	int i;
        Bus485Init();
	if (ReadInverter(InverterID, Addr,N,BandRate,parity,Value)==InverterOK)
        {
          rt_kprintf("�������ݣ� ");
          for ( i=0;i<N;i++)
          {
                  rt_kprintf(" 0x%02X",Value[i*2+3]);rt_kprintf("%02X ",Value[i*2+4]);
          }
          rt_kprintf(" \n\r ");
        }
}

#ifdef  RT_USING_FINSH 
 #include  <finsh.h> 
FINSH_FUNCTION_EXPORT (test_modbus, (BandRate parity InverterAddr DataAddr Len) e.g.test_modbus(9600,2,1,0,2)); 
#endif 


//////////////////////����DL645��Լ/////////////////////////////////////////////


/*******************************************************************************
* Function Name  : CalSUM
* Description    : �麯�������ɸ��ֽ�2���������ͣ�������256
* Input          : Messageԭ�� DataLenth ԭ�볤�� 
* Output         : CRCAddr ��������CRC��ַ
* Return         : None
*******************************************************************************/
uint8_t CalSUM(uint8_t *Message, uint8_t DataLenth)    
{    
  uint8_t sum = 0;
	uint8_t i;
  for(  i=0; i<DataLenth; i++ )  /*����У����*/
     {
      sum = sum + *Message; 
      Message++;
     }
  return sum;
}  

/*******************************************************************************
* Function Name  : Read645Inverter
* Description    : DL645������01����ȡ����, value��
* Input          : ��ʶ���룬�豸�ڵ�ַ�������ʣ�У��λ(0-�� 1-�� 2-ż 3-��У��2ֹͣ)��
*                  �������ݴ�ŵĵ�ַ���������ݵĳ��� 
* Output         : value ���ֵ��ַ
* Return         : None
*******************************************************************************/
TYPEDEF_InverterERR Read645Inverter(uint16_t SignCode, uint8_t Addr,uint8_t *len, 
                  uint16_t BandRate, uint8_t channelparity, uint8_t *value) 
{
  uint8_t i;
   memset(Inverter.SendBuf,0,16);
   /* �����ó�ֵ��*/
     TIMDisable();
      Inverter.Length=0;
     /*�ڷ��ͻ�������������*/
     memset(Inverter.SendBuf,0,16);
     Inverter.SendBuf[0] = 0x68;
     Inverter.SendBuf[1] = Addr;
     Inverter.SendBuf[2] = 0;
     Inverter.SendBuf[3] = 0;
     Inverter.SendBuf[4] = 0;
     Inverter.SendBuf[5] = 0;
     Inverter.SendBuf[6] = 0;
     Inverter.SendBuf[7] = 0x68;
     Inverter.SendBuf[8] = 0x01;
     Inverter.SendBuf[9] = 0x02;
     Inverter.SendBuf[10] = SignCode+0x33;
     Inverter.SendBuf[11] = (uint8_t)(SignCode>>8)+0x33;
     Inverter.SendBuf[12] = CalSUM(Inverter.SendBuf,12); 
     Inverter.SendBuf[13] = 0x16;
    /*ѡ��ͨ�������ò����ʣ������ж�ʹ�ܣ���RS4851����״̬*/
     Usart2Config( BandRate,channelparity);
     delay_ms(10);
     RS485Write();
     delay_ms(10);
     
    /*����ǰ���ֽڣ��Ի��ѶԷ�*/
     Usart2PutChar(0xFE);delay_ms(10);
     Usart2PutChar(0xFE);delay_ms(10);
     Usart2PutChar(0xFE);delay_ms(10);

    /*�������ݻ���*/
     for (i=0;i<14;i++)
     {
       Usart2PutChar(Inverter.SendBuf[i]);
     }
    /*��RS4851����״̬*/
     RS485Read();   

    /*����ջ���*/
    memset(Inverter.RecvBuf,0,128);
     /*����״̬*/
     Inverter.Comm=LISTENING;
    /*�ȶ���ʱ500ms����Inverter.Comm��ֵ�仯���˳�*/
     InverterDelay(50,Inverter.Comm); 
     
    /*ȫ���ر�*/
      //RS485OFF();

      Inverter.Error = InverterOK;
     /*���ջ��������ݳ���Ϊ0�����ʾ��ʱ��Ӧ��*/
     if(Inverter.Length==0)
     { 
         Inverter.Error=TimeOut;   rt_kprintf(" �Ǳ�Ӧ��ʱ"); /*("�Ǳ�Ӧ��ʱ");*/
          return TimeOut ;
     }
      Inverter.Comm=FINISHED;
     
     
      /*����У��*/
     if(Inverter.RecvBuf[9]!=Inverter.Length-12)
     {
        Inverter.Error=FrameErr;rt_kprintf(" ���ݳ��ȴ���"); 
         return FrameErr ; /*("���ݳ��ȴ���");*/
     }
     if(CalSUM(Inverter.RecvBuf,(10+Inverter.RecvBuf[9]))!=Inverter.RecvBuf[(Inverter.RecvBuf[9]+10)])
     {
        Inverter.Error=CheckErr;rt_kprintf(" У�����"); 
        return CheckErr; /*("У�����");*/
     }
      /*У��һ���������Ǳ�����ɹ�*/
     else if((Inverter.RecvBuf[10]==Inverter.SendBuf[10])
              &&(Inverter.RecvBuf[11]==Inverter.SendBuf[11]))
     {
         rt_kprintf(" ��������ɹ�,�ֽ�����%d \r\n",Inverter.RecvBuf[9]); //�洢�ֽڳ���
         Inverter.Error = InverterOK; /*("��������ɹ���");*/
         for(i=0;i<Inverter.RecvBuf[9];i++)
         {
           *value =Inverter.RecvBuf[10+i]-0x33;
           rt_kprintf(" 0x%02X",*value); /*�洢�ֽ�*/
           value++;
         }
           rt_kprintf("\n\r");
       /*У��һ���������Ǳ�����ɹ�*/
       *len = Inverter.RecvBuf[9];
        return InverterOK;
      }
     else
     {
       rt_kprintf(" ����δ֪����,");
         rt_kprintf(" ʵ���ֽ�����%d \r\n",Inverter.Length); //�洢�ֽڳ���
         for(i=0;i<Inverter.RecvBuf[9];i++)
         {
           *value =Inverter.RecvBuf[10+i]-0x33;
           rt_kprintf(" 0x%02X",*value); /*�洢�ֽ�*/
           value++;
         }
         return UnknowErr;
     }
 }
/*******************************************************************************
* DL645�����ܱ���Գ���
* 1.���ö�ʱ����500ms��֡���ֽڼ����ʱ
* 2.����Read645Inverter��������ȡĳ����ʶ��������
* 3.������TYPEDEF_InverterERR Read645Inverter(uint16_t SignCode, uint8_t Addr,uint8_t *len, 
                  uint16_t BandRate, uint8_t channelparity, uint8_t *value) 
* ����������豸�ţ��豸��Ҫ��ȡ�Ĵ����ĵ�ַ���������ݸ�����
*     �����ʣ�У��λ���������ݴ�ŵĵ�ַ���������ݳ���
* �������
********************************************************************************/
static void test_645(uint16_t BandRate, uint8_t channelparity,
                        uint16_t SignCode, uint8_t Addr)
{
  uint8_t Value[128] = {0x00};
        uint8_t len;
	int i;
        
        Bus485Init();
        
	if (Read645Inverter(SignCode, Addr, &len,  BandRate,channelparity, Value)==InverterOK)
        {
          rt_kprintf("�������� ���ȣ�%d \r\n",len );
          for ( i=0;i<len;i++)
          {
                  rt_kprintf(" 0x%02X ",Value[i]);
          }
          rt_kprintf(" \n\r ");
        }
}

#ifdef  RT_USING_FINSH 
 #include  <finsh.h> 
FINSH_FUNCTION_EXPORT (test_645, (BandRate parity InverterAddr DataAddr ) e.g.test_645(9600,2,36880,1)); 

#endif 

