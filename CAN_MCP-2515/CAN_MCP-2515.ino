// CAN Send Example
//

#include <mcp_can.h>
#include <SPI.h>
#include "CAN_Message.h"

#define CAN0_INT 22

MCP_CAN CAN0(5);  
long unsigned int rxId;
unsigned char len = 0;
uint8_t rxBuf[8];
uint8_t buff[8]{};
char msgString[128];


//-----------------------------------------------------------------------------------------------------------------------------//
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
//-----------------------------------------------------------------------------------------------------------------------------//

void setup()
{
  Serial.begin(115200);
  SPI.begin();
  pinMode(CAN0_INT,OUTPUT);

  // Initialize MCP2515 running at 8MHz with a baudrate of 500kb/s and the masks and filters disabled.
  //Mặc dù là MCP-2515 có thạch anh ngoại là 8MHz nhưng phải cấu hình theo đúng default là MCP_16MHZ mới chạy được, cấu hình MCP_8MHZ là không chạy được!
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted

  sysInforMsg->setSignal(buff, *temp_enviroment, 24);
  sysInforMsg->setSignal(buff, *check_engine, 1.0);
  sysInforMsg->setSignal(buff, *rpm_engine, 6533.0);
  sysInforMsg->setSignal(buff, *car_speed, 175);
  sysInforMsg->setSignal(buff, *lock, 0.0);
  sysInforMsg->setSignal(buff, *temp_engine, 124.0);
  sysInforMsg->setSignal(buff, *fuel_level, 78.0);
  sysInforMsg->setSignal(buff, *checksum_sysinfor, 2.0);
}

void MCP_2515_Sender(uint8_t *buff, CAN_Message *msg)
{
  byte sndStat;
  /*// send extended packet: id is 29 bits, packet can contain up to 8 bytes of data
  byte sndStat = CAN0.sendMsgBuf(0x14DCEF3B, 1, 8, buff);
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
  delay(500);   // send data per 500ms*/

  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
  sndStat = CAN0.sendMsgBuf(msg->getID(), 0, 8, buff);
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
  delay(500);   // send data per 500ms
}

void MCP_2515_Receiver()
{
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  
    Serial.print(msgString);
  
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }
    Serial.println();

    if(rxId == 1110) //hvacMsg
      print_signal(rxBuf,havcMsg);
    else if(rxId == 1109) //sysInforMsg
      print_signal(rxBuf,sysInforMsg);
    else if(rxId == 1108) //gearBoxMsg
      print_signal(rxBuf,gearboxMsg);
    else if(rxId == 1092) // indicatorMsg 
      print_signal(rxBuf,indicatorMsg);
  }
}

void print_signal(uint8_t *buff, CAN_Message *msg)
{
  msg->unpack_msg(buff);
  Serial.println("-----------Unpacked CAN Message Signals-----------");
  for(auto *count : msg->getSIGNAL()){
    Serial.println(count->name + String(": " + String(count->physValue)));
  }
}
void loop()
{
  MCP_2515_Sender(buff, sysInforMsg);
  //MCP_2515_Receiver();
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
