#include "stm32l0xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "Percentage_res.h"
#include "gpio.h"
#include "main.h"

extern uint16_t ADC_Value[4];	
extern uint32_t RES_value;	
extern float Voltage_value;
uint16_t ADC_Sum=0;
uint16_t ADC_Average=0;

int Percentage_res(void)
{
	 static int  Percentage;
	 HAL_ADC_Start_DMA (&hadc ,(uint32_t *)ADC_Value ,sizeof(ADC_Value)/sizeof(ADC_Value[0]));
	 for(uint8_t i=0;i<10;i++)
	 {
		ADC_Sum += ADC_Value[0];		
	 }
	 ADC_Average = ADC_Sum/10;
	 ADC_Sum = 0;
	 
#if  1
	 {
//他的第一版	 
	 RES_value = (ADC_Average*10000/(4095-ADC_Average))-1000;
	 Voltage_value = ADC_Average*3.3/4095;
	 }	 
#elif 0
	 {
//我的版本	 
	 RES_value = ADC_Average*10005/(4030-ADC_Average);
	 Voltage_value = ADC_Average*3.3/4030;
	 }
	 
#endif
{

}	
   if(RES_value>6000) Percentage = 0;
	 else if(RES_value>5000&&(RES_value<=6000))Percentage = 10;
	 else if(RES_value>4000&&(RES_value<=5000))Percentage = 20;
	 else if(RES_value>3000&&(RES_value<=4000))Percentage = 30;
	
	 else if(RES_value>2000&&(RES_value<=3000))Percentage = 40;
	 else if(RES_value>1700&&(RES_value<=2000))Percentage = 50;
	 else if(RES_value>1600&&(RES_value<=1700))Percentage = 60;
	 else if(RES_value>1500&&(RES_value<=1600))Percentage = 70;
	 else if(RES_value>1400&&(RES_value<=1500))Percentage = 80;
	 else if(RES_value>1300&&(RES_value<=1400))Percentage = 90;
	 else if(RES_value<=1300)Percentage = 100;
	
	 return Percentage;
} 


