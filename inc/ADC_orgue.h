/*
 * ADC_orgue.h
 *
 *  Created on: 4 déc. 2021
 *      Author: Andre
 */

#ifndef INC_ADC_ORGUE_H_
#define INC_ADC_ORGUE_H_

#include <stdint.h>

#define N_ADC_SAMPLES 50
#define ADC_MAX_VALUE 4095

void init_ADC();
void ADC_startSequence();
uint16_t ADC_getResultA();
uint16_t ADC_getResultB();
uint8_t ADC_isMeasureAReady();
uint8_t ADC_isMeasureBReady();
uint8_t ADC_areSequencesEnabled();


#endif /* INC_ADC_ORGUE_H_ */
