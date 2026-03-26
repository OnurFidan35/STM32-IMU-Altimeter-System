/*
 * timer6.h
 *
 *  Created on: Mar 23, 2026
 *      Author: Onur Fidan
 */

#ifndef TIMERMODULE_TIMER6_H_
#define TIMERMODULE_TIMER6_H_
#include "stm32f4xx.h"

void timer6_init(void);
uint16_t timer6_get_counter_value(void);
void timer6_enable(void);
void timer6_disable(void);
void timer_6_set_period(uint16_t period);

#endif /* TIMERMODULE_TIMER6_H_ */
