/*
 * CTIMER.c
 *
 *  Created on: 3 déc. 2021
 *      Author: Andre
 */

#include "CTIMER_orgue.h"
#include "LPC8xx.h"
#include "syscon.h"
#include "ctimer.h"
#include "GPIO_orgue.h"
#include "swm.h"

void init_CTIMER0(float frequency, float dutyCycle)
{
	//Init. Oscill. Periph.
	LPC_SYSCON->SYSAHBCLKCTRL0 |= CTIMER0;

	//Réglages timer
	LPC_CTIMER0->TCR = (1 << CRST);
	LPC_CTIMER0->PR = DIV_FREQ;
	LPC_CTIMER0->MCR = (1 << 10) | (1 << 25) | (1 << 27);
	CTIMER0_setPWM(frequency, dutyCycle);
	LPC_CTIMER0->EMR |= (0b11 << 4);  	//Match => toggle
	LPC_CTIMER0->PWMC |= (1 << 1);

	//Configuration switch matrix
	ConfigSWM(T0_MAT1, LED1_PIN);		//PWM_match1 -> LED 1 (Haut-parleur)

}

void CTIMER0_start()
{
	LPC_CTIMER0->TCR = (1 << CEN);
}

void CTIMER0_stop()
{
	LPC_CTIMER0->TCR = (1 << CRST);
}

void CTIMER0_setPWM(float frequency, float dutyCycle)
{
	LPC_CTIMER0->MSR[3] = (uint32_t)F_PWM(frequency);
	LPC_CTIMER0->MSR[1] = (uint32_t)PWM(dutyCycle, frequency);
}
