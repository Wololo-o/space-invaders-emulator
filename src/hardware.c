#include <SDL2/SDL_mixer.h>
#include "hardware.h"


// Registers for I/O functions
static uint8_t reg_inp1 = 0;
static uint8_t reg_inp2 = 0;
static uint8_t reg_shift_amount = 0;
static uint16_t reg_shift;
static uint8_t reg_sound_1 = 0;
static uint8_t reg_sound_2 = 0;

// Unique event object
static SDL_Event e;

// Sound effects array
#define NUM_SOUNDS 10
static Mix_Chunk *sounds[NUM_SOUNDS];

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
    /*
     * The UFO sound is played repeatedly
     * Other sounds are played only once.
     * So we played them only if the corresponding bit
     * in the register is 0. If it's 1, the sound has
     * already been played.
     */ 
    if(data & UFO_SOUND)
        Mix_PlayChannel(-1, sounds[UFO_SOUND_I], 0);

    if((data & SHOT_SOUND) && !(reg_sound_1 & SHOT_SOUND))
        Mix_PlayChannel(-1, sounds[SHOT_SOUND_I], 0);

    if((data & PLAYER_DEATH_SOUND) && !(reg_sound_1 & PLAYER_DEATH_SOUND))
        Mix_PlayChannel(-1, sounds[PLAYER_DEATH_SOUND_I], 0);

    if((data & INVADER_DEATH_SOUND) && !(reg_sound_1 & INVADER_DEATH_SOUND))
        Mix_PlayChannel(-1, sounds[INVADER_DEATH_SOUND_I], 0);

    if((data & EXTENDED_PLAY_SOUND) && !(reg_sound_1 & EXTENDED_PLAY_SOUND))
        Mix_PlayChannel(-1, sounds[EXTENDED_PLAY_SOUND_I], 0);

    reg_sound_1 = data;
}

void write_shift_data(uint8_t data) {
    reg_shift = (reg_shift >> 8) | (data << 8);
}

void write_sound_2(uint8_t data) {
    if((data & FLEET_MOV_1_SOUND) && !(reg_sound_2 & FLEET_MOV_1_SOUND))
        Mix_PlayChannel(-1, sounds[FLEET_MOV_1_SOUND_I], 0);

    if((data & FLEET_MOV_2_SOUND) && !(reg_sound_2 & FLEET_MOV_2_SOUND))
        Mix_PlayChannel(-1, sounds[FLEET_MOV_2_SOUND_I], 0);

    if((data & FLEET_MOV_3_SOUND) && !(reg_sound_2 & FLEET_MOV_3_SOUND))
        Mix_PlayChannel(-1, sounds[FLEET_MOV_3_SOUND_I], 0);

    if((data & FLEET_MOV_4_SOUND) && !(reg_sound_2 & FLEET_MOV_4_SOUND))
        Mix_PlayChannel(-1, sounds[FLEET_MOV_4_SOUND_I], 0);

    if((data & UFO_HIT_SOUND) && !(reg_sound_2 & UFO_HIT_SOUND))
        Mix_PlayChannel(-1, sounds[UFO_HIT_SOUND_I], 0);

    reg_sound_2 = data;
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

            // pixel displayed or not
            color = ((cpu->memory[VRAM_START + byte_n] >> (7 - bit_n)) & 1) ? 0xffffff : 0;

            // color selection
            // y in [32;64[ -> red
            // y in [184;240[ -> green
            // y in [240;256[ and x in [16;134[ -> green
            if((32 * SCREEN_MULTIPLIER) <= y_b && y_b < (64 * SCREEN_MULTIPLIER))
                color &= 0xff0000;
            else if(((184 * SCREEN_MULTIPLIER) <= y_b && y_b < (240 * SCREEN_MULTIPLIER)) ||
                    ((240 * SCREEN_MULTIPLIER) <= y_b && y_b < (256 * SCREEN_MULTIPLIER) && (16 * SCREEN_MULTIPLIER) <= x_b && x_b < (134 * SCREEN_MULTIPLIER)))
                color &= 0x00ff00;

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

bool init_audio() {
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
        return false;
    }

    Mix_Volume(-1, 1);

    // Load audio files
    sounds[UFO_SOUND_I] = Mix_LoadWAV("../sound/ufo.wav");
    sounds[SHOT_SOUND_I] = Mix_LoadWAV("../sound/shot.wav");
    sounds[PLAYER_DEATH_SOUND_I] = Mix_LoadWAV("../sound/player_death.wav");
    sounds[INVADER_DEATH_SOUND_I] = Mix_LoadWAV("../sound/invader_death.wav");
    sounds[EXTENDED_PLAY_SOUND_I] = Mix_LoadWAV("../sound/extended_play.wav");
    sounds[FLEET_MOV_1_SOUND_I] = Mix_LoadWAV("../sound/fleet_mov_1.wav");
    sounds[FLEET_MOV_2_SOUND_I] = Mix_LoadWAV("../sound/fleet_mov_2.wav");
    sounds[FLEET_MOV_3_SOUND_I] = Mix_LoadWAV("../sound/fleet_mov_3.wav");
    sounds[FLEET_MOV_4_SOUND_I] = Mix_LoadWAV("../sound/fleet_mov_4.wav");
    sounds[UFO_HIT_SOUND_I] = Mix_LoadWAV("../sound/ufo_hit.wav");

    return true;
}

void quit_audio() {
    Mix_Quit();
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
