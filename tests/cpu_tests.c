#include <assert.h>

#include "cpu.h"
#include "opcodes.h"


int main(int argc, char const *argv[]) {

    CPU cpu;
    cpu_init(&cpu);

    // add - z flag
    cpu.a = -1;
    cpu_add(&cpu, 1, false);
    assert(cpu.f.z);

    // add - s flag
    cpu.a = -2;
    cpu_add(&cpu, 1, false);
    assert(cpu.f.s);

    // add - p flag
    cpu.a = 0;
    cpu_add(&cpu, 3, false);
    assert(cpu.f.p);

    // add - cy flag
    cpu.a = 0xff;
    cpu_add(&cpu, 1, false);
    assert(cpu.f.cy);

    // add - ac flag
    cpu.a = 0x0f;
    cpu_add(&cpu, 1, false);
    assert(cpu.f.ac);

    // adc - z flag
    cpu.a = -1;
    cpu.f.cy = 1;
    cpu_add(&cpu, 0, true);
    assert(cpu.f.z);

    // adc - s flag
    cpu.a = -2;
    cpu.f.cy = 1;
    cpu_add(&cpu, 0, true);
    assert(cpu.f.s);

    // adc - p flag
    cpu.a = 0;
    cpu.f.cy = 1;
    cpu_add(&cpu, 2, true);
    assert(cpu.f.p);

    // adc - cy flag
    cpu.a = 0xff;
    cpu.f.cy = 1;
    cpu_add(&cpu, 0, true);
    assert(cpu.f.cy);

    // adc - ac flag
    cpu.a = 0x0f;
    cpu.f.cy = 1;
    cpu_add(&cpu, 0, true);
    assert(cpu.f.ac);

    // sub - z flag
    cpu.a = 1;
    cpu_sub(&cpu, 1, false);
    assert(cpu.f.z);

    // sub - s flag
    cpu.a = 0;
    cpu_sub(&cpu, 1, false);
    assert(cpu.f.s);

    // sub - p flag
    cpu.a = 6;
    cpu_sub(&cpu, 3, false);
    assert(cpu.f.p);

    // sub - cy flag
    cpu.a = 0;
    cpu_sub(&cpu, 1, false);
    assert(cpu.f.cy);

    // sub - ac flag
    cpu.a = 0x0f;
    cpu_sub(&cpu, 1, false);
    assert(cpu.f.ac);

    // sbb - z flag
    cpu.a = 1;
    cpu.f.cy = 1;
    cpu_sub(&cpu, 0, true);
    assert(cpu.f.z);

    // sbb - s flag
    cpu.a = 0;
    cpu.f.cy = 1;
    cpu_sub(&cpu, 0, true);
    assert(cpu.f.s);

    // sbb - p flag
    cpu.a = 6;
    cpu.f.cy = 1;
    cpu_sub(&cpu, 2, true);
    assert(cpu.f.p);

    // sbb - cy flag
    cpu.a = 0;
    cpu.f.cy = 1;
    cpu_sub(&cpu, 0, true);
    assert(cpu.f.cy);

    // sbb - ac flag
    cpu.a = 0x0f;
    cpu.f.cy = 1;
    cpu_sub(&cpu, 0, true);
    assert(cpu.f.ac);

    // inr - z flag
    cpu.a = -1;
    cpu_inr(&cpu, &cpu.a);
    assert(cpu.f.z);

    // inr - s flag
    cpu.a = -2;
    cpu_inr(&cpu, &cpu.a);
    assert(cpu.f.s);

    // inr - p flag
    write_byte(&cpu, 0x1000, 2);
    cpu_inr(&cpu, cpu.memory + 0x1000);
    assert(cpu.f.p);

    // inr - ac flag
    write_byte(&cpu, 0x1000, 0x0f);
    cpu_inr(&cpu, cpu.memory + 0x1000);
    assert(cpu.f.ac);

    // dcr - z flag
    cpu.a = 1;
    cpu_dcr(&cpu, &cpu.a);
    assert(cpu.f.z);

    // dcr - s flag
    cpu.a = -2;
    cpu_dcr(&cpu, &cpu.a);
    assert(cpu.f.s);

    // dcr - p flag
    write_byte(&cpu, 0x1000, 4);
    cpu_dcr(&cpu, cpu.memory + 0x1000);
    assert(cpu.f.p);

    // dcr - ac flag
    write_byte(&cpu, 0x1000, 0x0f);
    cpu_dcr(&cpu, cpu.memory + 0x1000);
    assert(cpu.f.ac);

    // rlc
    cpu.a = 0xbb; // 0b10111011
    cpu_rlc(&cpu);
    assert(cpu.f.cy);
    assert(cpu.a == 0x77); // 0b01110111

    // rrc
    cpu.a = 0xbb; // 0b10111011
    cpu_rrc(&cpu);
    assert(cpu.f.cy);
    assert(cpu.a == 0xdd); // 0b11011101


    return 0;
}
