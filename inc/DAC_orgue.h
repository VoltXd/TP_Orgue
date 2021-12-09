/*
 * DAC_orgue.h
 *
 *  Created on: 4 déc. 2021
 *      Author: Andre
 */

#ifndef INC_DAC_ORGUE_H_
#define INC_DAC_ORGUE_H_

#define N_SIN_SAMPLES 20

void init_DAC();
void turnOff_DAC();
void DAC_setVoltage(float voltage);
void DAC_sinWave();
void DAC_setVolume(float value);

#endif /* INC_DAC_ORGUE_H_ */
