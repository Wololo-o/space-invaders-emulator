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

    return 0;
}
