#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../memory/memory.hpp"

TEST_CASE("Test memory is initialized to zero")
{
    Memory memory;

    for (size_t address = 0; address < 0x100000; address++)
    {
        REQUIRE(memory.readByte(MemoryAddress(0, address)) == 0);
    }
}

TEST_CASE("Test memory set/read byte")
{
    Memory memory;

    // Location where to set the byte
    uint16_t segment = 0xABC0;
    uint16_t offset = 0xD5E6;
    uint8_t byteToSet = 0x43;

    // Set the byte
    memory.setByte(MemoryAddress(segment, offset), byteToSet);

    // Check the byte has been set while all other bytes in the segment have not been set
    for (size_t i = 0; i < 0xFFFF; i++)
    {
        if (i == offset)
        {
            REQUIRE(memory.readByte(MemoryAddress(segment, i)) == byteToSet);
        }
        else
        {
            REQUIRE(memory.readByte(MemoryAddress(segment, i)) == 0);
        }
    }
}

TEST_CASE("Test memory read word")
{
    Memory memory;

    // Location where to set the byte
    uint16_t segment = 0xAB00;
    uint16_t offset = 0xD5E6;

    // Set the word using setByte which has been tested in a previous test
    memory.setByte(MemoryAddress(segment, offset), 0xAB);
    memory.setByte(MemoryAddress(segment, offset + 1), 0xCD);

    // Check the word is read correctly
    REQUIRE(memory.readWord(MemoryAddress(segment, offset)) == 0xCDAB);
}

TEST_CASE("Test memory write word")
{
    Memory memory;

    // Location where to set the byte
    uint16_t segment = 0xABC0;
    uint16_t offset = 0xD5E6;

    // Set the word
    memory.setWord(MemoryAddress(segment, offset), 0xABCD);

    // Check the word has been set corectly using readByte (which has been tested previously)
    REQUIRE(memory.readByte(MemoryAddress(segment, offset)) == 0xCD);
    REQUIRE(memory.readByte(MemoryAddress(segment, offset + 1)) == 0xAB);
}
