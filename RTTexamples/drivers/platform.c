#include <rtthread.h>
#include <board.h>

#ifdef RT_USING_LWIP
#include "stm32_eth.h"
#endif /* RT_USING_LWIP */

#ifdef RT_USING_SPI
#include "rt_stm32f10x_spi.h"

#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
#include "msd.h"
#endif /* RT_USING_DFS */

#ifdef RT_USING_SPI3
#include "touch.h"
/*
SPI3_MOSI: PC12
SPI3_MISO: PC11
SPI3_SCK : PC10

CS3: PC8  SPI touch
*/
/*�궨�� */
#define TOUCH_SPI_X                 SPI3
#define TOUCH_SPI_RCC               RCC_APB1Periph_SPI3

#define TOUCH_PORT                  GPIOC                   
#define TOUCH_GPIO_RCC              RCC_APB2Periph_GPIOC

#define TOUCH_INT_PIN               GPIO_Pin_6 
#define TOUCH_CS_PIN                GPIO_Pin_8 
#define TOUCH_SCK_PIN               GPIO_Pin_10
#define TOUCH_MISO_PIN              GPIO_Pin_11
#define TOUCH_MOSI_PIN              GPIO_Pin_12     

static void rt_hw_spi3_init(void)
{
    /* register spi bus */
    {
        static struct stm32_spi_bus stm32_spi;
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(TOUCH_GPIO_RCC | RCC_APB2Periph_AFIO, ENABLE);

        GPIO_PinRemapConfig(GPIO_Remap_SPI3,ENABLE);
        
        /*!< SPI SCK  MISO MOSI pin configuration */
        GPIO_InitStructure.GPIO_Pin   = TOUCH_SCK_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;   
        GPIO_Init(TOUCH_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = TOUCH_MISO_PIN;
        GPIO_Init(TOUCH_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = TOUCH_MOSI_PIN;
        GPIO_Init(TOUCH_PORT, &GPIO_InitStructure);

        stm32_spi_register(SPI3, &stm32_spi, "spi3");
    }
    /* attach cs */
    {
        static struct rt_spi_device spi_device;
        static struct stm32_spi_cs  spi_cs;
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(TOUCH_GPIO_RCC , ENABLE);
        GPIO_InitStructure.GPIO_Pin   = TOUCH_CS_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 

        /* spi3: PC8 */
        spi_cs.GPIOx = GPIOC;
        spi_cs.GPIO_Pin = GPIO_Pin_8;

        GPIO_InitStructure.GPIO_Pin = spi_cs.GPIO_Pin;

        GPIO_Init(spi_cs.GPIOx, &GPIO_InitStructure);
        GPIO_SetBits(spi_cs.GPIOx, spi_cs.GPIO_Pin);

        rt_spi_bus_attach_device(&spi_device, "spi30", "spi3", (void*)&spi_cs);
    }
}
#endif
/* RT_USING_SPI3 */

/*
 * SPI1_MOSI: PA7
 * SPI1_MISO: PA6
 * SPI1_SCK : PA5
 *
 * CS0: PA4  SD card.
*/
static void rt_hw_spi_init(void)
{
#ifdef RT_USING_SPI1
    /* register spi bus */
    {
        static struct stm32_spi_bus stm32_spi;
        GPIO_InitTypeDef GPIO_InitStructure;

        /* Enable GPIO clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,
        ENABLE);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        stm32_spi_register(SPI1, &stm32_spi, "spi1");
    }

    /* attach cs */
    {
        static struct rt_spi_device spi_device;
        static struct stm32_spi_cs  spi_cs;

        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

        /* spi21: PG10 */
        spi_cs.GPIOx = GPIOA;
        spi_cs.GPIO_Pin = GPIO_Pin_4;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin = spi_cs.GPIO_Pin;
        GPIO_SetBits(spi_cs.GPIOx, spi_cs.GPIO_Pin);
        GPIO_Init(spi_cs.GPIOx, &GPIO_InitStructure);

        rt_spi_bus_attach_device(&spi_device, "spi10", "spi1", (void*)&spi_cs);
    }
#endif /* RT_USING_SPI1 */
}
#endif /* RT_USING_SPI */


void rt_platform_init(void)
{
#ifdef RT_USING_SPI
    rt_hw_spi_init();

#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
    /* init sdcard driver */
    {
        extern void rt_hw_msd_init(void);
        GPIO_InitTypeDef  GPIO_InitStructure;

        /* PC4 : SD Power */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        /* SD card power on. */
        GPIO_ResetBits(GPIOC, GPIO_Pin_4);
        rt_thread_delay(2);

        msd_init("sd0", "spi10");
    }
#endif /* RT_USING_DFS && RT_USING_DFS_ELMFAT */
    
#ifdef   RT_USING_SPI3
    rt_hw_spi3_init();
#endif
   
#ifdef RT_USING_RTGUI
    /* initilize touch panel */
    rtgui_touch_hw_init("spi30");
#endif /* RT_USING_RTGUI */
    
#endif // RT_USING_SPI

#ifdef RT_USING_LWIP
    /* initialize eth interface */
    rt_hw_stm32_eth_init();
#endif /* RT_USING_LWIP */
    
#ifdef RT_USING_RTGUI
    /* initilize  lcd controller */
     lcd_init();
#endif
    rt_thread_delay(50);
    rt_device_init_all();
}
