#include "hardware.h"


// Registers for I/O functions
static uint8_t reg_inp1 = 0;
static uint8_t reg_inp2 = 0;
static uint8_t reg_shift_amount = 0;
static uint16_t reg_shift;

// Unique event object
static SDL_Event e;

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
    int x_b, y_b; // bas coordiantes of the pixel
    int x_p, y_p; // final coordinates of the pixel to be drawn
    uint8_t bit_n; // bit rank in the byte
    uint16_t byte_n; // byte offset in the VRAM
    long color;
    int i, j;

    // iterate on each byte in VRAM
    for(byte_n = 0; byte_n < VRAM_SIZE; ++byte_n) {
        x_b = (byte_n / 32) * SCREEN_MULTIPLIER;
        for(bit_n = 0; bit_n < 8; ++bit_n) {
            y_b = ((SCREEN_HEIGHT - 1) * SCREEN_MULTIPLIER) - ((byte_n & 0x1f) * 8 * SCREEN_MULTIPLIER) - ((7 - bit_n) * SCREEN_MULTIPLIER);

            // select pixel color
            color = ((cpu->memory[VRAM_START + byte_n] >> (7 - bit_n)) & 1) ? 0xffffffff : 0xff000000;

            for(i = 0; i < SCREEN_MULTIPLIER; ++i) {
                x_p = x_b + i;
                for(j = 0; j < SCREEN_MULTIPLIER; ++j) {
                    y_p = y_b + j;
                    draw_pixel(surface, x_p, y_p, color);
                }
            }
        }
    }

    SDL_UpdateWindowSurface(window);
}

bool handle_input() {
    /*
    General inputs:
    RETURN: start game with one player
    Keypad 2 | p: start game with two players
    c: insert coin
    t: tilt

    Player 1 inputs:
    q | left arrow: left
    d | right arrow: right
    z | up arrow | space: shot

    Player 2 inputs:
    Keybad 4 | k: left
    Keypad 6 | m: right
    Keypad 8 | o: shot
    */
   

    while(SDL_PollEvent(&e)) {
        switch(e.type) {
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:
            switch(e.key.keysym.sym) {
            case SDLK_RETURN:
                reg_inp1 |= START_1P;
                break;
            case SDLK_KP_2:
            case SDLK_p:
                reg_inp1 |= START_2P;
                break;
            case SDLK_c:
                reg_inp1 |= CREDIT;
                break;
            case SDLK_t:
                reg_inp2 |= TILT;
                break;
            case SDLK_q:
            case SDLK_LEFT:
                reg_inp1 |= LEFT_1P;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                reg_inp1 |= RIGHT_1P;
                break;
            case SDLK_z:
            case SDLK_UP:
            case SDLK_SPACE:
                reg_inp1 |= SHOT_1P;
                break;
            case SDLK_KP_4:
            case SDLK_k:
                reg_inp2 |= LEFT_P2;
                break;
            case SDLK_KP_6:
            case SDLK_m:
                reg_inp2 |= RIGHT_P2;
                break;
            case SDLK_KP_8:
            case SDLK_o:
                reg_inp2 |= SHOT_P2;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch(e.key.keysym.sym) {
            case SDLK_RETURN:
                reg_inp1 &= ~START_1P;
                break;
            case SDLK_KP_2:
            case SDLK_p:
                reg_inp1 &= ~START_2P;
                break;
            case SDLK_c:
                reg_inp1 &= ~CREDIT;
                break;
            case SDLK_t:
                reg_inp2 &= ~TILT;
                break;
            case SDLK_q:
            case SDLK_LEFT:
                reg_inp1 &= ~LEFT_1P;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                reg_inp1 &= ~RIGHT_1P;
                break;
            case SDLK_z:
            case SDLK_UP:
            case SDLK_SPACE:
                reg_inp1 &= ~SHOT_1P;
                break;
            case SDLK_KP_4:
            case SDLK_k:
                reg_inp2 &= ~LEFT_P2;
                break;
            case SDLK_KP_6:
            case SDLK_m:
                reg_inp2 &= ~RIGHT_P2;
                break;
            case SDLK_KP_8:
            case SDLK_o:
                reg_inp2 &= ~SHOT_P2;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    return true;
}
