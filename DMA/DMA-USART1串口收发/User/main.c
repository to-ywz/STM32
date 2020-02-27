/****************************************************************
 * 
 *	@brief		 		DMA接收串口数据并转发
 *	@author		秦殇
 *  @version	2020/01/23
 *	@attention 如果想发送任意长度的数据,需要自行打包与解包
 * 
******************************************************************/

#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./KEY/psb_key.h"
//#include "dma.h"
#include "./usart/bsp_usart.h"

void LED(int n);
void Delay(__IO u32 nCount);

int main(void)
{
	//LED初始化
	LED_GPIO_Config();
	//按键初始化
	KEY_GPIO_Config();

	Init_Usart_Config(); //串口初始化
	DMA_USART_Config();  //DMA——USART配置

	while (1)
	{
		//接收到一帧数据
		if (flag)
		{
			//将接收到的数据发送到设备
			DMA_Send_Enable(USART_TX_DMA_STREAM, rx_len);

			//数据帧接收标志清零
			flag = 0;
		}
		//LED灯闪烁，表示程序正常运行
		LED2_TOGGLE;
		Delay(0x6fffff);
	}
}

void Delay(__IO u32 nCount)
{
	while (nCount--)
		;
}

void LED(int n)
{
	switch (n)
	{
	case 1:
		LED_RED;
		;
		break;
	case 2:
		LED_RGBOFF;
		;
		break;
	}
}

/*********************************************END OF FILE**********************/
