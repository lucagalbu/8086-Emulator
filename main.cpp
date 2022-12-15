#include <iostream>
#include "registers/registers.hpp"
#include "flags/flags.hpp"
#include "executor/executor.hpp"

using namespace std;

#define MEMORY_SIZE_BYTE 1024 * 1024
#define SEGMENT_SIZE_BYTE 1

int main(int argc, char **argv)
{
    uint8_t *memory = new uint8_t[MEMORY_SIZE_BYTE];

    Registers registers;
    Flags flags;
    Executor executor(registers, flags, memory, SEGMENT_SIZE_BYTE);

    memory[0] = 0x15;
    memory[1] = 0x81;
    memory[2] = 0xFD;
    cout << hex;
    cout << (unsigned int)registers.AX() << " - Sign: " << (flags.isSet(Flags::S) ? "set" : "not set") << " - Overflow: " << (flags.isSet(Flags::O) ? "set" : "not set") << " - Carry: " << (flags.isSet(Flags::C) ? "set" : "not set") << endl;
    registers.AX(0x4F3D);
    flags.set(Flags::C);
    cout << (unsigned int)registers.AX() << " - " << (flags.isSet(Flags::A) ? "flag is set" : "flag not set") << endl;
    executor.executeNextOperation();
    cout << (unsigned int)registers.AX() << " - Sign: " << (flags.isSet(Flags::S) ? "set" : "not set") << " - Overflow: " << (flags.isSet(Flags::O) ? "set" : "not set") << " - Carry: " << (flags.isSet(Flags::C) ? "set" : "not set") << endl;
    cout << (unsigned int)registers.IP() << endl;
    return (0);
}
