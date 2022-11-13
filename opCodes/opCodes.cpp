#include "opCodes.hpp"

using namespace std;

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

void OpCodes::AAD(uint8_t param1)
{
    if (param1 != 0x0A)
    {
        cerr << "Operation AAD recognized only with 0x0A as parameter" << endl;
        exit(1);
    }
    registers.AH(registers.AH() * 0x0A);
    registers.AL(registers.AL() + registers.AH());
    registers.AH(0b0);

    Flags::checkParity(registers.AL()) ? flags.set(Flags::P) : flags.unset(Flags::P);
    Flags::checkSign(registers.AL()) ? flags.set(Flags::S) : flags.unset(Flags::S);
    registers.AL() == 0 ? flags.set(Flags::Z) : flags.unset(Flags::Z);
}