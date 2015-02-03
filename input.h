/*
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

#ifndef _INPUT_H_
#define _INPUT_H_

#ifdef __cplusplus
    extern "C" {
#endif

#include "main.h"

typedef struct ctrlData{
    int    Buttons;
    uint    TimeStamp;
}ctrlData;

typedef struct ctrlData SceCtrlData;

#define CTRL_UPRIGHT        7
#define CTRL_UPLEFT         1
#define CTRL_DOWNRIGHT      5
#define CTRL_DOWNLEFT       3
#define CTRL_MASK           0x07fff
#define CTRL_RIGHT	        114
#define CTRL_UP		        111
#define CTRL_DOWN	        116
#define CTRL_LEFT	        113
#define CTRL_A              42 // g
#define CTRL_B              39 // s
#define CTRL_X              40 // d
#define CTRL_Y              38 // a
#define CTRL_SELECT         41 // f
#define CTRL_QUIT           9 // ESC
#define ATARI_CTRL_UP       0x00001
#define ATARI_CTRL_RIGHT    0x00002
#define ATARI_CTRL_DOWN     0x00004
#define ATARI_CTRL_LEFT     0x00008
#define ATARI_CTRL_A        0X00080
#define ATARI_CTRL_B        0x00020
#define ATARI_CTRL_X        0x00040
#define ATARI_CTRL_Y        0x00010
#define minjoy              10000

extern int ctrlPeekBufferPositive(ctrlData *, int);

# ifdef __cplusplus
}
# endif

# endif
