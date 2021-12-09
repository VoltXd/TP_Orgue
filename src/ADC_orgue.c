/*
 * ADC_orgue.c
 *
 *  Created on: 4 déc. 2021
 *      Author: Andre
 */

#include "ADC_orgue.h"
#include "LPC8xx.h"
#include "syscon.h"
#include "adc.h"
#include "swm.h"
#include "toolbox.h"

uint16_t potLSamples[N_ADC_SAMPLES];
float potLMean;
uint8_t potLIndex = 0;
uint8_t measureLReady = 0;
uint16_t potRSamples[N_ADC_SAMPLES];
float potRMean;
uint8_t potRIndex = 0;
uint8_t measureRReady = 0;

void init_ADC()
{
	LPC_SYSCON->PDRUNCFG &= ~(ADC_PD);
	LPC_SYSCON->SYSAHBCLKCTRL0 |= ADC;
	LPC_SYSCON->ADCCLKSEL = 0;
	LPC_SYSCON->ADCCLKDIV = 1;

	NVIC->ISER[0] |= (1 << (uint8_t)ADC_SEQA_IRQn) | (1 << (uint8_t)ADC_SEQB_IRQn);
	NVIC->IP[4] &= ~(0x0000C0C0);
	NVIC->IP[4] |= 0x00004040;

	LPC_SWM->PINENABLE0 &= ~(ADC_8 | ADC_7);
	LPC_IOCON->PIO0_10 = 0;
	LPC_IOCON->PIO0_15 = 0;

	LPC_ADC->SEQA_CTRL |= (1 << ADC_CHANNEL8) | (1 << ADC_TRIGPOL);
	LPC_ADC->SEQB_CTRL |= (1 << ADC_CHANNEL7) | (1 << ADC_TRIGPOL);
	LPC_ADC->INTEN |= (1 << SEQA_INTEN) | (1 << SEQB_INTEN);
}

void ADC_startSequence()
{
	if (~(LPC_ADC->SEQA_CTRL & (1 << ADC_SEQ_ENA)))
		LPC_ADC->SEQA_CTRL |= (1 << ADC_START) | (1 << ADC_SEQ_ENA);
	if (~(LPC_ADC->SEQB_CTRL & (1 << ADC_SEQ_ENA)))
		LPC_ADC->SEQB_CTRL |= (1 << ADC_START) | (1 << ADC_SEQ_ENA);
}

uint16_t ADC_getResultA()
{
	return round(potLMean);
}

void ADC_SEQA_IRQHandler()
{
	//LPC_ADC->FLAGS |= ADC_SEQA_INT;
	if (LPC_ADC->SEQA_GDAT & (1 << ADC_DATAVALID))
	{
		//Clear start conversion register
		LPC_ADC->SEQA_CTRL &= ~((1 << ADC_START) | (1 << ADC_SEQ_ENA));
		//Conversion result stored in a sample array
		potLSamples[potLIndex++] = (LPC_ADC->SEQA_GDAT & 0xfff0) >> 4;
		if (potLIndex >= N_ADC_SAMPLES)
		{
			potLIndex = 0;
			measureLReady = 1;
			//Calculation of the mean value
			potLMean = 0;
			for (int i = 0; i < N_ADC_SAMPLES; i++)
				potLMean += potLSamples[i];
			potLMean /= N_ADC_SAMPLES;
		}
	}
}

uint16_t ADC_getResultB()
{
	return round(potRMean);
}

void ADC_SEQB_IRQHandler()
{
	//LPC_ADC->FLAGS |= ADC_SEQB_INT;
	if (LPC_ADC->SEQB_GDAT & (1 << ADC_DATAVALID))
	{
		//Clear start conversion register
		LPC_ADC->SEQB_CTRL &= ~((1 << ADC_START) | (1 << ADC_SEQ_ENA));
		//Conversion result stored in a sample array
		potRSamples[potRIndex++] = (LPC_ADC->SEQB_GDAT & 0xfff0) >> 4;
		if (potRIndex >= N_ADC_SAMPLES)
		{
			potRIndex = 0;
			measureRReady = 1;
			//Calculation of the mean value
			potRMean = 0;
			for (int i = 0; i < N_ADC_SAMPLES; i++)
				potRMean += potRSamples[i];
			potRMean /= N_ADC_SAMPLES;
		}
	}
}

uint8_t ADC_isMeasureAReady()
{
	uint8_t returnValue = measureLReady;
	measureLReady = 0;
	return returnValue;
}

uint8_t ADC_isMeasureBReady()
{
	uint8_t returnValue = measureRReady;
	measureRReady = 0;
	return returnValue;
}

uint8_t ADC_areSequencesEnabled()
{
	return (LPC_ADC->SEQA_CTRL & (1 << ADC_SEQ_ENA)) | (LPC_ADC->SEQB_CTRL & (1 << ADC_SEQ_ENA));
}
