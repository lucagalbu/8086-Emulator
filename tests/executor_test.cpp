#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "trompeloeil/doctest/trompeloeil.hpp"

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

TEST_CASE("Test AAA")
{
    Registers registers;
    Flags_mocked flags_mocked;
    uint8_t memory[1];
    Executor executor(registers, flags_mocked, memory);

    memory[0] = 0x37;
    registers.AX(0);

    SUBCASE("Test with low nibble AL>9 and flag A unset")
    {
        ALLOW_CALL(flags_mocked, isSet(Flags::A)).RETURN(false);
        REQUIRE_CALL(flags_mocked, set(Flags::A));
        REQUIRE_CALL(flags_mocked, set(Flags::C));

        registers.AL(0b0000'1111);
        executor.executeNextOperation();

        REQUIRE(registers.AX() == 0x0105);
    };

    SUBCASE("Test with low nibble AL<9 and flag A unset")
    {
        REQUIRE_CALL(flags_mocked, isSet(Flags::A)).RETURN(false);
        REQUIRE_CALL(flags_mocked, unset(Flags::A));
        REQUIRE_CALL(flags_mocked, unset(Flags::C));

        registers.AL(0b0110'0001);
        executor.executeNextOperation();

        REQUIRE(registers.AX() == 0x1);
    };

    SUBCASE("Test with low nibble AL<9 and flag A set")
    {
        REQUIRE_CALL(flags_mocked, isSet(Flags::A)).RETURN(true);
        REQUIRE_CALL(flags_mocked, set(Flags::A));
        REQUIRE_CALL(flags_mocked, set(Flags::C));

        registers.AL(0b0110'0001);
        executor.executeNextOperation();

        REQUIRE(registers.AX() == 0x0107);
    };
}
