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


// SCREEN DIMENSIONS
#define SCREEN_HEIGHT 256
#define SCREEN_WIDTH 224
#define SCREEN_MULTIPLIER 3

#define VRAM_START 0x2400
#define VRAM_SIZE (SCREEN_HEIGHT * (SCREEN_WIDTH / 8))


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

#endif
