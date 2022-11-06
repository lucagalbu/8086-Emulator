#ifndef REGISTERS
#define REGISTERS

class Registers
{
private:
    // General purpose
    uint16_t AX_;
    uint16_t BX_;
    uint16_t CX_;
    uint16_t DX_;

    // Segment
    uint16_t CS_;
    uint16_t DS_;
    uint16_t SS_;
    uint16_t ES_;

    // Pointer
    uint16_t SP_;
    uint16_t BP_;
    uint16_t IP_;

    // Index
    uint16_t SI_;
    uint16_t DI_;

public:
    Registers() : AX_(0),
                  BX_(0),
                  CX_(0),
                  DX_(0),
                  CS_(0),
                  DS_(0),
                  SS_(0),
                  ES_(0),
                  SP_(0),
                  BP_(0),
                  IP_(0),
                  SI_(0),
                  DI_(0)
    {
    }

    // whole registers
    uint16_t AX() { return AX_; }
    uint16_t BX() { return BX_; }
    uint16_t CX() { return CX_; }
    uint16_t DX() { return DX_; }
    uint16_t CS() { return CS_; }
    uint16_t DS() { return DS_; }
    uint16_t SS() { return SS_; }
    uint16_t ES() { return ES_; }
    uint16_t SP() { return SP_; }
    uint16_t BP() { return BP_; }
    uint16_t IP() { return IP_; }
    uint16_t SI() { return SI_; }
    uint16_t DI() { return DI_; }

    void AX(uint16_t val) { AX_ = val; }
    void BX(uint16_t val) { BX_ = val; }
    void CX(uint16_t val) { CX_ = val; }
    void DX(uint16_t val) { DX_ = val; }
    void CS(uint16_t val) { CS_ = val; }
    void DS(uint16_t val) { DS_ = val; }
    void SS(uint16_t val) { SS_ = val; }
    void ES(uint16_t val) { ES_ = val; }
    void SP(uint16_t val) { SP_ = val; }
    void BP(uint16_t val) { BP_ = val; }
    void IP(uint16_t val) { IP_ = val; }
    void SI(uint16_t val) { SI_ = val; }
    void DI(uint16_t val) { DI_ = val; }

    // half registers
    // note that conversion from 16 to 8 bit integers drops the higher byte,
    // so I could drop the mask with 0xFF. But I leave it for clarity.
    uint8_t AL() { return AX_ & 0x00FF; }
    uint8_t AH() { return (AX_ >> 8) & 0x00FF; }
    void AL(uint8_t val) { AX_ = (AH() << 8) | val; }
    void AH(uint8_t val) { AX_ = (val << 8) | AL(); }

    uint8_t BL() { return BX_ & 0x00FF; }
    uint8_t BH() { return (BX_ >> 8) & 0x00FF; }
    void BL(uint8_t val) { BX_ = (BH() << 8) | val; }
    void BH(uint8_t val) { BX_ = (val << 8) | BL(); }

    uint8_t CL() { return CX_ & 0x00FF; }
    uint8_t CH() { return (CX_ >> 8) & 0x00FF; }
    void CL(uint8_t val) { CX_ = (CH() << 8) | val; }
    void CH(uint8_t val) { CX_ = (val << 8) | CL(); }

    uint8_t DL() { return DX_ & 0x00FF; }
    uint8_t DH() { return (DX_ >> 8) & 0x00FF; }
    void DL(uint8_t val) { DX_ = (DH() << 8) | val; }
    void DH(uint8_t val) { DX_ = (val << 8) | DL(); }
};

#endif