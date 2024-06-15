#include "../include/memory.h"

u8* memory;

void initialise_memory() {
    memory = calloc(MEMORY_SIZE, sizeof(u8));
}

void uninitialise_memory() {
    free(memory);
}

u8 read_8(u16 location) {
    return memory[location];
}

void write_8(u16 location, u8 content) {
    memory[location] = content;
}

u16 read_16(u16 location) {
    return (memory[location + 1] << 8) | (memory[location]);
}

void write_16(u16 location, u16 content) {
    memory[location + 1] = (u8)((content & 0xFF00) >> 8);
    memory[location] = (u8)(content & 0x00FF); 
}

void push_16(u16* SP, u16 value) {
    *SP -= 0x0002;
    memory[*SP+1] = (u8)((value & 0xFF00) >> 8);
    memory[*SP+2] = (u8)(value & 0x00FF);
}

u16 pop_16(u16* SP) {
    u16 val = ((memory[*SP+1] << 8) | memory[*SP+2]);
    *SP += 0x0002;
    return val;
}