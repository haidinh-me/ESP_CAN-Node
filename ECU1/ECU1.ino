#include <CAN_Message.h>
#include <CAN.h>

#define TX_GPIO_NUM   22  // Connects to CTX
#define RX_GPIO_NUM   23  // Connects to CRX

#define GEARP  18
#define GEARR  19
#define GEARN  16
#define GEARD  21
#define LEDP   14
#define LEDR   5
#define LEDN   13
#define LEDD   17

#define TEM   32
#define TEME  35
#define RPM   25
#define SPEED 33
#define FUEL  26

QueueHandle_t gearQueue;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;
const int NUM_POT = 5;
const int POT_PIN[NUM_POT] ={TEM, TEME, RPM, SPEED, FUEL};

enum GearState {
  STATE_P,
  STATE_R,
  STATE_N,
  STATE_D
};

struct Potentiometer {
  int pin;
  int lastValue;
};
Potentiometer pots[NUM_POT];

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

void IRAM_ATTR change_gp(){
  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();
    GearState msg = STATE_P;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(gearQueue, &msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}
void IRAM_ATTR change_gr(){
  if ((millis() - lastDebounceTime) > debounceDelay){
    lastDebounceTime = millis();
    GearState msg = STATE_R;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(gearQueue, &msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}
void IRAM_ATTR change_gn(){
  if ((millis() - lastDebounceTime) > debounceDelay){
    lastDebounceTime = millis();
    GearState msg = STATE_N;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(gearQueue, &msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}
void IRAM_ATTR change_gd(){
  if ((millis() - lastDebounceTime) > debounceDelay){
    lastDebounceTime = millis();
    GearState msg = STATE_D;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(gearQueue, &msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}

uint8_t buff[8]{};
int readADC(int pot){
  if(pot == TEM){
    int value = map(analogRead(pot), 0, 4095, 0, 45);
    Serial.println("TEMP: " + String(value));
    sysInforMsg->setSignal(buff, *temp_enviroment, value);
    canSender(buff, sysInforMsg);
    return value;
  }

  else if(pot == TEME){
    int value = map(analogRead(pot), 0, 4095, 0, 150);
    Serial.println("TEMPE: " + String(value));
    sysInforMsg->setSignal(buff, *temp_engine, value);
    canSender(buff, sysInforMsg);
    return value;
  }

  else if(pot == SPEED){
    int value = map(analogRead(pot), 0, 4095, 0, 240);
    Serial.println("SPEED: " + String(value));
    sysInforMsg->setSignal(buff, *car_speed, value);
    canSender(buff, sysInforMsg);
    return value;
  }
  
  else if(pot == RPM){
    int value = map(analogRead(pot), 0, 4095, 0, 10000);
    Serial.println("RPM: " + String(value));
    sysInforMsg->setSignal(buff, *rpm_engine, value);
    canSender(buff, sysInforMsg);
    return value;
  }

  else if(pot == FUEL){
    int value = map(analogRead(pot), 0, 4095, 0, 100);
    Serial.println("FUEL: " + String(value));
    sysInforMsg->setSignal(buff, *fuel_level, value);
    canSender(buff, sysInforMsg);
    return value;
  }
}

void setup() {
  Serial.begin(115200);

  analogReadResolution(12);
  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  if (!CAN.begin (500E3)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }

  pinMode(GEARP, INPUT_PULLUP);pinMode(GEARR, INPUT_PULLUP);pinMode(GEARN, INPUT_PULLUP);pinMode(GEARD, INPUT_PULLUP);
  pinMode(LEDP, OUTPUT);pinMode(LEDR, OUTPUT);pinMode(LEDN, OUTPUT);pinMode(LEDD, OUTPUT);

  for (int i = 0; i < NUM_POT; i++) {
    pots[i].pin = POT_PIN[i];
    pots[i].lastValue = -999; //init value
    pinMode(pots[i].pin, INPUT);
  }

  attachInterrupt(digitalPinToInterrupt(GEARP),change_gp,FALLING);
  attachInterrupt(digitalPinToInterrupt(GEARR),change_gr,FALLING);
  attachInterrupt(digitalPinToInterrupt(GEARN),change_gn,FALLING);
  attachInterrupt(digitalPinToInterrupt(GEARD),change_gd,FALLING);

  gearQueue = xQueueCreate(10, sizeof(GearState));
  if (gearQueue == NULL) {
    Serial.println("Create Queue fail!");
    while(1);
  }
  xTaskCreatePinnedToCore(
    otherHandle,      // Task function
    "InforTask",    // Task name
    4096,         // Stack size
    NULL,         // Parameter
    2,            // Priority
    NULL,         // Handle
    1             // Core 1
  );
  xTaskCreatePinnedToCore(gearHandle, "GearTask", 4096, NULL, 1, NULL, 0);
  Serial.println("Task created!");
}

void loop() {vTaskDelete(NULL);}

void otherHandle(void *parameter)
{
  for(;;){
    for(int i=0; i<NUM_POT; i++){
      readADC(pots[i].pin);
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void gearHandle(void *parameter) //task for GEAR
{
  GearState receivedGear;
  for(;;){
    if(xQueueReceive(gearQueue, &receivedGear, portMAX_DELAY) == pdTRUE){
      uint8_t buff[8]{};

      switch(receivedGear){
        case STATE_P:
          digitalWrite(LEDP,true);
          digitalWrite(LEDR,false);
          digitalWrite(LEDN,false);
          digitalWrite(LEDD,false);

          gearboxMsg->setSignal(buff, *gear_p, 1.0);
          gearboxMsg->setSignal(buff, *checksum_gearbox, 0.0);
          break;
        case STATE_R:
          digitalWrite(LEDP,false);
          digitalWrite(LEDR,true);
          digitalWrite(LEDN,false);
          digitalWrite(LEDD,false);

          gearboxMsg->setSignal(buff, *gear_r, 1.0);
          gearboxMsg->setSignal(buff, *checksum_gearbox, 5.0);
          break;
        case STATE_N:
          digitalWrite(LEDP,false);
          digitalWrite(LEDR,false);
          digitalWrite(LEDN,true);
          digitalWrite(LEDD,false);

          gearboxMsg->setSignal(buff, *gear_n, 1.0);
          gearboxMsg->setSignal(buff, *checksum_gearbox, 10.0);
          break;
        case STATE_D:
          digitalWrite(LEDP,false);
          digitalWrite(LEDR,false);
          digitalWrite(LEDN,false);
          digitalWrite(LEDD,true);

          gearboxMsg->setSignal(buff, *gear_d, 1.0);
          gearboxMsg->setSignal(buff, *checksum_gearbox, 15.0);
          break;
      }
      canSender(buff, gearboxMsg);
    }
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
}
