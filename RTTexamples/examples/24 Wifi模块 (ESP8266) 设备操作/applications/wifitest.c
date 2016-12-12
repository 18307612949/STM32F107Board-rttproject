/*****************************2012-2016, NJUT, Edu.****************************** 
FileName: wifitest.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2016.11.30
Description:   WIFIģ���Գ��� post ��ǰ�¶Ⱥ� GET LED1
��ǰ�¶ȴ�11-99  
ע�⣺�������� TEMPDATA LEDDATA����������Լ�ע���YEELINK�豸��ID�ţ����� �����͵����ߵ��豸��
device��350393
sensor��393199 397707
U-ApiKey: 91bd74f5f8b8a809aaa6037911f7c382

Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    16/11/30     1.0     �ļ�����   
********************************************************************************/ 
#include "esp8266.h"
#include "led.h"

#define TEMPDATA "POST /v1.1/device/350393/sensor/393199/datapoints HTTP/1.1\r\nHost: api.yeelink.net\r\nU-ApiKey: 91bd74f5f8b8a809aaa6037911f7c382\r\nContent-Length:12\r\n\r\n{\"value\":23}"
#define LEDDATA "GET /v1.1/device/350393/sensor/397707/datapoints HTTP/1.1\r\nHost: api.yeelink.net\r\nU-ApiKey: 91bd74f5f8b8a809aaa6037911f7c382\r\n\r\n}"

char netdata[] = TEMPDATA;
uint8_t i = 11;

void test_wifi(void)
{
  rt_bool_t res = RT_TRUE;
  LED_Init();
  res = wificonfig();
  res = wifiinit(); 
  res = wifijap();  
  res = wificonnect(); 
  
  /*����POST����������*/
  i++; if(i>=99)i=11;
  netdata[rt_strlen(netdata)-3]= 0x30+ i/10;
  netdata[rt_strlen(netdata)-2]= 0x30+ i%10;
  /*����POST���������ݽ���*/
  
  res = wifisend(netdata);//�����¶�ֵ ����POST����
  res = wifisend(LEDDATA); //��ȡ��ֵ ����GET����
  
  res = wificloseconnect();
  res = wifiexit();  
}

#include "finsh.h"
FINSH_FUNCTION_EXPORT(test_wifi, wifi module test e.g. test_wifi());

