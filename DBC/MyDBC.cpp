#include <iostream>
#include <vector>
#include "CAN_Message.cpp"

/*
BO_ 1110 HAVC: 5 SYSTEM
 SG_ TMEP_INSIDE : 17|10@0- (0.1,-5) [-5|52.1] "degree c"  DISPLAY
 SG_ AUTO_SIGNAL : 0|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ VOLUME_LEVEL : 7|7@0+ (1,0) [0|100] "%"  DISPLAY
 SG_ HAVC_PASSENGER : 8|6@0+ (1,0) [0|50] "degree c"  DISPLAY
 SG_ HAVC_DRIVER : 15|6@0+ (1,0) [0|50] "degree c"  DISPLAY
 SG_ CHECKSUM : 34|3@0+ (1,0) [0|3] ""  DISPLAY

BO_ 1109 SYS_INFOR: 8 SYSTEM
 SG_ TEMP_ENVIROMENT : 54|11@0- (0.1,-40) [-40|102.3] ""  DISPLAY
 SG_ CHECK_ENGINE : 16|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ RPM_ENGINE : 32|16@1+ (1,0) [0|65535] "rpm"  DISPLAY
 SG_ CAR_SPEED : 24|8@1+ (1,0) [0|255] "kph"  DISPLAY
 SG_ LOCK : 18|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ TEMP_ENGINE : 15|13@0- (0.1,-10) [-10|409.5] "degree c"  DISPLAY
 SG_ FUEL_LEVEL : 7|7@0+ (1,0) [0|100] "%"  DISPLAY
 SG_ CHECKSUM : 58|3@0+ (1,0) [0|3] ""  DISPLAY

BO_ 1108 GEARBOX_INFOR: 6 GEAR
 SG_ GEAR_D : 15|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ GEAR_N : 2|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ GEAR_R : 21|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ GEAR_P : 27|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ CHECKSUM : 34|5@0+ (1,0) [0|15] ""  DISPLAY

BO_ 1092 INDICATOR_BLOCK: 5 INDICATOR
 SG_ FOG_LIGHT : 19|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ HAZARD : 13|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ LOW_BEAM : 25|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ HIGHT_BEAM : 4|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ TURN_RIGHT : 0|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ TURN_LEFT : 7|1@0+ (1,0) [0|1] ""  DISPLAY
 SG_ CHECKSUM : 34|3@0+ (1,0) [0|3] ""  DISPLAY

*/

CanSignal*  temp_inside = new CanSignal("TMEP_INSIDE",              17, 10, Endianness::Motorola, true, 0.1, -5.0, -5.0, 52.1);
CanSignal*  auto_signal = new CanSignal("AUTO_SIGNAL",              0, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal*  volume_level = new CanSignal("VOLUME_LEVEL",            7, 7, Endianness::Motorola, false, 1.0, 0.0, 0.0, 100.0);
CanSignal*  havc_passenger = new CanSignal("HAVC_PASSENGER",        8, 6, Endianness::Motorola, false, 1.0, 0.0, 0.0, 50.0);
CanSignal*  havc_driver = new CanSignal("HAVC_DRIVER",              15, 6, Endianness::Motorola, false, 1.0, 0.0, 0.0, 50.0);
CanSignal*  checksum_havc = new CanSignal("CHECKSUM",               34, 3, Endianness::Motorola, false, 1.0, 0.0, 0.0, 3.0);
const std::vector<CanSignal*> havcSignals = {
    temp_inside,
    auto_signal,
    volume_level,
    havc_passenger,
    havc_driver,
    checksum_havc
};
CAN_Message *havcMsg = new CAN_Message(0x456, 5, havcSignals);


CanSignal* temp_enviroment = new CanSignal("TEMP_ENVIROMENT",       54, 11, Endianness::Motorola, true, 0.1, -40.0, -40.0, 102.3);
CanSignal* check_engine = new CanSignal("CHECK_ENGINE",             16, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal* rpm_engine = new CanSignal("RPM_ENGINE",                 32, 16, Endianness::Intel, false, 1.0, 0.0, 0.0, 65535.0);
CanSignal* car_speed = new CanSignal("CAR_SPEED",                   24, 8, Endianness::Intel, false, 1.0, 0.0, 0.0, 255.0);
CanSignal* lock = new CanSignal("LOCK",                             18, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal* temp_engine = new CanSignal("TEMP_ENGINE",               15, 13, Endianness::Motorola, true, 0.1, -10.0, -10.0, 409.5);
CanSignal* fuel_level = new CanSignal("FUEL_LEVEL",                 7, 7, Endianness::Motorola, false, 1.0, 0.0, 0.0, 100.0);
CanSignal* checksum_sysinfor = new CanSignal("CHECKSUM",            58, 3, Endianness::Motorola, false, 1.0, 0.0, 0.0, 3.0);
const std::vector<CanSignal*> sysinforSignals = {
    temp_enviroment,
    check_engine,
    rpm_engine,
    car_speed,
    lock,
    temp_engine,
    fuel_level,
    checksum_sysinfor
};
CAN_Message *sysInforMsg = new CAN_Message(0x455, 8, sysinforSignals);


CanSignal* gear_d = new CanSignal("GEAR_D",                         15, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal* gear_n = new CanSignal("GEAR_N",                         2, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal* gear_r = new CanSignal("GEAR_R",                         21, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal* gear_p = new CanSignal("GEAR_P",                         27, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal* checksum_gearbox = new CanSignal("CHECKSUM",             34, 5, Endianness::Motorola, false, 1.0, 0.0, 0.0, 15.0);  
const std::vector<CanSignal*> gearboxSignals = {
    gear_d,
    gear_n,
    gear_r,
    gear_p,
    checksum_gearbox
};
CAN_Message *gearboxMsg = new CAN_Message(0x454, 6, gearboxSignals);


CanSignal* fog_light = new CanSignal("FOG_LIGHT",                   19, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal* hazard = new CanSignal("HAZARD",                         13, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal* low_beam = new CanSignal("LOW_BEAM",                     25, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal* hight_beam = new CanSignal("HIGHT_BEAM",                 4, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal* turn_right = new CanSignal("TURN_RIGHT",                 0, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal* turn_left = new CanSignal("TURN_LEFT",                   7, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal* checksum_indicator = new CanSignal("CHECKSUM",           34, 3, Endianness::Motorola, false, 1.0, 0.0, 0.0, 3.0);
const std::vector<CanSignal*> indicatorSignals = {
    fog_light,
    hazard,
    low_beam,
    hight_beam,
    turn_right,
    turn_left,
    checksum_indicator
};
CAN_Message *indicatorMsg = new CAN_Message(0x444, 5, indicatorSignals);

void print_signal(uint8_t *buff, CAN_Message *msg)
{
    std::cout << "Packed CAN Message Data Succecssfully: ";
    std::cout <<"ID: " << msg->getID() << "    DLC: " << msg->getDLC() << "    Data: ";
    for(int i = 0; i < msg->getDLC(); ++i)
        std::cout <<std::hex <<std::uppercase << static_cast<int>(buff[i]) << " ";
    std::cout << std::dec << std::endl;

    msg->unpack_msg(buff);
    std::cout<< "-----------Unpacked CAN Message Signals-----------"<< std::endl;
    for(auto *count : msg->getSIGNAL())
    {
        std::cout<< count->name << ": "<< count->physValue << std::endl;
    }
}

void pack_signal_HAVC()
{
    uint8_t buff[5]{};

    havcMsg->setSignal(buff, *temp_inside, 43.63);
    havcMsg->setSignal(buff, *auto_signal, 0.0);
    havcMsg->setSignal(buff, *volume_level, 67.0);
    havcMsg->setSignal(buff, *havc_passenger, 23.0);
    havcMsg->setSignal(buff, *havc_driver, 16.0);
    havcMsg->setSignal(buff, *checksum_havc, 1.0);

    print_signal(buff, havcMsg);
}

void pack_signal_SYSINFOR()
{
    uint8_t buff[8]{};

    sysInforMsg->setSignal(buff, *temp_enviroment, 24);
    sysInforMsg->setSignal(buff, *check_engine, 1.0);
    sysInforMsg->setSignal(buff, *rpm_engine, 6533.0);
    sysInforMsg->setSignal(buff, *car_speed, 175);
    sysInforMsg->setSignal(buff, *lock, 0.0);
    sysInforMsg->setSignal(buff, *temp_engine, 124.0);
    sysInforMsg->setSignal(buff, *fuel_level, 78.0);
    sysInforMsg->setSignal(buff, *checksum_sysinfor, 2.0);

    print_signal(buff, sysInforMsg);
}

void pack_signal_GEARBOX()
{
    uint8_t buff[6]{};
    
    gearboxMsg->setSignal(buff, *gear_p, 0.0);
    gearboxMsg->setSignal(buff, *gear_r, 1.0);
    gearboxMsg->setSignal(buff, *gear_n, 0.0);
    gearboxMsg->setSignal(buff, *gear_d, 0.0);
    gearboxMsg->setSignal(buff, *checksum_gearbox, 15.0);

    print_signal(buff, gearboxMsg);
}

void pack_signal_INDICATOR()
{
    uint8_t buff[5]{};

    indicatorMsg->setSignal(buff , *fog_light, 1.0);
    indicatorMsg->setSignal(buff , *hazard, 1.0);
    indicatorMsg->setSignal(buff , *low_beam, 1.0);
    indicatorMsg->setSignal(buff , *hight_beam, 1.0);
    indicatorMsg->setSignal(buff , *turn_right, 1.0);
    indicatorMsg->setSignal(buff , *turn_left, 1.0);
    indicatorMsg->setSignal(buff , *checksum_indicator, 3.0);

    print_signal(buff, indicatorMsg);
}

int main()
{
    pack_signal_HAVC();
    pack_signal_SYSINFOR();
    pack_signal_GEARBOX();
    pack_signal_INDICATOR();
    return 0;
}   