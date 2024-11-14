#include <assert.h>
#include <stdio.h>

#include "cpu.h"
#include "opcodes.h"

#include "disassembler.h"

// #define DEBUG_ON

CPU cpu;
static bool end = false;

void test_end(uint8_t data) {
    (void) data;
    end = true;
}

void PRINT(uint8_t data) {
    (void)data;
    if (cpu.c == 9) {
        char *str = (char *)&cpu.memory[get_de(&cpu)];
        while (*str != '$') {
            printf("%c", *str++);
        }
    } else if (cpu.c == 2) {
        printf("%c", cpu.e);
    }
}

void print_40_bytes(uint16_t start) {
    for(int i = 0; i <= 40; ++i) {
        printf("%02hhx ", cpu.memory[i + start]);
        if(i%10 == 9) printf("\n");
    }
    printf("\n");
}

int main(int argc, char const *argv[]) {

    cpu_init(&cpu);

    cpu_load_rom_at(&cpu, "../roms/tests/TST8080.COM", 0x100);
    cpu.pc = 0x100;

    cpu.out_ports[0] = test_end;
    cpu.memory[0] = OUT;
    cpu.memory[1] = 0x00;

    cpu.out_ports[1] = PRINT;
    cpu.memory[0x5] = OUT;
    cpu.memory[0x5 + 1] = 0x01;
    cpu.memory[0x5 + 2] = RET;

    // cpu.memory[368] = 0x7;

    cpu.pc = 0x100;

    #ifdef DEBUG_ON
        init_instr_size();
    #endif

    while(!end) {
        #ifdef DEBUG_ON
            print_cpu_status(&cpu);
            disassemble(&cpu);
            cpu_tick(&cpu);
            getchar();
        #else
            cpu_tick(&cpu);
        #endif

    }

    printf("\n");

    return 0;
}
