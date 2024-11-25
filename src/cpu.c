#include <stdio.h>

#include "cpu.h"
#include "opcodes.h"

const uint8_t operation_cycle[256] = {
    4, 10, 7,  5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5,  7, 4,
    4, 10, 7,  5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5,  7, 4,
    4, 10, 16, 5,  5,  5,  7,  4,  4,  10, 16, 5,  5,  5,  7, 4,
    4, 10, 13, 5,  10, 10, 10, 4,  4,  10, 13, 5,  5,  5,  7, 4,
    5, 5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7, 5,
    5, 5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7, 5,
    5, 5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7, 5,
    7, 7,  7,  7,  7,  7,  7,  7,  5,  5,  5,  5,  5,  5,  7, 5,
    4, 4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7, 4,
    4, 4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7, 4,
    4, 4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7, 4,
    4, 4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7, 4,
    5, 10, 10, 10, 11, 11, 7,  11, 5,  10, 10, 10, 11, 17, 7, 11,
    5, 10, 10, 10, 11, 11, 7,  11, 5,  10, 10, 10, 11, 17, 7, 11,
    5, 10, 10, 18, 11, 11, 7,  11, 5,  5,  10, 4,  11, 17, 7, 11,
    5, 10, 10, 4,  11, 11, 7,  11, 5,  5,  10, 4,  11, 17, 7, 11
};

void cpu_init(CPU *cpu) {
    cpu->a = cpu->b = cpu->c = cpu->d = cpu->e = cpu->h = cpu->l = 0;

    cpu->pc = cpu->sp = 0;

    clear_flags(cpu);

    for(int i = 0; i < MEM_SIZE; ++i) {
        cpu->memory[i] = 0;
    }

    cpu->hlt = cpu->interrupts_enabled = cpu->interrupt_requested = false;
    cpu->enabling_interrputs_timer = 0;
    cpu->cycle_count = 0;
    cpu->interrupt_op = 0;
}

bool cpu_load_rom_at(CPU *cpu, char const *filename, uint16_t start) {
    bool result = true;
    FILE *f = NULL;
    int fsize = 0;
    uint8_t *buffer = NULL;

    f = fopen(filename, "rb");

    if (f == NULL) {
        result = false;
    }

    if(result) {
        fseek(f, 0L, SEEK_END);
        fsize = ftell(f);
        fseek(f, 0L, SEEK_SET);

        buffer = cpu->memory + start;

        if(fread(buffer, fsize, 1, f) != 1) {
            result = false;
        }

        fclose(f);

    }

    return result;
}

void cpu_tick(CPU *cpu) {
    uint8_t opcode;

    if(cpu->interrupt_requested && cpu->interrupts_enabled) {
        opcode = cpu->interrupt_op;
        cpu->interrupts_enabled = false;
        cpu->interrupt_requested = false;
    } else {
        opcode = cpu->memory[cpu->pc++];
    }

    cpu->cycle_count += operation_cycle[opcode];

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
    case MOV_M_C: cpu->memory[get_hl(cpu)] = cpu->c; break;
    case MOV_M_D: cpu->memory[get_hl(cpu)] = cpu->d; break;
    case MOV_M_E: cpu->memory[get_hl(cpu)] = cpu->e; break;
    case MOV_M_H: cpu->memory[get_hl(cpu)] = cpu->h; break;
    case MOV_M_L: cpu->memory[get_hl(cpu)] = cpu->l; break;
    case MOV_M_A: cpu->memory[get_hl(cpu)] = cpu->a; break;

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

    case LDAX_B: cpu->a = read_byte(cpu, get_bc(cpu)); break;
    case LDAX_D: cpu->a = read_byte(cpu, get_de(cpu)); break;
    case STAX_B: write_byte(cpu, get_bc(cpu), cpu->a); break;
    case STAX_D: write_byte(cpu, get_de(cpu), cpu->a); break;

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

    case ADC_B: cpu_add(cpu, cpu->b, get_flag(cpu, CY)); break;
    case ADC_C: cpu_add(cpu, cpu->c, get_flag(cpu, CY)); break;
    case ADC_D: cpu_add(cpu, cpu->d, get_flag(cpu, CY)); break;
    case ADC_E: cpu_add(cpu, cpu->e, get_flag(cpu, CY)); break;
    case ADC_H: cpu_add(cpu, cpu->h, get_flag(cpu, CY)); break;
    case ADC_L: cpu_add(cpu, cpu->l, get_flag(cpu, CY)); break;
    case ADC_M: cpu_add(cpu, read_byte(cpu, get_hl(cpu)), get_flag(cpu, CY)); break;
    case ADC_A: cpu_add(cpu, cpu->a, get_flag(cpu, CY)); break;

    case ACI: cpu_add(cpu, next_byte(cpu), get_flag(cpu, CY)); break;
    
    case SUB_B: cpu_sub(cpu, cpu->b, false); break;
    case SUB_C: cpu_sub(cpu, cpu->c, false); break;
    case SUB_D: cpu_sub(cpu, cpu->d, false); break;
    case SUB_E: cpu_sub(cpu, cpu->e, false); break;
    case SUB_H: cpu_sub(cpu, cpu->h, false); break;
    case SUB_L: cpu_sub(cpu, cpu->l, false); break;
    case SUB_M: cpu_sub(cpu, read_byte(cpu, get_hl(cpu)), false); break;
    case SUB_A: cpu_sub(cpu, cpu->a, false); break;

    case SUI: cpu_sub(cpu, next_byte(cpu), false); break;

    case SBB_B: cpu_sub(cpu, cpu->b, get_flag(cpu, CY)); break;
    case SBB_C: cpu_sub(cpu, cpu->c, get_flag(cpu, CY)); break;
    case SBB_D: cpu_sub(cpu, cpu->d, get_flag(cpu, CY)); break;
    case SBB_E: cpu_sub(cpu, cpu->e, get_flag(cpu, CY)); break;
    case SBB_H: cpu_sub(cpu, cpu->h, get_flag(cpu, CY)); break;
    case SBB_L: cpu_sub(cpu, cpu->l, get_flag(cpu, CY)); break;
    case SBB_M: cpu_sub(cpu, read_byte(cpu, get_hl(cpu)), get_flag(cpu, CY)); break;
    case SBB_A: cpu_sub(cpu, cpu->a, get_flag(cpu, CY)); break;

    case SBI: cpu_sub(cpu, next_byte(cpu), get_flag(cpu, CY)); break;

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

    case DAD_B: set_flag(cpu, CY, ((uint32_t)(get_hl(cpu) + get_bc(cpu)) > 0xffff)); set_hl(cpu, get_hl(cpu) + get_bc(cpu)); break;
    case DAD_D: set_flag(cpu, CY, ((uint32_t)(get_hl(cpu) + get_de(cpu)) > 0xffff)); set_hl(cpu, get_hl(cpu) + get_de(cpu)); break;
    case DAD_H: set_flag(cpu, CY, (get_hl(cpu) >= 0x8000)) ; set_hl(cpu, get_hl(cpu) + get_hl(cpu)); break;
    case DAD_SP: set_flag(cpu, CY, ((uint32_t)get_hl(cpu) + cpu->sp > 0xffff)) ; set_hl(cpu, get_hl(cpu) + cpu->sp); break;

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

    case CMC: set_flag(cpu, CY, !get_flag(cpu, CY)); break;
    case STC: set_flag(cpu, CY, true); break;

    // Branch group
    case JMP:
    case JMP_ALT:
        cpu->pc = next_word(cpu); break;
    
    case JNZ: cpu_jmp(cpu, !get_flag(cpu, Z)); break;
    case JZ: cpu_jmp(cpu, get_flag(cpu, Z)); break;
    case JNC: cpu_jmp(cpu, !get_flag(cpu, CY)); break;
    case JC: cpu_jmp(cpu, get_flag(cpu, CY)); break;
    case JPO: cpu_jmp(cpu, !get_flag(cpu, P)); break;
    case JPE: cpu_jmp(cpu, get_flag(cpu, P)); break;
    case JP: cpu_jmp(cpu, !get_flag(cpu, S)); break;
    case JM: cpu_jmp(cpu, get_flag(cpu, S)); break;

    case CALL:
    case CALL_ALT_1:
    case CALL_ALT_2:
    case CALL_ALT_3:
        cpu_call(cpu);
        break;

    case CNZ: cpu_call_condition(cpu, !get_flag(cpu, Z)); break;
    case CZ: cpu_call_condition(cpu, get_flag(cpu, Z)); break;
    case CNC: cpu_call_condition(cpu, !get_flag(cpu, CY)); break;
    case CC: cpu_call_condition(cpu, get_flag(cpu, CY)); break;
    case CPO: cpu_call_condition(cpu, !get_flag(cpu, P)); break;
    case CPE: cpu_call_condition(cpu, get_flag(cpu, P)); break;
    case CP: cpu_call_condition(cpu, !get_flag(cpu, S)); break;
    case CM: cpu_call_condition(cpu, get_flag(cpu, S)); break;

    case RET:
    case RET_ALT:
        cpu->pc = pop(cpu);
        break;

    case RNZ: cpu_ret_condition(cpu, !get_flag(cpu, Z)); break;
    case RZ: cpu_ret_condition(cpu, get_flag(cpu, Z)); break;
    case RNC: cpu_ret_condition(cpu, !get_flag(cpu, CY)); break;
    case RC: cpu_ret_condition(cpu, get_flag(cpu, CY)); break;
    case RPO: cpu_ret_condition(cpu, !get_flag(cpu, P)); break;
    case RPE: cpu_ret_condition(cpu, get_flag(cpu, P)); break;
    case RP: cpu_ret_condition(cpu, !get_flag(cpu, S)); break;
    case RM: cpu_ret_condition(cpu, get_flag(cpu, S)); break;

    case RST_0:
    case RST_1:
    case RST_2:
    case RST_3:
    case RST_4:
    case RST_5:
    case RST_6:
    case RST_7:
        push(cpu, cpu->pc);
        cpu->pc = (opcode & 0x38);
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

    case IN: cpu->a = cpu->in_ports[next_byte(cpu)](); break;
    case OUT: cpu->out_ports[next_byte(cpu)](cpu->a); break;

    case EI: cpu->enabling_interrputs_timer = 2; break;
    case DI: cpu->interrupts_enabled = false; break;

    case HLT: cpu->hlt = true;

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

    if(cpu->enabling_interrputs_timer > 0) cpu->interrupts_enabled = !(--cpu->enabling_interrputs_timer);
}

void clear_flags(CPU *cpu) {
    cpu->f = 0x02;
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

void interrupt(CPU *cpu, uint8_t opcode) {
    cpu->interrupt_requested = true;
    cpu->interrupt_op = opcode;
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

bool get_flag(CPU const * const cpu, flag_mask flag) {
    return cpu->f & flag;
}

void set_flag(CPU *cpu, flag_mask flag, bool set) {
    if(set)
        cpu->f |= flag;
    else
        cpu->f &= ~flag;
}

void update_zsp_flags(CPU *cpu, uint8_t res) {
    set_flag(cpu, Z, !res);
    set_flag(cpu, S, (res >> 7) & 1);
    set_flag(cpu, P, parity(res));
}

void update_cy_flag(CPU *cpu, uint8_t val1, uint8_t val2, bool carry) {
    uint16_t res = val1 + val2 + carry;
    set_flag(cpu, CY, (res > 0xff));
}

void update_ac_flag(CPU *cpu, uint8_t val1, uint8_t val2, bool carry) {
    uint8_t res = (val1 & 0xf) + (val2 & 0xf) + carry;
    set_flag(cpu, AC, (res & 0x10));
}

void update_flags_cmp(CPU *cpu, uint8_t val) {
    update_zsp_flags(cpu, cpu->a - val);
    update_ac_flag(cpu, cpu->a, ~val, true);
    update_cy_flag(cpu, cpu->a, ~val, true);
    set_flag(cpu, CY, !get_flag(cpu, CY));
}

bool parity(uint8_t val) {
    uint8_t parity = 0;
    while(val) {
        parity += val & 1;
        val >>= 1;
    }
    return !(parity % 2);
}
