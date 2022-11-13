#include "flags.hpp"

bool Flags::checkParity(uint16_t number)
{
    int numberOnes = 0;

    while (number)
    {
        numberOnes += number & 0b1;
        number = number >> 1;
    }

    return (numberOnes % 2 == 0 ? true : false);
}

bool Flags::checkSign(uint16_t number)
{
    return (number >> 15 ? true : false);
}

bool Flags::checkSign(uint8_t number)
{
    return (number >> 7 ? true : false);
}