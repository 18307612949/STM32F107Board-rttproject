/*******************************2012-2014, NJUT, Edu.************************** 
FileName: testgui.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2014.06.04
Description:    ��������������   

    1.test_gui() ����ʾ��򣬼��м��ַ�����
    2.startgui()����ʼ��GUI������ ��ִ��������󣬲ſɷֱ��������º���
      2.1 DynamicCali() ����̬У׼����
      2.2 StopCali()���˳���̬У׼����
      2.3 TouchCali() ��������У׼����
      2.4 guidemo() : ����demo����

Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/06/04     1.0     �ļ�����   
******************************************************************************/ 
#include "rtthread.h"
#include "timer5.h"
#include "touch.h"
#include "GUI.h"
#include "GUIDEMO.h"
#include "rs485.h"

uint8_t GUI_Initialized   = 0;

/*******************************************************************************
*  Function Name  :   testgui 
*  Description    : GUI���Ժ�������������ʾ����
*  Input          : None
*  Output         : None
*  Return         : None
*******************************************************************************/
void test_gui(void)
{
  int i;  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    GUI_Init();
    GUI_Initialized = 1;
    GUI_DispChars('/', 53);     
    for( i= 0; i < 240; i += 8)   
    {        
        GUI_DispCharAt('/', 312, i);
        GUI_DispCharAt('/', 1,   i);
    } 
    GUI_DispChars('/', 52); 
    GUI_SetFont(&GUI_Font24B_ASCII);
    GUI_SetColor(GUI_RED); 
    GUI_DispStringAt("This is a test program!",10,110);
}

void rt_gui_thread_entry(void* parameter) 
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
  /*����Ĭ�ϴ������WM Ȼ��Ὣ WM_PAINT ��Ϣ����ض��򵽴洢�豸�У��ٸ��Ƶ���
  ʾ���С�����������ڵ��ڴ治�������Զ�ʹ�÷ֶ�*/
  WM_SetCreateFlags(WM_CF_MEMDEV);
  touch_config();
  Timer5Config();
  GUI_Init();
  GUI_Initialized = 1;
  ADC_Config();
  LED_Init();
  RS485GPIOConfig();
  RS485Write();
  
  GUIDEMO_Main();

}

void startgui(void)
{
    rt_thread_t init_thread;
    init_thread = rt_thread_create("GUI_Exec", rt_gui_thread_entry, RT_NULL, 0x0800, 6, 5); 
    if(init_thread != RT_NULL)
    {
            rt_thread_startup(init_thread);
    }
}

void guidemo(void)
{
	GUIDEMO_Main(); 
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(test_gui, startup test gui e.g.test_gui());
FINSH_FUNCTION_EXPORT(startgui, startup startgui);
FINSH_FUNCTION_EXPORT(guidemo, startup guidemo);
#endif

