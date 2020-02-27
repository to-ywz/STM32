#ifndef _BSP_EXTI_H
#define _BSP_EXTI_H

#include "stm32f4xx.h"

//引脚定义
/*******************************************************/
#define KEY1_INT_GPIO_PORT                GPIOA
#define KEY1_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define KEY1_INT_GPIO_PIN                 GPIO_Pin_0
#define KEY1_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE           EXTI_PinSource0
#define KEY1_INT_EXTI_LINE                EXTI_Line0
#define KEY1_INT_EXTI_IRQ                 EXTI0_IRQn

#define KEY1_IRQHandler                   EXTI0_IRQHandler//

#define KEY2_INT_GPIO_PORT                GPIOC
#define KEY2_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define KEY2_INT_GPIO_PIN                 GPIO_Pin_13
#define KEY2_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOC
#define KEY2_INT_EXTI_PINSOURCE           EXTI_PinSource13
#define KEY2_INT_EXTI_LINE                EXTI_Line13
#define KEY2_INT_EXTI_IRQ                 EXTI15_10_IRQn

#define KEY2_IRQHandler                   EXTI15_10_IRQHandler//与stm32f4xx文件相关

#define KEY3_INT_GPIO_PORT                GPIOB
#define KEY3_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define KEY3_INT_GPIO_PIN                 GPIO_Pin_3
#define KEY3_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOB
#define KEY3_INT_EXTI_PINSOURCE           EXTI_PinSource3
#define KEY3_INT_EXTI_LINE                EXTI_Line3
#define KEY3_INT_EXTI_IRQ                 EXTI3_IRQn

#define KEY3_IRQHandler                   EXTI3_IRQHandler//

/*******************************************************/

void EXTI_Key_Config();
static void NVIC_Configuration(void);

#endif
