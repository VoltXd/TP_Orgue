/*
 * MRT.c
 *
 *  Created on: 3 déc. 2021
 *      Author: Andre
 */

#include "MRT_orgue.h"
#include <stdio.h>
#include "LPC8xx.h"
#include "syscon.h"
#include "mrt.h"
#include "CTIMER_orgue.h"
#include "GPIO_orgue.h"
#include "DAC_orgue.h"
#include "toolbox.h"
#include "math.h"
#include "music_auto.h"

void init_MRT0(float frequency)
{
	LPC_SYSCON->SYSAHBCLKCTRL0 |= MRT;

	//Réglage MRT
	MRT0_setFrequency(frequency);
	LPC_MRT->Channel[0].CTRL = (1 << MRT_INTEN) | (0 << MRT_MODE);	//Interrupt Enable
	//Configuration des interruptions
	NVIC->ISER[0] |= (1 << (uint32_t)MRT_IRQn);	//Activation des interrupt MRT
	NVIC->IP[2] &= ~(0x00C00000);						//Prio forte sur MRT
}

void init_MRT1(float frequency)
{

	//à lancer apres l'init du mrt0
	MRT1_setFrequency(frequency);
	LPC_MRT->Channel[1].CTRL = 0;
}

void turnOn_MRT1()
{
	LPC_MRT->Channel[1].CTRL |= (1 << MRT_INTEN);
}

void turnOff_MRT1()
{
	LPC_MRT->Channel[1].CTRL &= ~(1 << MRT_INTEN);
}

void MRT_IRQHandler()
{
	if (LPC_MRT->Channel[0].STAT & (1 << MRT_INTFLAG))
	{
		LPC_MRT->Channel[0].STAT |= (1 << MRT_INTFLAG);
		LPC_GPIO_PORT->NOT0 = 1 << LED3_PIN;
		if (isMusicPlaying())
		{
			setNextKey();
		}
	}
	else if (LPC_MRT->Channel[1].STAT & (1 << MRT_INTFLAG))
	{
		LPC_MRT->Channel[1].STAT |= (1 << MRT_INTFLAG);
		DAC_sinWave();
	}
}

void MRT0_setFrequency(float frequency)
{
	LPC_MRT->Channel[0].INTVAL = (uint32_t)(FCLK / frequency); //Timer Period
}

void MRT1_setFrequency(float frequency)
{
	LPC_MRT->Channel[1].INTVAL = (uint32_t)(FCLK / frequency); //Timer Period
}
