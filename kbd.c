/*
 * modified in 01/2015, by carloscrz
 *
 *  Copyright (C) 2009 Ludovic Jacomme (ludovic.jacomme@gmail.com)
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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>
#include <SDL.h>
#include "main.h"
#include "kbd.h"
#include "sdl.h"

static ctrlData     loc_button_data;
static long         first_time_stamp = -1;
static char         loc_button_press[KBD_MAX_BUTTONS]; 
static char         loc_button_release[KBD_MAX_BUTTONS]; 

static unsigned int loc_button_mask[ KBD_MAX_BUTTONS ] = {
   ATARI_CTRL_UP,
   ATARI_CTRL_RIGHT,
   ATARI_CTRL_DOWN,
   ATARI_CTRL_LEFT,
   ATARI_CTRL_A,
   ATARI_CTRL_B,
   ATARI_CTRL_X,
   ATARI_CTRL_Y
};

static int loc_default_mapping[KBD_ALL_BUTTONS] = {
    ATARI_JOY1_UP          , /*  KBD_UP         */
    ATARI_JOY1_RIGHT       , /*  KBD_RIGHT      */
    ATARI_JOY1_DOWN        , /*  KBD_DOWN       */
    ATARI_JOY1_LEFT        , /*  KBD_LEFT       */
    ATARI_LDIFF            , /*  KBD_TRIANGLE   */
    ATARI_JOY1_FIRE2       , /*  KBD_CIRCLE     */
    ATARI_JOY1_FIRE1       , /*  KBD_CROSS      */
    ATARI_RDIFF            , /*  KBD_SQUARE     */
    -1                     , /*  KBD_SELECT     */
    -1                     , /*  KBD_START      */
    KBD_LTRIGGER_MAPPING    , /*  KBD_LTRIGGER   */
    KBD_RTRIGGER_MAPPING    , /*  KBD_RTRIGGER   */
    ATARI_JOY1_FIRE1        , /*  KBD_JOY_FIRE   */
    ATARI_JOY1_UP           , /*  KBD_JOY_UP     */
    ATARI_JOY1_RIGHT        , /*  KBD_JOY_RIGHT  */
    ATARI_JOY1_DOWN         , /*  KBD_JOY_DOWN   */
    ATARI_JOY1_LEFT           /*  KBD_JOY_LEFT   */
};

static int loc_default_mapping_L[KBD_ALL_BUTTONS] = {
    ATARI_JOY1_UP          , /*  KBD_UP         */
    ATARI_JOY1_RIGHT       , /*  KBD_RIGHT      */
    ATARI_JOY1_DOWN        , /*  KBD_DOWN       */
    ATARI_JOY1_LEFT        , /*  KBD_LEFT       */
    ATARIC_LOAD            , /*  KBD_TRIANGLE   */
    ATARIC_JOY             , /*  KBD_CIRCLE     */
    ATARIC_SAVE            , /*  KBD_CROSS      */
    ATARIC_FPS             , /*  KBD_SQUARE     */
    -1                     , /*  KBD_SELECT     */
    -1                     , /*  KBD_START      */
    KBD_LTRIGGER_MAPPING    , /*  KBD_LTRIGGER   */
    KBD_RTRIGGER_MAPPING    , /*  KBD_RTRIGGER   */
    ATARI_JOY1_FIRE1        , /*  KBD_JOY_FIRE   */
    ATARI_JOY1_UP          , /*  KBD_JOY_UP     */
    ATARI_JOY1_RIGHT       , /*  KBD_JOY_RIGHT  */
    ATARI_JOY1_DOWN        , /*  KBD_JOY_DOWN   */
    ATARI_JOY1_LEFT          /*  KBD_JOY_LEFT   */
};

static int loc_default_mapping_R[KBD_ALL_BUTTONS] = {
    ATARI_JOY1_UP          , /*  KBD_UP         */
    ATARIC_INCFIRE         , /*  KBD_RIGHT      */
    ATARI_JOY1_DOWN        , /*  KBD_DOWN       */
    ATARIC_DECFIRE         , /*  KBD_LEFT       */
    ATARI_RESET            , /*  KBD_TRIANGLE   */
    ATARIC_AUTOFIREB       , /*  KBD_CIRCLE     */
    ATARIC_AUTOFIRE        , /*  KBD_CROSS      */
    ATARI_PAUSE            , /*  KBD_SQUARE     */
    -1                     , /*  KBD_SELECT     */
    -1                     , /*  KBD_START      */
    KBD_LTRIGGER_MAPPING    , /*  KBD_LTRIGGER   */
    KBD_RTRIGGER_MAPPING    , /*  KBD_RTRIGGER   */
    ATARI_JOY1_FIRE1        , /*  KBD_JOY_FIRE   */
    ATARI_JOY1_UP          , /*  KBD_JOY_UP     */
    ATARI_JOY1_RIGHT       , /*  KBD_JOY_RIGHT  */
    ATARI_JOY1_DOWN        , /*  KBD_JOY_DOWN   */
    ATARI_JOY1_LEFT          /*  KBD_JOY_LEFT   */
};

static int kbd_mapping[KBD_ALL_BUTTONS];
static int kbd_mapping_L[KBD_ALL_BUTTONS];
static int kbd_mapping_R[KBD_ALL_BUTTONS];
static int kbd_presses[KBD_ALL_BUTTONS];
static int kbd_ltrigger_mapping_active;
static int kbd_rtrigger_mapping_active;

static int atari_key_event(int atari_idx, int button_press){ 
    if((atari_idx >= 0) && (atari_idx <= ATARI_RDIFF)){
        if(button_press){
            ATARI.keyboard_data[atari_idx] = 1;
        }else{
            ATARI.keyboard_data[atari_idx] = 0;
        }
    }

    return 0;
}

void kbd_default_settings(){
    memcpy(kbd_mapping, loc_default_mapping, sizeof(loc_default_mapping));
    memcpy(kbd_mapping_L, loc_default_mapping_L, sizeof(loc_default_mapping_L));
    memcpy(kbd_mapping_R, loc_default_mapping_R, sizeof(loc_default_mapping_R));
}

static int atari_decode_key(int b, int button_pressed){    
    if(kbd_mapping[b] >= 0){       
        if(button_pressed){
            if(!kbd_ltrigger_mapping_active){
                if(!kbd_rtrigger_mapping_active){
                    kbd_presses[b] = kbd_mapping[b];
                    atari_key_event(kbd_presses[b], button_pressed);
                }
            }
        }else{
            atari_key_event(kbd_presses[b], button_pressed);
        }
    }

    return 0;
}

static int kbd_scan_keyboard(void){
    ctrlData  c;
    int       event;
    int       b;

    event = 0;
    ctrlPeekBufferPositive( &c, 1 );

    for(b = 0; b < KBD_MAX_BUTTONS; b++){
        if(c.Buttons & loc_button_mask[b]){    
            loc_button_press[b] = 1;
            event = 1;
        }else{
            if(loc_button_data.Buttons & loc_button_mask[b]){
                loc_button_release[b] = 1;
                event = 1;
            }
        }
    }

    memcpy(&loc_button_data,&c,sizeof(ctrlData));

    return event;
}

void init_keyboard(void){    
    kbd_ltrigger_mapping_active = 0;
    kbd_rtrigger_mapping_active = 0;
}

int update_keys(void){
    int       b;
    ctrlData  c;
    static int w;
    static char first_time = 1;
    static int release_pending = 0;
    
    if(first_time){
        ctrlPeekBufferPositive(&c, 1);
    
        if(first_time_stamp == -1) first_time_stamp = c.TimeStamp;

        first_time      = 0;
        release_pending = 0;

        for(b = 0; b < KBD_MAX_BUTTONS; b++){
            loc_button_release[b] = 0;
            loc_button_press[b] = 0;
        }
    
        ctrlPeekBufferPositive(&loc_button_data, 1);
        init_keyboard();
        return 0;
    }

    if(release_pending){
        release_pending = 0;
    
        for(b = 0; b < KBD_MAX_BUTTONS; b++){
            if(loc_button_release[b]){
                loc_button_release[b] = 0;
                loc_button_press[b] = 0;
                atari_decode_key(b, 0);
            }
        }
    }

    kbd_scan_keyboard();

    /* check press event */
    for(b = 0; b < KBD_MAX_BUTTONS; b++){
        if(loc_button_press[b]){
            loc_button_press[b] = 0;
            release_pending     = 0;
            atari_decode_key(b, 1);
        }
    }
  
    /* check release event */
    for(b = 0; b < KBD_MAX_BUTTONS; b++){
        if(loc_button_release[b]){
            release_pending = 1;
            break;
        }
    }

    return 0;
}
