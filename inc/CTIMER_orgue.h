/*
 * CTIMER.h
 *
 *  Created on: 3 déc. 2021
 *      Author: Andre
 */

#ifndef INC_CTIMER_ORGUE_H_
#define INC_CTIMER_ORGUE_H_

//Define PWM
#define FCLK 15000000.0
#define DIV_FREQ 1
#define F_PWM(freq) (FCLK / freq / DIV_FREQ)
#define PWM(x, freq) (F_PWM(freq) - (float)x * F_PWM(freq) / 100)

//Functions
void init_CTIMER0(float frequency, float dutyCycle);
void CTIMER0_start();
void CTIMER0_stop();
void CTIMER0_setPWM(float frequency, float dutyCycle);

#endif /* INC_CTIMER_ORGUE_H_ */
