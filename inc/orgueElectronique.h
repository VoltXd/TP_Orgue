/*
 * orgueElectronique.h
 *
 *  Created on: 3 déc. 2021
 *      Author: Andre
 */

#ifndef INC_ORGUEELECTRONIQUE_H_
#define INC_ORGUEELECTRONIQUE_H_

//Define Menu
#define MENU_PRINCIPALE 	0
#define MENU_WAVEFORM 		1
#define MENU_COMMAND 		2
#define MENU_TEMPO 			3
#define MENU_MUSIQUE_AUTO 	4
#define MENU_SON 			5

//Define wave form
#define WAVEFORM_PWM 0
#define WAVEFORM_SIN 1

//Define command type
#define COMMAND_BUTTON 			0
#define COMMAND_POTENTIOMETER 	1

//Define Notes Musique (Pour plus aigüe => note * 2, plus grave => note / 2)
#define DO3 		(LA3 * pow(2, -9.0 / 12))
#define DO_DIESE3 	(LA3 * pow(2, -8.0 / 12))
#define RE3 		(LA3 * pow(2, -7.0 / 12))
#define RE_DIESE3 	(LA3 * pow(2, -6.0 / 12))
#define MI3 		(LA3 * pow(2, -5.0 / 12))
#define FA3 		(LA3 * pow(2, -4.0 / 12))
#define FA_DIESE3 	(LA3 * pow(2, -3.0 / 12))
#define SOL3 		(LA3 * pow(2, -2.0 / 12))
#define SOL_DIESE3 	(LA3 * pow(2, -1.0 / 12))
#define LA3 		440.0
#define LA_DIESE3 	(LA3 * pow(2, 1.0 / 12))
#define SI3 		(LA3 * pow(2, 2.0 / 12))

//Prototypes fonctions
uint8_t telerupteur(uint8_t btnPin, uint8_t currentBtnState, uint8_t prevBtnState);
float processUartReceivedCharacter(uint8_t character, float frequency);
void setWaveform(uint8_t waveform, float frequency, float dutyCycle);
float convertKeyToFrequency(const uint8_t *key);
void turnOnSound(uint8_t waveform);
void turnOffSound(uint8_t waveform);
void setSoundFrequency(uint8_t waveform, float frequency, float volume);
void affichageLcd(uint8_t indiceMenu, uint8_t waveform, uint8_t command, uint16_t tempo, uint8_t musicAuto, float freq);

#endif /* INC_ORGUEELECTRONIQUE_H_ */
