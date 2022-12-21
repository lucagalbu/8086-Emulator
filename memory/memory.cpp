#include "memory.hpp"

#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

uint32_t Memory::getPhysicalAddress(MemoryAddress address)
{
    uint32_t physicalAddress = (address.segment << 4) + address.offset;

    if (physicalAddress > memorySizeByte)
    {
        cerr << "Memory address out of range" << endl;
        exit(1);
    }

    return physicalAddress;
}

uint8_t Memory::readByte(MemoryAddress address)
{
    uint32_t physicalAddress = getPhysicalAddress(address);
    return memory[physicalAddress];
}

void Memory::setByte(MemoryAddress address, uint8_t byte)
{
    uint32_t physicalAddress = getPhysicalAddress(address);
    memory[physicalAddress] = byte;
}

uint16_t Memory::readWord(MemoryAddress address)
{
    uint32_t physicalAddress = getPhysicalAddress(address);

    uint8_t lowByte = memory[physicalAddress];
    uint8_t highByte = memory[physicalAddress + 1];

    return (highByte << 8) + lowByte;
}

void Memory::setWord(MemoryAddress address, uint16_t word)
{
    uint32_t physicalAddress = getPhysicalAddress(address);

    uint8_t lowByte = word & 0xFF;
    uint8_t highByte = (word >> 8) & 0xFF;

    memory[physicalAddress] = lowByte;
    memory[physicalAddress + 1] = highByte;
}

void Memory::load(MemoryAddress address, uint8_t *data, size_t numBytes)
{
    uint32_t physicalAddress = getPhysicalAddress(address);
    copy_n(data, numBytes, &memory[physicalAddress]);
}

void Memory::print(MemoryAddress address, size_t numBytes)
{
    uint32_t physicalAddress = getPhysicalAddress(address);

    cout << hex;
    cout << setfill('0') << std::setw(5) << physicalAddress << "\t";
    for (size_t i = physicalAddress; i < physicalAddress + numBytes; i += 2)
    {
        uint16_t value = (memory[i] << 8) + memory[i + 1];
        cout << setfill('0') << std::setw(4) << (unsigned int)value;
        cout << " ";
    }

    cout << endl;
    cout << dec;
}