#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./KEY/psb_key.h"
#include "bsp_iic_ee.h"
#include "string.h"
#define EEP_Firstpage 0x00

const char str[] = "祖张依世界第一可爱！荔枝第二！";
uint8_t I2C_Buf_Write[256];
uint8_t I2C_Buf_Read[256];
uint16_t len;

uint8_t I2C_Test(void);
uint8_t Write_EEPROM_TEST(void);
uint8_t Read_EEPROM_TEST(void);
void Delay(__IO u32 nCount);
void LED(int n);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	//char input;
	//int count = 0;
	/* LED 端口初始化 */
	LED_GPIO_Config();
	KEY_GPIO_Config();
	Debug_USART_Config();
	/* 控制LED灯 */
	//Usart_SendString(DEBUG_USART, "祖张依，我……想你\n");
	LED_RED;
	printf("\r\n 欢迎使用野火  STM32 F407 开发板。\r\n");
	printf("\r\n 这是一个I2C外设(AT24C02)读写测试例程 \r\n");
	I2C_EE_Init();

	LED_BLUE;
	while (1)
	{
		if (KEY_ON == KEY_Scan(KEY1_GPIO_PORT, KEY1_PIN))
		{
			Write_EEPROM_TEST();
			LED_PURPLE;
		}
		if (KEY_ON == KEY_Scan(KEY2_GPIO_PORT, KEY2_PIN))
		{
			Read_EEPROM_TEST();
			LED_GREEN;
		}
	}
}

uint8_t Write_EEPROM_TEST(void)
{
	MainFuncationGetData(I2C_Buf_Write, &len);
	//len = GetLength();
	EEPROM_INFO("写入的数据");

//	for (i = 0; i < len; i++)
//	{
//		I2C_Buf_Write[i] = usart_RecData[i];
//	}
	printf("%s\r\n", I2C_Buf_Write);
	I2C_EE_BufferWrite(I2C_Buf_Write, EEP_Firstpage, len);

	EEPROM_INFO("I2C(AT24C02)读测试成功");
	return 1;
}

uint8_t Read_EEPROM_TEST(void)
{
	u16 i;
	EEPROM_INFO("读出的数据");

	I2C_EE_BufferRead(I2C_Buf_Read, EEP_Firstpage, len);

	for (i = 0; i < len; i++)
	{
		if (I2C_Buf_Read[i] != I2C_Buf_Write[i])
		{
			printf("%s\r\n", I2C_Buf_Read);
			EEPROM_ERROR("错误:I2C EEPROM写入与读出的数据不一致");
			return 0;
		}
	}
	printf("%s\r\n", I2C_Buf_Read);
	EEPROM_INFO("I2C(AT24C02)读测试成功");
	return 1;
}

/**
  * @brief  I2C(AT24C02)读写测试
  * @param  无
  * @retval 正常返回1 ，不正常返回0
  */
uint8_t I2C_Test(void)
{
	u16 i, len;
	len = strlen(str);
	EEPROM_INFO("写入的数据");

	for (i = 0; i <= len; i++) //填充缓冲
	{
		I2C_Buf_Write[i] = str[i];

		if (i % 16 == 15)
			printf("\n\r");
	}
	printf("%s\r\n ", I2C_Buf_Write);
	//将I2C_Buf_Write中顺序递增的数据写入EERPOM中
	I2C_EE_BufferWrite(I2C_Buf_Write, EEP_Firstpage, len);

	EEPROM_INFO("写成功");

	EEPROM_INFO("读出的数据");
	//将EEPROM读出数据顺序保持到I2C_Buf_Read中
	I2C_EE_BufferRead(I2C_Buf_Read, EEP_Firstpage, len);

	//将I2C_Buf_Read中的数据通过串口打印
	for (i = 0; i < len; i++)
	{
		if (I2C_Buf_Read[i] != I2C_Buf_Write[i])
		{
			printf("%s\r\n", I2C_Buf_Read);
			EEPROM_ERROR("错误:I2C EEPROM写入与读出的数据不一致");
			return 0;
		}

		//    if(i%16 == 15)
		//        printf("\n\r");
	}
	printf("%s\r\n", I2C_Buf_Read);
	EEPROM_INFO("I2C(AT24C02)读写测试成功");
	return 1;
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
