/*
 * GPIO.c
 *
 *  Created on: 3 déc. 2021
 *      Author: Andre
 */

#include "GPIO_orgue.h"
#include "LPC8xx.h"
#include "syscon.h"


void init_GPIO()
{
	LPC_SYSCON->SYSAHBCLKCTRL0 |= GPIO;

	LPC_GPIO_PORT->DIR0 |= (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN) | (1 << LED4_PIN);
}

void GPIO_turnOnDACOUT()
{
	LPC_GPIO_PORT->DIR0 |= 1 << LED1_PIN;
}

void GPIO_turnOffDACOUT()
{
	LPC_GPIO_PORT->DIR0 &= ~(1 << LED1_PIN);
}
