#include "stm32l0xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "KEY.h"

uint8_t KEY_Scan(void)
{
	uint8_t key_val;
	if(HAL_GPIO_ReadPin (GPIOA,GPIO_PIN_8) == 0)
	{
		while(HAL_GPIO_ReadPin (GPIOA,GPIO_PIN_8) == 0)
			key_val = 1;		
	}
	else 
		key_val = 0;
	
	return key_val;
	
}


