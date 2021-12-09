/*
 * DAC_orgue.c
 *
 *  Created on: 4 déc. 2021
 *      Author: Andre
 */

#include "DAC_orgue.h"
#include "syscon.h"
#include "LPC8xx.h"
#include "swm.h"
#include "toolbox.h"
#include "stdint.h"

//Le nombre d'échantillons de sinus étant de N, la fréquence du timer doit être N fois celle du sinus voulu
uint16_t sinTable[N_SIN_SAMPLES] = {};
uint8_t sinIndex;
float volume = 1;

void init_DAC()
{
	for (sinIndex = 0; sinIndex < N_SIN_SAMPLES; sinIndex++)
		sinTable[sinIndex] = (uint16_t)((3.3 * (sin(2 * PI * sinIndex / N_SIN_SAMPLES) + 1) / 2) * 1023 / 3.3);
	sinIndex = 0;
	LPC_SYSCON->PDRUNCFG &= ~(DAC0_PD);
	LPC_SYSCON->SYSAHBCLKCTRL0 |= DAC0;

	LPC_SWM->PINENABLE0 |= DACOUT0;
	LPC_IOCON->PIO0_19 = 1 << 16;
	LPC_DAC0->CR = 512 << 6;
}

void turnOff_DAC()
{
	LPC_SYSCON->PDRUNCFG |= DAC0_PD;
	LPC_SYSCON->SYSAHBCLKCTRL0 &= ~(DAC0);
}

void DAC_setVoltage(float voltage)
{
	voltage = limitToInterval(voltage, 0, 3.3);
	LPC_DAC0->CR = (uint16_t)(voltage * 1023 / 3.3) << 6;
}

void DAC_sinWave()
{
	LPC_DAC0->CR = (uint32_t)(volume * sinTable[sinIndex++]);
	if (sinIndex >= N_SIN_SAMPLES)
				sinIndex = 0;
}

void DAC_setVolume(float value)
{
	volume = limitToInterval(value, 0, 1);
}
