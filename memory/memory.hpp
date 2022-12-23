#ifndef MEMORY
#define MEMORY

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <memory>
#include <algorithm>

using namespace std;

struct MemoryAddress
{
    uint16_t segment;
    uint16_t offset;

    MemoryAddress(uint16_t segment, uint16_t offset) : segment(segment), offset(offset){};
    MemoryAddress() : segment(0), offset(0) {}

    bool operator==(MemoryAddress const &other) const
    {
        return this->segment == other.segment && this->offset == other.offset;
    }
};

class Memory
{
public:
    Memory()
    {
        memory = make_unique<uint8_t[]>(memorySizeByte);
        fill_n(memory.get(), memorySizeByte, 0);
    }

    virtual uint8_t readByte(MemoryAddress address);
    virtual uint16_t readWord(MemoryAddress address);
    virtual void setByte(MemoryAddress address, uint8_t byte);
    virtual void setWord(MemoryAddress address, uint16_t word);

    void load(MemoryAddress address, uint8_t *data, size_t numBytes);
    void print(MemoryAddress address, size_t numBytes = 16);

private:
    const size_t memorySizeByte = 0x100000;
    unique_ptr<uint8_t[]> memory;

    uint32_t getPhysicalAddress(MemoryAddress address);
};

#endif