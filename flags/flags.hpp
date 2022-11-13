#ifndef FLAGS
#define FLAGS

#include <cstdint>

class Flags
{
private:
    uint16_t flags;

public:
    // position of the flags as binary mask
    enum FlagMasks
    {
        S = 0b000000001,
        Z = 0b000000010,
        A = 0b000000100,
        P = 0b000001000,
        C = 0b000010000,
        O = 0b000100000,
        D = 0b001000000,
        I = 0b010000000,
        T = 0b100000000
    };

    // getters
    bool isSet(FlagMasks flag) const { return (flags & flag); }

    // setters
    void set(FlagMasks flag) { flags |= flag; }
    void unset(FlagMasks flag) { flags &= ~flag; }

    // constructor
    Flags() : flags(0b0) {}

    // helper functions
    static bool checkParity(uint16_t number);
};

#endif
