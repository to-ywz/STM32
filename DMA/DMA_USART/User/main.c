/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   演示使用DMA进行串口传输
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stdio.h"
#include "string.h"
#include "stm32f4xx.h"
#include "./USART/DMA_usart.h"
#include "./led/bsp_led.h"

extern uint8_t SendBuff[SENDBUFF_SIZE];
uint8_t SendData[] = "祖张依,真是超级可爱呢^+^\"喵~\r\n";

static void Delay(__IO u32 nCount);
static int GetStringLength(char *str);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    int Len = sizeof(SendData)/sizeof(char);
    /* 初始化USART */
    USART_Config();

    /* 配置使用DMA模式 */
    USART_DMA_Config();

    /* 配置RGB彩色灯 */
    LED_GPIO_Config();

    //printf("\r\n USART1 DMA TX 测试 \r\n");

    /*填充将要发送的数据*/
	//strcpy(SendBuff, SendData);
	for (int i = 1; i < Len; i++)
	{//汉字DMA转串口数组索引必须从差1，第一个汉字存在1~2中
		SendBuff[i] = SendData[i-1];
	}
	
	printf("\r\n");
	
    /*为演示DMA持续运行而CPU还能处理其它事情，持续使用DMA发送数据，量非常大，
  *长时间运行可能会导致电脑端串口调试助手会卡死，鼠标乱飞的情况，
  *或把DMA配置中的循环模式改为单次模式*/

    /* USART1 向 DMA发出TX请求 */
    USART_DMACmd(USART, USART_DMAReq_Tx, ENABLE);

    /* 此时CPU是空闲的，可以干其他的事情 */
    //例如同时控制LED
    while (1)
    {
        LED1_TOGGLE
        Delay(0xFFFFF);
    }
}

static void Delay(__IO uint32_t nCount) //简单的延时函数
{
    for (; nCount != 0; nCount--)
        ;
}

static int GetStringLength(char *str)
{
    int length = 0;
    while ('\0' != str[length])
    {
        length++;
    }
    return length;
}

/*********************************************END OF FILE**********************/
