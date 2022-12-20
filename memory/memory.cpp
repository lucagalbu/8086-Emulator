#include "memory.hpp"

#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

uint32_t Memory::makeAddress(uint16_t segment, uint16_t offset)
{
    uint32_t address = (segment << 4) + offset;

    if (address > memorySizeByte)
    {
        cerr << "Memory address out of range" << endl;
        exit(1);
    }

    return address;
}

uint8_t Memory::readByte(uint16_t segment, uint16_t offset)
{
    uint32_t address = makeAddress(segment, offset);
    return memory[address];
}

void Memory::setByte(uint16_t segment, uint16_t offset, uint8_t byte)
{
    uint32_t address = makeAddress(segment, offset);
    memory[address] = byte;
}

uint16_t Memory::readWord(uint16_t segment, uint16_t offset)
{
    uint32_t address = makeAddress(segment, offset);

    uint8_t lowByte = memory[address];
    uint8_t highByte = memory[address + 1];

    return (highByte << 8) + lowByte;
}

void Memory::setWord(uint16_t segment, uint16_t offset, uint16_t word)
{
    uint32_t address = makeAddress(segment, offset);

    uint8_t lowByte = word & 0xFF;
    uint8_t highByte = (word >> 8) & 0xFF;

    memory[address] = lowByte;
    memory[address + 1] = highByte;
}

void Memory::load(uint16_t segment, uint16_t offset, uint8_t *data, size_t numBytes)
{
    uint32_t address = makeAddress(segment, offset);
    memcpy(&memory[address], data, numBytes);
}

void Memory::print(uint16_t segment, uint16_t offset, size_t numBytes)
{
    uint32_t address = makeAddress(segment, offset);

    cout << hex;
    cout << setfill('0') << std::setw(4) << address << "\t";
    for (size_t i = address; i < address + numBytes; i += 2)
    {
        uint16_t value = (memory[i] << 8) + memory[i + 1];
        cout << setfill('0') << std::setw(4) << (unsigned int)value;
        cout << " ";
    }

    cout << endl;
    cout << dec;
}