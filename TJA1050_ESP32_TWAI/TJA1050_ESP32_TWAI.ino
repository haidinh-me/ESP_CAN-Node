//==================================================================================//
#include <CAN.h>
#include "CAN_Message.h"

#define TX_GPIO_NUM   27  // Connects to CTX
#define RX_GPIO_NUM   26  // Connects to CRX

//==================================================================================//
char msgString[128];
uint8_t buff[8]{};
uint8_t store[8]{};
//==================================================================================//

//==================================================================================//
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

CanSignal *fog_light = new CanSignal("FOG_LIGHT",                   19, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *hazard = new CanSignal("HAZARD",                         13, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *low_beam = new CanSignal("LOW_BEAM",                     25, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *hight_beam = new CanSignal("HIGHT_BEAM",                 4, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *turn_right = new CanSignal("TURN_RIGHT",                 0, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *turn_left = new CanSignal("TURN_LEFT",                   7, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal *checksum_indicator = new CanSignal("CHECKSUM",           34, 3, Endianness::Motorola, false, 1.0, 0.0, 0.0, 3.0);
const std::vector<CanSignal*> indicatorSignals = {
    fog_light,
    hazard,
    low_beam,
    hight_beam,
    turn_right,
    turn_left,
    checksum_indicator
};
CAN_Message *indicatorMsg = new CAN_Message(1092, 5, indicatorSignals);
//==================================================================================//

void print_signal(uint8_t *buff, CAN_Message *msg)
{
  msg->unpack_msg(buff);
  Serial.println("-----------Unpacked CAN Message Signals-----------");
  for(auto *count : msg->getSIGNAL()){
    Serial.println(count->name + String(": " + String(count->physValue)));
  }
}

void setup() {
  Serial.begin (115200);
  while (!Serial);
  delay (1000);

  Serial.println ("CAN Receiver/Receiver");

  // Set the pins
  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);

  // start the CAN bus at 500 kbps
  if (!CAN.begin (500E3)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }

  sysInforMsg->setSignal(buff, *temp_enviroment, 24);
  sysInforMsg->setSignal(buff, *check_engine, 1.0);
  sysInforMsg->setSignal(buff, *rpm_engine, 6533.0);
  sysInforMsg->setSignal(buff, *car_speed, 175);
  sysInforMsg->setSignal(buff, *lock, 0.0);
  sysInforMsg->setSignal(buff, *temp_engine, 124.0);
  sysInforMsg->setSignal(buff, *fuel_level, 78.0);
  sysInforMsg->setSignal(buff, *checksum_sysinfor, 2.0);

}
//==================================================================================//
void canReceiver() {
  // try to parse packet
  int packetSize = CAN.parsePacket();

  if (packetSize)
  {
    if (CAN.packetExtended()) {
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (CAN.packetId() & 0x1FFFFFFF), CAN.packetDlc());
    }
    else 
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", CAN.packetId(), CAN.packetDlc());
    Serial.print(msgString);

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    }
    else 
    {
      uint8_t i = 0;
      while (CAN.available()){
        store[i] = CAN.read();
        sprintf(msgString, " 0x%.2X", store[i]);
        Serial.print(msgString);
        i++;
      }
    }

    Serial.println();

    if(CAN.packetId() == 1110) //hvacMsg
      print_signal(store,havcMsg);
    else if(CAN.packetId() == 1109) //sysInforMsg
      print_signal(store,sysInforMsg);
    else if(CAN.packetId() == 1108) //gearBoxMsg
      print_signal(store,gearboxMsg);
    else if(CAN.packetId() == 1092) // indicatorMsg 
      print_signal(store,indicatorMsg);

  }
}

void canSender(uint8_t *buff, CAN_Message *msg) {
  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
  Serial.print ("Sending packet ... ");

  CAN.beginPacket (msg->getID());
  for(uint8_t i=0; i<8; i++){
    CAN.write(buff[i]);
  }
  CAN.endPacket();
  Serial.println ("done");
  delay (500);

  /*// send extended packet: id is 29 bits, packet can contain up to 8 bytes of data
  Serial.print("Sending extended packet ... ");
  CAN.beginExtendedPacket(0xABCDEF);
  for(uint8_t i=0; i<8; i++){
    CAN.write(buff[i]);
  }
  CAN.endPacket();

  Serial.println ("done");
  delay (500);*/

}


//==================================================================================//
void loop() {
  //canSender(buff, sysInforMsg);
  canReceiver();
}
//==================================================================================//
