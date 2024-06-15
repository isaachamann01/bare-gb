#include "../include/cpu.h"
#include <stdio.h>  
#include "../include/memory.h"

int cycles_per_instruction[] = {
  /* 0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f       */
     4, 12,  8,  8,  4,  4,  8,  4, 20,  8,  8,  8,  4,  4,  8,  4, /* 0 */
     4, 12,  8,  8,  4,  4,  8,  4, 12,  8,  8,  8,  4,  4,  8,  4, /* 1 */
     8, 12,  8,  8,  4,  4,  8,  4,  8,  8,  8,  8,  4,  4,  8,  4, /* 2 */
     8, 12,  8,  8, 12, 12, 12,  4,  8,  8,  8,  8,  4,  4,  8,  4, /* 3 */
     4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4, /* 4 */
     4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4, /* 5 */
     4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4, /* 6 */
     8,  8,  8,  8,  8,  8,  4,  8,  4,  4,  4,  4,  4,  4,  8,  4, /* 7 */
     4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4, /* 8 */
     4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4, /* 9 */
     4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4, /* a */
     4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  8,  4,  8,  4, /* b */
     8, 12, 12, 16, 12, 16,  8, 16,  8, 16, 12,  0, 12, 24,  8, 16, /* c */
     8, 12, 12,  4, 12, 16,  8, 16,  8, 16, 12,  4, 12,  4,  8, 16, /* d */
    12, 12,  8,  4,  4, 16,  8, 16, 16,  4, 16,  4,  4,  4,  8, 16, /* e */
    12, 12,  8,  4,  4, 16,  8, 16, 12,  8, 16,  4,  0,  4,  8, 16, /* f */
};

int cycles_per_instruction_cb[] = {
  /* 0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f       */
     8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8, /* 0 */
     8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8, /* 1 */
     8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8, /* 2 */
     8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8, /* 3 */
     8,  8,  8,  8,  8,  8, 12,  8,  8   ,  8,  8,  8,  8,  8, 12,  8, /* 4 */
     8,  8,  8,  8,  8,  8, 12,  8,  8,  8,  8,  8,  8,  8, 12,  8, /* 5 */
     8,  8,  8,  8,  8,  8, 12,  8,  8,  8,  8,  8,  8,  8, 12,  8, /* 6 */
     8,  8,  8,  8,  8,  8, 12,  8,  8,  8,  8,  8,  8,  8, 12,  8, /* 7 */
     8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8, /* 8 */
     8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8, /* 9 */
     8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8, /* a */
     8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8, /* b */
     8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8, /* c */
     8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8, /* d */
     8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8, /* e */
     8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8, /* f */
};


// This is a real time lookup to get the required jmp condition for our conditional jumps.
u8 condition_lookup(u8 lookup) {
  switch(lookup) {
    case 0b00:
      return !gb->flags.ZF;
    case 0b01:
      return gb->flags.ZF;
    case 0b10:
      return !gb->flags.CF;
    case 0b11:
      return gb->flags.CF;
  }

  // If we get here theres an invalid lookup.
  printf("Invalid condition lookup: Op Code = %x\n", read_8(gb->PC));
  exit(-1);

}

u16 reg16_lookup_mem(u8 lookup) {
  switch(lookup) {
    case 0b00:
      return gb->reg16.BC;
    case 0b01:
      return gb->reg16.DE;
    case 0b10:
      return gb->reg16.HL;
    case 0b11:
      return gb->reg16.HL;
  }

  // If we get here theres an invalid lookup.
  printf("Invalid reg16 lookup: Op Code = %x\n", read_8(gb->PC));
  exit(-1);
}

// This is a lookup table of values for the 8bit and 16bit registers.
// The index is the number found in the opcode and the value is a pointer to the register.

static u8* reg8_lookup[8]; 
static u16* reg16_lookup[4];
static u16* reg16_lookupstk[4];

struct state* gb;

u8 cycle_count;

void initialise_cpu() {
    gb = malloc(sizeof(struct state));
    
    gb->SP = 0x0000;
    gb->PC = 0x0000;
    gb->halted = 0;
    gb->interrupts_master_enabled = 0;
    gb->reg16.AF = 0x0000;
    gb->reg16.BC = 0x0000;
    gb->reg16.DE = 0x0000;
    gb->reg16.HL = 0x0000;

    // Set up the reg8 lookup table.
    reg8_lookup[0] = &gb->reg8.B;
    reg8_lookup[1] = &gb->reg8.C;
    reg8_lookup[2] = &gb->reg8.D;
    reg8_lookup[3] = &gb->reg8.E;
    reg8_lookup[4] = &gb->reg8.H;
    reg8_lookup[5] = &gb->reg8.L;
    reg8_lookup[6] = NULL; // This is for HL and we gotta dynamically address memory
    reg8_lookup[7] = &gb->reg8.A;

    // Set up the reg16 lookup table.
    reg16_lookup[0] = &gb->reg16.BC;
    reg16_lookup[1] = &gb->reg16.DE;
    reg16_lookup[2] = &gb->reg16.HL;
    reg16_lookup[3] = &gb->SP;

    // Set up the reg16 lookup table.
    reg16_lookupstk[0] = &gb->reg16.BC;
    reg16_lookupstk[1] = &gb->reg16.DE;
    reg16_lookupstk[2] = &gb->reg16.HL;
    reg16_lookupstk[3] = &gb->reg16.AF;

    if (memory == NULL) {
        printf("Failed to allocate memory\n");
        exit(1);
    }

    if (gb == NULL) {
        printf("Failed to allocate gb\n");
        exit(1);
    }

    // Set current cycle count to 0
    u8 cycle_count = 0;

    // Initalise the interrupt registers.
    gb->IE = &memory[0xFFFF];
    gb->IF = &memory[0xFF0F];

}

void uninitialise_cpu() {
    free(gb);
}

#define CF gb->flags.CF 
#define HF gb->flags.HF
#define NF gb->flags.NF
#define ZF gb->flags.ZF

#define AF gb->reg16.AF
#define BC gb->reg16.BC
#define DE gb->reg16.DE
#define HL gb->reg16.HL

#define F gb->reg8.F
#define A gb->reg8.A
#define C gb->reg8.C
#define B gb->reg8.B
#define E gb->reg8.E
#define D gb->reg8.D
#define L gb->reg8.L
#define H gb->reg8.H

#define SP gb->SP
#define PC gb->PC

#define halted gb->halted
#define interupts_master_enabled gb->interupts_master_enabled

#define true 1
#define false 0

/* ------------------------------------------------------ */

// Family 1 Instructions

//NOP
int NOP() {
  return cycles_per_instruction[read_8(PC++)];
}

//RLCA 
int RLCA() {
  CF = (A & 0x80) >> 7;
  A = (A << 1) | CF;
  HF = 0;
  NF = 0;
  return cycles_per_instruction[read_8(PC++)];
}

//RRCA
int RRCA() {
  CF = A & 0x1;
  A = (A >> 1) | (CF << 7);
  HF = 0;
  NF = 0;
  return cycles_per_instruction[read_8(PC++)];
}

//RLA
int RLA() {
  u8 temp = A;
  A = (A << 1) | CF;
  CF = (temp & 0x80) >> 7;
  HF = 0;
  NF = 0;
  return cycles_per_instruction[read_8(PC++)];
}

//RRA
int RRA() {
  u8 temp = A;
  A = (A >> 1) | (CF << 7);
  CF = temp & 0x1;
  HF = 0;
  NF = 0;
  return cycles_per_instruction[read_8(PC++)];
}

//DAA
int DAA() {
  // INSERT BINARY CODED DECIMAL HERE
  // DO LATER
  return cycles_per_instruction[read_8(PC++)];
}

//CPL
int CPL() {
  A = ~A;
  NF = 1;
  HF = 1;
  return cycles_per_instruction[read_8(PC++)];
}

//SCF
int SCF() {
  CF = 1;
  HF = 0;
  NF = 0;
  return cycles_per_instruction[read_8(PC++)];
}

//CCF
int CCF() {
  CF = !CF;
  HF = 0;
  NF = 0;
  return cycles_per_instruction[read_8(PC++)];
}

//JMP_IMM8
int JMP_IMM8() {
  u8 OP = read_8(PC);
  PC += read_8(PC+1);
  return cycles_per_instruction[OP];
}

//LD_IMM16_SP
int LD_IMM16_SP() {
  u16 tempPC = PC;
  u16 tempSP = read_16(PC+1);
  write_16(tempSP, SP);
  PC += 3;
  return cycles_per_instruction[read_8(tempPC)];
}

//STOP
int STOP() {
  // DO LATER
  return cycles_per_instruction[read_8(PC++)];
}

//INC_R16
int INC_R16(u16* reg) {
  *reg += 1;
  return cycles_per_instruction[read_8(PC)];
  PC++;
}

//DEC_R16
int DEC_R16(u16* reg) {
  *reg -= 1;
  return cycles_per_instruction[read_8(PC++)];
}

//ADD_HL_R16
int ADD_HL_R16(u16* reg) {
  u16 temp = HL + *reg;
  CF = (temp > 0xFFFF);
  HF = ((*reg & 0xfff) + (HL & 0xfff)) > 0xfff;
  HL = temp;
  return cycles_per_instruction[read_8(PC++)];
}

//LD_R16_IMM16
int LD_R16_IMM16(u16* reg) {
  u16 tempPC = PC;
  *reg = read_16(PC+1);
  PC += 3;
  return cycles_per_instruction[read_8(tempPC)];
}

//LD_R16mem_a
int LD_R16mem_a(u16 reg) {

  write_8(reg, A);

  u8 OP = read_8(PC);

  // Hardcoding the HL issue.
  if (OP == 0x22) {
    HL++;
  }
  else if (OP == 0x32) {
    HL--;
  }

  return cycles_per_instruction[read_8(PC++)];
}

//LD_a_R16mem
int LD_a_R16mem(u16 reg) {

  A = read_8(reg);

  // Double read OP honestly we can pass this in as a parameter all the time.
  u8 OP = read_8(PC);

  // Hardcoding the HL issue.
  if (OP == 0x2A) {
    HL++;
  }
  else if (OP == 0x3A) {
    HL--;
  }

  return cycles_per_instruction[read_8(PC++)];
}

//INC_R8
int INC_R8(u8* reg) {
  *reg += 1;
  ZF = (*reg == 0);
  NF = 0;
  HF = (*reg & 0xf + 1) > 0xf;
  return cycles_per_instruction[read_8(PC++)];
}

//DEC_R8
int DEC_R8(u8* reg) {
  *reg -= 1;
  ZF = (*reg == 0);
  NF = 1;
  HF = (*reg & 0xf) == 0xf;
  return cycles_per_instruction[read_8(PC++)];
}

//LD_R8_IMM8
int LD_R8_IMM8(u8* reg) {
  *reg = read_8(PC+1);
  PC += 2;
  return cycles_per_instruction[read_8(PC-2)];
}

//JR_COND_IMM8
int JR_COND_IMM8(int condition) {

  u8 OP = read_8(PC);
  if (condition) {

    // Convert to signed 8 bit.
    PC += ((int8_t)read_8(PC+1) + 2);

    // Add 4 cycles because the branch condition was met.
    return cycles_per_instruction[OP] + 4;
  }
  PC += 2;
  return cycles_per_instruction[OP];
}

/* ------------------------------------------------------ */

// Family 2 Instructions

//HALT
int HALT() {
  halted = 1;
  return cycles_per_instruction[read_8(PC++)];
}

//LD_r8_r8
int LD_r8_r8(u8* dest, u8* source) {
  *dest = *source;
  return cycles_per_instruction[read_8(PC++)];
}

/* ------------------------------------------------------ */

// Family 3 Instructions

//ADD_A_R8
int ADD_A_R8(u8* reg) {
  u8 temp = A;
  A += *reg;
  CF = (A < temp);
  HF = ((temp & 0xf) + (*reg & 0xf) & 0x10) == 0x10;
  NF = 0;
  ZF = (A == 0);
  return cycles_per_instruction[read_8(PC++)];
}

//ADC_A_R8
int ADC_A_R8(u8* reg) {
  u8 temp = A;
  A += *reg + CF;
  CF = (A < temp);
  HF = ((temp & 0xf) + (*reg & 0xf) & 0x10) == 0x10;
  NF = 0;
  ZF = (A == 0);
  return cycles_per_instruction[read_8(PC++)];
}

//SUB_A_R8
int SUB_A_R8(u8* reg) {
  u8 temp = A;
  A -= *reg;
  CF = (A > temp);
  HF = (((temp & 0xf) - (*reg & 0xf)) & 0x10) == 0x10;
  NF = 1;
  ZF = (A == 0);
  return cycles_per_instruction[read_8(PC++)];
}

//SBC_A_R8
int SBC_A_R8(u8* reg) {
  u8 temp = A;
  A -= *reg - CF;
  HF = (((temp & 0xf) - (*reg & 0xf) - CF) & 0x10) == 0x10;
  CF = (A < temp);
  NF = 1;
  ZF = (A == 0);
  return cycles_per_instruction[read_8(PC++)];
}

//AND_A_R8
int AND_A_R8(u8* reg) {
  A &= *reg;
  CF = 0;
  HF = 1;
  NF = 0;
  ZF = (A == 0);
  return cycles_per_instruction[read_8(PC++)];
}

//XOR_A_R8
int XOR_A_R8(u8* reg) {
  A ^= *reg;
  CF = 0;
  HF = 0;
  NF = 0;
  ZF = (A == 0);
  return cycles_per_instruction[read_8(PC++)];
}

//OR_A_R8
int OR_A_R8(u8* reg) {
  A |= *reg;
  CF = 0;
  HF = 0;
  NF = 0;
  ZF = (A == 0);
  return cycles_per_instruction[read_8(PC++)];
}

//CP_A_R8
int CP_A_R8(u8* reg) {
  u8 temp = A - *reg;
  CF = (A < temp);
  HF = (((A & 0xf) - (*reg & 0xf)) & 0x10) == 0x10;
  NF = 1;
  ZF = (temp == 0);
  return cycles_per_instruction[read_8(PC++)];
}

/* ------------------------------------------------------ */

// Family 4 Instructions

//ADD_A_IMM8
int ADD_A_IMM8() {
  u8 OP = read_8(PC);
  u8 temp = A;
  A += read_8(PC+1);
  CF = (A < temp);
  HF = ((temp & 0xf) + (read_8(PC+1) & 0xf) & 0x10) == 0x10;
  NF = 0;
  ZF = (A == 0);
  PC+=2;
  return cycles_per_instruction[OP];
}

//ADC_A_IMM8
int ADC_A_IMM8() {
  u8 OP = read_8(PC);
  u8 temp = A;
  A += read_8(PC+1) + CF;
  CF = (A > temp);
  HF = ((temp & 0xf) + (read_8(PC+1) & 0xf) & 0x10) == 0x10;
  NF = 0;
  ZF = (A == 0);
  PC+=2;
  return cycles_per_instruction[OP];
}

//SUB_A_IMM8
int SUB_A_IMM8() {
  u8 OP = read_8(PC);
  u8 temp = A;
  A -= read_8(PC+1);
  CF = (A > temp);
  HF = (((A & 0xf) - (read_8(PC+1) & 0xf)) & 0x10) == 0x10;
  NF = 1;
  ZF = (A == 0);
  PC+=2;
  return cycles_per_instruction[OP];
}

//SBC_A_IMM8
int SBC_A_IMM8() {
  u8 OP = read_8(PC);
  u8 temp = A;
  A -= read_8(PC+1) + CF;
  HF = (((A & 0xf) - (read_8(PC+1) & 0xf)) & 0x10 - CF) == 0x10;
  CF = (A > temp);
  NF = 1;
  ZF = (A == 0);
  PC+=2;
  return cycles_per_instruction[OP];
}

//AND_A_IMM8
int AND_A_IMM8() {
  u8 OP = read_8(PC);
  A &= read_8(PC+1);
  CF = 0;
  HF = 1;
  NF = 0;
  ZF = (A == 0);
  PC+=2;
  return cycles_per_instruction[OP];
}

//XOR_A_IMM8
int XOR_A_IMM8() {
  u8 OP = read_8(PC);
  A ^= read_8(PC+1);
  CF = 0;
  HF = 0;
  NF = 0;
  ZF = (A == 0);
  PC+=2;
  return cycles_per_instruction[OP];
}

//OR_A_IMM8
int OR_A_IMM8() {
  u8 OP = read_8(PC);
  A |= read_8(PC+1);
  CF = 0;
  HF = 0;
  NF = 0;
  ZF = (A == 0);
  PC+=2;
  return cycles_per_instruction[OP];
}

//CP_A_IMM8
int CP_A_IMM8() {
  u8 OP = read_8(PC);
  u8 temp = A - read_8(PC+1);
  CF = (A < temp);
  HF = (((A & 0xf) - (read_8(PC+1) & 0xf)) & 0x10) == 0x10;
  NF = 1;
  ZF = (temp == 0);
  PC+=2;
  return cycles_per_instruction[OP];
}

//RET
int RET() {
  u8 OP = read_8(PC);
  PC = pop_16(&SP);
  return cycles_per_instruction[OP];
}

//RETI
int RETI() {
  u8 OP = read_8(PC);
  PC = pop_16(&SP);
  gb->interrupts_master_enabled = 1;
  return cycles_per_instruction[OP];
}

//JP_IMM16
int JP_IMM16() {
  u8 OP = read_8(PC);
  PC = read_16(PC+1);
  return cycles_per_instruction[OP];
}

//JP_HL
int JP_HL() {
  u8 OP = read_8(PC);
  PC = HL;
  return cycles_per_instruction[OP];
}

//CALL_IMM16
int CALL_IMM16() {
  u8 OP = read_8(PC);
  push_16(&SP, PC+1);
  PC = read_16(PC+1);
  return cycles_per_instruction[OP];
}

//LDH_C_A
int LDH_C_A() {
  write_8(0xFF00 + C, A);
  return cycles_per_instruction[read_8(PC++)];
}

//LDH_IMM8_A
int LDH_IMM8_A() {
  u8 OP = read_8(PC);
  write_8(0xFF00 + read_8(PC+1), A);
  PC += 2;
  return cycles_per_instruction[OP];
}

//LD_IMM16_A
int LD_IMM16_A() {
  u8 OP = read_8(PC);
  write_8(read_16(PC+1), A);
  PC += 3;
  return cycles_per_instruction[OP];
}

//LDH_A_C
int LDH_A_C() {
  A = read_8(0xFF00 + C);
  return cycles_per_instruction[read_8(PC++)];
}

//LDH_A_IMM8
int LDH_A_IMM8() {
  u8 OP = read_8(PC);
  A = read_8(0xFF00 + read_8(PC+1));
  PC += 2;
  return cycles_per_instruction[OP];
}

//LD_A_IMM16
int LD_A_IMM16() {
  u8 OP = read_8(PC);
  A = read_8(read_16(PC+1));
  PC += 3;
  return cycles_per_instruction[OP];
}

//ADD_SP_IMM8
int ADD_SP_IMM8() {
  u8 OP = read_8(PC);
  u8 temp = SP;
  // add SIGNED imm8
  SP += (int8_t)read_8(PC+1);
  CF = (SP < temp);
  HF = ((temp & 0xf) + (read_8(PC+1) & 0xf) & 0x10) == 0x10;
  NF = 0;
  PC += 2;
  return cycles_per_instruction[OP];
}

//LD_HL_SP_IMM8
int LD_HL_SP_IMM8() {
  u8 OP = (int8_t)read_8(PC);
  u8 temp = SP;
  // add SIGNED imm8
  HL = SP + read_8(PC+1);
  CF = (HL < temp);
  HF = (((temp & 0xf) + (read_8(PC+1) & 0xf)) & 0x10) == 0x10;
  NF = 0;
  PC += 2;
  return cycles_per_instruction[OP];
}

//LD_SP_HL
int LD_SP_HL() {
  SP = HL;
  return cycles_per_instruction[read_8(PC++)];
}

//DI
int DI() {
  gb->interrupts_master_enabled = 0;
  return cycles_per_instruction[read_8(PC++)];
}

//EI
int EI() {
  gb->interrupts_master_enabled = 1;
  return cycles_per_instruction[read_8(PC++)];
}

//RET_COND
int RET_COND(int condition) {
  u8 OP = read_8(PC);
  if (condition) {
    PC = pop_16(&SP);
    return cycles_per_instruction[OP] + 12;
  }
  PC++;
  return cycles_per_instruction[OP];
}

//JP_COND_IMM16
int JMP_COND_IMM16(int condition) {
  u8 OP = read_8(PC);
  if (condition) {
    PC = read_16(PC+1);
    return cycles_per_instruction[OP] + 4;
  }
  PC += 3;
  return cycles_per_instruction[OP];
}

//CALL_COND_IMM16
int CALL_COND_IMM16(int condition) {
  u8 OP = read_8(PC);
  if (condition) {
    push_16(&SP, PC+3);
    PC = read_16(PC+1);
    return cycles_per_instruction[OP] + 12;
  }
  PC += 3;
  return cycles_per_instruction[OP];
} 

//RST
int RST(u16 address) {
  u8 OP = read_8(PC);
  push_16(&SP, PC + 1);
  PC = read_8(address);
  return cycles_per_instruction[OP];
}

//POP_R16
int POP_R16(u16* reg) {
  *reg = pop_16(&SP);
  return cycles_per_instruction[read_8(PC++)];
}

//PUSH_R16
int PUSH_R16(u16* reg) {
  push_16(&SP, *reg);
  return cycles_per_instruction[read_8(PC++)];
}

/*-------------------------------------------------------*/

// CB Instructions

//RLC_R8
int RLC_R8(u8* reg) {
  u8 OP = read_8(PC);
  CF = (*reg & 0x80) >> 7;
  *reg = (*reg << 1) | CF;
  HF = 0;
  NF = 0;
  ZF = (*reg == 0);
  PC += 2;
  return cycles_per_instruction_cb[OP + 1];
}

//RRC_R8
int RRC_R8(u8* reg) {
  u8 OP = read_8(PC);
  CF = *reg & 0x1;
  *reg = (*reg >> 1) | (CF << 7);
  HF = 0;
  NF = 0;
  ZF = (*reg == 0);
  PC += 2;
  return cycles_per_instruction_cb[OP + 1];
}

//RL_R8
int RL_R8(u8* reg) {
  u8 OP = read_8(PC);
  u8 temp = *reg;
  *reg = (*reg << 1) | CF;
  CF = (temp & 0x80) >> 7;
  HF = 0;
  NF = 0;
  ZF = (*reg == 0);
  PC += 2;
  return cycles_per_instruction_cb[OP + 1];
}

//RR_R8
int RR_R8(u8* reg) {
  u8 OP = read_8(PC);
  u8 temp = *reg;
  *reg = (*reg >> 1) | (CF << 7);
  CF = temp & 0x1;
  HF = 0;
  NF = 0;
  ZF = (*reg == 0);
  PC += 2;
  return cycles_per_instruction_cb[OP + 1];
}

//SLA_R8
int SLA_R8(u8* reg) {
  u8 OP = read_8(PC);
  CF = (*reg & 0x80) >> 7;
  *reg = *reg << 1;
  HF = 0;
  NF = 0;
  ZF = (*reg == 0);
  PC += 2;
  return cycles_per_instruction_cb[OP + 1];
}

//SRA_R8
int SRA_R8(u8* reg) {
  u8 OP = read_8(PC);
  CF = *reg & 0x1;
  *reg = (*reg >> 1) | (*reg & 0x80);
  HF = 0;
  NF = 0;
  ZF = (*reg == 0);
  PC += 2;
  return cycles_per_instruction_cb[OP + 1];
}

//SWAP_R8
int SWAP_R8(u8* reg) {
  u8 OP = read_8(PC);
  *reg = (*reg >> 4) | (*reg << 4);
  CF = 0;
  HF = 0;
  NF = 0;
  ZF = (*reg == 0);
  return cycles_per_instruction_cb[OP + 1];
}

//SRL_R8
int SRL_R8(u8* reg) {
  u8 OP = read_8(PC);
  CF = *reg & 0x1;
  *reg = *reg >> 1;
  HF = 0;
  NF = 0;
  ZF = (*reg == 0);
  PC += 2;
  return cycles_per_instruction_cb[OP + 1];
}

//BIT__R8
int BIT_R8(u8* reg, u8 bit) {
  u8 OP = read_8(PC);
  ZF = !(*reg & (1 << bit));
  HF = 1;
  NF = 0;
  PC += 2;
  return cycles_per_instruction_cb[OP + 1];
}

//RES__R8
int RES_R8(u8* reg, u8 bit) {
  u8 OP = read_8(PC);
  *reg &= ~(1 << bit);
  PC += 2;
  return cycles_per_instruction_cb[OP + 1];
}

//SET__R8
int SET_R8(u8* reg, u8 bit) {
  u8 OP = read_8(PC);
  *reg |= (1 << bit);
  PC += 2;
  return cycles_per_instruction_cb[OP + 1];
}

int execute_cb_instruction() {
  
    u8 newOP = read_8(PC+1);
    u8 tempOP = newOP & 0b11111000;

    u8 lookup = newOP & 0b00000111;
  
    u8* r8;
    if (lookup == 0b110) {
      r8 = &memory[HL];
    } else {
      r8 = reg8_lookup[lookup];
    }
  
    // Deal with the bit ops with r8 operand.
    switch(tempOP) {
      case 0b00000000:
        return RLC_R8(r8);
      case 0b00001000:
        return RRC_R8(r8);
      case 0b00010000:
        return RL_R8(r8);
      case 0b00011000:
        return RR_R8(r8);
      case 0b00100000:
        return SLA_R8(r8);
      case 0b00101000:
        return SRA_R8(r8);
      case 0b00110000:
        return SWAP_R8(r8);
      case 0b00111000:
        return SRL_R8(r8);
    }

    tempOP   = newOP & 0b11000000;
    lookup   = newOP & 0b00000111;
    u8 bitindex = (newOP & 0b00111000) >> 3;

    switch(tempOP) {
      case 0b01000000:
        return BIT_R8(r8,bitindex);
      case 0b10000000:
        return RES_R8(r8,bitindex);
      case 0b11000000:
        return SET_R8(r8,bitindex);
    }
   
    // If we get here theres an invalid instruction.
    printf("Invalid instruction: Op Code = %x\n", newOP);
    return -1;

}

/*-------------------------------------------------------*/

int execute_family1_instruction(u8 OP) {
  
  // Family 1 is more of our miscellanous instructions.

  // First we should go through the instructions that have no arguments.
  switch(OP) {
      case 0b00000000:
        return NOP();
      case 0b00000111:
        return RLCA();
	    case 0b00001111:
        return RRCA();
      case 0b00010111:
        return RLA();
      case 0b00011111:
        return RRA();
      case 0b00100111:
        return DAA();
      case 0b00101111:
        return CPL();
      case 0b00110111:
        return SCF();
      case 0b00111111:
        return CCF();
      case 0b00011000:
        return JMP_IMM8();
      case 0b00001000:
        return LD_IMM16_SP();
      case 0b00010000:
        return STOP();
  }

  // Now we will go through the instructions that have arguments.
  // Especially we will load arguements in the 4th and 5th byte of the op code.
  // Have new arg with zeroed out bits 4 and 5. (Starting at 0.)
  // We need to inlcude the op code as arguements so they know the registers.

  u8 tempOP = OP & 0b11001111;
  u16* arg = reg16_lookup[(OP & 0b00110000) >> 4];
  u16 reg16mem = reg16_lookup_mem((OP & 0b00110000) >> 4);

  switch(tempOP) {
    case 0b00000011:
      return INC_R16(arg);
    case 0b00001011:
      return DEC_R16(arg);
    case 0b00001001:
      return ADD_HL_R16(arg);

    case 0b00000001:
      return LD_R16_IMM16(arg);
    case 0b00000010:
      return LD_R16mem_a(reg16mem);
    case 0b00001010:
      return LD_a_R16mem(reg16mem);

  }

  // Now go through instructions that have r8 arguments on bits 3,4,5.
  // Because its r8 we need to check if we have to load from hl.
  // We are kinda ignoring my idea to only access memory through my read and write functions.
  // Probably and issue. 

  tempOP = OP & 0b11000111;
  u8 lookupnum = (OP & 0b00111000) >> 3;
  u8* r8;
  if (lookupnum == 0b110) {
    r8 = &memory[HL];
  } else {
    r8 = reg8_lookup[lookupnum];
  }

  switch(tempOP) {
    case 0b00000100:
      return INC_R8(r8);
    case 0b00000101:
      return DEC_R8(r8);
    case 0b00000110:
      return LD_R8_IMM8(r8);
  }

  // Now go through jmp condition on bits 3 and 4.
  tempOP = OP & 0b11100111;
  u8 lookup = (OP & 0b00011000) >> 3;

  if (tempOP == 0b00100000) {
    // Note this passed a u8 and not a pointer to a u8.
    return JR_COND_IMM8(condition_lookup(lookup));
  }

  // If we get here theres an invalid instruction.
  printf("Invalid instruction: Op Code = %x\n", OP);
  return -1;

}

int execute_family2_instruction(u8 OP) {

  // Family 2 deals with 8 bit register loads.

  // First halt instruction is the outlier here.
  if (OP == 0b01110110) {
    return HALT();
  }

  u8 destlookup   = (OP & 0b00111000) >> 3;
  u8 sourcelookup = (OP & 0b00000111);
  u8* destr8;
  if (destlookup == 0b110) {
    destr8 = &memory[HL];
  } else {
    destr8 = reg8_lookup[destlookup]; 
  }

  u8* sourcer8;
  if (sourcelookup == 0b110) {
    sourcer8 = &memory[HL];
  } else {
    sourcer8 = reg8_lookup[sourcelookup];
  }
  
  return LD_r8_r8(destr8, sourcer8);

}

int execute_family3_instruction(u8 OP) {

  // Family 3 deals with 8 bit arithmetic.

  u8 tempop = OP & 0b11111000;

  u8 lookup = (OP & 0b00000111);

  u8* r8;

  // Deal with HL shenanigans.
  if (lookup == 0b110) {
    r8 = &memory[HL];
  } else {
    r8 = reg8_lookup[lookup]; 
  }

  switch(tempop) {
    case 0b10000000:
      return ADD_A_R8(r8);
    case 0b10001000:
      return ADC_A_R8(r8);	
    case 0b10010000:
      return SUB_A_R8(r8);	
    case 0b10011000:
      return SBC_A_R8(r8);	
    case 0b10100000:
      return AND_A_R8(r8);
    case 0b10101000:
      return XOR_A_R8(r8);
    case 0b10110000:
      return OR_A_R8(r8);
    case 0b10111000:	
      return CP_A_R8(r8);
  }
  
  // If we get here theres an invalid instruction.
  printf("Invalid instruction: Op Code = %x\n", OP);
  return -1;
  
}

int execute_family4_instruction(u8 OP) {

  // Family 4 deals odds and ends.

  // First lets deal with the 8 bit arithmetic with imm8.
  // Also we add other instructions that we dont add an argument for.

  switch (OP) {
    case 0b11000110:
      return ADD_A_IMM8();
    case 0b11001110:
      return ADC_A_IMM8();
    case 0b11010110:
      return SUB_A_IMM8();
    case 0b11011110:
      return SBC_A_IMM8();
    case 0b11100110:
      return AND_A_IMM8();
    case 0b11101110:
      return XOR_A_IMM8();
    case 0b11110110:
      return OR_A_IMM8();
    case 0b11111110:
      return CP_A_IMM8();
    case 0b11001001:
      return RET();
    case 0b11011001:
      return RETI();
    case 0b11000011:
      return JP_IMM16();
    case 0b11101001:
      return JP_HL();
    case 0b11001101:
      return CALL_IMM16();
    case 0b11001011:
      return execute_cb_instruction();
    case 0b11100010:
      return LDH_C_A();
    case 0b11100000:
      return LDH_IMM8_A();
    case 0b11101010:
      return LD_IMM16_A();
    case 0b11110010:
      return LDH_A_C();
    case 0b11110000:
      return LDH_A_IMM8();
    case 0b11111010:
      return LD_A_IMM16();
    case 0b11101000:
      return ADD_SP_IMM8();
    case 0b11111000:
      return LD_HL_SP_IMM8();
    case 0b11111001:
      return LD_SP_HL();
    case 0b11110011:
      return DI();
    case 0b11111011:
      return EI();

  }

  // Now deal with the conditional jumps (bits 4 and 3)

  u8 tempOP = OP & 0b11100111;
  u8 lookup = (OP & 0b00011000) >> 3;

  switch(tempOP) {
    case 0b11000000:
      return RET_COND(condition_lookup(lookup));
    case 0b11000010:
      return JMP_COND_IMM16(condition_lookup(lookup));
    case 0b11000100:
      return CALL_COND_IMM16(condition_lookup(lookup));
  }

  // Now deal with the RST instructions

  lookup = (OP & 0b00111000) >> 3;
  if ((OP & 0b11000111) == 0b11000111) {
    return RST(lookup);
  }
  // Now deal with push and pop instructions.

  tempOP = OP & 0b11001111;
  if (tempOP == 0b11000001) {
    return POP_R16(reg16_lookupstk[lookup]);
  } 
  if (tempOP == 0b11000101) {
    return PUSH_R16(reg16_lookupstk[lookup]);
  }

  // If we get here theres an invalid instruction.
  printf("Invalid instruction: Op Code = %x\n", OP);
  return -1;
    
}


int execute_instruction() {

  /*
      Function executes the instruction thats at the program counter.
      It returns the number of cycles the instruction took to execute.
  */

  u8 OP = read_8(PC);

  // Executing instructions starting here
  // We will execute instructions by 4 families.
  // These are denoted by the last two bits of the op code.

  u8 family = ((OP & 0b11000000) >> 6);

  switch(family) {

    case 0b00: //Family 1
      return execute_family1_instruction(OP);
    case 0b01: //Family 2
      return execute_family2_instruction(OP);
    case 0b10: //Family 3
      return execute_family3_instruction(OP);
    case 0b11: //Family 4 
      return execute_family4_instruction(OP);

  }

  // If we get here theres an invalid instruction.
  printf("Invalid instruction: Op Code = %x\n", OP);
  return -1;

}

void execute_cycle() {
  // If the current cycle count is at 0 we execute the next instruction.
  if (cycle_count == 0) {
    cycle_count = execute_instruction();
  } else {
    cycle_count--;
  }
}

