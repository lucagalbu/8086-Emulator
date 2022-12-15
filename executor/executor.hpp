#ifndef EXECUTOR
#define EXECUTOR

#include <iostream>
#include "../registers/registers.hpp"
#include "../flags/flags.hpp"

using namespace std;

class Executor
{
private:
    Registers &registers;
    Flags &flags;

    uint8_t *memory;
    uint8_t *codeSegment;
    uint8_t *dataSegment;
    uint8_t *stackSegment;
    uint8_t *extraSegment;

    uint32_t getMemAddress(uint8_t rm, uint16_t displacement = 0)
    {
        switch (rm)
        {
        case 0b000:
            return (registers.DS() * 0x10) + registers.BX() + registers.SI() + displacement;
            break;
        case 0b001:
            return (registers.DS() * 0x10) + registers.BX() + registers.DI() + displacement;
            break;
        case 0b010:
            return (registers.SS() * 0x10) + registers.BP() + registers.SI() + displacement;
            break;
        case 0b011:
            return (registers.SS() * 0x10) + registers.BP() + registers.DI() + displacement;
            break;
        case 0b100:
            return (registers.DS() * 0x10) + registers.SI() + displacement;
            break;
        case 0b101:
            return (registers.DS() * 0x10) + registers.DI() + displacement;
            break;
        case 0b110:
            return (registers.SS() * 0x10) + registers.BP() + displacement;
            break;
        case 0b111:
            return (registers.DS() * 0x10) + registers.BX() + registers.SI() + displacement;
            break;
        default:
            cerr << hex;
            cerr << "rm=" << (unsigned int)rm << " of modRM not recognized." << endl;
            cerr << dec;
            exit(1);
        }
    }

    uint8_t getReg8(uint8_t code)
    {
        switch (code)
        {
        case 0x0:
            return (registers.AL());
            break;
        case 0x1:
            return (registers.CL());
            break;
        case 0x2:
            return (registers.DL());
            break;
        case 0x3:
            return (registers.BL());
            break;
        case 0x4:
            return (registers.AH());
            break;
        case 0x5:
            return (registers.CH());
            break;
        case 0x6:
            return (registers.DH());
            break;
        case 0x7:
            return (registers.BH());
            break;
        default:
            cerr << "reg code not recognized" << endl;
            exit(1);
        }
    }

    void setReg8(uint8_t code, uint8_t value)
    {
        switch (code)
        {
        case 0x0:
            registers.AL(value);
            break;
        case 0x1:
            registers.CL(value);
            break;
        case 0x2:
            registers.DL(value);
            break;
        case 0x3:
            registers.BL(value);
            break;
        case 0x4:
            registers.AH(value);
            break;
        case 0x5:
            registers.CH(value);
            break;
        case 0x6:
            registers.DH(value);
            break;
        case 0x7:
            registers.BH(value);
            break;
        default:
            cerr << "reg code not recognized" << endl;
            exit(1);
        }
    }

    uint16_t getReg16(uint8_t code)
    {
        switch (code)
        {
        case 0x0:
            return (registers.AX());
            break;
        case 0x1:
            return (registers.CX());
            break;
        case 0x2:
            return (registers.DX());
            break;
        case 0x3:
            return (registers.BX());
            break;
        case 0x4:
            return (registers.SP());
            break;
        case 0x5:
            return (registers.BP());
            break;
        case 0x6:
            return (registers.SI());
            break;
        case 0x7:
            return (registers.DI());
            break;
        default:
            cerr << "reg code not recognized" << endl;
            exit(1);
        }
    }

    void setReg16(uint8_t code, uint16_t value)
    {
        switch (code)
        {
        case 0x0:
            registers.AX(value);
            break;
        case 0x1:
            registers.CX(value);
            break;
        case 0x2:
            registers.DX(value);
            break;
        case 0x3:
            registers.BX(value);
            break;
        case 0x4:
            registers.SP(value);
            break;
        case 0x5:
            registers.BP(value);
            break;
        case 0x6:
            registers.SI(value);
            break;
        case 0x7:
            registers.DI(value);
            break;
        default:
            cerr << "reg code not recognized" << endl;
            exit(1);
        }
    }

    void AAA();
    void AAD(uint8_t param1);
    void AAM(uint8_t param1);
    void AAS();
    void ADC_ac_data(uint8_t w, uint8_t param1, uint8_t param2);
    void ADC_memreg_data();

public:
    Executor(Registers &registers, Flags &flags, uint8_t *memory, const size_t segment_size_byte) : registers(registers), flags(flags), memory(memory)
    {
        codeSegment = &memory[registers.CS() * segment_size_byte];
        dataSegment = &memory[registers.DS() * segment_size_byte];
        stackSegment = &memory[registers.SS() * segment_size_byte];
        extraSegment = &memory[registers.ES() * segment_size_byte];
    }

    void executeNextOperation()
    {
        uint8_t codeOffset = registers.IP();
        uint8_t opCode = codeSegment[codeOffset];
        switch (opCode)
        {
        case 0x37:
            AAA();
            registers.IP(registers.IP() + 1);
            break;
        case 0xD5:
            AAD(codeSegment[++codeOffset]);
            registers.IP(registers.IP() + 2);
            break;
        case 0xD4:
            AAM(codeSegment[++codeOffset]);
            registers.IP(registers.IP() + 2);
            break;
        case 0x3F:
            AAS();
            registers.IP(registers.IP() + 1);
            break;
        case 0x14:
            ADC_ac_data(0, codeSegment[++codeOffset], 0);
            registers.IP(registers.IP() + 2);
            break;
        case 0x15:
            ADC_ac_data(1, codeSegment[codeOffset + 1], codeSegment[codeOffset + 2]);
            registers.IP(registers.IP() + 3);
            break;
        case 0x81:
            ADC_memreg_data();
            break;
        default:
            cout << "OpCode 0x" << hex << (unsigned int)opCode << dec << " not recognized" << endl;
            exit(1);
        }
    }
};

#endif