#include "executor.hpp"

using namespace std;

void Executor::AAA()
{
    uint8_t lowAL = registers.AL() & 0b0000'1111;
    if (lowAL > 0x9 || flags.isSet(Flags::A))
    {
        registers.AX(registers.AX() + 0x0106);
        flags.set(Flags::A);
        flags.set(Flags::C);
    }
    else
    {
        flags.unset(Flags::A);
        flags.unset(Flags::C);
    }
    registers.AL(registers.AL() & uint8_t(0b0000'1111));
}

void Executor::AAD(uint8_t param1)
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

void Executor::AAM(uint8_t param1)
{
    if (param1 != 0x0A)
    {
        cerr << "Operation AAM recognized only with 0x0A as parameter" << endl;
        exit(1);
    }

    uint8_t quotient = registers.AL() / 0x0A;
    uint8_t remainder = registers.AL() % 0x0A;

    registers.AH(quotient);
    registers.AL(remainder);

    Flags::checkParity(registers.AL()) ? flags.set(Flags::P) : flags.unset(Flags::P);
    Flags::checkSign(registers.AL()) ? flags.set(Flags::S) : flags.unset(Flags::S);
    registers.AL() == 0 ? flags.set(Flags::Z) : flags.unset(Flags::Z);
}

void Executor::AAS()
{
    uint8_t lowAL = registers.AL() & 0b0000'1111;

    if (lowAL > 0xA || !flags.isSet(Flags::A))
    {
        registers.AX(registers.AX() - 0x0106);
        flags.set(Flags::A);
    }
    registers.AL(registers.AL() & uint8_t(0b0000'1111));
    flags.isSet(Flags::A) ? flags.set(Flags::C) : flags.unset(Flags::C);
}

void Executor::ADC_ac_data(uint8_t w, uint8_t param1, uint8_t param2 = 0)
{
    uint8_t carry = flags.isSet(Flags::C) ? 1 : 0;

    if (w == 0b0)
    {
        registers.AL(registers.AL() + param1 + carry);
    }
    else if (w == 0b1)
    {
        uint16_t operand = (param2 << 8) + param1 + carry;
        registers.AX(registers.AX() + operand);
    }
    else
    {
        cerr << "Operation ADC only recognized is w bit is 0 or 1" << endl;
        exit(1);
    }
}

void Executor::ADC_memreg_data(uint8_t opCode)
{
    uint8_t modRM = readByteFromIP();

    uint8_t w = opCode & 0b1;
    uint8_t s = (opCode >> 1) & 0b1;

    ModRM modRm = getModRmFromByte(modRM);

    uint8_t carry = flags.isSet(Flags::C) ? 1 : 0;

    if (modRm.reg != 0b010)
    {
        cerr << "Operation ADC between data and mem/reg requires modRM xx010xxx" << endl;
        exit(1);
    }

    if (modRm.mod == 0b11)
    {
        ADC_reg_data(w, s, modRm.rm, carry);
    }
    else
    {
        ADC_mem_data(w, s, modRm, carry);
    }
}

void Executor::ADC_mem_data(uint8_t w, uint8_t s, ModRM modRm, uint8_t carry)
{
    MemoryAddress memoryAddress = getMemAddressFromModRm(modRm);

    if (w == 0)
    {
        uint8_t operand = readByteFromIP();
        uint8_t memoryVal = memory.readByte(memoryAddress);
        uint8_t result = memoryVal + operand + carry;
        memory.setByte(memoryAddress, result);
    }
    else if (w == 1)
    {
        uint16_t operand;
        if (s == 0)
        {
            operand = readWordFromIP();
        }
        else
        {
            operand = readByteFromIP();
            operand = (((operand & 0b1000'0000) * 0b1111'11111) << 8) + operand;
        }

        uint16_t memoryVal = memory.readWord(memoryAddress);
        uint16_t result = memoryVal + operand + carry;

        memory.setWord(memoryAddress, result);
    }
}
