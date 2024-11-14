#include "opcodes.h"

void cpu_xchg(CPU *cpu) {
    uint16_t tmp;
    tmp = get_hl(cpu);
    set_hl(cpu, get_de(cpu));
    set_de(cpu, tmp);
}

void cpu_add(CPU *cpu, uint8_t value, bool is_carry) {
    bool carry_val = is_carry & get_flag(cpu, CY);
    update_ac_flag_add(cpu, cpu->a, value, is_carry);
    update_cy_flag_add(cpu, cpu->a, value, is_carry);
    cpu->a += value + (carry_val ? 1 : 0);
    update_zsp_flags(cpu, cpu->a);
}

void cpu_sub(CPU *cpu, uint8_t value, bool is_borrow) {
    bool borrow_val = is_borrow & get_flag(cpu, CY);
    update_ac_flag_sub(cpu, cpu->a, value, is_borrow);
    update_cy_flag_sub(cpu, cpu->a, value, is_borrow);
    cpu->a -= value + (borrow_val ? 1 : 0);
    update_zsp_flags(cpu, cpu->a);
}

void cpu_inr(CPU *cpu, uint8_t * const rm) {
    update_ac_flag_add(cpu, *rm, 1, false);
    ++(*rm);
    update_zsp_flags(cpu, *rm);
}

void cpu_dcr(CPU *cpu, uint8_t * const rm) {
    update_ac_flag_sub(cpu, *rm, 1, false);
    --(*rm);
    update_zsp_flags(cpu, *rm);
}

void cpu_daa(CPU *cpu) {
    uint8_t to_add = 0;
    if((cpu->a & 0x0f) > 9 || get_flag(cpu, AC)) to_add += 0x06;
    if(((cpu->a + to_add) >> 4) > 9 || get_flag(cpu, CY)) to_add += 0x60;
    update_ac_flag_add(cpu, cpu->a, to_add, false);
    update_cy_flag_add(cpu, cpu->a, to_add, false);
    cpu->a += to_add;
    update_zsp_flags(cpu, cpu->a);
}

void cpu_ana(CPU *cpu, uint8_t * const rm) {
    cpu->a &= *rm;
    update_zsp_flags(cpu, cpu->a);
    set_flag(cpu, CY, false);
    set_flag(cpu, AC, false);
}

void cpu_ani(CPU *cpu) {
    cpu->a &= next_byte(cpu);
    update_zsp_flags(cpu, cpu->a);
    set_flag(cpu, CY, false);
    set_flag(cpu, AC, false);
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

void cpu_call(CPU *cpu, bool condition) {
    if(condition) {
        push(cpu, cpu->pc + 2);
        cpu->pc = next_word(cpu);
    } else {
        cpu->pc += 2;
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
    set_flag(cpu, CY, psw & 0x01);
    set_flag(cpu, P, (psw & 0x04) >> 2);
    set_flag(cpu, AC, (psw & 0x10) >> 4);
    set_flag(cpu, Z, (psw & 0x40) >> 6);
    set_flag(cpu, S, (psw & 0x80) >> 7);
}

void cpu_xthl(CPU *cpu) {
    uint16_t tmp = read_word(cpu, cpu->sp);
    write_word(cpu, cpu->sp, get_hl(cpu));
    set_hl(cpu, tmp);
}
