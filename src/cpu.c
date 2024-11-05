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

    // Data transfer group
    case MOV_B_B: cpu->b = cpu->b; break;
    case MOV_B_C: cpu->b = cpu->c; break;
    case MOV_B_D: cpu->b = cpu->d; break;
    case MOV_B_E: cpu->b = cpu->e; break;
    case MOV_B_H: cpu->b = cpu->h; break;
    case MOV_B_L: cpu->b = cpu->l; break;
    case MOV_B_M: cpu->b = cpu->memory[get_hl(cpu)]; break;
    case MOV_B_A: cpu->b = cpu->a; break;

    case MOV_C_B: cpu->c = cpu->b; break;
    case MOV_C_C: cpu->c = cpu->c; break;
    case MOV_C_D: cpu->c = cpu->d; break;
    case MOV_C_E: cpu->c = cpu->e; break;
    case MOV_C_H: cpu->c = cpu->h; break;
    case MOV_C_L: cpu->c = cpu->l; break;
    case MOV_C_M: cpu->c = cpu->memory[get_hl(cpu)]; break;
    case MOV_C_A: cpu->c = cpu->a; break;

    case MOV_D_B: cpu->d = cpu->b; break;
    case MOV_D_C: cpu->d = cpu->c; break;
    case MOV_D_D: cpu->d = cpu->d; break;
    case MOV_D_E: cpu->d = cpu->e; break;
    case MOV_D_H: cpu->d = cpu->h; break;
    case MOV_D_L: cpu->d = cpu->l; break;
    case MOV_D_M: cpu->d = cpu->memory[get_hl(cpu)]; break;
    case MOV_D_A: cpu->d = cpu->a; break;

    case MOV_E_B: cpu->e = cpu->b; break;
    case MOV_E_C: cpu->e = cpu->c; break;
    case MOV_E_D: cpu->e = cpu->d; break;
    case MOV_E_E: cpu->e = cpu->e; break;
    case MOV_E_H: cpu->e = cpu->h; break;
    case MOV_E_L: cpu->e = cpu->l; break;
    case MOV_E_M: cpu->e = cpu->memory[get_hl(cpu)]; break;
    case MOV_E_A: cpu->e = cpu->a; break;

    case MOV_H_B: cpu->h = cpu->b; break;
    case MOV_H_C: cpu->h = cpu->c; break;
    case MOV_H_D: cpu->h = cpu->d; break;
    case MOV_H_E: cpu->h = cpu->e; break;
    case MOV_H_H: cpu->h = cpu->h; break;
    case MOV_H_L: cpu->h = cpu->l; break;
    case MOV_H_M: cpu->h = cpu->memory[get_hl(cpu)]; break;
    case MOV_H_A: cpu->h = cpu->a; break;

    case MOV_L_B: cpu->l = cpu->b; break;
    case MOV_L_C: cpu->l = cpu->c; break;
    case MOV_L_D: cpu->l = cpu->d; break;
    case MOV_L_E: cpu->l = cpu->e; break;
    case MOV_L_H: cpu->l = cpu->h; break;
    case MOV_L_L: cpu->l = cpu->l; break;
    case MOV_L_M: cpu->l = cpu->memory[get_hl(cpu)]; break;
    case MOV_L_A: cpu->l = cpu->a; break;

    case MOV_M_B: cpu->memory[get_hl(cpu)] = cpu->b; break;
    case MOV_M_C: cpu->memory[get_hl(cpu)] = cpu->b; break;
    case MOV_M_D: cpu->memory[get_hl(cpu)] = cpu->b; break;
    case MOV_M_E: cpu->memory[get_hl(cpu)] = cpu->b; break;
    case MOV_M_H: cpu->memory[get_hl(cpu)] = cpu->b; break;
    case MOV_M_L: cpu->memory[get_hl(cpu)] = cpu->b; break;
    case MOV_M_A: cpu->memory[get_hl(cpu)] = cpu->b; break;

    case MOV_A_B: cpu->a = cpu->b; break;
    case MOV_A_C: cpu->a = cpu->c; break;
    case MOV_A_D: cpu->a = cpu->d; break;
    case MOV_A_E: cpu->a = cpu->e; break;
    case MOV_A_H: cpu->a = cpu->h; break;
    case MOV_A_L: cpu->a = cpu->l; break;
    case MOV_A_M: cpu->a = cpu->memory[get_hl(cpu)]; break;
    case MOV_A_A: cpu->a = cpu->a; break;

    case MVI_B: cpu->b = next_byte(cpu); break;
    case MVI_C: cpu->c = next_byte(cpu); break;
    case MVI_D: cpu->d = next_byte(cpu); break;
    case MVI_E: cpu->e = next_byte(cpu); break;
    case MVI_H: cpu->h = next_byte(cpu); break;
    case MVI_L: cpu->l = next_byte(cpu); break;
    case MVI_M: cpu->memory[get_hl(cpu)] = next_byte(cpu); break;
    case MVI_A: cpu->a = next_byte(cpu); break;

    case LXI_B: set_bc(cpu, next_word(cpu)); break;
    case LXI_D: set_de(cpu, next_word(cpu)); break;
    case LXI_H: set_hl(cpu, next_word(cpu)); break;
    case LXI_SP: cpu->sp = next_word(cpu); break;

    case LDA: cpu->a = read_byte(cpu, next_word(cpu)); break;
    case STA: write_byte(cpu, next_word(cpu), cpu->a); break;

    case SHLD: write_word(cpu, next_word(cpu), get_hl(cpu)); break;
    case LHLD: set_hl(cpu, read_word(cpu, next_word(cpu))); break;

    case LDAX_B: cpu->a = read_word(cpu, get_bc(cpu)); break;
    case LDAX_D: cpu->a = read_word(cpu, get_de(cpu)); break;
    case STAX_B: write_word(cpu, get_bc(cpu), cpu->a); break;
    case STAX_D: write_word(cpu, get_bc(cpu), cpu->a); break;

    case XCHG: cpu_xchg(cpu); break;

    // Arithmetic group
    case ADD_B: cpu_add(cpu, cpu->b, false); break;
    case ADD_C: cpu_add(cpu, cpu->c, false); break;
    case ADD_D: cpu_add(cpu, cpu->d, false); break;
    case ADD_E: cpu_add(cpu, cpu->e, false); break;
    case ADD_H: cpu_add(cpu, cpu->h, false); break;
    case ADD_L: cpu_add(cpu, cpu->l, false); break;
    case ADD_M: cpu_add(cpu, read_byte(cpu, get_hl(cpu)), false); break;
    case ADD_A: cpu_add(cpu, cpu->a, false); break;

    case ADI: cpu_add(cpu, next_byte(cpu), false); break;

    case ADC_B: cpu_add(cpu, cpu->b, true); break;
    case ADC_C: cpu_add(cpu, cpu->c, true); break;
    case ADC_D: cpu_add(cpu, cpu->d, true); break;
    case ADC_E: cpu_add(cpu, cpu->e, true); break;
    case ADC_H: cpu_add(cpu, cpu->h, true); break;
    case ADC_L: cpu_add(cpu, cpu->l, true); break;
    case ADC_M: cpu_add(cpu, read_byte(cpu, get_hl(cpu)), true); break;
    case ADC_A: cpu_add(cpu, cpu->a, true); break;

    case ACI: cpu_add(cpu, next_byte(cpu), true); break;
    
    case SUB_B: cpu_sub(cpu, cpu->b, false); break;
    case SUB_C: cpu_sub(cpu, cpu->c, false); break;
    case SUB_D: cpu_sub(cpu, cpu->d, false); break;
    case SUB_E: cpu_sub(cpu, cpu->e, false); break;
    case SUB_H: cpu_sub(cpu, cpu->h, false); break;
    case SUB_L: cpu_sub(cpu, cpu->l, false); break;
    case SUB_M: cpu_sub(cpu, read_byte(cpu, get_hl(cpu)), false); break;
    case SUB_A: cpu_sub(cpu, cpu->a, false); break;

    case SUI: cpu_sub(cpu, next_byte(cpu), false); break;

    case SBB_B: cpu_sub(cpu, cpu->b, true); break;
    case SBB_C: cpu_sub(cpu, cpu->c, true); break;
    case SBB_D: cpu_sub(cpu, cpu->d, true); break;
    case SBB_E: cpu_sub(cpu, cpu->e, true); break;
    case SBB_H: cpu_sub(cpu, cpu->h, true); break;
    case SBB_L: cpu_sub(cpu, cpu->l, true); break;
    case SBB_M: cpu_sub(cpu, read_byte(cpu, get_hl(cpu)), true); break;
    case SBB_A: cpu_sub(cpu, cpu->a, true); break;

    case SBI: cpu_sub(cpu, next_byte(cpu), true); break;

    case INR_B: cpu_inr(cpu, &cpu->b); break;
    case INR_C: cpu_inr(cpu, &cpu->c); break;
    case INR_D: cpu_inr(cpu, &cpu->d); break;
    case INR_E: cpu_inr(cpu, &cpu->e); break;
    case INR_H: cpu_inr(cpu, &cpu->h); break;
    case INR_L: cpu_inr(cpu, &cpu->l); break;
    case INR_M: cpu_inr(cpu, cpu->memory + get_hl(cpu)); break;
    case INR_A: cpu_inr(cpu, &cpu->a); break;

    // Logical group

    // Branch group

    // Stack, I/O and machine control group
    case NOP:
    case NOP_ALT_1:
    case NOP_ALT_2:
    case NOP_ALT_3:
    case NOP_ALT_4:
    case NOP_ALT_5:
    case NOP_ALT_6:
    case NOP_ALT_7:
        break;

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

void update_zsp_flags(CPU *cpu, uint8_t res) {
    uint8_t parity = 0;

    cpu->f.z = !res;
    cpu->f.s = (res >> 7) & 1;

    while(res) {
        parity += res & 1;
        res >>= 1;
    }
    cpu->f.p = !(parity % 2);
}

void update_cy_flag_add(CPU *cpu, uint8_t val1, uint8_t val2, bool is_carry) {
    uint8_t carry = is_carry ? cpu->f.cy : 0;
    uint16_t res = val1 + val2 + carry;
    cpu->f.cy = (res > 0xff);
}

void update_cy_flag_sub(CPU *cpu, uint8_t val1, uint8_t val2, bool is_borrow) {
    uint8_t borrow = is_borrow ? cpu->f.cy : 0;
    uint16_t res = val1 + (~val2 + 1) + (~borrow + 1);
    cpu->f.cy = (res > 0xff);
}

void update_ac_flag_add(CPU *cpu, uint8_t val1, uint8_t val2, bool is_carry) {
    uint8_t carry = is_carry ? cpu->f.cy : 0;
    uint8_t res = (val1 & 0xf) + (val2 & 0xf) + carry;
    cpu->f.ac = ((res & 0x10) >> 4);
}

void update_ac_flag_sub(CPU *cpu, uint8_t val1, uint8_t val2, bool is_borrow) {
    uint8_t borrow = is_borrow ? cpu->f.cy : 0;
    uint8_t res = (val1 & 0x0f) + ((~val2 + 1) & 0x0f) + ((~borrow + 1) & 0x0f);
    cpu->f.ac = ((res & 0x10) >> 4);
}
