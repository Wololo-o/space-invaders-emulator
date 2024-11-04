#include <stdio.h>

#include "cpu.h"
#include "opcodes.h"

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
    case NOP:
    case NOP_ALT_1:
    case NOP_ALT_2:
    case NOP_ALT_3:
    case NOP_ALT_4:
    case NOP_ALT_5:
    case NOP_ALT_6:
    case NOP_ALT_7:
        break;

    case MOV_B_B: cpu->b = cpu->b;
    case MOV_B_C: cpu->b = cpu->c;
    case MOV_B_D: cpu->b = cpu->d;
    case MOV_B_E: cpu->b = cpu->e;
    case MOV_B_H: cpu->b = cpu->h;
    case MOV_B_L: cpu->b = cpu->l;
    case MOV_B_M: cpu->b = cpu->memory[get_hl(cpu)];
    case MOV_B_A: cpu->b = cpu->a;

    case MOV_C_B: cpu->c = cpu->b;
    case MOV_C_C: cpu->c = cpu->c;
    case MOV_C_D: cpu->c = cpu->d;
    case MOV_C_E: cpu->c = cpu->e;
    case MOV_C_H: cpu->c = cpu->h;
    case MOV_C_L: cpu->c = cpu->l;
    case MOV_C_M: cpu->c = cpu->memory[get_hl(cpu)];
    case MOV_C_A: cpu->c = cpu->a;

    case MOV_D_B: cpu->d = cpu->b;
    case MOV_D_C: cpu->d = cpu->c;
    case MOV_D_D: cpu->d = cpu->d;
    case MOV_D_E: cpu->d = cpu->e;
    case MOV_D_H: cpu->d = cpu->h;
    case MOV_D_L: cpu->d = cpu->l;
    case MOV_D_M: cpu->d = cpu->memory[get_hl(cpu)];
    case MOV_D_A: cpu->d = cpu->a;

    case MOV_E_B: cpu->e = cpu->b;
    case MOV_E_C: cpu->e = cpu->c;
    case MOV_E_D: cpu->e = cpu->d;
    case MOV_E_E: cpu->e = cpu->e;
    case MOV_E_H: cpu->e = cpu->h;
    case MOV_E_L: cpu->e = cpu->l;
    case MOV_E_M: cpu->e = cpu->memory[get_hl(cpu)];
    case MOV_E_A: cpu->e = cpu->a;

    case MOV_H_B: cpu->h = cpu->b;
    case MOV_H_C: cpu->h = cpu->c;
    case MOV_H_D: cpu->h = cpu->d;
    case MOV_H_E: cpu->h = cpu->e;
    case MOV_H_H: cpu->h = cpu->h;
    case MOV_H_L: cpu->h = cpu->l;
    case MOV_H_M: cpu->h = cpu->memory[get_hl(cpu)];
    case MOV_H_A: cpu->h = cpu->a;

    case MOV_L_B: cpu->l = cpu->b;
    case MOV_L_C: cpu->l = cpu->c;
    case MOV_L_D: cpu->l = cpu->d;
    case MOV_L_E: cpu->l = cpu->e;
    case MOV_L_H: cpu->l = cpu->h;
    case MOV_L_L: cpu->l = cpu->l;
    case MOV_L_M: cpu->l = cpu->memory[get_hl(cpu)];
    case MOV_L_A: cpu->l = cpu->a;

    case MOV_M_B: cpu->memory[get_hl(cpu)] = cpu->b;
    case MOV_M_C: cpu->memory[get_hl(cpu)] = cpu->b;
    case MOV_M_D: cpu->memory[get_hl(cpu)] = cpu->b;
    case MOV_M_E: cpu->memory[get_hl(cpu)] = cpu->b;
    case MOV_M_H: cpu->memory[get_hl(cpu)] = cpu->b;
    case MOV_M_L: cpu->memory[get_hl(cpu)] = cpu->b;
    case MOV_M_A: cpu->memory[get_hl(cpu)] = cpu->b;

    case MOV_A_B: cpu->a = cpu->b;
    case MOV_A_C: cpu->a = cpu->c;
    case MOV_A_D: cpu->a = cpu->d;
    case MOV_A_E: cpu->a = cpu->e;
    case MOV_A_H: cpu->a = cpu->h;
    case MOV_A_L: cpu->a = cpu->l;
    case MOV_A_M: cpu->a = cpu->memory[get_hl(cpu)];
    case MOV_A_A: cpu->a = cpu->a;

    case MVI_B: cpu->b = next_byte(cpu);
    case MVI_C: cpu->c = next_byte(cpu);
    case MVI_D: cpu->d = next_byte(cpu);
    case MVI_E: cpu->e = next_byte(cpu);
    case MVI_H: cpu->h = next_byte(cpu);
    case MVI_L: cpu->l = next_byte(cpu);
    case MVI_M: cpu->memory[get_hl(cpu)] = next_byte(cpu);
    case MVI_A: cpu->a = next_byte(cpu);

    case LXI_B: set_bc(cpu, next_word(cpu));
    case LXI_D: set_de(cpu, next_word(cpu));
    case LXI_H: set_hl(cpu, next_word(cpu));
    case LXI_SP: cpu->sp = next_word(cpu);

    case LDA: cpu->a = read_byte(cpu, next_word(cpu));
    case STA: write_byte(cpu, next_word(cpu), cpu->a);

    case SHLD: write_word(cpu, next_word(cpu), get_hl(cpu));
    case LHLD: set_hl(cpu, read_word(cpu, next_word(cpu)));

    case LDAX_B: cpu->a = read_word(cpu, get_bc(cpu));
    case LDAX_D: cpu->a = read_word(cpu, get_de(cpu));
    case STAX_B: write_word(cpu, get_bc(cpu), cpu->a);
    case STAX_D: write_word(cpu, get_bc(cpu), cpu->a);

    case XCHG: cpu_xchg(cpu);

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

void cpu_xchg(CPU *cpu) {
    uint16_t tmp;
    tmp = get_hl(cpu);
    set_hl(cpu, get_de(cpu));
    set_de(cpu, tmp);
}
