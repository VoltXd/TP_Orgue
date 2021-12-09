/*
 * UART.h
 *
 *  Created on: 3 déc. 2021
 *      Author: Andre
 */

#ifndef INC_UART_ORGUE_H_
#define INC_UART_ORGUE_H_

#include "stdint.h"
#include "CTIMER_orgue.h"

//Define UART
#define OSRVAL 0xF
#define BRGVAL(baudrate) (FCLK / (OSRVAL + 1) / baudrate - 0.5)

void init_UART0();
uint8_t UART0_checkCharacterReception(uint8_t *character);

#endif /* INC_UART_ORGUE_H_ */
