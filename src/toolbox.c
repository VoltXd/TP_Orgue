/*
 * toolbox.c
 *
 *  Created on: 4 déc. 2021
 *      Author: Andre
 */

#include "toolbox.h"

float limitToInterval(float value, float min, float max)
{
	if (value > max)
		return max;
	else if (value < min)
		return min;
	else
		return value;
}

uint32_t round(float value)
{
	return (uint32_t)(value + 0.5);
}
