#include <CAN_Message.h>

//====================================================================================================================================//
CanSignal *temp_inside = new CanSignal("TMEP_INSIDE",              17, 10, Endianness::Motorola, true, 0.1, -5.0, -5.0, 52.1);
CanSignal *auto_signal = new CanSignal("AUTO_SIGNAL",              0, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *volume_level = new CanSignal("VOLUME_LEVEL",            7, 7, Endianness::Motorola, false, 1.0, 0.0, 0.0, 100.0);
CanSignal *havc_passenger = new CanSignal("HAVC_PASSENGER",        8, 6, Endianness::Motorola, false, 1.0, 0.0, 0.0, 50.0);
CanSignal *havc_driver = new CanSignal("HAVC_DRIVER",              15, 6, Endianness::Motorola, false, 1.0, 0.0, 0.0, 50.0);
CanSignal *checksum_havc = new CanSignal("CHECKSUM",               34, 3, Endianness::Motorola, false, 1.0, 0.0, 0.0, 3.0);
const std::vector<CanSignal*> havcSignals = {
    temp_inside,
    auto_signal,
    volume_level,
    havc_passenger,
    havc_driver,
    checksum_havc
};
CAN_Message *havcMsg = new CAN_Message(1110, 5, havcSignals);

CanSignal *temp_enviroment = new CanSignal("TEMP_ENVIROMENT",       54, 11, Endianness::Motorola, true, 0.1, -40.0, -40.0, 102.3);
CanSignal *check_engine = new CanSignal("CHECK_ENGINE",             16, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *rpm_engine = new CanSignal("RPM_ENGINE",                 32, 16, Endianness::Intel, false, 1.0, 0.0, 0.0, 65535.0);
CanSignal *car_speed = new CanSignal("CAR_SPEED",                   24, 8, Endianness::Intel, false, 1.0, 0.0, 0.0, 255.0);
CanSignal *lock = new CanSignal("LOCK",                             18, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *temp_engine = new CanSignal("TEMP_ENGINE",               15, 13, Endianness::Motorola, true, 0.1, -10.0, -10.0, 409.5);
CanSignal *fuel_level = new CanSignal("FUEL_LEVEL",                 7, 7, Endianness::Motorola, false, 1.0, 0.0, 0.0, 100.0);
CanSignal *checksum_sysinfor = new CanSignal("CHECKSUM",            58, 3, Endianness::Motorola, false, 1.0, 0.0, 0.0, 3.0);
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
CAN_Message *sysInforMsg = new CAN_Message(1109, 8, sysinforSignals);
//====================================================================================================================================//

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
