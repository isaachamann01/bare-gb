#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

//16bit ADDRESS SPACE
#define MEMORY_SIZE 0xFFFF

typedef u_int8_t u8;
typedef u_int16_t u16;

extern u8* memory;

//FUNCTION INITIALISES/UNINITIALISES MEMORY
void initialise_memory();
void uninitialise_memory();

//MEMORY READING AND WRITING
u8 read_8(u16 location);
void write_8(u16 location, u8 content);
u16 read_16(u16 location);
void write_16(u16 location, u16 content);
void push_16(u16 *SP, u16 value);
u16 pop_16(u16 *SP);

#endif // MEMORY_H
