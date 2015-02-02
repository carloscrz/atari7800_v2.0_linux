/*
 *  modified in 01/2015, by carlos
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
 
#include <SDL.h>
#include <SDL_events.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "input.h"
#include "kbd.h"

extern SDL_Joystick *joy;
extern int loc_Volume;

static int    loc_LastEventMask    = 0;
static int    loc_CurrEventMask    = 0;
static int    loc_CurrEventButtons = 0;
static uint    loc_LastTimeStamp    = 0;
static uint    loc_CurrTimeStamp    = 0;

static int convertJoystickButton(int);
static inline int convertMaskToButtons(int);

int ctrlPeekBufferPositive(ctrlData* c, int v){
    SDL_Event SDLEvent;    
    int flag = 0;
    int Event = 0;
    int ButtonPress = 0;
    int ButtonRelease = 0;
    int Mask = 0;

    memset(c, 0x0, sizeof(ctrlData));
    loc_CurrTimeStamp = SDL_GetTicks() * 1000;

    if(SDL_PollEvent(&SDLEvent)){      
        Event=((SDL_KeyboardEvent*)(&SDLEvent))->keysym.scancode;

        if(SDLEvent.type == SDL_KEYDOWN){
            ButtonPress = 1;
        }else{
            if(SDLEvent.type == SDL_KEYUP){
                ButtonRelease = 1;
            }
        }

        switch(SDLEvent.type){
		    case SDL_JOYBUTTONDOWN:		
   			    Event = convertJoystickButton(SDLEvent.jbutton.button);			
                	    ButtonPress = 1;
			    break;

		    case SDL_JOYBUTTONUP:
   			    Event = convertJoystickButton(SDLEvent.jbutton.button);			
                	    ButtonRelease = 1;
			    break;
/*
		    case SDL_JOYAXISMOTION:
			    Event = get_SDL_joystick_state(joy, &flag);
			    if(flag){
				    ButtonRelease = 1;
			    }else{				
				    ButtonPress = 1;
			    }
			    break;
*/
	    }

	    if(Event == CTRL_QUIT) sdl_exit(0); 

	    switch(Event){
		    case CTRL_UP:		Mask = ATARI_CTRL_UP;
			    break;
		    case CTRL_DOWN:		Mask = ATARI_CTRL_DOWN;
			    break;
		    case CTRL_LEFT:		Mask = ATARI_CTRL_LEFT;
			    break;
		    case CTRL_RIGHT:	Mask = ATARI_CTRL_RIGHT;
			    break;
		    case CTRL_X:	    Mask = ATARI_CTRL_X;
			    break;
		    case CTRL_Y:	    Mask = ATARI_CTRL_Y;
			    break;
		    case CTRL_B:	    Mask = ATARI_CTRL_B;
			    break;
	        case CTRL_A:        Mask = ATARI_CTRL_A;
                break;
        }

        loc_LastEventMask = loc_CurrEventMask;
        
        if(ButtonPress){
            loc_CurrEventMask |= Mask;
        }else{ 
            if(ButtonRelease){
                loc_CurrEventMask &= ~Mask;
            }
        }

        loc_CurrEventButtons = convertMaskToButtons(loc_CurrEventMask);
        c->Buttons   = loc_CurrEventButtons;
        c->TimeStamp = loc_CurrTimeStamp;
        loc_LastTimeStamp = loc_CurrTimeStamp;

        /*
	    fix direcional release 
        if(ButtonRelease){
	        if(Event != CTRL_Y && Event != CTRL_X && Event != CTRL_B){ 
		        loc_CurrEventMask = Mask = loc_CurrEventButtons &= 0x0FFFFFFF0;
            }
        }
        */

    }else{
        c->Buttons   = loc_CurrEventButtons;
        c->TimeStamp = loc_CurrTimeStamp;
    }

    return (c->Buttons != 0);
}

/*
static int get_SDL_joystick_state(SDL_Joystick *joystick, int *r){
	int x;
	int y;
	static int anterior;
	*r = 0;

	x = SDL_JoystickGetAxis(joystick, 0);
	y = SDL_JoystickGetAxis(joystick, 1);

	if (x > minjoy) {
		if (y < -minjoy){
			anterior = CTRL_UPRIGHT; 
			return CTRL_UPRIGHT;
		}else if (y > minjoy){
			anterior = CTRL_DOWNRIGHT; 
			return CTRL_DOWNRIGHT;
		}else{
			anterior = CTRL_RIGHT; 
			return CTRL_RIGHT;
		}
	}
	else if (x < -minjoy) {
		if (y < -minjoy){
			anterior = CTRL_UPLEFT; 
			return CTRL_UPLEFT;
		}else if (y > minjoy){
			anterior = CTRL_DOWNLEFT; 
			return CTRL_DOWNLEFT;
		}else{
			anterior = CTRL_LEFT; 
			return CTRL_LEFT;
		}
		
	}
	else {
		if (y < -minjoy){
			anterior = CTRL_UP; 
			return CTRL_UP;
		}else if (y > minjoy){
			anterior = CTRL_DOWN; 
			return CTRL_DOWN;
		}else{
         		*r = 1;
			return anterior;
		}
	}
}
*/

static int convertJoystickButton(int Event){
    switch(Event){
/*
        case 0: return CTRL_X;
        case 1: return CTRL_B;
*/
        case 0: return CTRL_UP;
        case 1: return CTRL_RIGHT;
        case 2: return CTRL_DOWN;
        case 3: return CTRL_LEFT;
        case 11: return CTRL_A;
        case 6: return CTRL_Y;
        case 4: return CTRL_B;
	    case 5:
        case 7: return CTRL_X;

        case 8: return CTRL_QUIT;
        default: return Event;
    }
}

static inline int convertMaskToButtons(int Mask){
    int Buttons = Mask & CTRL_MASK;
    if(Mask & 0x10000) Buttons |= CTRL_UP|CTRL_RIGHT;
    if(Mask & 0x20000) Buttons |= CTRL_UP|CTRL_LEFT;
    if(Mask & 0x40000) Buttons |= CTRL_DOWN|CTRL_RIGHT;
    if(Mask & 0x80000) Buttons |= CTRL_DOWN|CTRL_LEFT;
    return Buttons;
}
