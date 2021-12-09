/*
 * UART.c
 *
 *  Created on: 3 déc. 2021
 *      Author: Andre
 */

#include "UART_orgue.h"
#include "syscon.h"
#include "LPC8xx.h"
#include "swm.h"
#include "chip_setup.h"
#include "uart.h"

void init_UART0()
{
	LPC_SYSCON->SYSAHBCLKCTRL0 |= UART0;
	LPC_SYSCON->UART0CLKSEL &= ~0b111; //UART Clk <- Free Run Oscillator

	LPC_USART0->CFG = 1 ;							//UART Enable, no parity, 1 stop bit, asynchronous, Normal mode,
	LPC_USART0->CFG |= (1 << 2);					//8 bits
	LPC_USART0->BRG = (uint32_t)BRGVAL(115200);		//115200 baud

	ConfigSWM(U0_TXD, DBGTXPIN);		//TX -> Pin_TX
	ConfigSWM(U0_RXD, DBGRXPIN);		//RX -> Pin_RX
}

uint8_t UART0_checkCharacterReception(uint8_t *character)
{
	*character = 0;
	if (LPC_USART0->STAT & RXRDY)
	{
		*character = LPC_USART0->RXDAT;
		return 1;
	}
	else
		return 0;
}
