/*
 * MRT.h
 *
 *  Created on: 3 déc. 2021
 *      Author: Andre
 */

#ifndef INC_MRT_ORGUE_H_
#define INC_MRT_ORGUE_H_

void init_MRT0(float frequency);
void init_MRT1(float frequency);
void turnOn_MRT1();
void turnOff_MRT1();
void MRT0_setFrequency(float frequency);
void MRT1_setFrequency(float frequency);

#endif /* INC_MRT_ORGUE_H_ */
