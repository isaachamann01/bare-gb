#include "Memory.h"
#include <stdlib.h>
#include <array>

typedef uint8_t byte;

const int ADDRESS_SPACE = 0xFFFF;

using namespace std;

Memory::Memory() {
	memory.fill(0);
}

byte* Memory::ld_8_bit(byte* address) {
	return memory.at(*address); 
}

void Memory::sv_8_bit(byte* address, byte* value) {
	memory.at(*address) = *value;
}

Memory::~Memory() {

}