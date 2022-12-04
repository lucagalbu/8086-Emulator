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

    void AAA();
    void AAD(uint8_t param1);
    void AAM(uint8_t param1);
    void AAS();
    void ADC(uint8_t w, uint8_t param1, uint8_t param2);

    uint8_t getInstructionAddress() { return (registers.CS() + registers.IP()); }

public:
    Executor(Registers &registers, Flags &flags, const uint8_t *memory) : registers(registers), flags(flags), memory(memory) {}
    void executeNextOperation()
    {
        uint8_t codeAddress = getInstructionAddress();
        uint8_t opCode = memory[codeAddress];
        switch (opCode)
        {
        case 0x37:
            AAA();
            registers.IP(registers.IP() + 1);
            break;
        case 0xD5:
            AAD(memory[++codeAddress]);
            registers.IP(registers.IP() + 2);
            break;
        case 0xD4:
            AAM(memory[++codeAddress]);
            registers.IP(registers.IP() + 2);
            break;
        case 0x3F:
            AAS();
            registers.IP(registers.IP() + 1);
            break;
        case 0x14:
            ADC(0, memory[++codeAddress], 0);
            registers.IP(registers.IP() + 2);
            break;
        case 0x15:
            ADC(1, memory[codeAddress + 1], memory[codeAddress + 2]);
            registers.IP(registers.IP() + 3);
            break;
        default:
            cout << "OpCode 0x" << hex << (unsigned int)opCode << dec << " not recognized" << endl;
            exit(1);
        }
    }
};

#endif