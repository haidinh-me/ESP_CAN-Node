#include "CAN_Message.h"

CAN_Message::CAN_Message()
    : ID(0), DLC(0)
{}

CAN_Message::CAN_Message(uint32_t id, int dlc, const std::vector<CanSignal*>  &signal)
    : ID(id), DLC(dlc), SIGNAL{signal}
{}

/*-------------------------------------------------------------------------------------------*/
static inline int nextMotorolaBit(int currentPos) 
{
    int byteIdx = currentPos / 8;
    int bitIdx = currentPos % 8;
    
    if (bitIdx == 0) 
        return (byteIdx + 1) * 8 + 7;
    else 
        return currentPos - 1;
}

static inline void setBit(uint8_t* buffer, int pos, int val) 
{
    int byteIdx = pos / 8;
    int bitIdx = pos % 8;
    if (val) buffer[byteIdx] |= (1 << bitIdx);
    else     buffer[byteIdx] &= ~(1 << bitIdx);
}

static inline int getBit(const uint8_t* buffer, int pos) 
{
    int byteIdx = pos / 8;
    int bitIdx = pos % 8;
    return (buffer[byteIdx] >> bitIdx) & 1;
}
    
static void pack(uint8_t* buffer, CanSignal& sig, double value) 
{
    if (value < sig.minValue) value = sig.minValue;
    if (value > sig.maxValue) value = sig.maxValue;

    int64_t raw_signed = (int64_t)round((value - sig.offset) / sig.factor);

    uint64_t raw = (uint64_t)raw_signed;

    sig.rawValue = static_cast<double>(raw_signed); 
    
    uint64_t mask = (1ULL << sig.length) - 1;
    raw &= mask;
    
    if (sig.endian == Endianness::Intel) 
    { // Little Endian
        for (int i = 0; i < sig.length; ++i) {
            int bitVal = (raw >> i) & 1;
            int pos = sig.startBit + i; 
            setBit(buffer, pos, bitVal);
        }
    } else 
    { // Motorola (Big Endian)
        
        int pos = sig.startBit; 
        
        for (int i = sig.length - 1; i >= 0; --i) 
        {
            int bitVal = (raw >> i) & 1;
            setBit(buffer, pos, bitVal);
            
            pos = nextMotorolaBit(pos);
        }
    }
}

// --- UNPACK ---
static void unpack(const uint8_t* buffer, CanSignal& sig) 
{
    uint64_t raw = 0;

    if (sig.endian == Endianness::Intel) 
    {
        for (int i = 0; i < sig.length; ++i) {
            int pos = sig.startBit + i;
            if (getBit(buffer, pos)) 
                raw |= ((uint64_t)1 << i);
        }
    } 
    else
    { // Motorola
        int pos = sig.startBit;
        
        for (int i = sig.length - 1; i >= 0; --i) 
        {
            if (getBit(buffer, pos))
                raw |= ((uint64_t)1 << i);

            pos = nextMotorolaBit(pos);
        }
    }

    int64_t final_raw_signed = 0;

    if (sig.isSigned)
    {
        uint64_t sign_bit_mask = (1ULL << (sig.length - 1));
        
        if (raw & sign_bit_mask) 
        {
            uint64_t mask = (1ULL << sig.length) - 1;
            raw |= ~mask;
            
            final_raw_signed = (int64_t)raw; 
        }
        else
        {
            final_raw_signed = (int64_t)raw;
        }
    }
    else
    {
        final_raw_signed = (int64_t)raw;
    }

    sig.rawValue = static_cast<double>(final_raw_signed);
    sig.physValue = sig.rawValue * sig.factor + sig.offset;
}
/*-------------------------------------------------------------------------------------------*/

void CAN_Message::setSignal(uint8_t *buff, CanSignal &sig, double value)
{
    pack(buff, sig, value);
}

double CAN_Message::unpack_msg(const uint8_t* buf)
{
    for (auto* sig : SIGNAL) 
    {
        unpack(buf, *sig);
    }
    return 0.0;
}
