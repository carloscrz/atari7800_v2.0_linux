/*
 *
 *  Ludovic Jacomme <Ludovic.Jacomme@gmail.com>
 *
 *  modified in 01/2015 by carloscrz
 *
*/

#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "main.h"
#include "kbd.h"
#include "sdl.h"

#include "Core/Cartridge.h"
#include "Core/ProSystem.h"
#include "Core/Database.h"
#include "Core/Sound.h"

extern SDL_Joystick *joy;
extern char rom_name[];

    static word display_palette16[256];

/*
    static void atari_reset(){
        prosystem_Reset();
    }

    static int atari_load_state(char *filename){
        int error;
        std::string std_filename(filename);
        error = prosystem_Load(filename) ? 0 : 1;
        return error;
    }

    static int atari_save_state(char *filename){
        int error;
        std::string std_filename(filename);
        error = prosystem_Save(std_filename, true) ? 0 : 1;
        return error;
    }

    static int atari_load_rom_buffer(char *rom_buffer, int rom_size){
        if(!cartridge_Load_buffer(rom_buffer, rom_size)) return 1;
        database_Load(cartridge_digest);
        prosystem_Reset();
        return 0;
    }
*/

    static void atari_synchronize(){
        static uint nextclock = 1;
        static uint next_sec_clock = 0;
        static uint cur_num_frame = 0;

        uint curclock = SDL_GetTicks();

        if(ATARI.atari_speed_limiter){
    
            while(curclock < nextclock) curclock = SDL_GetTicks();
    
            uint f_period = 1000 / ATARI.atari_speed_limiter;
            nextclock += f_period;
        
            if(nextclock < curclock) nextclock = curclock + f_period;
        }

        if(ATARI.atari_view_fps){
            cur_num_frame++;
    
            if(curclock > next_sec_clock){
                next_sec_clock = curclock + 1000;
                ATARI.atari_current_fps = cur_num_frame * (1 + ATARI.skip_max_frame);
                cur_num_frame = 0;
            }
        }
    }

    static void atari_init_palette16(){
       uint index = 0;
        for(index; index < 256; index++){
            word r = palette_data[(index * 3) + 0];
            word g = palette_data[(index * 3) + 1];
            word b = palette_data[(index * 3) + 2];
            display_palette16[index] = sdl_rgb(r, g, b);
        }
    }

    static inline void atari_put_image_normal(){
        uint height = maria_visibleArea.GetHeight;
        uint length = maria_visibleArea.GetLength;

        const byte* buffer = maria_surface + 
            ((maria_visibleArea.top - maria_displayArea.top) * length);

        uint* surface = (uint*)((word *)back_surface->pixels + 8 * LINE_SIZE);
        uint indexY = height;

        while(indexY-- > 0){
            uint indexX = length >> 2;

            while(indexX > 0){
                uint v = display_palette16[*buffer++];
                v |= display_palette16[*buffer++] << 16;
                *surface++ = v;
                v = display_palette16[*buffer++];
                v |= display_palette16[*buffer++] << 16;
                *surface++ = v;
            	indexX--;
            }
            surface += (LINE_SIZE - length) >> 1;
        }
    }

    static void atari_refresh_screen(){
        if(ATARI.skip_cur_frame <= 0){
            ATARI.skip_cur_frame = ATARI.skip_max_frame;
            atari_put_image_normal(); 
            atari_synchronize();

            if(ATARI.atari_view_fps){
                char buffer[32];
                sprintf(buffer, "%03d %3d", ATARI.atari_current_clock, (int)ATARI.atari_current_fps );
            }
            sdl_flip();
        }
    }

    static int atari_load_rom(char *filename){
        if(!cartridge_Load(filename)) return 1;
        database_Load(cartridge_digest);
	prosystem_Reset();
        return 0;
    }

    void atari_main_loop(){
        if(bios_Load("./7800.rom")) bios_enabled = 1;

        if(atari_load_rom(rom_name)){
            puts("Error!");
            sdl_exit(1);
        }


        atari_init_palette16();

        for(;;){
            update_keys();
    
            if(prosystem_active && !prosystem_paused) {
                prosystem_ExecuteFrame(ATARI.keyboard_data);
                sound_Store();
                atari_refresh_screen();
            }
        }
    }
