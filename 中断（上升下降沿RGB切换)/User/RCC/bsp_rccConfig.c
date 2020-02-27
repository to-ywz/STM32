#include "bsp_rccConfig.h"

void HSE_SetSysClock(void)
{
	ErrorStatus HSE_ErrorStatus = ERROR;
	//复位RCC的所有寄存器
	RCC_DeInit();
	
	RCC_HSEConfig((uint32_t)RCC_HSE_ON);
	
	//使能 HSE
	HSE_ErrorStatus = RCC_WaitForStartUp();

	if (HSE_ErrorStatus == SUCCESS)
	{
		//选择电压调节器的模式1
		RCC->APB1ENR |= RCC_APB1ENR_PWREN;
		PWR->CR |= PWR_CR_VOS;

		RCC_HCLKConfig(RCC_HCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div4);
		RCC_PCLK2Config(RCC_HCLK_Div2);
	}
	else
	{
		//HSE启动失败，添加异常处理代码
	}
	
} 