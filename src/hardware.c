#include "hardware.h"


// Registers for I/O functions
static uint8_t reg_inp1 = 0;
static uint8_t reg_inp2 = 0;
static uint8_t reg_shift_amount = 0;
static uint16_t reg_shift;

// I/O functions
uint8_t read_inp1() {
    return reg_inp1;
}

uint8_t read_inp2() {
    return reg_inp2;
}

uint8_t read_shift() {
    return reg_shift >> (8 - reg_shift_amount);
}

void write_shift_amount(uint8_t data) {
    reg_shift_amount = data & 7;
}

void write_sound_1(uint8_t data) {
    (void)data; // sound not implemented yet
}

void write_shift_data(uint8_t data) {
    reg_shift = (reg_shift >> 8) | (data << 8);
}

void write_sound_2(uint8_t data) {
    (void)data; // sound not implemented yet
}

void write_watchdog(uint8_t data) {
    (void)data; // useless in the case of the emulator
}

void draw_pixel(SDL_Surface *surface, int x, int y, long color) {
    uint32_t *pixels = surface->pixels;
    pixels[x + (y * surface->w)] = color;
}

void draw_screen(SDL_Window *window, SDL_Surface *surface, CPU const * const cpu) {
    int x_o, y_o; // original coordinates of the pixel (without screen multiplier)
    int x_p, y_p; // final coordinates of the pixel to be drawn
    uint8_t bit_n; // bit rank in the byte
    uint16_t byte_n; // byte offset in the VRAM
    long color;
    int i, j;

    // iterate on each byte in VRAM
    for(byte_n = 0; byte_n < VRAM_SIZE; ++byte_n) {
        x_o = SCREEN_WIDTH - ((byte_n & 0xffe0) / 32) - 1;
        for(bit_n = 0; bit_n < 8; ++bit_n) {
            y_o = ((byte_n & 0x1f) * 8) + bit_n;

            // select pixel color
            color = ((cpu->memory[VRAM_START + byte_n] >> (7 - bit_n)) & 1) ? 0xffffffff : 0xff000000;

            for(i = 0; i < SCREEN_MULTIPLIER; ++i) {
                x_p = (x_o * SCREEN_MULTIPLIER) + i;
                for(j = 0; j < SCREEN_MULTIPLIER; ++j) {
                    y_p = (y_o * SCREEN_MULTIPLIER) + j;
                    draw_pixel(surface, x_p, y_p, color);
                }
            }
        }
    }

    SDL_UpdateWindowSurface(window);
}
