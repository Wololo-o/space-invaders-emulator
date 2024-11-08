#include "opcodes.h"

void cpu_xchg(CPU *cpu) {
    uint16_t tmp;
    tmp = get_hl(cpu);
    set_hl(cpu, get_de(cpu));
    set_de(cpu, tmp);
}

void cpu_add(CPU *cpu, uint8_t value, bool is_carry) {
    update_ac_flag_add(cpu, cpu->a, value, is_carry);
    update_cy_flag_add(cpu, cpu->a, value, is_carry);
    cpu->a += value + (is_carry ? 1 : 0);
    update_zsp_flags(cpu, cpu->a);
}

void cpu_sub(CPU *cpu, uint8_t value, bool is_borrow) {
    update_ac_flag_sub(cpu, cpu->a, value, is_borrow);
    update_cy_flag_sub(cpu, cpu->a, value, is_borrow);
    cpu->a -= value + (is_borrow ? 1 : 0);
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
    if((cpu->a & 0x0f) > 9 || cpu->f.ac) to_add += 0x06;
    if((cpu->a >> 4) > 9 || cpu->f.cy) to_add += 0x60;
    update_ac_flag_add(cpu, cpu->a, to_add, false);
    update_cy_flag_add(cpu, cpu->a, to_add, false);
    cpu->a += to_add;
    update_zsp_flags(cpu, cpu->a);
}

void cpu_ana(CPU *cpu, uint8_t * const rm) {
    cpu->a &= *rm;
    update_zsp_flags(cpu, cpu->a);
    cpu->f.cy = 0;
    cpu->f.ac = 0;
}

void cpu_ani(CPU *cpu) {
    cpu->a &= next_byte(cpu);
    update_zsp_flags(cpu, cpu->a);
    cpu->f.cy = 0;
    cpu->f.ac = 0;
}

void cpu_xra(CPU *cpu, uint8_t * const rm) {
    cpu->a ^= *rm;
    update_zsp_flags(cpu, cpu->a);
    cpu->f.cy = 0;
    cpu->f.ac = 0;
}

void cpu_xri(CPU *cpu) {
    cpu->a ^= next_byte(cpu);
    update_zsp_flags(cpu, cpu->a);
    cpu->f.cy = 0;
    cpu->f.ac = 0;
}

void cpu_ora(CPU *cpu, uint8_t * const rm) {
    cpu->a |= *rm;
    update_zsp_flags(cpu, cpu->a);
    cpu->f.cy = 0;
    cpu->f.ac = 0;
}

void cpu_ori(CPU *cpu) {
    cpu->a |= next_byte(cpu);
    update_zsp_flags(cpu, cpu->a);
    cpu->f.cy = 0;
    cpu->f.ac = 0;
}

void cpu_cmp(CPU *cpu, uint8_t * const rm) {
    update_flags_cmp(cpu, *rm);
}

void cpu_cpi(CPU *cpu) {
    update_flags_cmp(cpu, next_byte(cpu));
}

void cpu_rlc(CPU *cpu) {
    cpu->f.cy = (cpu->a >> 7);
    cpu->a <<= 1;
    cpu->a |= cpu->f.cy;
}

void cpu_rrc(CPU *cpu) {
    cpu->f.cy = (cpu->a & 1);
    cpu->a >>= 1;
    cpu->a |= (cpu->f.cy << 7);
}

void cpu_ral(CPU *cpu) {
    uint8_t high_order_bit = cpu->a >> 7;
    cpu->a <<= 1;
    cpu->a |= cpu->f.cy;
    cpu->f.cy = high_order_bit;
}

void cpu_rar(CPU *cpu) {
    uint8_t low_order_bit = cpu->a & 1;
    cpu->a >>= 1;
    cpu->a |= (cpu->f.cy << 7);
    cpu->f.cy = low_order_bit;
}

void cpu_call(CPU *cpu, bool condition) {
    if(condition) {
        push(cpu, cpu->pc);
        cpu->pc = next_word(cpu);
    }
}

void cpu_push_psw(CPU *cpu) {
    uint16_t to_push = 0;
    to_push |= (cpu->a << 8);
    to_push |= cpu->f.cy;
    to_push |= (cpu->f.p << 2);
    to_push |= (cpu->f.ac << 4);
    to_push |= (cpu->f.z << 6);
    to_push |= (cpu->f.s << 7);
    to_push |= 2;
    push(cpu, to_push);
}
