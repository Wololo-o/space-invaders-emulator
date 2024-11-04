#include "opcodes.h"

void cpu_xchg(CPU *cpu) {
    uint16_t tmp;
    tmp = get_hl(cpu);
    set_hl(cpu, get_de(cpu));
    set_de(cpu, tmp);
}

void cpu_add(CPU *cpu, uint8_t value) {
    update_ac_flag_add(cpu, cpu->a, value, false);
    update_cy_flag_add(cpu, cpu->a, value, false);
    cpu->a += value;
    update_zsp_flags(cpu, cpu->a);
}