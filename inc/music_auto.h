/*
 * music_auto.h
 *
 *  Created on: 5 déc. 2021
 *      Author: Andre
 */

#ifndef INC_MUSIC_AUTO_H_
#define INC_MUSIC_AUTO_H_

#include <stdint.h>

//Define Musique auto
#define N_MUSIC					3
#define MUSIC_OGGY 				0
#define MUSIC_ANNIE 			1
#define MUSIC_EVANGELION 		2

//Define Musics BPM
#define BPM_OGGY 				133
#define BPM_ANNIE 				112
#define BPM_EVANGELION 			2*128.85

void startMusic(uint8_t musicIndex);
void stopMusic();
uint8_t isMusicPlaying();
uint8_t isNextKeyReady();
uint8_t isSongFinished();
void setNextKey();
void musicGetKey(uint8_t *keyToPlay);

#endif /* INC_MUSIC_AUTO_H_ */
