/*
 * GPIO.h
 *
 *  Created on: 3 déc. 2021
 *      Author: Andre
 */

#ifndef INC_GPIO_ORGUE_H_
#define INC_GPIO_ORGUE_H_

//Define Pins
#define BTN1_PIN 13
#define BTN2_PIN 12
#define LED1_PIN 19
#define LED2_PIN 17
#define LED3_PIN 21
#define LED4_PIN 11

//Define I/O States
#define BTN1 LPC_GPIO_PORT->B0[13]
#define BTN2 LPC_GPIO_PORT->B0[12]
#define LED1 LPC_GPIO_PORT->B0[19]
#define LED2 LPC_GPIO_PORT->B0[17]
#define LED3 LPC_GPIO_PORT->B0[21]
#define LED4 LPC_GPIO_PORT->B0[11]


void init_GPIO();
void GPIO_turnOnDACOUT();
void GPIO_turnOffDACOUT();


#endif /* INC_GPIO_ORGUE_H_ */
