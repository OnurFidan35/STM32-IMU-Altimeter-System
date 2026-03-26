/*
 * timer6.c
 *
 *  Created on: Mar 23, 2026
 *      Author: Onur Fidan
 */


#include "timer6.h"
#include "stm32f4xx_hal.h"

extern sensor_flag;


void timer6_init(void){

	//Enable TIM6 Clock
	__HAL_RCC_TIM6_CLK_ENABLE();

	// Timer clock=168mhz/168000=1000hz(1ms period)
	TIM6->PSC=167999;

	// Reload in every 100 ms
	TIM6->ARR=99;

	// Enable timer update interrupt
	TIM6->DIER |=TIM_DIER_UIE;

	// Enable TIM6 Update interrupt
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	NVIC_SetPriority(TIM6_DAC_IRQn,2);



}

//Set period(ms)
void timer_6_set_period(uint16_t period){

	TIM6->ARR=period-1;

}

uint16_t timer6_get_counter_value(void){

	uint16_t cnt=TIM6->CNT;

	return cnt;
}

void timer6_enable(void){

	TIM6->CR1 |= TIM_CR1_CEN;

}



void timer6_disable(void){

	TIM6->CR1 &= ~(TIM_CR1_CEN);

}


void TIM6_DAC_IRQHandler(void){

	TIM6->SR &= ~(TIM_SR_UIF); //UIF=0

	sensor_flag = 1; //IRQHandler içerisinde ağır iş yapılmaz mikrosaniye seviyesinde kod çalışmalı

}




