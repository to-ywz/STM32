#ifndef _BSP_IIC_GPIO_H
#define _BSP_IIC_GPIO_H

#include "stm32f4xx.h"
#include <inttypes.h>

#define REGISTER            0           /* IOʵ�ַ�ʽѡ�� �����register��*/

#define EEPROM_I2C_WR       0           /* д����λ*/
#define EEPROM_I2C_RD       1           /* ������λ*/

// ����I2C��������GPIO�˿�, һ����˵RCCʱ���߲��øĶ�ֻ��Ҫ�Ķ�PORT �� Pin 
#define EEPROM_I2C_GPIO_PORT            GPIOB                   //GPIO�˿�
#define EEPROM_I2C_GPIO_CLK             RCC_AHB1Periph_GPIOB    //GPIOʱ���߶˿ڣ�һ�㲻�ģ�        
#define EEPROM_I2C_SCL_PIN              GPIO_Pin_8              //���ӵ�SCLʱ���ߵ�GPIO����
#define EEPROM_I2C_SDA_PIN              GPIO_Pin_9              //���ӵ�SDA�����ߵ�GPIO����

/* �����дSCL��SDA�ĺ� */
#if !REGISTER   /* ��׼�����IO */
    #define EEPROM_I2C_SCL_1()            GPIO_SetBits(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SCL_PIN)              // SCL = 1
    #define EEPROM_I2C_SCL_0()            GPIO_ResetBits(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SCL_PIN)            // SCL = 0

    #define EEPROM_I2C_SDA_1()            GPIO_SetBits(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SDA_PIN)              // SDA = 1
    #define EEPROM_I2C_SDA_0()            GPIO_ResetBits(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SDA_PIN)            // SDA = 0

	#define EEPROM_I2C_SDA_READ()         GPIO_ReadInputDataBit(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SDA_PIN)	  //��SDA����״̬
#else /* �Ĵ�������IO */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
    #define EEPROM_I2C_SCL_1()            EEPROM_I2C_GPIO_PORT->BSRRL = EEPROM_I2C_SCL_PIN;
    #define EEPROM_I2C_SCL_0()            EEPROM_I2C_GPIO_PORT->BSRRH = EEPROM_I2C_SCL_PIN;

    #define EEPROM_I2C_SDA_0()            EEPROM_I2C_GPIO_PORT->BSRRL = EEPROM_I2C_SDA_PIN;
    #define EEPROM_I2C_SDA_1()            EEPROM_I2C_GPIO_PORT->BSRRH = EEPROM_I2C_SDA_PIN;

    #define EEPROM_I2C_SDA_READ()         ((EEPROM_I2C_GPIO_PORT->IDR & EEPROM_I2C_SDA_PIN) != 0)
#endif

void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(uint8_t _ucByte);
uint8_t I2C_ReadByte(void);
uint8_t I2C_WaitAck(void);
void I2C_Ack(void);
void I2C_NAck(void);
uint8_t I2C_CheackDevice(uint8_t _Address);

#endif /* _BSP_I2C_GPIO_H */

