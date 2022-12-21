#ifndef MEMORY
#define MEMORY

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <memory>
#include <algorithm>

using namespace std;

class Memory
{
public:
    Memory()
    {
        memory = make_unique<uint8_t[]>(memorySizeByte);
        fill_n(memory.get(), memorySizeByte, 0);
    }

    uint8_t readByte(uint16_t segment, uint16_t offset);
    uint16_t readWord(uint16_t segment, uint16_t offset);
    void setByte(uint16_t segment, uint16_t offset, uint8_t byte);
    void setWord(uint16_t segment, uint16_t offset, uint16_t word);

    void load(uint16_t segment, uint16_t offset, uint8_t *data, size_t numBytes);
    void print(uint16_t segment, uint16_t offset, size_t numBytes = 16);

private:
    const size_t memorySizeByte = 0x100000;
    unique_ptr<uint8_t[]> memory;

    uint32_t makeAddress(uint16_t segment, uint16_t offset);
};

#endif