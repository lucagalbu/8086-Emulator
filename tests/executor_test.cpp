#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "trompeloeil/doctest/trompeloeil.hpp"

#include "../memory/memory.hpp"
#include "../registers/registers.hpp"
#include "../flags/flags.hpp"
#include "../executor/executor.hpp"

#include <iostream>

class Flags_mocked : public Flags
{
public:
    MAKE_CONST_MOCK1(isSet, bool(FlagMasks flag));
    MAKE_MOCK1(set, void(FlagMasks flag));
    MAKE_MOCK1(unset, void(FlagMasks flag));
};

class Memory_mocked : public Memory
{
public:
    MAKE_MOCK1(readByte, uint8_t(MemoryAddress));
    MAKE_MOCK2(setByte, void(MemoryAddress, uint8_t));
    MAKE_MOCK1(readWord, uint16_t(MemoryAddress));
    MAKE_MOCK2(setWord, void(MemoryAddress, uint16_t));
};

TEST_CASE("Test AAA")
{
    Registers registers;
    Flags_mocked flags_mocked;
    Memory_mocked memory_mocked;

    Executor executor(registers, flags_mocked, memory_mocked);

    registers.AX(0);

    SUBCASE("Test with low nibble AL>9 and flag A unset")
    {
        ALLOW_CALL(flags_mocked, isSet(Flags::A)).RETURN(false);
        REQUIRE_CALL(flags_mocked, set(Flags::A));
        REQUIRE_CALL(flags_mocked, set(Flags::C));
        ALLOW_CALL(memory_mocked, readByte(trompeloeil::_)).RETURN(0x37);

        registers.AL(0b0000'1111);
        executor.executeNextOperation();

        REQUIRE(registers.AX() == 0x0105);
    };

    SUBCASE("Test with low nibble AL<9 and flag A unset")
    {
        REQUIRE_CALL(flags_mocked, isSet(Flags::A)).RETURN(false);
        REQUIRE_CALL(flags_mocked, unset(Flags::A));
        REQUIRE_CALL(flags_mocked, unset(Flags::C));
        ALLOW_CALL(memory_mocked, readByte(trompeloeil::_)).RETURN(0x37);

        registers.AL(0b0110'0001);
        executor.executeNextOperation();

        REQUIRE(registers.AX() == 0x1);
    };

    SUBCASE("Test with low nibble AL<9 and flag A set")
    {
        REQUIRE_CALL(flags_mocked, isSet(Flags::A)).RETURN(true);
        REQUIRE_CALL(flags_mocked, set(Flags::A));
        REQUIRE_CALL(flags_mocked, set(Flags::C));
        ALLOW_CALL(memory_mocked, readByte(trompeloeil::_)).RETURN(0x37);

        registers.AL(0b0110'0001);
        executor.executeNextOperation();

        REQUIRE(registers.AX() == 0x0107);
    };
}

TEST_CASE("Test ADC")
{
    Registers registers;
    Flags_mocked flags_mocked;
    Memory_mocked memory_mocked;

    Executor executor(registers, flags_mocked, memory_mocked);

    SUBCASE("Test with mem/data")
    {
        ALLOW_CALL(memory_mocked, readWord(MemoryAddress(0xE400, 0x0040))).RETURN(0x6B90);
        REQUIRE_CALL(memory_mocked, setWord(MemoryAddress(0xE400, 0x0040), 0x98C1));

        ALLOW_CALL(memory_mocked, readByte(MemoryAddress(0, 0))).RETURN(0x81);
        ALLOW_CALL(memory_mocked, readByte(MemoryAddress(0, 1))).RETURN(0x14);
        ALLOW_CALL(memory_mocked, readWord(MemoryAddress(0, 2))).RETURN(0x2D31);

        REQUIRE_CALL(flags_mocked, isSet(Flags::C)).RETURN(false);

        registers.DS(0xE400);
        registers.SI(0x0040);
        executor.executeNextOperation();
    }
}
