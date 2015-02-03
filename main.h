#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef __cplusplus
    extern "C" {
#endif

/*
typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;
*/

typedef unsigned char   byte;
typedef unsigned short  word;
typedef unsigned int    uint;

typedef unsigned char   uchar;
typedef unsigned int    uint;
typedef unsigned short  ushort;

#include <SDL.h>
#include <time.h>
#include <dirent.h>

#include "input.h"

#define ATARI_RENDER_NORMAL     0
#define ATARI_LAST_RENDER       0
#define ATARI_WIDTH             320
#define ATARI_HEIGHT            240
#define ATARI_BLIT_WIDTH        320
#define ATARI_BLIT_HEIGHT       240
#define SNAP_WIDTH              (ATARI_WIDTH*10/25)
#define SNAP_HEIGHT             (ATARI_HEIGHT*10/25)
#define MAX_PATH                256
#define ATARI_MAX_SAVE_STATE    5
#define ATARI_MAX_CHEAT         10
#define ATARI_RAM_SIZE          65536

typedef struct Atari_cheat_t {
    unsigned char  type;
    unsigned short addr;
    unsigned char  value;
    char           comment[25];
}Atari_cheat_t;

typedef struct Atari_save_t {
    SDL_Surface    *surface;
    char            used;
    char            thumb;
    time_t          date;
}Atari_save_t;

typedef struct Atari_t{
    Atari_save_t atari_save_state[ATARI_MAX_SAVE_STATE];
    Atari_cheat_t atari_cheat[ATARI_MAX_CHEAT];
    char atari_save_name[MAX_PATH];
    char atari_home_dir[MAX_PATH];
    int  comment_present;
    int  cpu_clock;
    int  reverse_analog;
    int  atari_current_clock;
    int  atari_view_fps;
    int  atari_current_fps;
    int  active_joystick;
    int  atari_snd_enable;
    int  atari_render_mode;
    int  atari_speed_limiter;
    int  atari_vsync;
    int  skip_max_frame;
    int  skip_cur_frame;
    unsigned char keyboard_data[19];
}ATARI_t;

extern ATARI_t ATARI;

#ifdef __cplusplus
}
#endif
#endif
