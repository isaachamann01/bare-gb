#ifndef CPUHPP
#define CPUHPP

#include "stdlib.h"
#include <array>
#include "Memory.hpp"

typedef uint8_t byte;
typedef uint16_t byte_16;

class Cpu
{
private:
    //All 8-bit registers here. 
    byte register_A;
    byte register_F;
    byte register_B;
    byte register_C;
    byte register_D;
    byte register_E;
    byte register_H;
    byte register_L;
    
    //Last 16bit registers
    byte_16 register_SP;
    byte_16 register_PC;

    
};

#endif