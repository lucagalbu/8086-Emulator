#include "executor.hpp"

uint8_t Executor::readByteFromIP()
{
    MemoryAddress address(registers.CS(), registers.IP());

    uint8_t operand = memory.readByte(address);
    registers.IP(registers.IP() + 1);
    return operand;
}

uint16_t Executor::readWordFromIP()
{
    MemoryAddress address(registers.CS(), registers.IP());

    uint16_t operand = memory.readWord(address);
    registers.IP(registers.IP() + 2);
    return operand;
}

ModRM Executor::getModRmFromByte(uint8_t modRm)
{
    uint8_t mod = (modRm >> 6) & 0b011;
    uint8_t reg = (modRm >> 3) & 0b111;
    uint8_t rm = modRm & 0b011;

    return ModRM(mod, reg, rm);
}

uint16_t Executor::getDisplacementFromMod(uint8_t mod)
{
    uint16_t displacement = 0;
    switch (mod)
    {
    case 0b00:
        displacement = 0;
        break;
    case 0b01:
        displacement = readByteFromIP();
        break;
    case 0x10:
        displacement = readWordFromIP();
        break;
    default:
        cerr << hex;
        cerr << "mod=" << (unsigned int)mod << " not recognized as displacement." << endl;
        cerr << dec;
        exit(1);
    }

    return displacement;
}

MemoryAddress Executor::getMemAddressFromModRm(ModRM modRm)
{
    uint16_t displacement = getDisplacementFromMod(modRm.mod);
    MemoryAddress memoryAddress;

    switch (modRm.rm)
    {
    case 0b000:
        memoryAddress.segment = registers.DS();
        memoryAddress.offset = registers.BX() + registers.SI() + displacement;
        break;
    case 0b001:
        memoryAddress.segment = registers.DS();
        memoryAddress.offset = registers.BX() + registers.DI() + displacement;
        break;
    case 0b010:
        memoryAddress.segment = registers.SS();
        memoryAddress.offset = registers.BP() + registers.SI() + displacement;
        break;
    case 0b011:
        memoryAddress.segment = registers.SS();
        memoryAddress.offset = registers.BP() + registers.DI() + displacement;
        break;
    case 0b100:
        memoryAddress.segment = registers.DS();
        memoryAddress.offset = registers.SI() + displacement;
        break;
    case 0b101:
        memoryAddress.segment = registers.DS();
        memoryAddress.offset = registers.DI() + displacement;
        break;
    case 0b110:
        memoryAddress.segment = registers.SS();
        memoryAddress.offset = registers.BP() + displacement;
        break;
    case 0b111:
        memoryAddress.segment = registers.DS();
        memoryAddress.offset = registers.BX() + registers.SI() + displacement;
        break;
    default:
        cerr << hex;
        cerr << "rm=" << (unsigned int)modRm.rm << " of modRM not recognized." << endl;
        cerr << dec;
        exit(1);
    }

    return memoryAddress;
}

uint8_t Executor::getReg8(uint8_t code)
{
    switch (code)
    {
    case 0x0:
        return (registers.AL());
        break;
    case 0x1:
        return (registers.CL());
        break;
    case 0x2:
        return (registers.DL());
        break;
    case 0x3:
        return (registers.BL());
        break;
    case 0x4:
        return (registers.AH());
        break;
    case 0x5:
        return (registers.CH());
        break;
    case 0x6:
        return (registers.DH());
        break;
    case 0x7:
        return (registers.BH());
        break;
    default:
        cerr << "reg code not recognized" << endl;
        exit(1);
    }
}

void Executor::setReg8(uint8_t code, uint8_t value)
{
    switch (code)
    {
    case 0x0:
        registers.AL(value);
        break;
    case 0x1:
        registers.CL(value);
        break;
    case 0x2:
        registers.DL(value);
        break;
    case 0x3:
        registers.BL(value);
        break;
    case 0x4:
        registers.AH(value);
        break;
    case 0x5:
        registers.CH(value);
        break;
    case 0x6:
        registers.DH(value);
        break;
    case 0x7:
        registers.BH(value);
        break;
    default:
        cerr << "reg code not recognized" << endl;
        exit(1);
    }
}

uint16_t Executor::getReg16(uint8_t code)
{
    switch (code)
    {
    case 0x0:
        return (registers.AX());
        break;
    case 0x1:
        return (registers.CX());
        break;
    case 0x2:
        return (registers.DX());
        break;
    case 0x3:
        return (registers.BX());
        break;
    case 0x4:
        return (registers.SP());
        break;
    case 0x5:
        return (registers.BP());
        break;
    case 0x6:
        return (registers.SI());
        break;
    case 0x7:
        return (registers.DI());
        break;
    default:
        cerr << "reg code not recognized" << endl;
        exit(1);
    }
}

void Executor::setReg16(uint8_t code, uint16_t value)
{
    switch (code)
    {
    case 0x0:
        registers.AX(value);
        break;
    case 0x1:
        registers.CX(value);
        break;
    case 0x2:
        registers.DX(value);
        break;
    case 0x3:
        registers.BX(value);
        break;
    case 0x4:
        registers.SP(value);
        break;
    case 0x5:
        registers.BP(value);
        break;
    case 0x6:
        registers.SI(value);
        break;
    case 0x7:
        registers.DI(value);
        break;
    default:
        cerr << "reg code not recognized" << endl;
        exit(1);
    }
}
