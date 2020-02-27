#ifndef _I2C_EE_H_
#define _I2C_EE_H_

#include "stm32f4xx.h"

#define EEPROM_DEV_ADDR			0xA0		        /* 24xx02的设备地址 */
#define EEPROM_PAGE_SIZE		  8			        /* 24xx02的页面大小 */
#define EEPROM_SIZE				  256			    /* 24xx02总容量 */

uint8_t ee_CheckOk(void);
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
void ee_Erase(void);
uint8_t ee_Test(int kind, uint8_t Rec[]);
void ee_Read(uint8_t read_buf[], uint8_t write_buf[], uint16_t len, uint8_t kind);
void ee_Write(uint8_t write_buf[], uint16_t len);
#endif
