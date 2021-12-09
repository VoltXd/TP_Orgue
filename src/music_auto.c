/*
 * music_auto.c
 *
 *  Created on: 5 déc. 2021
 *      Author: Andre
 */

#include "music_auto.h"
#include "MRT_orgue.h"
#include <string.h>
#include <stdlib.h>

//index
uint8_t currentlyPlayedMusic = N_MUSIC;
uint16_t keyIndex = 0;
uint16_t currentKeyIndex = 0;
//booleans
uint8_t musicPlaying = 0;
volatile uint8_t musicKeyReady = 0;
volatile uint8_t songFinished = 0;
//values
float musicTempo;
uint8_t *currentSheet;
uint8_t currentKey[4];

//songs' sheet (Do = a, do# = a#, re = b, etc.)
uint8_t oggySheet[] = "b#3b#3b#3c3d3d3d3c3b#3b#3b#3c3d3d3c3c3b#3b#3b#3c3d3d3d3c3b#3b#3c3c3d3d3a4a4b#3b#3c3c3d3d3d#3d#3e3e3e3e3e3e3e3e3b#3b#3b#3c3d3d3d3c3b#3b#3b#3c3d3d3c3c3b#3b#3c3c3d3d3e3e3e#3e#3e#3e#3e#3e#3e#3e#3e#3e#3e#3e#3e#3e#3e#3e#3b#3b#3b#3c3d3d3d3c3b#3b#3c3c3d3d3c3c3c3c3c3d3d#3d#3d#3d3c3c3c3d3d#3d#3d3d3c3c3d3d3d#3d#3e3e3e#3e#3e#3e#3e#3e#3e#3e#3c3c3c3d3d#3d#3d#3d3c3c3d3d3d#3d#3e#3e#3f3f3f3f3";
uint8_t annieSheet[] = "e#3e#3e#3d#3d3d#3e#3e#3f#3f#3d3d3d3d3d3d3d#3d#3d#3d3b#3b#3a#3a#3e#2e#2e#2e#2e#2e#2e#2e#2f#2f#2a#3f#2a3a3b#3b#3f#2f#2a#3a#3b#3b#3b#3b#3f#2f#2a#3f#2a3a3b#3b#3a#3a#3a#3a#3";
uint8_t evangelionSheet[] = "a3a3a3a3b#3b#3b#3b#3d3d3d3b#3b#3b#3d3d3d3d3d3d3f#3f#3e#3e#3e3d3d3e3e3e3e3e3e3e3e3e3e3f#3f#3f#3f#3a4a4a4d3d3d3b#3b#3b3b3b3b3a3a3b3b3d3b#3b#3b#3b#3b#3b#3b#3b#3a3a3a3a3b#3b#3b#3b#3d3d3d3b#3b#3b#3d3d3d3d3d3d3f#3f#3e#3e#3e3d3d3e3e3e3e3e3e3e3e3e3e3f#3f#3f#3f#3a4a4a4d3d3d3b#3b#3f#3f#3f#3f#3e3e3f#3f#3f#3f#3f#3a4a4a4a4";

void startMusic(uint8_t musicIndex)
{
	musicPlaying = 1;
	currentlyPlayedMusic = musicIndex;
	keyIndex = 0;
	switch (currentlyPlayedMusic)
	{
		case MUSIC_OGGY:
			musicTempo = BPM_OGGY;
			currentSheet = (char *)malloc(strlen(oggySheet) + 1);
			strcpy(currentSheet, oggySheet);
			break;

		case MUSIC_ANNIE:
			musicTempo = BPM_ANNIE;
			currentSheet = (char *)malloc(strlen(annieSheet) + 1);
			strcpy(currentSheet, annieSheet);
			break;

		case MUSIC_EVANGELION:
			musicTempo = BPM_EVANGELION;
			currentSheet = (char *)malloc(strlen(evangelionSheet) + 1);
			strcpy(currentSheet, evangelionSheet);
			break;
	}
	MRT0_setFrequency(2.0 * musicTempo / 60);
}

void stopMusic()
{
	musicPlaying = 0;
}

uint8_t isMusicPlaying()
{
	return musicPlaying;
}

uint8_t isNextKeyReady()
{
	return musicKeyReady;
}

uint8_t isSongFinished()
{
	uint8_t returnValue = songFinished;
	songFinished = 0;
	return returnValue;
}

void setNextKey()
{
	musicKeyReady = 1;
	currentKeyIndex = 0;

	do
	{
		currentKey[currentKeyIndex++] = currentSheet[keyIndex++];
	}while((currentSheet[keyIndex] < 'a' || currentSheet[keyIndex] > 'g') && !(currentSheet[keyIndex] == 0));
	currentKey[currentKeyIndex++] = 0;

	if (currentSheet[keyIndex] == 0)
	{
		musicPlaying = 0;
		songFinished = 1;
	}
}

void musicGetKey(uint8_t *keyToPlay)
{
	//Return next frequency or -1 (code for continuing current frequency)
	if (musicKeyReady)
	{
		musicKeyReady = 0;
		strcpy(keyToPlay, currentKey);
	}
	else
		keyToPlay[0] = 0;

}

