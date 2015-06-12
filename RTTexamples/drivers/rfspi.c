/******************************2014-2015, NJTECH, Edu.************************** 
FileName: rfspi.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2015.05.30
Description:    ��Ƶģ��SPI1 �� SPI3 �����ӿں���     
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    15/05/30     1.0     �ļ�����  
1.SPI�˿ڳ�ʼ��
SPI1: SCK SDI SDO = PA5 PA6 PA7
SPI3:SCK SDI SDO =PC10 PC11 PC12
2.�����ٶȵ�SPI����
3.��ʼ��SPI�˿ڣ�����RF����
*******************************************************************************/
#include "rfspi.h"
#include "stm32f10x.h"


/*******************************************************************************
* Function Name  : SPI3_Init
* Description    : ������ӿ�SPI3�ĳ�ʼ����SPI���ó���ģʽ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI3_Init(void)
{	 
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE );	
        GPIO_PinRemapConfig(GPIO_Remap_SPI3,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);   
	//SPI3�ڳ�ʼ��
	/* Configure SPI1 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = SPI3_MISO| SPI3_MOSI| SPI3_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI3, &GPIO_InitStructure);

    GPIO_SetBits(GPIO_SPI3,SPI3_MISO| SPI3_MOSI| SPI3_SCK);

	/* SPI3 configuration */                                            //��ʼ��SPI1�ṹ��
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI1����Ϊ����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //����SPI1Ϊ��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		                    //����ʱ���ڲ�����ʱ��ʱ��Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	                    //��һ��ʱ���ؿ�ʼ��������
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //NSS�ź��������ʹ��SSIλ������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //SPI������Ԥ��ƵֵΪ8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRCֵ����Ķ���ʽ

	SPI_Init(SPI3, &SPI_InitStructure);                                 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPI3�Ĵ���
	
	/* Enable SPI3  */
	SPI3_SetSpeed(SPI_SPEED_256);                                           //����Ϊ����ģʽ
	SPI_Cmd(SPI3, ENABLE);                                              //ʹ��SPI1����
	SPI3_ReadWriteByte(0xff);                                           //��������		 

}  

/*******************************************************************************
* Function Name  : SPI1_Init
* Description    : ������ӿ�SPI1�ĳ�ʼ����SPI���ó���ģʽ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_Init(void)
{	 
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1|RCC_APB2Periph_AFIO, ENABLE );	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	

	//SPI1�ڳ�ʼ��
	/* Configure SPI1 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = SPI1_MISO| SPI1_MOSI| SPI1_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI1, &GPIO_InitStructure);

        GPIO_SetBits(GPIO_SPI1,SPI1_MISO| SPI1_MOSI| SPI1_SCK);

	/* SPI1 configuration */                                            //��ʼ��SPI1�ṹ��
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI1����Ϊ����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //����SPI1Ϊ��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//Low		            //����ʱ���ڲ�����ʱ��ʱ��Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//1Edge	                //��һ��ʱ���ؿ�ʼ��������
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //NSS�ź��������ʹ��SSIλ������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //SPI������Ԥ��ƵֵΪ8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRCֵ����Ķ���ʽ

	SPI_Init(SPI1, &SPI_InitStructure);                                 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPI1�Ĵ���
	
	/* Enable SPI1  */
	SPI1_SetSpeed(SPI_SPEED_256);                                           //����Ϊ����ģʽ
	SPI_Cmd(SPI1, ENABLE);                                              //ʹ��SPI1����
	SPI1_ReadWriteByte(0xff);                                           //��������		 
}  
/*******************************************************************************
* Function Name  : SPI3_ReadWriteByte
* Description    : SPI3��д���ݺ���
* Input          : Ҫд�������
* Output         : None
* Return         : ����������
*******************************************************************************/
u8 SPI3_ReadWriteByte(u8 TxData)                                       
{		
	u8 retry=0;				 	
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)      //���ͻ����־λΪ��
		{
		retry++;
		if(retry>200)return 0;
		}			  
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI3, TxData);                                    //ͨ������SPI1����һ������
	retry=0;
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);   //���ջ����־λ��Ϊ��
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI3);                                 //ͨ��SPI1���ؽ�������				    
}

/*******************************************************************************
* Function Name  : SPI1_ReadWriteByte
* Description    : SPI1��д���ݺ���
* Input          : Ҫд�������
* Output         : None
* Return         : ����������
*******************************************************************************/
u8 SPI1_ReadWriteByte(u8 TxData)                                       
{		
	u8 retry=0;				 	
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)      //���ͻ����־λΪ��
		{
		retry++;
		if(retry>200)return 0;
		}			  
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, TxData);                                    //ͨ������SPI1����һ������
	retry=0;
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)   //���ջ����־λΪ��ѭ���ȴ�
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);                                 //ͨ��SPI1���ؽ�������				    
}

/*******************************************************************************
* Function Name  : SPI1_SetSpeed
* Description    : SPI1����SPI����,��SPI �豸ʹ�� 
* Input          : Ҫ���õ��ٶ�  SPI_SPEED_2   8   16    256
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_SetSpeed(u8 SpeedSet)
  { 
      SPI1->CR1&=0XFFC7;   //Fsck=Fcpu/256 
      if(SpeedSet==SPI_SPEED_2) //����Ƶ 
     { 
       SPI1->CR1|=0<<3; //Fsck=Fpclk/2=36Mhz 
      }
      else if(SpeedSet==SPI_SPEED_8)
          //�˷�Ƶ 
      { 
         SPI1->CR1|=2<<3; //Fsck=Fpclk/8=9Mhz 
       }
      else if(SpeedSet==SPI_SPEED_16)
        //ʮ����Ƶ 
      { 
        SPI1->CR1|=3<<3; //Fsck=Fpclk/16=4.5Mhz 
      }
      else                       //256 ��Ƶ 
      { 
          SPI1->CR1|=7<<3;  //Fsck=Fpclk/256=281.25Khz             ����ģʽ 
       } 
        SPI1->CR1|=1<<6;    //SPI �豸ʹ�� 
} 

/*******************************************************************************
* Function Name  : SPI3_SetSpeed
* Description    : SPI3����SPI����,��SPI �豸ʹ�� 
* Input          : Ҫ���õ��ٶ�  SPI_SPEED_2   8   16    256
* Output         : None
* Return         : None
*******************************************************************************/
void SPI3_SetSpeed(u8 SpeedSet)
  { 
      SPI3->CR1&=0XFFC7;   //Fsck=Fcpu/256 
      if(SpeedSet==SPI_SPEED_2) //����Ƶ 
     { 
       SPI3->CR1|=0<<3; //Fsck=Fpclk/2=36Mhz 
      }
      else if(SpeedSet==SPI_SPEED_8)
          //�˷�Ƶ 
      { 
         SPI3->CR1|=2<<3; //Fsck=Fpclk/8=9Mhz 
       }
      else if(SpeedSet==SPI_SPEED_16)
        //ʮ����Ƶ 
      { 
        SPI3->CR1|=3<<3; //Fsck=Fpclk/16=4.5Mhz 
      }
      else                       //256 ��Ƶ 
      { 
          SPI3->CR1|=7<<3;  //Fsck=Fpclk/256=281.25Khz             ����ģʽ 
       } 
        SPI3->CR1|=1<<6;    //SPI �豸ʹ�� 
} 



















