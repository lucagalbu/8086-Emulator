#ifndef OPCODES
#define OPCODES

#include <unordered_map>
#include "../registers/registers.hpp"
#include "../flags/flags.hpp"

using namespace std;

class OpCodes
{
private:
    Registers &registers;
    Flags &flags;
    const uint8_t *memory;

    void AAA();

    uint8_t getInstructionAddress() { return (registers.CS() + registers.IP()); }

public:
    OpCodes(Registers &registers, Flags &flags, const uint8_t *memory) : registers(registers), flags(flags), memory(memory) {}
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
        }
    }
};

#endif