#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../registers/registers.hpp"
#include "../flags/flags.hpp"
#include "../executor/executor.hpp"

TEST_CASE("Test AAA")
{
    Registers registers;
    Flags flags;
    uint8_t memory[1];
    Executor executor(registers, flags, memory);

    memory[0] = 0x37;
    registers.AX(0);

    SUBCASE("Test with low nibble AL>9")
    {
        registers.AL(0b0000'1111);
        executor.executeNextOperation();

        bool are_CA_set = flags.isSet(Flags::A) && flags.isSet(Flags::C);
        bool are_any_other_set = flags.isSet(Flags::O) ||
                                 flags.isSet(Flags::D) ||
                                 flags.isSet(Flags::I) ||
                                 flags.isSet(Flags::T) ||
                                 flags.isSet(Flags::S) ||
                                 flags.isSet(Flags::Z) ||
                                 flags.isSet(Flags::P);

        REQUIRE(registers.AX() == 0x0105);
        REQUIRE(are_CA_set);
        REQUIRE_FALSE(are_any_other_set);
    };

    SUBCASE("Test with low nibble AL<9 and flag A unset")
    {
        flags.unset(Flags::A);
        flags.set(Flags::C);
        registers.AL(0b0110'0001);
        executor.executeNextOperation();

        bool is_any_flag_set = flags.isSet(Flags::A) ||
                               flags.isSet(Flags::C) ||
                               flags.isSet(Flags::O) ||
                               flags.isSet(Flags::D) ||
                               flags.isSet(Flags::I) ||
                               flags.isSet(Flags::T) ||
                               flags.isSet(Flags::S) ||
                               flags.isSet(Flags::Z) ||
                               flags.isSet(Flags::P);

        REQUIRE(registers.AX() == 0x1);
        REQUIRE_FALSE(is_any_flag_set);
    };

    SUBCASE("Test with low nibble AL<9 and flag A set")
    {
        flags.set(Flags::A);
        flags.unset(Flags::C);
        registers.AL(0b0110'0001);
        executor.executeNextOperation();

        bool are_CA_set = flags.isSet(Flags::A) && flags.isSet(Flags::C);
        bool are_any_other_set = flags.isSet(Flags::O) ||
                                 flags.isSet(Flags::D) ||
                                 flags.isSet(Flags::I) ||
                                 flags.isSet(Flags::T) ||
                                 flags.isSet(Flags::S) ||
                                 flags.isSet(Flags::Z) ||
                                 flags.isSet(Flags::P);

        REQUIRE(registers.AX() == 0x0107);
        REQUIRE(are_CA_set);
        REQUIRE_FALSE(are_any_other_set);
    };
}
