#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

#define MEM_SIZE 0x10000
#define NUM_PORTS 0xff

typedef struct flags {
    uint8_t z:1;
    uint8_t s:1;
    uint8_t p:1;
    uint8_t cy:1;
    uint8_t ac:1;
    uint8_t padding:3;
} flags;

typedef enum {
    CY = 0x01,
    P = 0x04,
    AC = 0x10,
    Z = 0x40,
    S = 0x80
} flag_mask;

typedef struct CPU {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;

    uint8_t f;
    
    uint16_t sp;
    uint16_t pc;

    uint8_t memory[MEM_SIZE];

    uint8_t (*in_ports[NUM_PORTS])(void);
    void (*out_ports[NUM_PORTS])(uint8_t data);

    bool hlt;
    bool interrupts_enabled;
    uint8_t enabling_interrputs_timer;
    long cycle_count;

    bool exit;
} CPU;

// Helper functions
void cpu_init(CPU *cpu);
bool cpu_load_rom_at(CPU *cpu, char const *filename, uint16_t start);
void cpu_tick(CPU *cpu);
void clear_flags(CPU *cpu);
uint8_t next_byte(CPU *cpu);
uint16_t next_word(CPU *cpu);
void push(CPU *cpu, uint16_t value);
uint16_t pop(CPU *cpu);

// Memory read/write helper functions
uint8_t read_byte(CPU *cpu, uint16_t address);
void write_byte(CPU *cpu, uint16_t address, uint8_t value);
uint16_t read_word(CPU *cpu, uint16_t address);
void write_word(CPU *cpu, uint16_t address, uint16_t value);

// Register access helper functions
uint16_t get_bc(CPU *cpu);
uint16_t get_de(CPU *cpu);
uint16_t get_hl(CPU *cpu);
void set_bc(CPU *cpu, uint16_t value);
void set_de(CPU *cpu, uint16_t value);
void set_hl(CPU *cpu, uint16_t value);

// Flags helper functions
bool get_flag(CPU const * const cpu, flag_mask flag);
void set_flag(CPU *cpu, flag_mask flag, bool set);
void update_zsp_flags(CPU *cpu, uint8_t res);
void update_cy_flag_add(CPU *cpu, uint8_t val1, uint8_t val2, bool is_carry);
void update_cy_flag_sub(CPU *cpu, uint8_t val1, uint8_t val2, bool is_borrow);
void update_ac_flag_add(CPU *cpu, uint8_t val1, uint8_t val2, bool is_carry);
void update_ac_flag_sub(CPU *cpu, uint8_t val1, uint8_t val2, bool is_borrow);
void update_flags_cmp(CPU *cpu, uint8_t val);
bool parity(uint8_t val);

#endif // CPU_H
