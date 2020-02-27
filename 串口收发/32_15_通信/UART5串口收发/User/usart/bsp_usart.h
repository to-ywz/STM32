#ifndef __BSP_USART_H
#define	__BSP_USART_H

#include "stm32f4xx.h"
#include <stdio.h>


//���Ŷ���
/*******************************************************/
#define DEBUG_USART                             UART5
#define DEBUG_USART_CLK                         RCC_APB1Periph_UART5
#define DEBUG_USART_BAUDRATE                    115200  //���ڲ�����

#define DEBUG_USART_RX_GPIO_PORT                GPIOD
#define DEBUG_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define DEBUG_USART_RX_PIN                      GPIO_Pin_2
#define DEBUG_USART_RX_AF                       GPIO_AF_UART5
#define DEBUG_USART_RX_SOURCE                   GPIO_PinSource2

#define DEBUG_USART_TX_GPIO_PORT                GPIOC
#define DEBUG_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define DEBUG_USART_TX_PIN                      GPIO_Pin_12
#define DEBUG_USART_TX_AF                       GPIO_AF_UART5
#define DEBUG_USART_TX_SOURCE                   GPIO_PinSource12

#define DEBUG_USART_IRQHandler                  UART5_IRQHandler
#define DEBUG_USART_IRQ                 		UART5_IRQn
/************************************************************/

void Debug_USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* __UART5_H */
