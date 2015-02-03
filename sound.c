/*
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "main.h"
#include "sdl.h"
#include "sound.h"

extern int loc_Volume;
static word *buffer = 0;
static word *mix_buffer = 0;
static int buffer_length = 0;

static volatile int buffer_read_index   = 0;
static volatile int buffer_write_index  = 0;
static volatile int buffer_full = 0;

int getSoundVolume(){
    return loc_Volume;
}

void sound_send_buffer(unsigned char *sample, int length){
    int index = 0;

    while(length-- > 0){
        int sample_value = ((signed char)sample[ index++ ]) << 8;
        if(sample_value > 44100) sample_value = 44100;
        if(sample_value < -44100) sample_value = -44100;

        buffer[buffer_write_index++] = sample_value;
        buffer[buffer_write_index++] = sample_value;

        if(buffer_write_index >= buffer_length){
            buffer_write_index = 0;
        }
    }
}

static void sound_callback(void *udata, byte *stream, int length){
    if(ATARI.atari_snd_enable){
        int index = 0;
        word* src = (word *)(buffer + buffer_read_index);
        word* tgt = (word *)mix_buffer;
        int length_w = length / 2;

        while(length_w-- > 0){
            /* Too fast ? */
            *tgt++ = *src++;
      
            if(++buffer_read_index >= buffer_length){
                buffer_read_index = 0;
                src = (word *)buffer;
            }
        }

        long volume = (SDL_MIX_MAXVOLUME * getSoundVolume()) / 100;
        SDL_MixAudio(stream, (unsigned char *)mix_buffer, length, volume);
    }else{
        memset(stream, 0, length);
    }
}

int sound_init(){
    SDL_AudioSpec wanted;
    SDL_AudioSpec obtained;

    memset(&wanted, 0, sizeof(wanted));
    memset(&obtained, 0, sizeof(obtained));

    wanted.freq     = 44100;
    wanted.format   = AUDIO_S16;
    wanted.channels = 1;
    wanted.samples  = 1024;
    wanted.callback = sound_callback;
    wanted.userdata = NULL;

    /* Open the sound device, forcing the desired format */
    if(SDL_OpenAudio(&wanted, &obtained) < 0 ){
        fprintf(stderr, "Couldn't open sound: %s\n", SDL_GetError());
        buffer_length = wanted.samples;
        buffer = (word *)malloc(buffer_length * sizeof(word)); 
        return(0);
    }
  
    buffer_length = 1 * obtained.samples;
    buffer = (word *)malloc(buffer_length * sizeof(word)); 
    memset(buffer, 0, sizeof(word) * buffer_length);
    mix_buffer = (word *)malloc(2 * buffer_length * sizeof(word)); 
    memset(mix_buffer, 0, 2 * sizeof(word) * buffer_length);

    buffer_read_index  = 0;
    buffer_write_index = 0;

    SDL_Delay(1000);        // Give sound some time to init
    SDL_PauseAudio(0);

    return(1);
}
