#ifndef cpu_h
#define cpu_h

#include <stdlib.h>
#include "memory.h"

typedef u_int8_t u8;
typedef u_int16_t u16;

//16bit ADDRESS SPACE
#define MEMORY_SIZE 0xFFFF

struct state {
    union {
            struct {
                u16 BC, DE, HL, AF;
            } reg16;
            struct {
                u8 C, B, E, D, L, H, F, A;
            } reg8;
            struct __attribute__((packed)) {
                char padding[6];
                u8 pad1:1;
                u8 pad2:1;
                u8 pad3:1;
                u8 pad4:1;
                u8 CF:1;
                u8 HF:1;
                u8 NF:1;
                u8 ZF:1;
            } flags;
    };
    u16 SP;
    u16 PC;
    int halted;
    int interrupts_master_enabled;

    //Interrupt memory registers.
    u8* IE;
    u8* IF;

};

//WE WILL INITIALISE THIS IN INITIALISE_STATE
extern struct state* gb;

//FUNCTION INITIALISES/UNINITIALISES STATE AND MEMORY
void initialise_cpu();
void uninitialise_cpu();

//COMPLETING INSTRUCTION
void execute_cycle();
int execute_instruction();


#endif

