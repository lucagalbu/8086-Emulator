#ifndef EXECUTOR
#define EXECUTOR

#include <iostream>
#include "../memory/memory.hpp"
#include "../registers/registers.hpp"
#include "../flags/flags.hpp"

using namespace std;

class ModRM
{
public:
    uint8_t mod;
    uint8_t reg;
    uint8_t rm;

    ModRM(uint8_t mod, uint8_t reg, uint8_t rm) : mod(mod), reg(reg), rm(rm) {}
};

class Executor
{
private:
    Registers &registers;
    Flags &flags;
    Memory &memory;

    // Utilities
    uint8_t readByteFromIP();
    uint16_t readWordFromIP();
    ModRM getModRmFromByte(uint8_t modrm);
    uint16_t getDisplacementFromMod(uint8_t mod);
    MemoryAddress getMemAddressFromModRm(ModRM modRm);
    uint8_t getReg8(uint8_t code);
    void setReg8(uint8_t code, uint8_t value);
    uint16_t getReg16(uint8_t code);
    void setReg16(uint8_t code, uint16_t value);

    // Operations
    void AAA();
    void AAD(uint8_t param1);
    void AAM(uint8_t param1);
    void AAS();
    void ADC_ac_data(uint8_t w, uint8_t param1, uint8_t param2);
    void ADC_memreg_data(uint8_t opCode);

    // Utilities for operations
    void ADC_mem_data(uint8_t w, uint8_t s, ModRM modRm, uint8_t carry);
    void ADC_reg_data(uint8_t w, uint8_t s, uint8_t rm, uint8_t carry);

public:
    Executor(Registers &registers, Flags &flags, Memory &memory) : registers(registers), flags(flags), memory(memory)
    {
    }

    void executeNextOperation()
    {
        uint8_t opCode = readByteFromIP();
        switch (opCode)
        {
        case 0x37:
            AAA();
            break;
        case 0xD5:
            AAD(readByteFromIP());
            break;
        case 0xD4:
            AAM(readByteFromIP());
            break;
        case 0x3F:
            AAS();
            break;
        case 0x14:
            ADC_ac_data(0, readByteFromIP(), 0);
            break;
        case 0x15:
            ADC_ac_data(1, readByteFromIP(), readByteFromIP());
            break;
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
            ADC_memreg_data(opCode);
            break;
        default:
            cout << "OpCode 0x" << hex << (unsigned int)opCode << dec << " not recognized" << endl;
            exit(1);
        }
    }
};

#endif