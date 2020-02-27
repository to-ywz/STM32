#include "bsp_exti.h"

//配置NVIC寄存器
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置NVIC为优先级组1 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* 配置中断源：按键1 */
    NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
    /* 配置抢占优先级：1 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    /* 配置子优先级：1 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    /* 使能中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 配置中断源：按键2，其他使用上面相关配置 */
    NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);
	
	//配置中断源：外置按键3
	//NVIC_InitStructure.NVIC_IRQChannel = KEY3_INT_EXTI_IRQ;
    //NVIC_Init(&NVIC_InitStructure);
}

#if 1

void EXTI_Key_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    //开启按键GPIO口的时钟
    RCC_AHB1PeriphClockCmd(KEY1_INT_GPIO_CLK | KEY2_INT_GPIO_CLK, ENABLE);

    //使能 SYSCFG 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    //配置 NVIC
    NVIC_Configuration();

    //选择按键1的引脚
    GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    /* 设置引脚不上拉也不下拉 */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    /* 使用上面的结构体初始化按键 */
    GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

    /* 连接 EXTI 中断源 到key1引脚 */
    SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);

    /* 选择 EXTI 中断源 */
    EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
    /* 中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿触发 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    /* 使能中断/事件线 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


    /* 选择按键2的引脚 */
    GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
    /* 其他配置与上面相同 */
    GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

    /* 连接 EXTI 中断源 到key2 引脚 */
    SYSCFG_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE);

    /* 选择 EXTI 中断源 */
    EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 上升沿触发 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	/* 选择外置按键3的引脚 */
//    GPIO_InitStructure.GPIO_Pin = KEY3_INT_GPIO_PIN;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    /* 其他配置与上面相同 */
//    GPIO_Init(KEY3_INT_GPIO_PORT, &GPIO_InitStructure);

//    /* 连接 EXTI 中断源 到key3 引脚 */
//    SYSCFG_EXTILineConfig(KEY3_INT_EXTI_PORTSOURCE, KEY3_INT_EXTI_PINSOURCE);

//    /* 选择 EXTI 中断源 */
//    EXTI_InitStructure.EXTI_Line = KEY3_INT_EXTI_LINE;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    /* 上升沿触发 */
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);
}

#endif
