/****************************2012-2013, NJUT, Edu.****************************** 
FileName: wdgtest.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2014.07.30
Description:    ���Ź����Գ��� 

Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/07/30     1.0     �ļ�����   
********************************************************************************/ 
#include  <rtthread.h >
#include "stm32f10x.h"
#include "led.h"
#include "key.h"

/*******************************************************************************
* Function Name  : iwdg_init
* Description    : ��ʼ�����Ź�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void iwdg_init()
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); // �ر� I WDG_PR �� IWDG_RLR ��д����
    IWDG_SetReload(0xfff); // ������װ��ֵΪ 0xfff
    IWDG_SetPrescaler(IWDG_Prescaler_32); // ����Ԥ��Ƶϵ��Ϊ 32 ���ʱ��3S �ο��ֲ�P317
    // IWDG_ReloadCounter(); �ڰ����жϷ�������������һ��ι��
    IWDG_Enable(); // ʹ�ܿ��Ź�
}

void test_iwdg(void)
{
    Key_Init();
    Set_Keyint();//��KEY2 �жϷ��������ι�� 
    LED_Init();
    iwdg_init();  
}
 
/*******************************************************************************
* Function Name  : EXTI13_IRQHandler
* Description    : �ⲿ�жϰ���2������,LED2 3��˸ ι������ ���д˺�����ע�͵�key.c�е��жϺ���
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
/* */void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
          LEDTog(LED2);
          LEDTog(LED3);
          IWDG_ReloadCounter(); // ����װ��ֵд�뿴�Ź��У��׳�ι��
          EXTI_ClearITPendingBit(EXTI_Line13);
          rt_kprintf("\r\n ����ι����");
	} 
}



/*******************************************************************************
* Function Name  : WWDGConfig
* Description    : ���ڿ��Ź�����
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void WWDGConfig(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;         
   /* ������ WWDG�Ĵ�������Ϊȱʡֵ ��ʼ���رմ��ڿ��Ź�ʱ��   WWDG_DeInit();  */
  
    /* �������ڿ��Ź�ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	 
   
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn ; //ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //ռ���жϵȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //��Ӧ�ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);                        //�������ȼ�
    
     
    /* Set WWDG prescaler to 8 ���� WWDG Ԥ��Ƶֵ ��PCLK1(72M)/4096/8=2197 Hz*/ 
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    /* Set WWDG window value to 0x41ָ���Ĵ���ֵ���ò���ȡֵ������ 0x40 �� 0x7F֮�䡣  */ 
    WWDG_SetWindowValue(0x40); 
    
    /* Enable WWDG and set counter value to 0x7F ʹ�ܴ��ڿ��Ź������ѿ��Ź���������ֵ��Ϊ0x7f*/
    WWDG_Enable(0x7f);
    /* Clear EWI flag ������ڻ����жϱ�־λ */ 
    WWDG_ClearFlag();  
    /* Enable WWDG Early wakeup interrupt ʹ�� WWDG ���ڻ����жϣ�EWI��  */ 
    WWDG_EnableIT();  
}


void test_wwdg(void)
{
      LED_Init();
      WWDGConfig();
}

   
/*******************************************************************************
* Function Name  : WWDG_IRQHandler
* Description    : ���ڿ��Ź��жϷ��� ��4��˸ ι������
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void WWDG_IRQHandler(void)//���ڿ��Ź��жϳ���ι������
{
        static uint32_t count=0;
        if(count>=10)//��10���ж�LEC4��תһ�� 10*0x40(64)/ 2197=291ms  
        {
          LEDTog(LED4);
          count = 0;
        }
        else count++;
        
        WWDG_SetCounter(0x7f); // ���ü�����ֵΪ0x7f 
        
        WWDG_ClearFlag(); // ������ڻ����жϱ�־λ 
}


   #ifdef  RT_USING_FINSH 
   #include  <finsh.h> 
   /*  ���CRCTest IWDGTest WWDGTest������finsh  shell�� */ 
   FINSH_FUNCTION_EXPORT (test_iwdg,  startup  IWDG test); 
   FINSH_FUNCTION_EXPORT (test_wwdg,  startup  WWDG test); 
     
   #endif 
