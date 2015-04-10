/*******************************2012-2013, NJUT, Edu.************************** 
FileName: I2C.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2013.07.30
Description:    ģ��I2C���� E2PROM ����      
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/07/30     1.0     �ļ�����   
  *          STM32Board Key Pin assignment
  *          =============================
  *          +-----------------------------------------------------------+
  *          |                     Pin assignment                        |
  *          +-----------------------------+-----------------------------+
  *          |      I2C Pin                |     Port & Pin              |
  *          +-----------------------------+-----------------------------+
  *          |        SCL                  |        B6                   |
  *          |        SDA                  |        B7                   |
  *          +-----------------------------+-----------------------------+
******************************************************************************/ 
#include "I2C.h"

/*******************************************************************************
* Function Name  : Delay
* Description    : ����ʱ���� �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Delay( void )
{ 
  int t = 1;
  while(t--) ;	
}
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : I2C GPIO ��ʼ�� �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( I2C_CLK, ENABLE); 

  GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;          
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(I2C_PORT, &GPIO_InitStructure); 
}

/*******************************************************************************
* Function Name  : AT24C02_SDA_IO_SET
* Description    : I2C E2PROM SDA���������������  �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AT24C02_SDA_IO_SET(unsigned char io_set) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  if(io_set)
  {
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_Init(I2C_PORT, &GPIO_InitStructure); 
  }
  else
  {
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_PORT, &GPIO_InitStructure); 
  }
}

/*******************************************************************************
* Function Name  : rt_I2C_init
* Description    : I2C E2PROM ��ʼ�� �ⲿ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void rt_I2C_init(void)
{
   GPIO_Configuration();
   GPIO_SetBits(I2C_PORT,I2C_SCL_PIN);
   Delay();
   GPIO_SetBits(I2C_PORT,I2C_SDA_PIN);
   Delay();
}

/*******************************************************************************
* Function Name  : start
* Description    : I2C E2PROM �������� �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void start(void)
{
  GPIO_SetBits(I2C_PORT,I2C_SDA_PIN);
  Delay();
  GPIO_SetBits(I2C_PORT,I2C_SCL_PIN);
  Delay();
  GPIO_ResetBits(I2C_PORT, I2C_SDA_PIN);
  Delay();
  GPIO_ResetBits(I2C_PORT, I2C_SCL_PIN);
  Delay();
}

/*******************************************************************************
* Function Name  : stop
* Description    : I2C E2PROM ֹͣ���� �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void stop(void)
{
  GPIO_ResetBits(I2C_PORT, I2C_SDA_PIN);
  Delay();
  GPIO_SetBits(I2C_PORT,I2C_SCL_PIN);
  Delay();
  GPIO_SetBits(I2C_PORT,I2C_SDA_PIN);
  Delay();
}

/*******************************************************************************
* Function Name  : writex
* Description    : I2C E2PROM дһ���ֽ� �ڲ�����
* Input          : j Ҫд����ֽ�
* Output         : None
* Return         : None
*******************************************************************************/
void writex(unsigned char j)

{
  unsigned char i,temp,temp1;
  
  temp=j;
  
  for (i=0;i<8;i++)
  {
    temp1=temp & 0x80;
    temp=temp<<1;
    
    GPIO_ResetBits(I2C_PORT, I2C_SCL_PIN);
    Delay();
    
    if(temp1==0x80)
    {
      GPIO_SetBits(I2C_PORT, I2C_SDA_PIN);
    }
    else
    {
      GPIO_ResetBits(I2C_PORT, I2C_SDA_PIN);
    }
    Delay();
    
    GPIO_SetBits(I2C_PORT,I2C_SCL_PIN);
    Delay();
  }
  GPIO_ResetBits(I2C_PORT, I2C_SCL_PIN);
  Delay();
  
  GPIO_SetBits(I2C_PORT,I2C_SDA_PIN);
  Delay();
}

/*******************************************************************************
* Function Name  : readx
* Description    : I2C E2PROM ��һ���ֽ� �ڲ�����
* Input          : None
* Output         : None
* Return         : ����������
*******************************************************************************/
unsigned char readx()
{
  unsigned char i,j,k=0;
  
  GPIO_ResetBits(I2C_PORT, I2C_SCL_PIN);
  Delay();
  GPIO_SetBits(I2C_PORT,I2C_SDA_PIN);
  
  AT24C02_SDA_IO_SET(1);
  for (i=0;i<8;i++)
  {
    Delay();
    GPIO_SetBits(I2C_PORT,I2C_SCL_PIN);
    Delay();
    if( GPIO_ReadInputDataBit(I2C_PORT,I2C_SDA_PIN)==1 ) 
      {
        j=1;
      }
    else 
    {
      j=0;
    }
    k=(k<<1)|j;
    GPIO_ResetBits(I2C_PORT, I2C_SCL_PIN);
  }
  
  AT24C02_SDA_IO_SET(0);
  Delay();
  return(k);
}

/*******************************************************************************
* Function Name  : clock
* Description    : I2C E2PROM ����һ��ʱ��  �ڲ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void clock()

{
  unsigned int i=0;
  AT24C02_SDA_IO_SET(1);
  GPIO_SetBits(I2C_PORT,I2C_SCL_PIN);
  Delay();
  while ((GPIO_ReadInputDataBit(I2C_PORT,I2C_SDA_PIN)==1)&&(i<5000))i++;
  GPIO_ResetBits(I2C_PORT, I2C_SCL_PIN);
  Delay();
  AT24C02_SDA_IO_SET(0);
}

/*******************************************************************************
* Function Name  : rt_I2C_read
* Description    : ��I2C E2PROM ��ַ�ж�ȡһ���ֽ� �ⲿ����
* Input          : address ��ַ
* Output         : None
* Return         : ��������
*******************************************************************************/
unsigned char rt_I2C_read(unsigned char address)

{
  unsigned char i;
  start();
  writex(0xa0);
  clock();
  writex(address);
  clock();
  start();
  writex(0xa1);
  clock();
  i=readx();
  stop();
  Delay();
  return(i);
}

/*******************************************************************************
* Function Name  : rt_I2C_write
* Description    : ��I2C E2PROM ��ַ��д��һ���ֽ� �ⲿ����
* Input          : address ��ַ info ����
* Output         : None
* Return         : None
*******************************************************************************/
void rt_I2C_write(unsigned char address,unsigned char info)

{
  start();
  writex(0xa0);
  clock();
  writex(address);
  clock();
  writex(info);
  clock();
  stop();
  Delay();
}

 
 

