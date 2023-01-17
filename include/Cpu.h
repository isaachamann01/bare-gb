#ifndef CPUH
#define CPUH

#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"

typedef uint8_t u8;
typedef uint16_t u16;

void cpu_init(size_t tester_instruction_mem_size, u8* tester_instruction_mem);
void cpu_reset(struct state* state);
void cpu_get_state(struct state* state);
int cpu_step(void);

struct mem_access {
    int type;
    u16 addr;
    u8 val;
};

struct state {
	//We allow all access to registers for 8bity 16bit and via array
	union {
        u8 regs[8];
        struct {
            u16 BC, DE, HL, AF;
        } reg16;
        struct { /* little-endian of x86 is not nice here. */
            u8 C, B, E, D, L, H, F, A;
        } reg8;
    };
    u16 SP;
    u16 PC;

    bool halted;
    bool interrupts_master_enabled;
    int num_mem_accesses;
    struct mem_access mem_accesses[16];
};

#endif