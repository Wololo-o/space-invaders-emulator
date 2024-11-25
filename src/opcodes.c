#include "opcodes.h"

void cpu_xchg(CPU *cpu) {
    uint16_t tmp;
    tmp = get_hl(cpu);
    set_hl(cpu, get_de(cpu));
    set_de(cpu, tmp);
}

void cpu_add(CPU *cpu, uint8_t value, bool cy) {
    update_ac_flag(cpu, cpu->a, value, cy);
    update_cy_flag(cpu, cpu->a, value, cy);
    cpu->a += value + cy;
    update_zsp_flags(cpu, cpu->a);
}

void cpu_sub(CPU *cpu, uint8_t value, bool cy) {
    cpu_add(cpu, (~value), !cy);
    set_flag(cpu, CY, !get_flag(cpu, CY));
}

void cpu_inr(CPU *cpu, uint8_t * const rm) {
    update_ac_flag(cpu, *rm, 1, false);
    ++(*rm);
    update_zsp_flags(cpu, *rm);
}

void cpu_dcr(CPU *cpu, uint8_t * const rm) {
    update_ac_flag(cpu, *rm, ~1, true);
    --(*rm);
    update_zsp_flags(cpu, *rm);
}

void cpu_daa(CPU *cpu) {
    bool cy = get_flag(cpu, CY);
    uint8_t to_add = 0;

    uint8_t low_nibble = cpu->a & 0x0f;
    uint8_t high_nibble = cpu->a >> 4;

    if(low_nibble > 9 || get_flag(cpu, AC)) to_add += 0x06;

    if(high_nibble > 9 || get_flag(cpu, CY) || (high_nibble >= 9 && low_nibble > 9)) {
        to_add += 0x60;
        cy = true;
    }

    cpu_add(cpu, to_add, false);
    set_flag(cpu, CY, cy);
}

void cpu_ana(CPU *cpu, uint8_t * const rm) {
    set_flag(cpu, AC, (cpu->a & 0x08) | (*rm & 0x08));
    cpu->a &= *rm;
    update_zsp_flags(cpu, cpu->a);
    set_flag(cpu, CY, false);
}

void cpu_ani(CPU *cpu) {
    uint8_t operand = next_byte(cpu);
    set_flag(cpu, AC, (cpu->a & 0x08) | (operand & 0x08));
    cpu->a &= operand;
    update_zsp_flags(cpu, cpu->a);
    set_flag(cpu, CY, false);
}

void cpu_xra(CPU *cpu, uint8_t * const rm) {
    cpu->a ^= *rm;
    update_zsp_flags(cpu, cpu->a);
    set_flag(cpu, CY, false);
    set_flag(cpu, AC, false);
}

void cpu_xri(CPU *cpu) {
    cpu->a ^= next_byte(cpu);
    update_zsp_flags(cpu, cpu->a);
    set_flag(cpu, CY, false);
    set_flag(cpu, AC, false);
}

void cpu_ora(CPU *cpu, uint8_t * const rm) {
    cpu->a |= *rm;
    update_zsp_flags(cpu, cpu->a);
    set_flag(cpu, CY, false);
    set_flag(cpu, AC, false);
}

void cpu_ori(CPU *cpu) {
    cpu->a |= next_byte(cpu);
    update_zsp_flags(cpu, cpu->a);
    set_flag(cpu, CY, false);
    set_flag(cpu, AC, false);
}

void cpu_cmp(CPU *cpu, uint8_t * const rm) {
    update_flags_cmp(cpu, *rm);
}

void cpu_cpi(CPU *cpu) {
    update_flags_cmp(cpu, next_byte(cpu));
}

void cpu_rlc(CPU *cpu) {
    set_flag(cpu, CY, (cpu->a & 0x80));
    cpu->a <<= 1;
    cpu->a |= get_flag(cpu, CY);
}

void cpu_rrc(CPU *cpu) {
    set_flag(cpu, CY, cpu->a & 1);
    cpu->a >>= 1;
    cpu->a |= (get_flag(cpu, CY) << 7);
}

void cpu_ral(CPU *cpu) {
    uint8_t high_order_bit = cpu->a >> 7;
    cpu->a <<= 1;
    cpu->a |= get_flag(cpu, CY);
    set_flag(cpu, CY, high_order_bit);
}

void cpu_rar(CPU *cpu) {
    uint8_t low_order_bit = cpu->a & 1;
    cpu->a >>= 1;
    cpu->a |= (get_flag(cpu, CY) << 7);
    set_flag(cpu, CY, low_order_bit);
}

void cpu_jmp(CPU *cpu, bool condition) {
    if(condition)
        cpu->pc = next_word(cpu);
    else
        cpu->pc += 2;
}

void cpu_call(CPU *cpu) {
    push(cpu, cpu->pc + 2);
    cpu->pc = next_word(cpu);
}

void cpu_call_condition(CPU *cpu, bool condition) {
    if(condition) {
        push(cpu, cpu->pc + 2);
        cpu->pc = next_word(cpu);
        cpu->cycle_count += 6;
    } else {
        cpu->pc += 2;
    }
}

void cpu_ret_condition(CPU *cpu, bool condition) {
    if(condition) {
        cpu->pc = pop(cpu);
        cpu->cycle_count += 6;
    }
}

void cpu_push_psw(CPU *cpu) {
    uint16_t to_push = 0;
    to_push |= (cpu->a << 8);
    to_push |= get_flag(cpu, CY);
    to_push |= (get_flag(cpu, P) << 2);
    to_push |= (get_flag(cpu, AC) << 4);
    to_push |= (get_flag(cpu, Z) << 6);
    to_push |= (get_flag(cpu, S) << 7);
    to_push |= 2;
    push(cpu, to_push);
}

void cpu_pop_psw(CPU *cpu) {
    uint16_t psw = pop(cpu);
    cpu->a = psw >> 8;
    set_flag(cpu, CY, psw & CY);
    set_flag(cpu, P, (psw & P) >> 2);
    set_flag(cpu, AC, (psw & AC) >> 4);
    set_flag(cpu, Z, (psw & Z) >> 6);
    set_flag(cpu, S, (psw & S) >> 7);
}

void cpu_xthl(CPU *cpu) {
    uint16_t tmp = read_word(cpu, cpu->sp);
    write_word(cpu, cpu->sp, get_hl(cpu));
    set_hl(cpu, tmp);
}
