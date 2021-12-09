/*
 * orgueElectronique.c
 *
 *  Created on: 23 sept. 2021
 *      Author: user
 */

//	Manuel:
//	Défilement des menus: 			Appuie sur le bouton 1
//	Action sur le menu courant: 	Appuie sur le bouton 2
//	Réglage du volume: 				Potentiometre gauche
//
//	Menus:
//	Forme d'onde: 	Permet de sélectionner le type d'onde sonore (pwm ou sinus)
//	Commande: 		Permet de choisir comment est piloter la fréquence dans le menu SON
//	Tempo: 			Permet de régler la fréquence du métronome (appuie btn2 => tempo++)
//	Musique Auto: 	Permet de jouer une des trois musiques disponibles.
//				  	La selection se fait via le potentiometre gauche.
//				  	La musique démarre et s'arrete sur appuie du bouton 2.
//	Son: 			Permet d'émettre la fréquence affichée à l'écran
//					Btn2 => +5 Hz
//					Utilisable via liaison série (A = DO, Z = RE, 2 = DO#, etc. c'est la même disposition que celle d'un piano.)
//					Utilisable via I2C
//
//	Attention, l'I2C à tendance à faire des while(1) lorsqu'on reflash le programme.
//	Si nécessaire, commenter les lignes 81 et 113 à 191. (en principe, débrancher et rebrancher la carte suffit)

#include <ctimer.h>
#include "LPC8xx.h"
#include "syscon.h"
#include "lib_ENS_II1_lcd.h"
#include "rom_api.h"
#include "swm.h"
#include "chip_setup.h"
#include "utilities.h"
#include <math.h>
#include <mrt.h>
#include <stdio.h>
#include <uart.h>
#include "orgueElectronique.h"
#include "GPIO_orgue.h"
#include "CTIMER_orgue.h"
#include "MRT_orgue.h"
#include "UART_orgue.h"
#include "I2C_orgue.h"
#include "DAC_orgue.h"
#include "ADC_orgue.h"
#include "toolbox.h"
#include "music_auto.h"

int main()
{
	//Variables de l'orgue
	uint8_t indiceMenu = MENU_PRINCIPALE;
	uint8_t waveform = WAVEFORM_PWM;
	uint8_t command = COMMAND_BUTTON;
	float tempo = 170;
	uint8_t indiceMusicAuto = 0;
	uint8_t indiceMusicAuto_1 = 0;
	float freq = LA3;
	uint8_t displayChanged = 0;
	float volume = 1.0;
	uint8_t receivedChar = 0;
	uint8_t keyToPlay[4];

	//Overclock 15 MHz
	LPC_PWRD_API->set_fro_frequency(30000);

	//Init. Oscill. Periph.
	LPC_SYSCON->SYSAHBCLKCTRL0 |= SWM | IOCON;

	//Init. Periph.
	init_GPIO();
	init_CTIMER0(freq, volume);
	init_MRT0((2.0*tempo) / 60);
	init_MRT1(freq * N_SIN_SAMPLES);
	init_UART0();
	init_ADC();
	init_lcd();
	lcd_puts("Orgue");
	init_I2C();			//Attention, l'I2C plante souvent lorsque l'on flash un code pendant une transmission I2C

	//Variables des boutons
	uint8_t currentBtn1State;
	uint8_t currentBtn2State;
	uint8_t prevBtn1State = BTN1;
	uint8_t prevBtn2State = BTN2;
	uint8_t rEdge1 = 0;
	uint8_t rEdge2 = 0;

	while (1)
	{
		//Lecture des front montants des boutons
		currentBtn1State = BTN1;
		currentBtn2State = BTN2;
		rEdge1 = telerupteur(BTN1_PIN, currentBtn1State, prevBtn1State);
		rEdge2 = telerupteur(BTN2_PIN, currentBtn2State, prevBtn2State);
		prevBtn1State = currentBtn1State;
		prevBtn2State = currentBtn2State;

		//Changement de la fréquence de PWM sur appuie clavier de certaines touches
		if (UART0_checkCharacterReception(&receivedChar) && indiceMenu == MENU_SON)
		{
			turnOnSound(waveform);
			freq = processUartReceivedCharacter(receivedChar, freq);
			if (freq == 0)
				volume = 0;
			freq = limitToInterval(freq, 10, 20000.0);
			setSoundFrequency(waveform, freq, volume);
			displayChanged = 1;
		}

		//Changement de fréquence de PWM via le clavier matriciel
		uint8_t codeI2C = Ftouche();
		if (codeI2C != 0xff && indiceMenu == MENU_SON)
		{
			codeI2C = decode_touche(codeI2C);
			switch(codeI2C)
			{
				case '1':
					turnOnSound(waveform);
					freq = DO3;
					break;

				case '2':
					turnOnSound(waveform);
					freq = RE3;
					break;

				case '3':
					turnOnSound(waveform);
					freq = MI3;
					break;

				case 'A':
					turnOnSound(waveform);
					freq = FA3;
					break;

				case '4':
					turnOnSound(waveform);
					freq = SOL3;
					break;

				case '5':
					turnOnSound(waveform);
					freq = LA3;
					break;

				case '6':
					turnOnSound(waveform);
					freq = SI3;
					break;

				case 'B':
					turnOnSound(waveform);
					freq = DO3 * 2;
					break;

				case '7':
					turnOnSound(waveform);
					freq = DO_DIESE3;
					break;

				case '8':
					turnOnSound(waveform);
					freq = RE_DIESE3;
					break;

				case '9':
					turnOnSound(waveform);
					freq = FA_DIESE3;
					break;

				case 'C':
					turnOnSound(waveform);
					freq = SOL_DIESE3;
					break;

				case '*':
					turnOnSound(waveform);
					freq = LA_DIESE3;
					break;

				default:
					turnOffSound(waveform);
					break;
			}
			displayChanged = 1;
			setSoundFrequency(waveform, freq, volume);
		}

		//Redémarrage des séquences lorsque les deux sont terminées (j'aurais pu mettre le mode burst)
		if (!ADC_areSequencesEnabled())
			ADC_startSequence();

		//Commande du volume par le potentiometre gauche
		if ((indiceMenu == MENU_SON || indiceMenu == MENU_MUSIQUE_AUTO) && ADC_isMeasureAReady())
		{
			if (waveform == WAVEFORM_PWM)
			{
				volume = 100.0 * ADC_getResultA() / 8191;
			}
			else
			{
				volume = (float)ADC_getResultA() / 4095;
			}
			setSoundFrequency(waveform, freq, volume);
		}

		//Commande de la fréquence par le potentiometre droit
		if (command == COMMAND_POTENTIOMETER && indiceMenu == MENU_SON)
		{
			if (ADC_isMeasureBReady())
			{
				if (waveform == WAVEFORM_PWM)
				{
					freq = (float)ADC_getResultB() * 19990 / ADC_MAX_VALUE + 10;
				}
				else
				{
					freq = (float)ADC_getResultB() * 990 / ADC_MAX_VALUE + 10;
				}
				setSoundFrequency(waveform, freq, volume);
				displayChanged = 1;
			}
		}
		else if (indiceMenu == MENU_MUSIQUE_AUTO)
		{
			if (ADC_isMeasureBReady())
			{
				indiceMusicAuto_1 = indiceMusicAuto;
				indiceMusicAuto = (uint32_t)(ADC_getResultB() / ((float)(ADC_MAX_VALUE + 1) / N_MUSIC));
				if (indiceMusicAuto != indiceMusicAuto_1)
					displayChanged = 1;
			}
		}

		//Commande de la musique auto
		if (isNextKeyReady())
		{
			musicGetKey(keyToPlay);
			freq = convertKeyToFrequency(keyToPlay);
			setSoundFrequency(waveform, freq, volume);
			displayChanged = 1;
		}
		else if (isSongFinished())
			turnOffSound(waveform);

		//Un appuie sur le bouton 1 fais défiler le menu
		if (rEdge1)
		{
			//Changement du menu courant
			indiceMenu++;
			//Si menu_son, active le son
			if (indiceMenu == MENU_SON)
				turnOnSound(waveform);
			else if (indiceMenu == MENU_SON + 1)
				turnOffSound(waveform);

			//Bouclage des menus
			if (indiceMenu >= MENU_SON + 1)
				indiceMenu = MENU_WAVEFORM;
		}

		//Un appuie sur le bouton 2 fais l'action correspondante au menu courant
		if (rEdge2)
		{
			switch(indiceMenu)
			{
				case MENU_WAVEFORM:
					waveform = !waveform;
					setWaveform(waveform, freq, volume);
					break;

				case MENU_COMMAND:
					command = !command;

				case MENU_TEMPO:
					tempo++;
					if (tempo > 300)
						tempo = 60;
					MRT0_setFrequency(2.0 * tempo / 60);
					break;

				case MENU_MUSIQUE_AUTO:
					if (!isMusicPlaying())
					{
						startMusic(indiceMusicAuto);
						turnOnSound(waveform);
						setSoundFrequency(waveform, freq, 0);
					}
					else
					{
						stopMusic();
						turnOffSound(waveform);
					}
					break;

				case MENU_SON:
					if (command == COMMAND_BUTTON)
					{
						freq += 5;
						if (freq > 20000)
							freq = limitToInterval(freq, 10, 20000);
						setSoundFrequency(waveform, freq, volume);
					}
					break;
			}
		}

		if (rEdge1 || rEdge2 || displayChanged)
		{
			displayChanged = 0;
			affichageLcd(indiceMenu, waveform, command, tempo, indiceMusicAuto, freq);
		}
	}
}

uint8_t telerupteur(uint8_t btnPin, uint8_t currentBtnState, uint8_t prevBtnState)
{
	if (((prevBtnState) != currentBtnState) && currentBtnState == 0)
		return 1;
	else
		return 0;
}

float processUartReceivedCharacter(uint8_t character, float frequency)
{
	//return a frequency according to the received character
	if (character == 'a' || character == 'A')
		frequency = DO3;
	else if (character == '2')
		frequency = DO_DIESE3;
	else if (character == 'z' || character == 'Z')
		frequency = RE3;
	else if (character == '"' || character == '3')
		frequency = RE_DIESE3;
	else if (character == 'e' || character == 'E')
		frequency = MI3;
	else if (character == 'r' || character == 'R')
		frequency = FA3;
	else if (character == '(' || character == '5')
		frequency = FA_DIESE3;
	else if (character == 't' || character == 'T')
		frequency = SOL3;
	else if (character == '-' || character == '6')
		frequency = SOL_DIESE3;
	else if (character == 'y' || character == 'Y')
		frequency = LA3;
	else if (character == '7')
		frequency = LA_DIESE3;
	else if (character == 'u' || character == 'U')
		frequency = SI3;
	else if (character == 'i' || character == 'I')
		frequency = DO3 * 2;
	else if (character == '=')
		frequency *= 2;
	else if (character == '$')
		frequency /= 2;
	else if (character == 32)
		frequency = 0;

	return frequency;
}

void setWaveform(uint8_t waveform, float frequency, float volume)
{
	if (waveform == WAVEFORM_PWM)
	{
		turnOff_DAC();
		turnOff_MRT1();
		GPIO_turnOnDACOUT();
	}
	else
	{
		CTIMER0_stop();
		GPIO_turnOffDACOUT();
		init_DAC();
	}
	setSoundFrequency(waveform, frequency, volume);
}

void turnOnSound(uint8_t waveform)
{
	if (waveform == WAVEFORM_PWM)
		CTIMER0_start();
	else
		turnOn_MRT1();
}

void turnOffSound(uint8_t waveform)
{
	if (waveform == WAVEFORM_PWM)
		CTIMER0_stop();
	else
		turnOff_MRT1();
}

void setSoundFrequency(uint8_t waveform, float frequency, float volume)
{
	volume = limitToInterval(volume, 0, 100);
	if (waveform == WAVEFORM_PWM)
	{
		frequency = limitToInterval(frequency, 0, 20000);
		CTIMER0_setPWM(frequency, volume/2);
	}
	else
	{
		frequency = limitToInterval(frequency, 0, 1000);
		MRT1_setFrequency(frequency * N_SIN_SAMPLES);
		DAC_setVolume(volume);
	}
}

float convertKeyToFrequency(const uint8_t *key)
{
	float frequencyToReturn = 0;
	int8_t tone = 0;

	if (key[1] == '#')
	{
		tone = key[2];
		switch (key[0])
		{
			case 'a':
				frequencyToReturn = DO_DIESE3;
				break;

			case 'b':
				frequencyToReturn = RE_DIESE3;
				break;

			case 'd':
				frequencyToReturn = FA_DIESE3;
				break;

			case 'e':
				frequencyToReturn = SOL_DIESE3;
				break;

			case 'f':
				frequencyToReturn = LA_DIESE3;
				break;
		}
	}
	else
	{
		tone = key[1];
		switch (key[0])
		{
			case 'a':
				frequencyToReturn = DO3;
				break;

			case 'b':
				frequencyToReturn = RE3;
				break;

			case 'c':
				frequencyToReturn = MI3;
				break;

			case 'd':
				frequencyToReturn = FA3;
				break;

			case 'e':
				frequencyToReturn = SOL3;
				break;

			case 'f':
				frequencyToReturn = LA3;
				break;

			case 'g':
				frequencyToReturn = SI3;
				break;
		}
	}
	tone = tone & 0xf;
	frequencyToReturn = (tone >= 3 ? frequencyToReturn * (tone - 2) : frequencyToReturn / (4 - tone));

	return frequencyToReturn;
}

void affichageLcd(uint8_t indiceMenu, uint8_t waveform, uint8_t command, uint16_t tempo, uint8_t musicAuto, float freq)
{
	lcd_clear();
	switch(indiceMenu)
	{
		case MENU_WAVEFORM:
			lcd_puts("Forme d'onde: ");
			lcd_position(1, 0);
			if (waveform == WAVEFORM_PWM)
				lcd_puts("PWM");
			else
				lcd_puts("SIN");
			break;

		case MENU_COMMAND:
			lcd_puts("Commande:");
			lcd_position(1, 0);
			if (command == COMMAND_BUTTON)
				lcd_puts("Bouton");
			else
				lcd_puts("Potentiometre");
			break;

		case MENU_TEMPO:
		{
			char tempoStr[4];
			lcd_puts("Tempo: ");
			lcd_position(1, 0);
			sprintf(tempoStr, "%3d", tempo);
			lcd_puts(tempoStr);
			break;
		}
		case MENU_MUSIQUE_AUTO:
			lcd_puts("Musique Auto:");
			lcd_position(1, 0);
			switch (musicAuto)
			{
				case MUSIC_OGGY:
					lcd_puts("Oggy & Cafards");
					break;

				case MUSIC_ANNIE:
					lcd_puts("Annie");
					break;

				case MUSIC_EVANGELION:
					lcd_puts("Evangelion");
					break;
			}
			break;

		case MENU_SON:
		{
			char freqStr[5];
			sprintf(freqStr, "Frequence: %5d", (uint16_t)freq);
			lcd_puts(freqStr);
			uint16_t intFreq = (uint16_t)freq;
			//Si la fréquence correspond à une note, on l'écrie
			if (intFreq == (uint16_t)DO3)
			{
				lcd_position(1, 0);
				lcd_puts("DO3");
			}
			else if (intFreq == (uint16_t)DO_DIESE3)
			{
				lcd_position(1, 0);
				lcd_puts("DO#3");
			}
			else if (intFreq == (uint16_t)RE3)
			{
				lcd_position(1, 0);
				lcd_puts("RE3");
			}
			else if (intFreq == (uint16_t)RE_DIESE3)
			{
				lcd_position(1, 0);
				lcd_puts("RE#3");
			}
			else if (intFreq == (uint16_t)MI3)
			{
				lcd_position(1, 0);
				lcd_puts("MI3");
			}
			else if (intFreq == (uint16_t)FA3)
			{
				lcd_position(1, 0);
				lcd_puts("FA3");
			}
			else if (intFreq == (uint16_t)FA_DIESE3)
			{
				lcd_position(1, 0);
				lcd_puts("FA#3");
			}
			else if (intFreq == (uint16_t)SOL3)
			{
				lcd_position(1, 0);
				lcd_puts("SOL3");
			}
			else if (intFreq == (uint16_t)SOL_DIESE3)
			{
				lcd_position(1, 0);
				lcd_puts("SOL#3");
			}
			else if (intFreq == (uint16_t)LA3)
			{
				lcd_position(1, 0);
				lcd_puts("LA3");
			}
			else if (intFreq == (uint16_t)LA_DIESE3)
			{
				lcd_position(1, 0);
				lcd_puts("LA#3");
			}
			else if (intFreq == (uint16_t)SI3)
			{
				lcd_position(1, 0);
				lcd_puts("SI3");
			}
			else if (intFreq == (uint16_t)(DO3 * 2))
			{
				lcd_position(1, 0);
				lcd_puts("DO4");
			}
			break;
		}
	}
}
