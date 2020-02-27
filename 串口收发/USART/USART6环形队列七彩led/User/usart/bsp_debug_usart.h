#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f4xx.h"
#include <stdio.h>


//引脚定义
/*******************************************************/
#define DEBUG_USART                             USART6
#define DEBUG_USART_CLK                         RCC_APB2Periph_USART6
#define DEBUG_USART_BAUDRATE                    115200UL  //串口波特率

#define DEBUG_USART_RX_GPIO_PORT                GPIOC
#define DEBUG_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define DEBUG_USART_RX_PIN                      GPIO_Pin_7
#define DEBUG_USART_RX_AF                       GPIO_AF_USART6
#define DEBUG_USART_RX_SOURCE                   GPIO_PinSource7

#define DEBUG_USART_TX_GPIO_PORT                GPIOC
#define DEBUG_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define DEBUG_USART_TX_PIN                      GPIO_Pin_6
#define DEBUG_USART_TX_AF                       GPIO_AF_USART6
#define DEBUG_USART_TX_SOURCE                   GPIO_PinSource6

#define DEBUG_USART_IRQHandler                  USART6_IRQHandler
#define DEBUG_USART_IRQ                 				USART6_IRQn
/************************************************************/

void Debug_USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);

void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* __USART6_H */
