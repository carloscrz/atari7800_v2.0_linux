#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

ATARI_VERSION=2.0

TARGET = atari7800-i386
SDL_CONFIG = /usr/bin/sdl-config

OBJCORE =    Core/Zip/ioapi.o \
  Core/Zip/zip.o \
  Core/Zip/unzip.o \
  Core/Sound.o \
  Core/Bios.o \
  Core/Cartridge.o \
  Core/Database.o \
  Core/Hash.o \
  Core/Logger.o \
  Core/Maria.o \
  Core/Memory.o \
  Core/Palette.o \
  Core/Pokey.o \
  Core/ProSystem.o \
  Core/Region.o \
  Core/Riot.o \
  Core/Sally.o \
  Core/Tia.o \
  Core/Archive.o

OBJS = kbd.o \
  input.o \
  sdl.o \
  font.o \
  sound.o \
  atari.o \
  main.o
  
CC=gcc
CXX=g++

DEFAULT_CFLAGS = $(shell $(SDL_CONFIG) --cflags)

MORE_CFLAGS = -I. -I/usr/include \
-DNOCRYPT \
-DNO_STDIO_REDIRECT -DATARI_VERSION=\"$(ATARI_VERSION)\"  \
-D_GNU_SOURCE=1 -D_REENTRANT -DIS_LITTLE_ENDIAN \
-fsigned-char -ffast-math -fomit-frame-pointer \
-fexpensive-optimizations -fno-strength-reduce -funroll-loops  -finline-functions

CFLAGS = -Wall -Ofast -fstrict-aliasing  $(DEFAULT_CFLAGS) $(MORE_CFLAGS) 

LIBS += -L/usr/lib \
	/usr/lib/i386-linux-gnu/libSDL_image.a \
	/usr/lib/i386-linux-gnu/libSDL.a \
	-lpng -lz -lm -lpthread -lstdc++ -ldl -lSDL -lSDL_image

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJCORE) $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJCORE) $(OBJS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

clear:
	rm -f $(OBJCORE) $(OBJS) $(TARGET)

ctags: 
	ctags *[ch] emucore/*[chp] common/*[chp]
