#ifndef MEMORYHPP
#define MEMORYHPP

#include <stdlib.h>
#include <array>

//MEMORY ADDRESS_SPACE FROM 0x0000 to 0xFFFF
const int ADDRESS_SPACE = 0xFFFF;

//USE BYTE TYPEDEF FOR READABILITY SCREW ME
typedef uint8_t byte;

//MEMORY CLASS HANDLES ALL MEMORY
class Memory
{
private:
    //ALL MEMORY IN ADDRESS SPACE
    std::array<byte, ADDRESS_SPACE> memory;
public:
    byte* ld_8_bit(byte address);
    byte* ld_16_bit(byte address);
    void sv_8_bit(byte address);
    void sv_16_bit(byte address);
};

#endif