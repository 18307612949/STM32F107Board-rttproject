/*
I2C_ee.C �������� ����Ӳ��I2C��ѯ��ʽ

*/
#include "I2C_ee.h"
#define DEVICE_ADDRESS 0xA0
/*******************************************************************************
* Function Name  : IIC_Configuration
* Description    : ��ʼ��IIC2����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IIC_Configuration(void)
{
  GPIO_InitTypeDef	 GPIO_InitStructure;
  I2C_InitTypeDef    I2C_InitStructure;

  /*��ʱ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);		 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 ,ENABLE);  

  /*����GPIO��Ϊ��©��� ����*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	
  GPIO_Init(GPIOB,&GPIO_InitStructure);			

  /*����I2C*/
  I2C_InitStructure.I2C_ClockSpeed = 50000;          /*ʱ��Ƶ��400kHz */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;        
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  /*CCR�Ĵ���ռ�ձ� 50% */
  I2C_InitStructure.I2C_OwnAddress1 = 0xB0 ;         /*��Ϊ�ӻ�ʱ�ĵ�ַ*/
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;        /*Ӧ��ʹ�� */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; /*7λ��ַ*/
  I2C_Init(I2C1,&I2C_InitStructure);

  /*ʹ��I2C*/
  I2C_Cmd(I2C1, ENABLE);
}


/*******************************************************************************
* Function Name  : IIC_Wait_Eeprom
* Description    : �ȴ�IIC���߿��У�ͬʱ�ȴ��豸���У���������������ȫû�б�Ҫ�ӣ����������������Ƶ�ϸ���д����������ģ�
                   �Ǹ��жϺ���while����Ķ�����������ģ����ı����ǵ�ADDR=1��������д���������𣿲��԰�
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void IIC_Wait_EEprom(void)
{
//	while((I2C_ReadRegister(I2C1, I2C_Register_SR2) & 0x0002))         /*�ж������Ƿ�Ϊ���ã�Ҳ����SR2_BUSY�Ƿ�Ϊ0��Ϊ0��ʾ���߲�æ*/
//	{}                                                               
//  do
//  {
//    I2C_GenerateSTART(I2C1, ENABLE);                                 /* ������ʹ�������ú�����CR1��startλ�����Ϊ1��
//																																		 ����ʹ����������Ӳ���Զ���Ϊ0 */  
//		
//		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)) /*�ж�EV5,���º���������Է��֣����¼���SB=1��MSL=1��BUSY=1 
//	  {}                                                         ��˼����ʼ�����Ѿ������ˣ�Ȼ������ģʽ��������ͨѶ*/
//   /* while(!(I2C_ReadRegister(I2C2, I2C_Register_SR1) & 0x0001));*//*�ȴ�SR1_SBΪ1��ʾ��ʼ�����Ѿ������ˣ������æ�أ�
//		                                                                 �������ǲ����ظ�������ʼ����������Ӱ�쵽���ˣ���һ���
//		                                                                 ���ж������Ƿ�Ϊæ*/
//		
//    I2C_Send7bitAddress(I2C1, DEVICE_ADDRESS, I2C_Direction_Transmitter);/* ���������ĵ�ַ����д��DR�Ĵ�������Ĭ�ϵ�������
//		                                                                     ��ַ����8λ��ɵģ���7λ�ǵ�ַ��bit0����0��
//		                                                                     ��I2C_Direction_Transmitter
//                                                                         �Ὣbit0�ĳ�0������д�ĺ��壻
//		                                                                     �����I2C_Direction_Receiver��Ὣbit0���1��
//		                                                                     ������յĺ��壬�μ�eeprom�����ַλ��
//                                                                         �����λR/W�ĺ���*/
//  }
//	while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));/*�ǿ�SR1�Ĵ�����ADDRλ�Ƿ�Ϊ1��1���˳�while��������Ǽ���ѭ��
//	                                                             �������ж�������eeprom�Ƿ�Ϊ���ã�Ҳ���������Ƿ񷢳�Ӧ����Ӧ
//	                                                             ��˵��������æ*/
//	
//  I2C_ClearFlag(I2C1, I2C_FLAG_AF);	                          /* ���SR1��AF��־λ��AFΪ1��ʾӦ��ʧ�ܣ�0��ʾ�ɹ���Ҫ������㣬
//	                                                             �����˳�while����ζ��Ӧ��ɹ��� */




vu16 SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    /* Read I2C1 SR1 register */
    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, DEVICE_ADDRESS, I2C_Direction_Transmitter); //��������仰ֻ��д���˵�ַ��DR�Ĵ������棬������һ���while
		                                                                      //�ж�ʱ��̣ܶ�Ҫ�����ݻ�û���꣬�豸Ҳû��Ӧ��Ҳ����ADDR=0��
		                                                                      //���Ѿ�����while����Ķ�SR1�ˣ������ǳ���ȥwhile�ˣ�
  }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);	


}

/*******************************************************************************
* Function Name  : IIC_Byte_Write
* Description    : ��eerpom��at24c02����д��������һ���ֽڵ�д������
* Input          : u8* pbuffer, u8 Word_Address ������ĵ�ַҪע��һ�¹���
* Output         : None
* Return         : None 
*******************************************************************************//**/
void IIC_Byte_Write( u8 pBuffer, u8 Word_Address)
{
	 //IIC_Wait_EEprom();                 /*�ȴ����߿��У��ȴ��豸�������У�������Ҫ������SR1��AFλ����Ӧ��ɹ���*/
	
	 I2C_GenerateSTART(I2C1, ENABLE);   /*����һ��s��Ҳ������ʼ�źţ���Ϊǰ��ĺ���û����ֹ����ο�ʼҲ����Ҫ���µ�һ����ʼ�ź�*/
	 //EV5�¼�
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); /*�ж�EV5,���º���������Է��֣����¼���SB=1��MSL=1��BUSY=1 
	                                                            ��˼����ʼ�����Ѿ������ˣ�Ȼ������ģʽ��������ͨѶ*/
	 
	 I2C_Send7bitAddress(I2C1, DEVICE_ADDRESS, I2C_Direction_Transmitter);  /*����������ַ�����һ��������ʾ��ַbit0Ϊ0����˼
	                                                                        ��д������ͬʱ����д��DR�Ĵ������ʻ����SBλ���0*/
	
	 //EV6 EV8_1�����¼��ж�ͬʱ�ж���EV8_1�¼���
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));/*�ж�EV6��EV8_1����������ʱSB=0��ADDR=1��ַ���ͽ�����
	                                                                     TXE=1���ݼĴ���DRΪ�գ�BUSY=1�ܲ�����,MSL=1��ģʽ,TRA=1
	                                                                        �����Ѿ������ˣ���Ϊ��д��������ʵ�ǵ�ַ�����Ѿ������ˣ�
	                                                                        ���������ģʽ�������EV6��TRA=1��ʾ���ݻ�û�յ���0��ʾ
	                                                                        �յ����ݣ�ע������TRA=1��ʾ�Ѿ������ˣ�ADDR=1���Ƿ������
	                                                                        ��,������¼���ADDR=0����*/                                                                           
	 
	 I2C_SendData(I2C1,Word_Address);      /*EV8_1�¼�����Ϊ��һ��ʱ��DR�Ѿ�Ϊ�գ����¼���д��data1������EEPROM��˵���data1��Ҫд��
	                                      �ֽڵĵĵ�ַ��data2��Ҫд������ݣ�data1Ϊ8λ1k����Ч�ֽ��ǵ�7λ��2kbit����Ч�ֽ���8λ��32ҳ��
	                                      ÿҳ8���ֽڣ�һ��2kλ��16kbit��Ҫ11λ����ô�ͣ���Ӳ����A1��2��3��GPIO��ѡ��洢��ҳ*/
	                         
	 
	 //EV8�¼�
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING)); /*Ϊ��һ��ִ��EV8�¼����ж�TXE=1 DR�Ĵ���Ϊ�գ�׼��д��DR�Ĵ���
	                                                                 data2��ע���ʱҲ�ж�TRA=1��˼��data1�Ѿ������˲���ʾ������ɣ�
	                                                                 ��λ�Ĵ����ǿձ�ʾ�����ڷ�������BTF=0˵��data1�ֽڷ���û��ɣ�
																																	 NOSTRETCHΪ0ʱ��BTF=1����ʾ������Ҫ�������ˣ���ζ���ֽڷ��ͣ�
	                                                                 ����˵��������ݻ�û��д�뵽DR���棬�����EV8�¼����ڵ�һ��һֱ
																																	 �������ݷ��͵ģ�������BTF=1�����*/ 
																																	 																															                                                                  
	 I2C_SendData(I2C1,pBuffer);                                    /*���ݼĴ���DRΪ�գ�������д��data2���ò���������EV8�¼�*/
	
	/*EV8_2�¼�������ͷ�������data�����Ծ�ֻ��һ��EV8�¼���
	  EV8_1��data1��EV8_1��data2��EV8_2��EV8�������Ǽ���
		���BTF,��EV8_2��дDR���ǳ���Ҫ��ֹͣ��*/
	  
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); /*EV8_2���¼��жϺ�EV8�¼���һ����TRA=1��ʾdata2�Ѿ������ˣ�
	                                                                  BTF=1�ֽڷ��ͽ�����˵����һʱ��û���ֽ��ڷ��ͣ���ʵ��ʾ��
																																		data2���ڷ��͵�ʱ��û��data3д�뵽DR���棬 Ȼ�����ڸ��ֵ�Ҫ
	                                                                  ����data3�ˣ�����DR�����ǿյģ���ʵ����data2��ʱ���м�ĳʱ
																																		��DR���Ѿ����ˣ�*/
	                                                                																																
	 I2C_GenerateSTOP(I2C1,ENABLE);                                  /*EV8_2�¼��еĳ���дֹͣ*/
}
/*��E2PROMдһҳ�����ܿ�ҳ*/
void IIC_EE_WriteOnePage(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
  if(NumByteToWrite>0)
  {
	 //IIC_Wait_EEprom();                 /*�ȴ����߿��У��ȴ��豸�������У�������Ҫ������SR1��AFλ����Ӧ��ɹ���*/
	
	 I2C_GenerateSTART(I2C1, ENABLE);   /*����һ��s��Ҳ������ʼ�źţ���Ϊǰ��ĺ���û����ֹ����ο�ʼҲ����Ҫ���µ�һ����ʼ�ź�*/
	 //EV5�¼�
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); /*�ж�EV5,���º���������Է��֣����¼���SB=1��MSL=1��BUSY=1 
	                                                            ��˼����ʼ�����Ѿ������ˣ�Ȼ������ģʽ��������ͨѶ*/
	 
	 I2C_Send7bitAddress(I2C1, DEVICE_ADDRESS, I2C_Direction_Transmitter);  /*����������ַ�����һ��������ʾ��ַbit0Ϊ0����˼
	                                                                        ��д������ͬʱ����д��DR�Ĵ������ʻ����SBλ���0*/
	
	 //EV6 EV8_1�����¼��ж�ͬʱ�ж���EV8_1�¼���
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));/*�ж�EV6��EV8_1����������ʱSB=0��ADDR=1��ַ���ͽ�����
	                                                                     TXE=1���ݼĴ���DRΪ�գ�BUSY=1�ܲ�����,MSL=1��ģʽ,TRA=1
	                                                                        �����Ѿ������ˣ���Ϊ��д��������ʵ�ǵ�ַ�����Ѿ������ˣ�
	                                                                        ���������ģʽ�������EV6��TRA=1��ʾ���ݻ�û�յ���0��ʾ
	                                                                        �յ����ݣ�ע������TRA=1��ʾ�Ѿ������ˣ�ADDR=1���Ƿ������
	                                                                        ��,������¼���ADDR=0����*/                                                                           
	 
	 I2C_SendData(I2C1,WriteAddr);      /*EV8_1�¼�����Ϊ��һ��ʱ��DR�Ѿ�Ϊ�գ����¼���д��data1������EEPROM��˵���data1��Ҫд��
	                                      �ֽڵĵĵ�ַ��data2��Ҫд������ݣ�data1Ϊ8λ1k����Ч�ֽ��ǵ�7λ��2kbit����Ч�ֽ���8λ��32ҳ��
	                                      ÿҳ8���ֽڣ�һ��2kλ��16kbit��Ҫ11λ����ô�ͣ���Ӳ����A1��2��3��GPIO��ѡ��洢��ҳ*/
	                         
	 
	 //EV8�¼�
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING)); /*Ϊ��һ��ִ��EV8�¼����ж�TXE=1 DR�Ĵ���Ϊ�գ�׼��д��DR�Ĵ���
	                                                                 data2��ע���ʱҲ�ж�TRA=1��˼��data1�Ѿ������˲���ʾ������ɣ�
	                                                                 ��λ�Ĵ����ǿձ�ʾ�����ڷ�������BTF=0˵��data1�ֽڷ���û��ɣ�
																																	 NOSTRETCHΪ0ʱ��BTF=1����ʾ������Ҫ�������ˣ���ζ���ֽڷ��ͣ�
	                                                                 ����˵��������ݻ�û��д�뵽DR���棬�����EV8�¼����ڵ�һ��һֱ
																																	 																															                                                                  
																																 �������ݷ��͵ģ�������BTF=1�����*/ 
	while(	NumByteToWrite--)
        {
         I2C_SendData(I2C1,*pBuffer++);                                    /*���ݼĴ���DRΪ�գ�������д��data2���ò���������EV8�¼�*/
	
	/*EV8_2�¼�������ͷ�������data�����Ծ�ֻ��һ��EV8�¼���
	  EV8_1��data1��EV8_1��data2��EV8_2��EV8�������Ǽ���
		���BTF,��EV8_2��дDR���ǳ���Ҫ��ֹͣ��*/
	  
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); /*EV8_2���¼��жϺ�EV8�¼���һ����TRA=1��ʾdata2�Ѿ������ˣ�
	                                                                  BTF=1�ֽڷ��ͽ�����˵����һʱ��û���ֽ��ڷ��ͣ���ʵ��ʾ��
																																		data2���ڷ��͵�ʱ��û��data3д�뵽DR���棬 Ȼ�����ڸ��ֵ�Ҫ
	                                                                  ����data3�ˣ�����DR�����ǿյģ���ʵ����data2��ʱ���м�ĳʱ
																																		��DR���Ѿ����ˣ�*/
	                                                                																																
        }
        
         I2C_GenerateSTOP(I2C1,ENABLE);                                  /*EV8_2�¼��еĳ���дֹͣ*/
  }
}

/*******************************************************************************
* Function Name  : IIC_Buffer_Write
* Description    : ��eerpom��at24c02����д������д���ֽ�
* Input          : u8* pbuffer, u8 WriterAddr��u16 NumByteToWrite
* Output         : None
* Return         : None 
*******************************************************************************//**/
void IIC_Buffer_Write(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
  u8 AlignedStartAddr = 0;
  u8 AlignedEndAddr = 0;
  u8 AlignedCount = 0;
  u8 StartDiffNum = 0;
  u8 EndDiffNum = 0;
  /*WriterAddr StartDiffNum AlignedStartAddr AlignedCount AlignedEndAddr EndDiffNum*/
  if((WriteAddr % I2C_PageSize) ==0)
  {
    AlignedStartAddr = WriteAddr;
    StartDiffNum = 0;
  }
  else
  {
    AlignedStartAddr = (WriteAddr / I2C_PageSize + 1)*I2C_PageSize;
    StartDiffNum = AlignedStartAddr - WriteAddr;
  }
  AlignedCount = (NumByteToWrite - StartDiffNum)/I2C_PageSize;
  AlignedEndAddr = AlignedStartAddr + AlignedCount*I2C_PageSize;
  EndDiffNum = NumByteToWrite - AlignedCount*I2C_PageSize - StartDiffNum;
  rt_kprintf("\r\n%d  %d %d  %d  %d  %d\n",WriteAddr, StartDiffNum,AlignedStartAddr,AlignedCount,AlignedEndAddr,EndDiffNum);
  IIC_EE_WriteOnePage(pBuffer, WriteAddr,StartDiffNum); pBuffer = pBuffer+StartDiffNum;
  rt_thread_delay(2);
  while(AlignedCount--)
  {
    IIC_EE_WriteOnePage(pBuffer, AlignedStartAddr,I2C_PageSize); 
    AlignedStartAddr = AlignedStartAddr + I2C_PageSize;
    pBuffer = pBuffer+I2C_PageSize;
    rt_thread_delay(2);
  }
  IIC_EE_WriteOnePage(pBuffer, AlignedEndAddr,EndDiffNum);  
  rt_thread_delay(2);
}

/*******************************************************************************
* Function Name  : IIC_Byte_Read
* Description    : ��eerpom��at24c02���Ķ���������һ���ֽڵĶ�����������һ����ַWord_AddressȻ���ȡ��ֵ����ֵ����pRead��
* Input          : u8* pRead, u8 Word_Address������ĵ�ַҪע��һ�¹���1,2kbit�ľ���8λ�����ܱ�ʾ���ˣ�4,8,16k��Ҫ��Ӳ������A0��1,2������GPIOѡ��ҳ��
* Output         : None
* Return         : None 
*******************************************************************************//**/
void IIC_Byte_Read( u8* pRead, u8 Word_Address)
{
	/*����д�����������豸��ַд�������ҵ��豸Ӧ���ˣ�д��Ҫ�����ݵĵ�ַ*/
   //IIC_Wait_EEprom(); 
   I2C_GenerateSTART(I2C1, ENABLE);
	
	 //EV5�¼�
   while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
   I2C_Send7bitAddress(I2C1, DEVICE_ADDRESS, I2C_Direction_Transmitter); 
	
	 //EV6 EV8_1�¼������¼��ж�ͬʱ�ж���EV8_1�¼���
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	 {}
	 I2C_SendData(I2C1,Word_Address); 
		 
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //����EV8_2���ж�������Ӧ��ʱ�Ѿ��յ�ACK��Word_Address������ɡ�
	 
	/*��Start�����������豸��ַ���������ҵ��豸Ӧ���ˣ������ݣ�������Ӧ����ֹ*/
	 I2C_GenerateSTART(I2C1, ENABLE);
		 
	//EV5�¼�
   while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
   I2C_Send7bitAddress(I2C1, DEVICE_ADDRESS, I2C_Direction_Receiver ); 
	
  //EV6�¼�
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
		 
	//EV6_1�¼���û�б�־λ��Ҫ����ACKʧ�ܺ�ֹͣλ����
	 I2C1->CR1 &= 0xFBFF ;   //ʧ��ACK
	 I2C1->CR1 |= 0x0020 ;   //ʹ��Stop
	
	//EV7�¼�����DR�Ĵ���
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
	 * pRead = I2C1->DR;		 
}



/*******************************************************************************
* Function Name  : IIC_Byte_Read
* Description    : ��eerpom��at24c02���Ķ���������һ���ֽڵĶ�����������һ����ַWord_AddressȻ���ȡ��ֵ����ֵ����pRead��
* Input          : u8* pRead, u8 Word_Address������ĵ�ַҪע��һ�¹���1,2kbit�ľ���8λ�����ܱ�ʾ���ˣ�4,8,16k��Ҫ��Ӳ������A0��1,2������GPIOѡ��ҳ��
* Output         : None
* Return         : None 
*******************************************************************************//**/
void IIC_Buffer_Read(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{
	/*����д�����������豸��ַд�������ҵ��豸Ӧ���ˣ�д��Ҫ�����ݵĵ�ַ*/
   //IIC_Wait_EEprom(); 
   I2C_GenerateSTART(I2C1, ENABLE);
	
	 //EV5�¼�
   while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
   I2C_Send7bitAddress(I2C1, DEVICE_ADDRESS, I2C_Direction_Transmitter); 
	
	 //EV6 EV8_1�¼������¼��ж�ͬʱ�ж���EV8_1�¼���
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	 {}
	 I2C_SendData(I2C1,ReadAddr); 
		 
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //����EV8_2���ж�������Ӧ��ʱ�Ѿ��յ�ACK��Word_Address������ɡ�
	 
	/*��Start�����������豸��ַ���������ҵ��豸Ӧ���ˣ������ݣ�������Ӧ����ֹ*/
	 I2C_GenerateSTART(I2C1, ENABLE);
		 
	//EV5�¼�
   while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
   I2C_Send7bitAddress(I2C1, DEVICE_ADDRESS, I2C_Direction_Receiver ); 
	
  //EV6�¼�
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
		 
	//EV6_1�¼���û�б�־λ��Ҫ����ACKʧ�ܺ�ֹͣλ����
//	 I2C1->CR1 &= 0xFBFF ;   //ʧ��ACK
//	 I2C1->CR1 |= 0x0020 ;   //ʹ��Stop
	while(NumByteToRead--)
        {
	//EV7�¼�����DR�Ĵ���
	 while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
	 * pBuffer++ = I2C1->DR;
//         if(NumByteToRead==1)
//         {
//           I2C1->CR1 &= 0xFBFF ;   //ʧ��ACK
//           I2C1->CR1 |= 0x0020 ;   //ʹ��Stop
//         }
        }
        I2C_GenerateSTART(I2C1, ENABLE);
}




