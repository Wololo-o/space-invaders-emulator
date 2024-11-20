#include "disassembler.h"
#include "hardware.h"
#include "opcodes.h"
#include <stdio.h>

bool init_SDL() {
	return SDL_Init(SDL_INIT_VIDEO);
}

SDL_Window *create_window() {
	int screen_width = SCREEN_WIDTH * SCREEN_MULTIPLIER;
	int screen_height = SCREEN_HEIGHT * SCREEN_MULTIPLIER;
	return SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_OPENGL);
}

void init_port(CPU *cpu) {
	cpu->in_ports[INP1] = read_inp1;
	cpu->in_ports[INP2] = read_inp2;
	cpu->in_ports[SHFT_IN] = read_shift;

	cpu->out_ports[SHFTAMNT] = write_shift_amount;
	cpu->out_ports[SOUND1] = write_sound_1;
	cpu->out_ports[SHFT_DATA] = write_shift_data;
	cpu->out_ports[SOUND2] = write_sound_2;
	cpu->out_ports[WATCHDOG] = write_watchdog;
}

int main() {
	CPU cpu;
	cpu_init(&cpu);
	bool running = true;
	uint32_t ticks;

	//Try to load ROMs into the memory
	if(!cpu_load_rom_at(&cpu, "../roms/space-invaders/invaders.h", 0x0000)
		|| !cpu_load_rom_at(&cpu, "../roms/space-invaders/invaders.g", 0x0800)
		|| !cpu_load_rom_at(&cpu, "../roms/space-invaders/invaders.f", 0x1000)
		|| !cpu_load_rom_at(&cpu, "../roms/space-invaders/invaders.e", 0x1800)
		)
	{
		fprintf(stderr, "Error in loading ROM files.\n");
		return 1;
	}

	init_port(&cpu);

	if(init_SDL() != 0) {
		fprintf(stderr, "Error during SDL initialization: %s\n", SDL_GetError());
		return 2;
	}

	SDL_Window *window = create_window();

	if(!window) {
		fprintf(stderr, "Error during SDL window creation: %s\n", SDL_GetError());
		return 3;
	}

	SDL_Surface *surface = SDL_GetWindowSurface(window);
	if (!surface) {
        fprintf(stderr, "Could not create SDL surface: %s\n", SDL_GetError());
        return 4;
    }

	while(running) {
		draw_screen(window, surface, &cpu);
		running = handle_input();

		ticks = SDL_GetTicks();
		
		while(cpu.cycle_count < VERTICAL_BLANK_FREQ / 2) {
			cpu_tick(&cpu);
		}
		interrupt(&cpu, RST_1);

		while(cpu.cycle_count < VERTICAL_BLANK_FREQ) {
			cpu_tick(&cpu);
		}
		interrupt(&cpu, RST_2);

		cpu.cycle_count = 0;
		SDL_Delay((1000 / GAME_FRAME_RATE) - (SDL_GetTicks() - ticks));
	}

	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return 0;
}
