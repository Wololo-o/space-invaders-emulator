#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

#define MEM_SIZE 0x10000

typedef struct flags {
    uint8_t z:1;
    uint8_t s:1;
    uint8_t p:1;
    uint8_t cy:1;
    uint8_t ac:1;
    uint8_t padding:3;
} flags;

typedef struct CPU {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;

    flags f;
    
    uint16_t sp;
    uint16_t pc;

    uint8_t memory[MEM_SIZE];

    bool exit;
} CPU;

void cpu_init(CPU *cpu);

bool cpu_load_rom_at(CPU *cpu, char const *filename, uint16_t start);

void cpu_tick(CPU *cpu);

void clear_flags(CPU *cpu);

uint8_t read_byte(CPU *cpu, uint16_t address);

void write_byte(CPU *cpu, uint16_t address, uint8_t value);

uint16_t read_word(CPU *cpu, uint16_t address);

void write_word(CPU *cpu, uint16_t address, uint16_t value);

uint8_t next_byte(CPU *cpu);

uint16_t next_word(CPU *cpu);



#endif // CPU_H