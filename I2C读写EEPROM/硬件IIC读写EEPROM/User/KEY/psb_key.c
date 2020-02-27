#include "./KEY/psb_key.h"

void KEY_GPIO_Config(void)
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启LED相关的GPIO外设时钟*/
    RCC_AHB1PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK, ENABLE);

    /*选择要控制的GPIO引脚*/
    GPIO_InitStructure.GPIO_Pin = KEY1_PIN;

    /*设置引脚模式为输入模式*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

    /*设置引脚为无模式*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;


    /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
	    /*设置引脚模式为输入模式*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

    /*设置引脚为无模式*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);

    
}

uint8_t KEY_Scan(GPIO_TypeDef *GPIO, uint16_t GPIO_Pin)
{
    if (GPIO_ReadInputDataBit(GPIO, GPIO_Pin) == KEY_ON)
    {
        while (GPIO_ReadInputDataBit(GPIO, GPIO_Pin) == KEY_ON);
        return KEY_ON;
    }
    else
    {
        return KEY_OFF;
    }
}
