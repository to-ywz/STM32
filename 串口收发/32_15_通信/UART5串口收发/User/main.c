/**
 * @brief  
 * @note   
 * @param  nCount: 
 * @retval None
 */
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./KEY/psb_key.h"
#define MAXRANGE 8

//void Delay(__IO u32 nCount);
//void LED(int n);
uint8_t RxData;
void Delay(__IO u32 nCount);
static void Show_Message(void);
void _7_ColorLED(unsigned char count);
unsigned char RX_DataProcess(char Data, unsigned char count);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	char temp = '\0';
	char RX_Data;
	unsigned char count = 0, cntemp;
	/* LED 端口初始化 */
	LED_GPIO_Config();
	KEY_GPIO_Config();
	Debug_USART_Config();
	/* 控制LED灯 */
	
	Show_Message();
	Usart_SendString(DEBUG_USART, "祖张依，我……想你\n");
	
	while (1)
	{
		//RX_Data = getchar();
		count = RX_DataProcess(RxData, count);
		if (RxData != temp || count != cntemp)
		{
			printf("Main:接收到的字符%c\r\n", RxData);
			printf("LED计数器：%d\n", count);
		}

		temp = RxData;
		cntemp = count;
		if (KEY_ON == KEY_Scan(KEY1_GPIO_PORT, KEY1_PIN))
		{
			Usart_SendByte(DEBUG_USART, 'A');
			LED_RED;
			//LED('A' - 64);
		}
		if (KEY_ON == KEY_Scan(KEY2_GPIO_PORT, KEY2_PIN))
		{
			Usart_SendByte(DEBUG_USART, 'B');
			LED_RGBOFF;
			//LED('B' - 64);
		}
		_7_ColorLED(count);
	}
}

static void Show_Message(void)
{
  printf("\r\n   这是一个通过串口通信指令控制RGB彩灯实验 \n");
  printf("使用  USART1  参数为：%d 8-N-1 \n", DEBUG_USART_BAUDRATE);
  printf("开发板接到指令后控制RGB彩灯颜色，指令对应如下：\n");
  printf("   指令   ------ 彩灯颜色 \n");
  printf("     1    ------    红 \n");
  printf("     2    ------    绿 \n");
  printf("     3    ------    蓝 \n");
  printf("     4    ------    黄 \n");
  printf("     5    ------    紫 \n");
  printf("     6    ------    青 \n");
  printf("     7    ------    白 \n");
  printf("     8    ------    灭 \n");  
}

unsigned char RX_DataProcess(char Data, unsigned char count)
{
	switch (Data)
	{
		case 'A': 
			return 0 == ((count+1)%MAXRANGE) ? 1 : ((count+1)%MAXRANGE);
		case 'B': 
			return 0 == ((count-1+MAXRANGE)%MAXRANGE) ? 7 : ((count-1+MAXRANGE)%MAXRANGE);
		case 'C': 
			return 8;
		default: return 0;
	}
}

void _7_ColorLED(unsigned char count)
{
	switch(count)
	{
	  case 1:
		LED_RED;
	  break;
	  case 2:
		LED_GREEN;
	  break;
	  case 3:
		LED_BLUE;
	  break;
	  case 4:
		LED_YELLOW;
	  break;
	  case 5:
		LED_PURPLE;
	  break;
	  case 6:
		LED_CYAN;
	  break;
	  case 7:
		LED_WHITE;
	  break;
	  case 8:
		LED_RGBOFF;
	  break;
	  default:
		/* 如果不是指定指令字符，打印提示信息 */
		//Show_Message();
		break;      
	}
}

//void LED(int n)
//{
//	switch (n)
//	{
//	case 1:
//		LED_RED;
//		;
//		break;
//	case 2:
//		LED_RGBOFF;
//		;
//		break;
//	}
//}
/*********************************************END OF FILE**********************/
