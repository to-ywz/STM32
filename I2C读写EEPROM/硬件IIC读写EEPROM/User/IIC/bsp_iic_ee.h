#ifndef __IIC_EE_H
#define __IIC_EE_H

#include "stm32f4xx.h"

// AT24C01/02每页有8个字节
#define I2C_PageSize                            8

// AT24C04/08A/16A每页有16个字节
//#define I2C_PageSize           16		

//STM32 I2C 快速模式
#define I2C_Speed                               400000

// 这个地址只要与STM32外挂的I2C器件地址不一样即可
#define I2C_OWN_ADDRESS7                        0X0A

//I2C interface
#define EEPROM_I2C                              I2C1
#define EEPROM_I2C_CLK                          RCC_APB1Periph_I2C1
#define EEPROM_I2C_CLK_INIT						RCC_APB1PeriphClockCmd

#define EEPROM_I2C_SCL_PIN                      GPIO_Pin_8
#define EEPROM_I2C_SCL_GPIO_PORT                GPIOB
#define EEPROM_I2C_SCL_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SCL_GPIO_SOURCE              GPIO_PinSource8
#define EEPROM_I2C_SCL_GPIO_AF                  GPIO_AF_I2C1


#define EEPROM_I2C_SDA_PIN                 		GPIO_Pin_9
#define EEPROM_I2C_SDA_GPIO_PORT                GPIOB
#define EEPROM_I2C_SDA_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SDA_GPIO_SOURCE              GPIO_PinSource9
#define EEPROM_I2C_SDA_GPIO_AF                  GPIO_AF_I2C1

//等待超时时间
#define I2CT_FLAG_TIMEOUT                       ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT                       (uint32_t)(10 * I2CT_FLAG_TIMEOUT)

//信息输出
#define EEPROM_DEBUG_ON                         0


#define EEPROM_INFO(fmt, args...)               printf("<<-EEPROM-INFO->>"fmt"\n", ##args)
#define EEPROM_ERROR(fmt,arg...)                printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define EEPROM_DEBUG(fmt,arg...)                do{\
                                                if(EEPROM_DEBUG_ON)\
                                                printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                                }while(0)

/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS                   0xA0   /* E2 = 0 */
//#define EEPROM_Block1_ADDRESS                 0xA2 /* E2 = 0 */
//#define EEPROM_Block2_ADDRESS                 0xA4 /* E2 = 0 */
//#define EEPROM_Block3_ADDRESS                 0xA6 /* E2 = 0 */

void I2C_EE_Init(void);
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
uint32_t I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr);
uint8_t I2C_EE_ByetsWrite(uint8_t* pBuffer,uint8_t WriteAddr, uint16_t NumByteToWrite);
uint32_t I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
uint32_t I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void);


#endif
