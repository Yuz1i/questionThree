#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

//ADC1：GPIOA0 ADC1通道0
//ADC2：GPIOA1 ADC2通道1

#define ADC1_GPIO         GPIOA
#define ADC2_GPIO         GPIOA

#define ADC1_CHANEL       0
#define ADC2_CHANEL       1

#define ADC1_PIN          0
#define ADC2_PIN          1

void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 

void adc_init(void);
u16 get_adc1(u8 times);
u16 get_adc2(u8 times);
#endif 
