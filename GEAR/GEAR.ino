#include <CAN_Message.h>

//====================================================================================================================================//
CanSignal *gear_d = new CanSignal("GEAR_D",                         15, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *gear_n = new CanSignal("GEAR_N",                         2, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *gear_r = new CanSignal("GEAR_R",                         21, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *gear_p = new CanSignal("GEAR_P",                         27, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *checksum_gearbox = new CanSignal("CHECKSUM",             34, 5, Endianness::Motorola, false, 1.0, 0.0, 0.0, 15.0);
const std::vector<CanSignal*> gearboxSignals = {
    gear_d,
    gear_n,
    gear_r,
    gear_p,
    checksum_gearbox
};
CAN_Message *gearboxMsg = new CAN_Message(1108, 6, gearboxSignals);
//====================================================================================================================================//

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
