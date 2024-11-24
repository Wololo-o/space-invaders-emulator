#ifndef HARDWARE_H
#define HARDWARE_H

#include "cpu.h"
#include <SDL2/SDL.h>
#include <stdint.h>

// PORTS MAPPING
#define INP1 1
#define INP2 2
#define SHFT_IN 3

#define SHFTAMNT 2
#define SOUND1 3
#define SHFT_DATA 4
#define SOUND2 5
#define WATCHDOG 6


// PORTS BITS
// INPUT 1
#define CREDIT 0x01
#define START_2P 0x02
#define START_1P 0x04
#define SHOT_1P 0x10
#define LEFT_1P 0x20
#define RIGHT_1P 0x40

// INPUT 2
#define TILT 0x04
#define SHOT_P2 0x10
#define LEFT_P2 0x20
#define RIGHT_P2 0x40

//OUTPUT 3
#define UFO_SOUND 0x01
#define SHOT_SOUND 0x02
#define PLAYER_DEATH_SOUND 0x04
#define INVADER_DEATH_SOUND 0x08
#define EXTENDED_PLAY_SOUND 0x10

// OUTPUT 5
#define FLEET_MOV_1_SOUND 0x01
#define FLEET_MOV_2_SOUND 0x02
#define FLEET_MOV_3_SOUND 0x04
#define FLEET_MOV_4_SOUND 0x08
#define UFO_HIT_SOUND 0x10


// SCREEN DIMENSIONS
#define SCREEN_HEIGHT 256
#define SCREEN_WIDTH 224
#define SCREEN_MULTIPLIER 3

#define VRAM_START 0x2400
#define VRAM_SIZE (SCREEN_HEIGHT * (SCREEN_WIDTH / 8))

// GAME SPEED CONSTANTS
#define CPU_FREQU 2000000
#define GAME_FRAME_RATE 60
#define VERTICAL_BLANK_FREQ (CPU_FREQU / GAME_FRAME_RATE)


// SOUND EFFECT INDICES
#define UFO_SOUND_I 0
#define SHOT_SOUND_I 1
#define PLAYER_DEATH_SOUND_I 2
#define INVADER_DEATH_SOUND_I 3
#define EXTENDED_PLAY_SOUND_I 4
#define FLEET_MOV_1_SOUND_I 5
#define FLEET_MOV_2_SOUND_I 6
#define FLEET_MOV_3_SOUND_I 7
#define FLEET_MOV_4_SOUND_I 8
#define UFO_HIT_SOUND_I 9


// I/O functions
uint8_t read_inp1();
uint8_t read_inp2();
uint8_t read_shift();

void write_shift_amount(uint8_t data);
void write_sound_1(uint8_t data);
void write_shift_data(uint8_t data);
void write_sound_2(uint8_t data);
void write_watchdog(uint8_t data);


void draw_pixel(SDL_Surface *surface, int x, int y, long color);
void draw_screen(SDL_Window *window, SDL_Surface *surface, CPU const * const cpu);

bool init_audio();
void quit_audio();

bool handle_input();

#endif
