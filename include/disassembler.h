#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "cpu.h"
#include "opcodes.h"

#include <stdio.h>

#define NUM_OPCODES 0x100

char const *instruction_name[NUM_OPCODES] = {
    "NOP",
    "LXI_B",
    "STAX_B",
    "INX_B",
    "INR_B",
    "DCR_B",
    "MVI_B",
    "RLC",
    "NOP_ALT_1",
    "DAD_B",
    "LDAX_B",
    "DCX_B",
    "INR_C",
    "DCR_C",
    "MVI_C",
    "RRC",
    "NOP_ALT_2",
    "LXI_D",
    "STAX_D",
    "INX_D",
    "INR_D",
    "DCR_D",
    "MVI_D",
    "RAL",
    "NOP_ALT_3",
    "DAD_D",
    "LDAX_D",
    "DCX_D",
    "INR_E",
    "DCR_E",
    "MVI_E",
    "RAR",
    "NOP_ALT_4",
    "LXI_H",
    "SHLD",
    "INX_H",
    "INR_H",
    "DCR_H",
    "MVI_H",
    "DAA",
    "NOP_ALT_5",
    "DAD_H",
    "LHLD",
    "DCX_H",
    "INR_L",
    "DCR_L",
    "MVI_L",
    "CMA",
    "NOP_ALT_6",
    "LXI_SP",
    "STA",
    "INX_SP",
    "INR_M",
    "DCR_M",
    "MVI_M",
    "STC",
    "NOP_ALT_7",
    "DAD_SP",
    "LDA",
    "DCX_SP",
    "INR_A",
    "DCR_A",
    "MVI_A",
    "CMC",
    "MOV_B_B",
    "MOV_B_C",
    "MOV_B_D",
    "MOV_B_E",
    "MOV_B_H",
    "MOV_B_L",
    "MOV_B_M",
    "MOV_B_A",
    "MOV_C_B",
    "MOV_C_C",
    "MOV_C_D",
    "MOV_C_E",
    "MOV_C_H",
    "MOV_C_L",
    "MOV_C_M",
    "MOV_C_A",
    "MOV_D_B",
    "MOV_D_C",
    "MOV_D_D",
    "MOV_D_E",
    "MOV_D_H",
    "MOV_D_L",
    "MOV_D_M",
    "MOV_D_A",
    "MOV_E_B",
    "MOV_E_C",
    "MOV_E_D",
    "MOV_E_E",
    "MOV_E_H",
    "MOV_E_L",
    "MOV_E_M",
    "MOV_E_A",
    "MOV_H_B",
    "MOV_H_C",
    "MOV_H_D",
    "MOV_H_E",
    "MOV_H_H",
    "MOV_H_L",
    "MOV_H_M",
    "MOV_H_A",
    "MOV_L_B",
    "MOV_L_C",
    "MOV_L_D",
    "MOV_L_E",
    "MOV_L_H",
    "MOV_L_L",
    "MOV_L_M",
    "MOV_L_A",
    "MOV_M_B",
    "MOV_M_C",
    "MOV_M_D",
    "MOV_M_E",
    "MOV_M_H",
    "MOV_M_L",
    "HLT",
    "MOV_M_A",
    "MOV_A_B",
    "MOV_A_C",
    "MOV_A_D",
    "MOV_A_E",
    "MOV_A_H",
    "MOV_A_L",
    "MOV_A_M",
    "MOV_A_A",
    "ADD_B",
    "ADD_C",
    "ADD_D",
    "ADD_E",
    "ADD_H",
    "ADD_L",
    "ADD_M",
    "ADD_A",
    "ADC_B",
    "ADC_C",
    "ADC_D",
    "ADC_E",
    "ADC_H",
    "ADC_L",
    "ADC_M",
    "ADC_A",
    "SUB_B",
    "SUB_C",
    "SUB_D",
    "SUB_E",
    "SUB_H",
    "SUB_L",
    "SUB_M",
    "SUB_A",
    "SBB_B",
    "SBB_C",
    "SBB_D",
    "SBB_E",
    "SBB_H",
    "SBB_L",
    "SBB_M",
    "SBB_A",
    "ANA_B",
    "ANA_C",
    "ANA_D",
    "ANA_E",
    "ANA_H",
    "ANA_L",
    "ANA_M",
    "ANA_A",
    "XRA_B",
    "XRA_C",
    "XRA_D",
    "XRA_E",
    "XRA_H",
    "XRA_L",
    "XRA_M",
    "XRA_A",
    "ORA_B",
    "ORA_C",
    "ORA_D",
    "ORA_E",
    "ORA_H",
    "ORA_L",
    "ORA_M",
    "ORA_A",
    "CMP_B",
    "CMP_C",
    "CMP_D",
    "CMP_E",
    "CMP_H",
    "CMP_L",
    "CMP_M",
    "CMP_A",
    "RNZ",
    "POP_B",
    "JNZ",
    "JMP",
    "CNZ",
    "PUSH_B",
    "ADI",
    "RST_0",
    "RZ",
    "RET",
    "JZ",
    "JMP_ALT",
    "CZ",
    "CALL",
    "ACI",
    "RST_1",
    "RNC",
    "POP_D",
    "JNC",
    "OUT",
    "CNC",
    "PUSH_D",
    "SUI",
    "RST_2",
    "RC",
    "RET_ALT",
    "JC",
    "IN",
    "CC",
    "CALL_ALT_1",
    "SBI",
    "RST_3",
    "RPO",
    "POP_H",
    "JPO",
    "XTHL",
    "CPO",
    "PUSH_H",
    "ANI",
    "RST_4",
    "RPE",
    "PCHL",
    "JPE",
    "XCHG",
    "CPE",
    "CALL_ALT_2",
    "XRI",
    "RST_5",
    "RP",
    "POP_PSW",
    "JP",
    "DI",
    "CP",
    "PUSH_PSW",
    "ORI",
    "RST_6",
    "RM",
    "SPHL",
    "JM",
    "EI",
    "CM",
    "CALL_ALT_3",
    "CPI",
    "RST_7"
};

int instruction_size[NUM_OPCODES];

void init_instr_size() {
    for (int i = 0; i < NUM_OPCODES; i++) {
        instruction_size[i] = 1;
    }

    instruction_size[MVI_B] = 2;
    instruction_size[MVI_C] = 2;
    instruction_size[MVI_D] = 2;
    instruction_size[MVI_E] = 2;
    instruction_size[MVI_H] = 2;
    instruction_size[MVI_L] = 2;
    instruction_size[MVI_M] = 2;
    instruction_size[MVI_A] = 2;
    instruction_size[ADI]   = 2;
    instruction_size[ACI]   = 2;
    instruction_size[OUT]   = 2;
    instruction_size[SUI]   = 2;
    instruction_size[IN]    = 2;
    instruction_size[SBI]   = 2;
    instruction_size[ANI]   = 2;
    instruction_size[XRI]   = 2;
    instruction_size[ORI]   = 2;
    instruction_size[CPI]   = 2;

    instruction_size[LXI_B]  = 3;
    instruction_size[LXI_D]  = 3;
    instruction_size[LXI_H]  = 3;
    instruction_size[SHLD]   = 3;
    instruction_size[LHLD]   = 3;
    instruction_size[LXI_SP] = 3;
    instruction_size[STA]    = 3;
    instruction_size[LDA]    = 3;
    instruction_size[JNZ]    = 3;
    instruction_size[JMP]    = 3;
    instruction_size[CNZ]    = 3;
    instruction_size[JZ]     = 3;
    instruction_size[CZ]     = 3;
    instruction_size[CALL]   = 3;
    instruction_size[JNC]    = 3;
    instruction_size[CNC]    = 3;
    instruction_size[JC]     = 3;
    instruction_size[CC]     = 3;
    instruction_size[JPO]    = 3;
    instruction_size[CPO]    = 3;
    instruction_size[JPE]    = 3;
    instruction_size[CPE]    = 3;
    instruction_size[JP]     = 3;
    instruction_size[CP]     = 3;
    instruction_size[JM]     = 3;
    instruction_size[CM]     = 3;
}

void disassemble(CPU const * const cpu) {
    printf("%04hx: %s ", cpu->pc, instruction_name[cpu->memory[cpu->pc]]);
    if(instruction_size[cpu->memory[cpu->pc]] > 2) printf("%02hx", cpu->memory[cpu->pc+2]);
    if(instruction_size[cpu->memory[cpu->pc]] > 1) printf("%02hx", cpu->memory[cpu->pc+1]);
    printf("\t\t(%02hhx", cpu->memory[cpu->pc]);
    if(instruction_size[cpu->memory[cpu->pc]] > 1) printf(" %02hx", cpu->memory[cpu->pc+1]);
    if(instruction_size[cpu->memory[cpu->pc]] > 2) printf(" %02hx", cpu->memory[cpu->pc+2]);
    printf(")\n");
}

void print_cpu_status(CPU const * const cpu) {
    printf("A=%02hhx, F=%02hhx, B=%02hhx, C=%02hhx, D=%02hhx, E=%02hx, H=%02hhx, L=%02hhx, SP=%04hx\n", cpu->a, cpu->f, cpu->b, cpu->c, cpu->d, cpu->e, cpu->h, cpu->l, cpu->sp);
    printf("FLAGS: %s %s %s %s %s\n", get_flag(cpu, S)?"S ":"- ", get_flag(cpu, Z)?"Z ":"- ", get_flag(cpu, AC)?"AC":"- ", get_flag(cpu, P)?"P ":"- ", get_flag(cpu, CY)?"CY":"- ");
    printf("Cycles = %ld\n", cpu->cycle_count);
}


#endif // DISASSEMBLER_H
