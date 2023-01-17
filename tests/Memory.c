#include "Memory.h"
#include <malloc.h>

//Different memory access types.
#define MEM_ACCESS_READ 0
#define MEM_ACCESS_WRITE 1

void init_memory(size_t address_space, u8* address_start) {
	MEMORY = address_start;
	ADDRESS_SPACE = address_space; 
}

u8 mem_read(struct state* s, u16 addr) {

	s->mem_accesses[s->num_mem_accesses].type = MEM_ACCESS_READ;
	s->mem_accesses[s->num_mem_accesses].addr = addr;
	s->mem_accesses[s->num_mem_accesses].val = MEMORY[addr];

	s->num_mem_accesses++;

	return MEMORY[addr];

}

u16 mem_read16(struct state* s, u16 addr) {

	//TO_DO Implement

}

void mem_write(struct state* s, u16 addr, u8 val) {

	s->mem_accesses[s->num_mem_accesses].type = MEM_ACCESS_WRITE;
	s->mem_accesses[s->num_mem_accesses].addr = addr;
	s->mem_accesses[s->num_mem_accesses].val = val;

	MEMORY[addr] = val;
	return;

}

void mem_write16(struct state* s, u16 addr, u16 val) {

	//TO_DO Implement

}

u16 mem_pop16(struct state* s) {

	//TO_DO Implement

}

void mem_push16(struct state* s, u16 value) {

	//TO_DO Implement

}
