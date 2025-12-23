#ifndef CAN_MESSAGE_H
#define CAN_MESSAGE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iomanip>

enum class Endianness { Intel, Motorola };

struct CanSignal {
    const char* name; 
    int startBit;       
    int length; 
    Endianness endian;
    bool isSigned;        
    double factor;      
    double offset;      
    double minValue;
    double maxValue;

    double rawValue;
    double physValue;

    CanSignal(const char* n, int sb, int l, Endianness e, bool issigned, double f, double o, double minv, double maxv)
        : name(n), startBit(sb), length(l), endian(e), isSigned(issigned), 
          factor(f), offset(o), minValue(minv), maxValue(maxv), rawValue(0), physValue(0) 
        {}
};

class CAN_Message
{
public:

    CAN_Message();

    CAN_Message(uint32_t id, int dlc, const std::vector<CanSignal*>  &signal);

    double unpack_msg(const uint8_t* buf);

    uint32_t inline getID() const { return ID; }

    int inline getDLC() const { return DLC; }

    std::vector<CanSignal*> inline getSIGNAL() const {return SIGNAL;}
    
    void setSignal(uint8_t *buff, CanSignal &sig, double value);

private:
    uint32_t ID{};
    int DLC{};
    std::vector<CanSignal*> SIGNAL{};
};

#endif // CAN_MESSAGE_H