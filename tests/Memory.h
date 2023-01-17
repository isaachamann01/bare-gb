#ifndef MEMORYH
#define MEMORYH

#include <Cpu.h>

//USE BYTE TYPEDEF FOR READABILITY SCREW ME
typedef uint8_t u8;
typedef uint16_t u16;

//MEMORY ADDRESS_SPACE FROM 0x0000 to 0xFFFF
static u8* MEMORY;
static size_t ADDRESS_SPACE;

void init_memory(size_t address_space, u8* address_start);

void mem_write(struct state* s, u16 addr, u8 val);
u8   mem_read(struct state* s, u16 addr);
u16  mem_read16(struct state* s, u16 location);
void mem_write16(struct state* s, u16 location, u16 value);
u16  mem_pop16(struct state* s);
void mem_push16(struct state* s, u16 value);

#endif