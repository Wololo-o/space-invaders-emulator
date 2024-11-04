#include <assert.h>

#include "cpu.h"
#include "opcodes.h"


// test flags update on add

int main(int argc, char const *argv[]) {

    CPU cpu;
    cpu_init(&cpu);

    // add - z flag
    cpu.a = -1;
    cpu_add(&cpu, 1);
    assert(cpu.f.z);

    // add - s flag
    cpu.a = -2;
    cpu_add(&cpu, 1);
    assert(cpu.f.s);

    // add - p flag
    cpu.a = 0;
    cpu_add(&cpu, 3);
    assert(cpu.f.p);

    // add - cy flag
    cpu.a = 0xff;
    cpu_add(&cpu, 1);
    assert(cpu.f.cy);

    // add - ac flag
    cpu.a = 0x0f;
    cpu_add(&cpu, 1);
    assert(cpu.f.ac);

    return 0;
}
