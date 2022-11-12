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

public:
    OpCodes(Registers &registers, Flags &flags) : registers(registers), flags(flags) {}
    void executeOperation(uint8_t opCode)
    {
        switch (opCode)
        {
        }
    }
};

#endif