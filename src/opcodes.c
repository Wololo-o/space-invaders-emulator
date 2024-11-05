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
