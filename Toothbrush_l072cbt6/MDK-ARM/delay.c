#include "stm32l0xx_hal.h"
#include "delay.h"


//ֻ������F1ϵ��72MHz
//void delay_us(uint32_t us)
//{
//    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
//    while (delay--)
//	{
//		;
//	}
//}
//ȫϵ��ͨ�ã�ֻ���޸���Ƶ��������HAL���ʼ��

#define CPU_FREQUENCY_MHZ    32		// STM32??
void delay_us(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}

