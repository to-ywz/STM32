#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./KEY/psb_key.h"
#include "bsp_iic_ee.h"
#include "string.h"
#define EEP_Firstpage 0x00

const char str[] = "�����������һ�ɰ�����֦�ڶ���";
uint8_t I2C_Buf_Write[256];
uint8_t I2C_Buf_Read[256];
uint16_t len;

uint8_t I2C_Test(void);
uint8_t Write_EEPROM_TEST(void);
uint8_t Read_EEPROM_TEST(void);
void Delay(__IO u32 nCount);
void LED(int n);

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	//char input;
	//int count = 0;
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();
	KEY_GPIO_Config();
	Debug_USART_Config();
	/* ����LED�� */
	//Usart_SendString(DEBUG_USART, "���������ҡ�������\n");
	LED_RED;
	printf("\r\n ��ӭʹ��Ұ��  STM32 F407 �����塣\r\n");
	printf("\r\n ����һ��I2C����(AT24C02)��д�������� \r\n");
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
	EEPROM_INFO("д�������");

//	for (i = 0; i < len; i++)
//	{
//		I2C_Buf_Write[i] = usart_RecData[i];
//	}
	printf("%s\r\n", I2C_Buf_Write);
	I2C_EE_BufferWrite(I2C_Buf_Write, EEP_Firstpage, len);

	EEPROM_INFO("I2C(AT24C02)�����Գɹ�");
	return 1;
}

uint8_t Read_EEPROM_TEST(void)
{
	u16 i;
	EEPROM_INFO("����������");

	I2C_EE_BufferRead(I2C_Buf_Read, EEP_Firstpage, len);

	for (i = 0; i < len; i++)
	{
		if (I2C_Buf_Read[i] != I2C_Buf_Write[i])
		{
			printf("%s\r\n", I2C_Buf_Read);
			EEPROM_ERROR("����:I2C EEPROMд������������ݲ�һ��");
			return 0;
		}
	}
	printf("%s\r\n", I2C_Buf_Read);
	EEPROM_INFO("I2C(AT24C02)�����Գɹ�");
	return 1;
}

/**
  * @brief  I2C(AT24C02)��д����
  * @param  ��
  * @retval ��������1 ������������0
  */
uint8_t I2C_Test(void)
{
	u16 i, len;
	len = strlen(str);
	EEPROM_INFO("д�������");

	for (i = 0; i <= len; i++) //��仺��
	{
		I2C_Buf_Write[i] = str[i];

		if (i % 16 == 15)
			printf("\n\r");
	}
	printf("%s\r\n ", I2C_Buf_Write);
	//��I2C_Buf_Write��˳�����������д��EERPOM��
	I2C_EE_BufferWrite(I2C_Buf_Write, EEP_Firstpage, len);

	EEPROM_INFO("д�ɹ�");

	EEPROM_INFO("����������");
	//��EEPROM��������˳�򱣳ֵ�I2C_Buf_Read��
	I2C_EE_BufferRead(I2C_Buf_Read, EEP_Firstpage, len);

	//��I2C_Buf_Read�е�����ͨ�����ڴ�ӡ
	for (i = 0; i < len; i++)
	{
		if (I2C_Buf_Read[i] != I2C_Buf_Write[i])
		{
			printf("%s\r\n", I2C_Buf_Read);
			EEPROM_ERROR("����:I2C EEPROMд������������ݲ�һ��");
			return 0;
		}

		//    if(i%16 == 15)
		//        printf("\n\r");
	}
	printf("%s\r\n", I2C_Buf_Read);
	EEPROM_INFO("I2C(AT24C02)��д���Գɹ�");
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
