#include <stdio.h>

#include "cpu.h"


void cpu_init(CPU *cpu) {
    cpu->exit = false;

    cpu->a = cpu->b = cpu->c = cpu->d = cpu->e = cpu->h = cpu->l = 0;

    cpu->pc = cpu->sp = 0;

    clear_flags(cpu);

    for(int i = 0; i <= MEM_SIZE; ++i) {
        cpu->memory[i] = 0;
    }
}

bool cpu_load_rom_at(CPU *cpu, char const *filename, uint16_t start) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        return false;
    }

    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    uint8_t *buffer = cpu->memory + start;
    fread(buffer, fsize, 1, f);
    fclose(f);

    return true;
}

void cpu_tick(CPU *cpu) {
    uint8_t opcode = cpu->memory[cpu->pc++];

    switch(opcode) {
    default:
        break;
    }
}

void clear_flags(CPU *cpu) {
    cpu->f.ac = cpu->f.cy = cpu->f.p = cpu->f.s = cpu->f.z = 0;
}

uint8_t next_byte(CPU *cpu) {
    return read_byte(cpu, cpu->pc++);
}

uint16_t next_word(CPU *cpu) {
    cpu->pc += 2;
    return read_word(cpu, cpu->pc - 2);
}

uint8_t read_byte(CPU *cpu, uint16_t address) {
    return cpu->memory[address];
}

void write_byte(CPU *cpu, uint16_t address, uint8_t value) {
    cpu->memory[address] = value;
}

uint16_t read_word(CPU *cpu, uint16_t address) {
    return cpu->memory[address + 1] << 8 | cpu->memory[address];
}

void write_word(CPU *cpu, uint16_t address, uint16_t value) {
    cpu->memory[address] = value & 0xff;
    cpu->memory[address + 1] = value >> 8;
}

uint16_t get_bc(CPU *cpu) {
    return ((cpu->b << 8) | cpu->c);
}

uint16_t get_de(CPU *cpu) {
    return ((cpu->d << 8) | cpu->e);
}

uint16_t get_hl(CPU *cpu) {
    return ((cpu->h << 8) | cpu->l);
}

void set_bc(CPU *cpu, uint16_t value) {
    cpu->b = value >> 8;
    cpu->c = value & 0xff;
}

void set_de(CPU *cpu, uint16_t value) {
    cpu->d = value >> 8;
    cpu->e = value & 0xff;
}

void set_hl(CPU *cpu, uint16_t value) {
    cpu->h = value >> 8;
    cpu->l = value & 0xff;
}
