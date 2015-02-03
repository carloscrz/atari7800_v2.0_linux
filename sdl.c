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
#include <png.h>
#include "main.h"
#include "sound.h"
#include "sdl.h"

SDL_Joystick *joy;
extern unsigned char font_8x8[];
extern unsigned char font_6x10[];

unsigned char *font;
int            font_width  = 8; 
int            font_height = 8; 

SDL_Surface *back_surface;

uint sdl_rgb(uchar R, uchar G, uchar B){
    return SDL_MapRGB(back_surface->format, R,G,B);
}

unsigned char convert_utf8_to_iso_8859_1(unsigned char c1, unsigned char c2){
    unsigned char res = 0;
    if (c1 == 0xc2){
        res = c2;
    }else{
        if(c1 == 0xc3) res = c2 | 0x40;
    }
    return res;
}

void sdl_unlock(void){
    SDL_UnlockSurface(back_surface);
}

void sdl_flip(void){
    SDL_Flip(back_surface);
}

int sdl_init(void){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK) < 0) return 0;
    
    SDL_ShowCursor(0);
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
    joy = SDL_JoystickOpen(0);

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) return 0;
  
    sdl_select_font_6x10();
    back_surface=SDL_SetVideoMode(SDL_SCREEN_WIDTH, SDL_SCREEN_HEIGHT, 16 ,SDL_SWSURFACE);

    if(!back_surface) return 0;

    sound_init();
    return 1;
}

void sdl_exit(int status){
    SDL_CloseAudio();
    SDL_Quit();
    sleep(1);
    exit(status);
}

void sdl_select_font_8x8(){
    font = font_8x8;
    font_height = 8;
    font_width  = 8;
}

void sdl_select_font_6x10(){
  font = font_6x10;
  font_height = 10;
  font_width  = 6;
}
