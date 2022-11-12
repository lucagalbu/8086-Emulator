#include "opCodes.hpp"

void OpCodes::AAA()
{
    uint8_t lowAL = registers.AL() & 0b0000'1111;

    if (lowAL <= 0x9 && !flags.isSet(Flags::A))
    {
        flags.unset(Flags::A);
        flags.unset(Flags::C);
    }
    else
    {
        registers.AX(registers.AX() + 0x0106);
        flags.set(Flags::A);
        flags.set(Flags::C);
    }
    registers.AL(registers.AL() & uint8_t(0b0000'1111));
}