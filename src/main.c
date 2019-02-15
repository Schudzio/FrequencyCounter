#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "font.h"
#include "lcd.h"
#include <math.h>
#include "setclock.h"

#define TIMER_Enable                      ((uint16_t)0x0001)
#define TIMER_EGR_UD                      ((uint16_t)0x0001)



void vhTIMER2_Init(void){

	RCC->APB1ENR |= (uint32_t)(1);
	TIM2->PSC = (uint16_t)719;
	TIM2->ARR = (uint32_t)0xFFFFFFFF;
	TIM2->EGR |= TIMER_EGR_UD;
	TIM2->CR1 = TIMER_Enable;
}

uint32_t vhTIMER2_GetTicks(void){

	return (TIM2->CNT);
}

uint32_t vhTIMER2_GetTimeDifference(uint32_t ticks){

	return ((TIM2->CNT) - ticks);

}

volatile float fi0;
volatile float fi;
volatile int i;
volatile int j;
volatile int edge_value;
volatile int between_two_edge;
volatile int correct_between_two_edge;
volatile int ch2_bte;
volatile int ch2_cbte;
volatile int ch2_hcbte;
volatile float ch2_averge_value;
volatile float f;
volatile float hundred_cbte;
volatile int averge_value;

char result[15];
char cosfi[15];
char period_buff [15];
char freq_buff[15];
char ph_shift_buff[15];


int main(void){
	
	SetSysClockTo72();

	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;
	SPI_InitTypeDef spi;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_StructInit(&gpio);
	 gpio.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7; 
	 gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	 gpio.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA, &gpio);

	 gpio.GPIO_Pin = GPIO_Pin_6; // MISO
	 gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	 GPIO_Init(GPIOA, &gpio);

	 gpio.GPIO_Pin = LCD_DC|LCD_CE|LCD_RST;
	 gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOC, &gpio);
	 GPIO_SetBits(GPIOC, LCD_CE|LCD_RST);

	SPI_StructInit(&spi);
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_Init(SPI1, &spi);
	SPI_Cmd(SPI1, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_0;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &gpio);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource10);

	EXTI_StructInit(&exti);
	exti.EXTI_Line = EXTI_Line10;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 0x00;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);

	EXTI_StructInit(&exti);
	exti.EXTI_Line = EXTI_Line0;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 0x00;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);



	lcd_setup();
	lcd_clear();
	vhTIMER2_Init();

	while (1) {
	lcd_clear();
	
	sprintf(period_buff, "T=%3.2f ms",averge_value/100);
	lcd_draw_text(0, 5, period_buff);
	//----------------------------------
	
	f  = averge_value/100000;
	f= 1/f;
	if (averge_value==0){ f=0;}
	sprintf(freq_buff, "f =%3.3f Hz", f);
	lcd_draw_text(1, 5, freq_buff);
	//----------------------------------

	fi0 =((v6*360)/averge_value);
	if (fi0<91){
		fi=fi0;
	}
	else if(fi0>90){
		fi = -(360-fi0);	
	}
	sprintf(ph_shift_buff, "fi= %2.1f", fi);
	lcd_draw_text(2, 5, ph_shift_buff);
	//----------------------------------

	if(fi0>0 && fi0<91){
		sprintf(cosfi_buff,"cos(fi)=%2.2f",cos(3.1416*(fi/180)));
		lcd_draw_text(3, 5, cosfi_buff);
		sprintf(result,"Indukcyjny");
		lcd_draw_text(4, 5, result);
		}
	else if(fi0>91){
		sprintf(cosfi,"cos(fi)=%2.2f",cos(3.1416*(fi/180)));
		lcd_draw_text(3, 5,cosfi);
		sprintf(result,"Pojemnosciowy");
		lcd_draw_text(4, 5, result);
		}
	//----------------------------------
	lcd_copy();

	}
}


void EXTI15_10_IRQHandler(){
	
	if (EXTI_GetITStatus(EXTI_Line10)) {
		
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10) == 1) {
			
			between_two_edge = vhTIMER2_GetTimeDifference(rise_edge_counter_value);
			
				if (between_two_edge>0){
					correct_between_two_edge=between_two_edge;
					hundred_cbte+= correct_between_two_edge;
					i++;
					if (i==100){

						averge_value = hundred_cbte/100.0;
						i=0;
						hundred_cbte=0;
					}

				}

	edge_value = vhTIMER2_GetTicks();

		}


EXTI_ClearITPendingBit(EXTI_Line10);
	}
}

void EXTI0_IRQHandler(){

	if (EXTI_GetITStatus(EXTI_Line0)){
		
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0) == 1){
			
			ch2_bte =vhTIMER2_GetTimeDifference(v);
			if(ch2_bte>0){
				ch2_cbte=ch2_bte;
				ch2_hcbte +=ch2_cbte;
				j++;
				if(j==100){
					ch2_averge_value = ch2_hcbte/100.0;
					j=0;
					ch2_hcbte=0;

				}
			}

		}
	EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
