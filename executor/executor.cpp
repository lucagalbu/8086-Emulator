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

void Executor::ADC_memreg_data()
{
    uint8_t opCode = memory[registers.IP()];
    registers.IP(registers.IP() + 1);

    uint8_t modRM = memory[registers.IP()];
    registers.IP(registers.IP() + 1);

    uint8_t w = opCode & 0b1;
    uint8_t s = (opCode >> 1) & 0b1;

    uint8_t mod = (modRM >> 6) & 0b011;
    uint8_t reg = (modRM >> 3) & 0b111;
    uint8_t rm = modRM & 0b011;

    uint8_t carry = flags.isSet(Flags::C) ? 1 : 0;

    if (reg != 0b010)
    {
        cerr << "Operation ADC between data and mem/reg requires modRM xx010xxx" << endl;
        exit(1);
    }

    if (w == 0)
    {
        if (mod == 0b00)
        {
            uint32_t memoryAddress = getMemAddress(rm, 0);
            uint8_t operand = codeSegment[registers.IP()];
            registers.IP(registers.IP() + 1);
            memory[memoryAddress] = memory[memoryAddress] + operand + carry;
        }
        if (mod == 0b01)
        {
            uint8_t displacement = codeSegment[registers.IP()];
            registers.IP(registers.IP() + 1);
            uint32_t memoryAddress = getMemAddress(rm, displacement);
            uint8_t operand = codeSegment[registers.IP()];
            registers.IP(registers.IP() + 1);
            memory[memoryAddress] = memory[memoryAddress] + operand + carry;
        }
        else if (mod == 0b10)
        {
            uint16_t displacement = (codeSegment[registers.IP() + 1] << 8) + codeSegment[registers.IP()];
            registers.IP(registers.IP() + 2);
            uint32_t memoryAddress = getMemAddress(rm, displacement);
            uint8_t operand = codeSegment[registers.IP()];
            registers.IP(registers.IP() + 1);
            memory[memoryAddress] = memory[memoryAddress] + operand + carry;
        }
        else if (mod == 0b11)
        {
            uint8_t reg_value = getReg8(rm);
            uint8_t operand = codeSegment[registers.IP()];
            registers.IP(registers.IP() + 1);
            setReg8(rm, reg_value + operand + carry);
        }
    }
    else if (w == 1)
    {
        if (mod == 0b00)
        {
            uint32_t memoryAddress = getMemAddress(rm, 0);
            uint16_t operand;
            if (s == 0)
            {
                operand = (codeSegment[registers.IP() + 1] << 8) + codeSegment[registers.IP()];
                registers.IP(registers.IP() + 2);
            }
            else
            {
                operand = (((codeSegment[registers.IP()] & 0b1000'0000) * 0b1111'11111) << 8) + codeSegment[registers.IP()];
                registers.IP(registers.IP() + 1);
            }

            uint16_t memoryVal = (memory[memoryAddress + 1] << 8) + memory[memoryAddress];
            uint16_t result = memoryVal + operand + carry;

            memory[memoryAddress] = result & 0b0000'0000'1111'1111;
            memory[memoryAddress + 1] = result >> 8;
        }
        if (mod == 0b01)
        {
            uint8_t displacement = codeSegment[registers.IP()];
            registers.IP(registers.IP() + 1);
            uint32_t memoryAddress = getMemAddress(rm, displacement);
            uint16_t operand;
            if (s == 0)
            {
                operand = (codeSegment[registers.IP() + 1] << 8) + codeSegment[registers.IP()];
                registers.IP(registers.IP() + 2);
            }
            else
            {
                operand = (((codeSegment[registers.IP()] & 0b1000'0000) * 0b1111'11111) << 8) + codeSegment[registers.IP()];
                registers.IP(registers.IP() + 1);
            }
            memory[memoryAddress] = memory[memoryAddress] + operand + carry;
        }
        else if (mod == 0b10)
        {
            uint16_t displacement = (codeSegment[registers.IP() + 1] << 8) + codeSegment[registers.IP()];
            registers.IP(registers.IP() + 2);
            uint32_t memoryAddress = getMemAddress(rm, displacement);
            uint16_t operand;
            if (s == 0)
            {
                operand = (codeSegment[registers.IP() + 1] << 8) + codeSegment[registers.IP()];
                registers.IP(registers.IP() + 2);
            }
            else
            {
                operand = (((codeSegment[registers.IP()] & 0b1000'0000) * 0b1111'11111) << 8) + codeSegment[registers.IP()];
                registers.IP(registers.IP() + 1);
            }
            memory[memoryAddress] = memory[memoryAddress] + operand + carry;
        }
        else if (mod == 0b11)
        {
            uint16_t reg_value = getReg16(rm);
            uint16_t operand;
            if (s == 0)
            {
                operand = (codeSegment[registers.IP() + 1] << 8) + codeSegment[registers.IP()];
                registers.IP(registers.IP() + 2);
            }
            else
            {
                operand = (((codeSegment[registers.IP()] & 0b1000'0000) * 0b1111'11111) << 8) + codeSegment[registers.IP()];
                registers.IP(registers.IP() + 1);
            }
            setReg16(rm, reg_value + operand + carry);
        }
    }
}
