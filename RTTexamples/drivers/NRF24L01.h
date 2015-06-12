/******************************2014-2015, NJUT, Edu.*************************** 
FileName: NRF24L01.h 
Author:  �ﶬ÷       Version :  1.0        Date: 2015.05.30
Description:    NRF24L01���� ͷ�ļ�     
Version:         1.0 
History:         
*******************************************************************************/ 
#ifndef __NRF24L01_H
#define __NRF24L01_H	 		    
#include "stm32f10x.h"
#include "rfspi.h"
#include "rtthread.h"

void NRF24L01_Init(void);                                //NRF24l01��ʼ��
void RX_Mode(void);                                      //����Ϊ����ģʽ
void TX_Mode(void);                                      //����Ϊ����ģʽ
rt_uint8_t NRF24L01_Write_Buf(rt_uint8_t regaddr, rt_uint8_t *pBuf, rt_uint8_t datalen); //д������
rt_uint8_t NRF24L01_Read_Buf(rt_uint8_t regaddr, rt_uint8_t *pBuf, rt_uint8_t datalen);  //��������		  
rt_uint8_t NRF24L01_Read_Reg(rt_uint8_t regaddr);		                 //���Ĵ���
rt_uint8_t NRF24L01_Write_Reg(rt_uint8_t regaddr, rt_uint8_t data);              //д�Ĵ���
rt_err_t NRF24L01_Check(void);                                 //���NRF24L01�Ƿ���λ
rt_err_t NRF24L01_TxPacket(rt_uint8_t *txbuf);                         //����һ����������
rt_err_t NRF24L01_RxPacket(rt_uint8_t *rxbuf);                         //����һ����������

#endif











