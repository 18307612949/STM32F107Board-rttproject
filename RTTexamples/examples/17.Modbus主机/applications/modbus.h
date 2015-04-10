
#ifndef __MODBUS_H
#define __MODBUS_H

#include <rtthread.h>
#include <stdlib.h>   
#include <stdint.h>   
#include <string.h>  
#include "usart2.h"
#include "stm32f10x.h"
#include "RS485.h"


/*�����Э�������ַ�*/
typedef enum
{
	InverterOK, TimeOut, FrameErr,CheckErr, UnknowErr = 0xff
}TYPEDEF_InverterERR;


void Bus485Init(void);
/*MODBUS��03���ܺţ�������Ĵ���  ��ֵ���� value�� */
TYPEDEF_InverterERR ReadInverter(uint8_t InverterID, uint16_t Addr,
                  uint8_t N,uint16_t BandRate, uint8_t channelparity,uint8_t *value);

/*DL645��Լ������01����ȡ����, value�� */
TYPEDEF_InverterERR  Read645Inverter(uint16_t SignCode, uint8_t Addr, uint8_t *len,
                  uint16_t BandRate, uint8_t channelparity, uint8_t *value);

#endif