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

    const uint8_t *memory;
    const uint8_t *codeSegment;
    const uint8_t *dataSegment;
    const uint8_t *stackSegment;
    const uint8_t *extraSegment;

    void AAA();
    void AAD(uint8_t param1);
    void AAM(uint8_t param1);
    void AAS();
    void ADC(uint8_t w, uint8_t param1, uint8_t param2);

public:
    Executor(Registers &registers, Flags &flags, const uint8_t *memory, const size_t segment_size_byte) : registers(registers), flags(flags), memory(memory)
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
            AAD(memory[++codeOffset]);
            registers.IP(registers.IP() + 2);
            break;
        case 0xD4:
            AAM(memory[++codeOffset]);
            registers.IP(registers.IP() + 2);
            break;
        case 0x3F:
            AAS();
            registers.IP(registers.IP() + 1);
            break;
        case 0x14:
            ADC(0, memory[++codeOffset], 0);
            registers.IP(registers.IP() + 2);
            break;
        case 0x15:
            ADC(1, memory[codeOffset + 1], memory[codeOffset + 2]);
            registers.IP(registers.IP() + 3);
            break;
        default:
            cout << "OpCode 0x" << hex << (unsigned int)opCode << dec << " not recognized" << endl;
            exit(1);
        }
    }
};

#endif