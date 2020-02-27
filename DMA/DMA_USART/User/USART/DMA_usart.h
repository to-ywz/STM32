#ifndef _DMA_USART_H_
#define _DMA_USART_H_

#include "stm32f4xx.h"
#include <stdio.h>
//USART
#define USART                   USART1
#define USART_BAUDRATE          115200L
#define USART_CLK               RCC_APB2Periph_USART1

//RX
#define USART_RX_GPIO_PORT      GPIOA
#define USART_RX_GPIO_PIN       GPIO_Pin_10
#define USART_RX_GPIO_CLK       RCC_AHB1Periph_GPIOA
#define USART_RX_AF             GPIO_AF_USART1
#define USART_RX_SOURCE         GPIO_PinSource10
//TX
#define USART_TX_GPIO_PORT      GPIOA
#define USART_TX_GPIO_PIN       GPIO_Pin_9
#define USART_TX_GPIO_CLK       RCC_AHB1Periph_GPIOA
#define USART_TX_AF             GPIO_AF_USART1
#define USART_TX_SOURCE         GPIO_PinSource9

//DMA
#define USART_DR_BASE           (USART1_BASE + 0x04) //通道4数据流7 DMA to USART
#define SENDBUFF_SIZE           32                //发送数据大小
#define USART_DMA_CLK           RCC_AHB1Periph_DMA2
#define USART_DMA_CHANNEL       DMA_Channel_4       //通道4
#define USART_DMA_STREAM        DMA2_Stream7        //数据流7

void USART_DMA_Config(void);
void USART_Config(void);
#endif
