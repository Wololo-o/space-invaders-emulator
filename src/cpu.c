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

    case DCR_B: cpu_dcr(cpu, &cpu->b); break;
    case DCR_C: cpu_dcr(cpu, &cpu->c); break;
    case DCR_D: cpu_dcr(cpu, &cpu->d); break;
    case DCR_E: cpu_dcr(cpu, &cpu->e); break;
    case DCR_H: cpu_dcr(cpu, &cpu->h); break;
    case DCR_L: cpu_dcr(cpu, &cpu->l); break;
    case DCR_M: cpu_dcr(cpu, cpu->memory + get_hl(cpu)); break;
    case DCR_A: cpu_dcr(cpu, &cpu->a); break;

    case INX_B: set_bc(cpu, get_bc(cpu) + 1); break;
    case INX_D: set_de(cpu, get_de(cpu) + 1); break;
    case INX_H: set_hl(cpu, get_hl(cpu) + 1); break;
    case INX_SP: ++(cpu->sp); break;

    case DCX_B: set_bc(cpu, get_bc(cpu) - 1); break;
    case DCX_D: set_de(cpu, get_de(cpu) - 1); break;
    case DCX_H: set_hl(cpu, get_hl(cpu) - 1); break;
    case DCX_SP: --(cpu->sp); break;

    case DAD_B: cpu->f.cy = ((uint32_t)(get_hl(cpu) + get_bc(cpu)) > 0xffff); set_hl(cpu, get_hl(cpu) + get_bc(cpu)); break;
    case DAD_D: cpu->f.cy = ((uint32_t)(get_hl(cpu) + get_de(cpu)) > 0xffff); set_hl(cpu, get_hl(cpu) + get_de(cpu)); break;
    case DAD_H: cpu->f.cy = (get_hl(cpu) >= 0x8000); set_hl(cpu, get_hl(cpu) + get_hl(cpu)); break;
    case DAD_SP: cpu->f.cy = ((uint32_t)get_hl(cpu) + cpu->sp > 0xffff); set_hl(cpu, get_hl(cpu) + cpu->sp); break;

    case DAA: cpu_daa(cpu); break;

    // Logical group
    case ANA_B: cpu_ana(cpu, &cpu->b); break;
    case ANA_C: cpu_ana(cpu, &cpu->c); break;
    case ANA_D: cpu_ana(cpu, &cpu->d); break;
    case ANA_E: cpu_ana(cpu, &cpu->e); break;
    case ANA_H: cpu_ana(cpu, &cpu->h); break;
    case ANA_L: cpu_ana(cpu, &cpu->l); break;
    case ANA_M: cpu_ana(cpu, cpu->memory + get_hl(cpu)); break;
    case ANA_A: cpu_ana(cpu, &cpu->a); break;

    case ANI: cpu_ani(cpu); break;
    
    case XRA_B: cpu_xra(cpu, &cpu->b); break;
    case XRA_C: cpu_xra(cpu, &cpu->c); break;
    case XRA_D: cpu_xra(cpu, &cpu->d); break;
    case XRA_E: cpu_xra(cpu, &cpu->e); break;
    case XRA_H: cpu_xra(cpu, &cpu->h); break;
    case XRA_L: cpu_xra(cpu, &cpu->l); break;
    case XRA_M: cpu_xra(cpu, cpu->memory + get_hl(cpu)); break;
    case XRA_A: cpu_xra(cpu, &cpu->a); break;

    case XRI: cpu_xri(cpu); break;
    
    case ORA_B: cpu_ora(cpu, &cpu->b); break;
    case ORA_C: cpu_ora(cpu, &cpu->c); break;
    case ORA_D: cpu_ora(cpu, &cpu->d); break;
    case ORA_E: cpu_ora(cpu, &cpu->e); break;
    case ORA_H: cpu_ora(cpu, &cpu->h); break;
    case ORA_L: cpu_ora(cpu, &cpu->l); break;
    case ORA_M: cpu_ora(cpu, cpu->memory + get_hl(cpu)); break;
    case ORA_A: cpu_ora(cpu, &cpu->a); break;

    case ORI: cpu_ori(cpu); break;

    case CMP_B: cpu_cmp(cpu, &cpu->b); break;
    case CMP_C: cpu_cmp(cpu, &cpu->c); break;
    case CMP_D: cpu_cmp(cpu, &cpu->d); break;
    case CMP_E: cpu_cmp(cpu, &cpu->e); break;
    case CMP_H: cpu_cmp(cpu, &cpu->h); break;
    case CMP_L: cpu_cmp(cpu, &cpu->l); break;
    case CMP_M: cpu_cmp(cpu, cpu->memory + get_hl(cpu)); break;
    case CMP_A: cpu_cmp(cpu, &cpu->a); break;

    case CPI: cpu_cpi(cpu); break;

    case RLC: cpu_rlc(cpu); break;
    case RRC: cpu_rrc(cpu); break;
    case RAL: cpu_ral(cpu); break;
    case RAR: cpu_rar(cpu); break;

    case CMA: cpu->a = ~cpu->a; break;
    case CMC: cpu->f.cy = !cpu->f.cy; break;

    case STC: cpu->f.cy = 1; break;

    // Branch group
    case JMP:
    case JMP_ALT:
        cpu->pc = next_word(cpu); break;
    
    case JNZ: if(!cpu->f.z) cpu->pc = next_word(cpu); break;
    case JZ: if(cpu->f.z) cpu->pc = next_word(cpu); break;
    case JNC: if(!cpu->f.cy) cpu->pc = next_word(cpu); break;
    case JC: if(cpu->f.cy) cpu->pc = next_word(cpu); break;
    case JPO: if(!cpu->f.p) cpu->pc = next_word(cpu); break;
    case JPE: if(cpu->f.p) cpu->pc = next_word(cpu); break;
    case JP: if(!cpu->f.s) cpu->pc = next_word(cpu); break;
    case JM: if(cpu->f.s) cpu->pc = next_word(cpu); break;

    case CALL:
    case CALL_ALT_1:
    case CALL_ALT_2:
    case CALL_ALT_3:
        cpu_call(cpu, true);
        break;

    case CNZ: cpu_call(cpu, !cpu->f.z); break;
    case CZ: cpu_call(cpu, cpu->f.z); break;
    case CNC: cpu_call(cpu, !cpu->f.cy); break;
    case CC: cpu_call(cpu, cpu->f.cy); break;
    case CPO: cpu_call(cpu, !cpu->f.p); break;
    case CPE: cpu_call(cpu, cpu->f.p); break;
    case CP: cpu_call(cpu, !cpu->f.s); break;
    case CM: cpu_call(cpu, cpu->f.s); break;

    case RET:
    case RET_ALT:
        cpu->pc = pop(cpu);
        break;

    case RNZ: if(!cpu->f.z) cpu->pc = pop(cpu); break;
    case RZ: if(cpu->f.z) cpu->pc = pop(cpu); break;
    case RNC: if(!cpu->f.cy) cpu->pc = pop(cpu); break;
    case RC: if(cpu->f.cy) cpu->pc = pop(cpu); break;
    case RPO: if(!cpu->f.p) cpu->pc = pop(cpu); break;
    case RPE: if(cpu->f.p) cpu->pc = pop(cpu); break;
    case RP: if(!cpu->f.s) cpu->pc = pop(cpu); break;
    case RM: if(cpu->f.s) cpu->pc = pop(cpu); break;

    case RST_0:
    case RST_1:
    case RST_2:
    case RST_3:
    case RST_4:
    case RST_5:
    case RST_6:
    case RST_7:
        push(cpu, cpu->pc);
        cpu->pc = (opcode >> 3) & 7;
        break;

    case PCHL: cpu->pc = get_hl(cpu); break;

    // Stack, I/O and machine control group
    case PUSH_B: push(cpu, get_bc(cpu)); break;
    case PUSH_D: push(cpu, get_de(cpu)); break;
    case PUSH_H: push(cpu, get_hl(cpu)); break;
    case PUSH_PSW: cpu_push_psw(cpu); break;
    
    case POP_B: set_bc(cpu, pop(cpu)); break;
    case POP_D: set_de(cpu, pop(cpu)); break;
    case POP_H: set_hl(cpu, pop(cpu)); break;
    case POP_PSW: cpu_pop_psw(cpu); break;

    case XTHL: cpu_xthl(cpu); break;

    case SPHL: cpu->sp = get_hl(cpu); break;

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

void push(CPU *cpu, uint16_t value) {
    write_word(cpu, cpu->sp - 2, value);
    cpu->sp -= 2;
}

uint16_t pop(CPU *cpu) {
    cpu->sp += 2;
    return read_word(cpu, cpu->sp -2);
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

    cpu->f.z = !res;
    cpu->f.s = (res >> 7) & 1;
    cpu->f.p = parity(res);
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

void update_flags_cmp(CPU *cpu, uint8_t val) {
    uint8_t res = cpu->a - val;

    if(cpu->a == val) cpu->f.z = 1;
    if(cpu->a < val) cpu->f.cy = 1;
    update_ac_flag_sub(cpu, cpu->a, val, false);
    cpu->f.s = (res >> 7) & 1;
    cpu->f.p = parity(cpu->a - val);
}

uint8_t parity(uint8_t val) {
    uint8_t parity = 0;
    while(val) {
        parity += val & 1;
        val >>= 1;
    }
    return !(parity % 2);
}
