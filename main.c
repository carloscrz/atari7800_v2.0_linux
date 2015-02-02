/*
 * modified in 01/2015, by carloscrz
 *
 *  Copyright (C) 2006 Ludovic Jacomme (ludovic.jacomme@gmail.com)
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
#include <string.h>
#include <zlib.h>
#include <sys/stat.h>

#include "main.h"
#include "atari.h"
#include "sdl.h"
#include "kbd.h"
#include "input.h"

#define STDOUT_FILE  "stdout.txt"
#define STDERR_FILE  "stderr.txt"

ATARI_t ATARI;
char rom_name[255];
int loc_Volume = 25;

void atari_default_settings(){
  ATARI.atari_snd_enable    = 1;
  ATARI.atari_speed_limiter = 60;
}

void global_initialize(){
  memset(&ATARI, 0, sizeof(ATARI_t));
  strcpy(ATARI.atari_home_dir,".");
  atari_default_settings();
  kbd_default_settings();
  sdl_init();
}

int main(int argc, char *argv[]){

  if(argc > 2) loc_Volume = atoi(argv[1]);
  strcpy(rom_name, argv[argc - 1]);

  #ifndef NO_STDIO_REDIRECT
    /* Redirect standard output and standard error. */
    freopen(STDOUT_FILE, "w", stdout);
    freopen(STDERR_FILE, "w", stderr);
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);  /* Line buffered */
    setbuf(stderr, NULL);          /* No buffering */
  #endif /* NO_STDIO_REDIRECT */
  global_initialize();
  atari_main_loop();
  sdl_exit(0);

  return 0;
}
