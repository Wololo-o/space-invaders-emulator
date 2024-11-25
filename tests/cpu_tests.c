#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "opcodes.h"

#include "disassembler.h"

// #define DEBUG_ON

CPU cpu;
static bool end = false;
static bool debug = false;

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

void run_test(char const * filename) {
    cpu_init(&cpu);

    cpu_load_rom_at(&cpu, filename, 0x100);
    cpu.pc = 0x100;

    cpu.out_ports[0] = test_end;
    cpu.memory[0] = OUT;
    cpu.memory[1] = 0x00;

    cpu.out_ports[1] = PRINT;
    cpu.memory[0x5] = OUT;
    cpu.memory[0x5 + 1] = 0x01;
    cpu.memory[0x5 + 2] = RET;

    end = false;

    printf("\n===RUN TEST %s===\n", filename);

    while(!end) {
        if(debug) {
            print_cpu_status(&cpu);
            disassemble(&cpu);
            cpu_tick(&cpu);
            getchar();
        } else {
            cpu_tick(&cpu);
        }
    }

    printf("\n");
}

int main(int argc, char **argv) {

    if(argc > 2 || (argc == 2 && (atoi(argv[1]) != 0) && (atoi(argv[1]) != 1))) {
        printf("Usage: ./cpu-tests [0 | 1]\n0 is for no debug and 1 for debug. It is 0 by default.\n");
        return 1;
    }
    
    if(argc == 2 && atoi(argv[1]) == 1)
        debug = true;

    if(debug)
        init_instr_size();
    
    run_test("../roms/tests/TST8080.COM");
    run_test("../roms/tests/CPUTEST.COM");
    run_test("../roms/tests/8080PRE.COM");
    run_test("../roms/tests/8080EXM.COM");

    return 0;
}
