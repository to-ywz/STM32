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
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	char temp = '\0';
	char RX_Data;
	unsigned char count = 0, cntemp;
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();
	KEY_GPIO_Config();
	Debug_USART_Config();
	/* ����LED�� */
	
	Show_Message();
	Usart_SendString(DEBUG_USART, "���������ҡ�������\n");
	
	while (1)
	{
		//RX_Data = getchar();
		count = RX_DataProcess(RxData, count);
		if (RxData != temp || count != cntemp)
		{
			printf("Main:���յ����ַ�%c\r\n", RxData);
			printf("LED��������%d\n", count);
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
  printf("\r\n   ����һ��ͨ������ͨ��ָ�����RGB�ʵ�ʵ�� \n");
  printf("ʹ��  USART1  ����Ϊ��%d 8-N-1 \n", DEBUG_USART_BAUDRATE);
  printf("������ӵ�ָ������RGB�ʵ���ɫ��ָ���Ӧ���£�\n");
  printf("   ָ��   ------ �ʵ���ɫ \n");
  printf("     1    ------    �� \n");
  printf("     2    ------    �� \n");
  printf("     3    ------    �� \n");
  printf("     4    ------    �� \n");
  printf("     5    ------    �� \n");
  printf("     6    ------    �� \n");
  printf("     7    ------    �� \n");
  printf("     8    ------    �� \n");  
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
		/* �������ָ��ָ���ַ�����ӡ��ʾ��Ϣ */
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
