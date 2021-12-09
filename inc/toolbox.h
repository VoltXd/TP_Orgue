/*
 * toolbox.h
 *
 *  Created on: 4 déc. 2021
 *      Author: Andre
 */

#ifndef INC_TOOLBOX_H_
#define INC_TOOLBOX_H_

#include <stdint.h>

#define PI 3.14159265358979323846264338

float limitToInterval(float value, float min, float max);
uint32_t round(float value);

#endif /* INC_TOOLBOX_H_ */
