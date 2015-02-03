/*
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

# ifndef _SDL_H_
# define _SDL_H_

# ifdef __cplusplus
    extern "C" {
# endif

#define SDL_NOP             0
#define SDL_XOR             1
#define LINE_SIZE           320
#define SDL_SCREEN_WIDTH    320
#define SDL_SCREEN_HEIGHT   240

extern SDL_Surface* back_surface;

extern uint sdl_rgb(uchar, uchar, uchar);
extern void sdl_black_screen();
extern void sdl_unlock();
extern void sdl_flip();
extern int sdl_init();
extern void sdl_exit(int status);
extern void sdl_select_font_6x10();
extern void sdl_select_font_8x8();

#ifdef __cplusplus
}
#endif
# endif
