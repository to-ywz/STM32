#ifndef _BSP_IIC_GPIO_H
#define _BSP_IIC_GPIO_H

#include "stm32f4xx.h"
#include <inttypes.h>

#define REGISTER            0           /* IO实现方式选择 （库和register）*/

#define EEPROM_I2C_WR       0           /* 写控制位*/
#define EEPROM_I2C_RD       1           /* 读操作位*/

// 定义I2C总线连接GPIO端口, 一般来说RCC时钟线不用改动只需要改动PORT 和 Pin 
#define EEPROM_I2C_GPIO_PORT            GPIOB                   //GPIO端口
#define EEPROM_I2C_GPIO_CLK             RCC_AHB1Periph_GPIOB    //GPIO时钟线端口（一般不改）        
#define EEPROM_I2C_SCL_PIN              GPIO_Pin_8              //连接到SCL时钟线的GPIO引脚
#define EEPROM_I2C_SDA_PIN              GPIO_Pin_9              //连接到SDA数据线的GPIO引脚

/* 定义读写SCL和SDA的宏 */
#if !REGISTER   /* 标准库操作IO */
    #define EEPROM_I2C_SCL_1()            GPIO_SetBits(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SCL_PIN)              // SCL = 1
    #define EEPROM_I2C_SCL_0()            GPIO_ResetBits(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SCL_PIN)            // SCL = 0

    #define EEPROM_I2C_SDA_1()            GPIO_SetBits(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SDA_PIN)              // SDA = 1
    #define EEPROM_I2C_SDA_0()            GPIO_ResetBits(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SDA_PIN)            // SDA = 0

	#define EEPROM_I2C_SDA_READ()         GPIO_ReadInputDataBit(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SDA_PIN)	  //读SDA口线状态
#else /* 寄存器操作IO */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
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

