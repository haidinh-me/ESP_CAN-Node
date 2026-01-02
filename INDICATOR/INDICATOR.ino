#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include <CAN_Message.h>
#include <CAN.h>

#define FOG_LIGHT   15
#define HAZARD      14
#define LOW_BEAM    17
#define HIGHT_BEAM  16
#define TURN_RIGHT  12
#define TURN_LEFT   13
#define HEAD        5

//====================================================================================================================================//
CanSignal fog_light("FOG_LIGHT",                   19, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal hazard("HAZARD",                         13, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal low_beam("LOW_BEAM",                     25, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal hight_beam("HIGHT_BEAM",                 4, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal turn_right("TURN_RIGHT",                 0, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal turn_left("TURN_LEFT",                   7, 1, Endianness::Motorola, false, 1.0, 0.0, 0.0, 1.0);
CanSignal checksum_indicator("CHECKSUM",           34, 3, Endianness::Motorola, false, 1.0, 0.0, 0.0, 3.0);
const std::vector<CanSignal*> indicatorSignals = {
    &fog_light,
    &hazard,
    &low_beam,
    &hight_beam,
    &turn_right,
    &turn_left,
    &checksum_indicator
};
CAN_Message indicatorMsg(1092, 5, indicatorSignals);
//====================================================================================================================================//
QueueHandle_t signalQueue;

TaskHandle_t taskInputHandle = NULL;
TaskHandle_t taskCommsHandle = NULL;

uint8_t buff[8]{};

//====================================================================================================================================//
void print_signal(uint8_t *buff, CAN_Message *msg)
{
  msg->unpack_msg(buff);
  Serial.println("-----------Unpacked CAN Message Signals-----------");
  for(auto *count : msg->getSIGNAL()){
    Serial.println(count->name + String(": " + String(count->physValue)));
  }
}

void IRAM_ATTR onSwitchChange() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(taskInputHandle, &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken) portYIELD_FROM_ISR();
}



void inputTask(void *parameter) {

  uint8_t last_buff[8] = {0};

  for (;;) 
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(20));

    bool val_fog   = !digitalRead(FOG_LIGHT);
    bool val_head  = !digitalRead(HEAD);
    bool val_low   = !digitalRead(LOW_BEAM);
    bool val_high  = !digitalRead(HIGHT_BEAM);
    bool val_right = !digitalRead(TURN_RIGHT);
    bool val_left  = !digitalRead(TURN_LEFT);
    bool val_hazard= false;

    if(val_left && val_right)    val_hazard= true;

    if(!val_head) val_low = false;

    indicatorMsg.setSignal(buff, fog_light, val_fog);
    indicatorMsg.setSignal(buff, hazard, val_hazard);
    indicatorMsg.setSignal(buff, low_beam, val_low);
    indicatorMsg.setSignal(buff, hight_beam, val_high);
    indicatorMsg.setSignal(buff, turn_right, val_right);
    indicatorMsg.setSignal(buff, turn_left, val_left);

    if (memcmp(buff, last_buff, 8) != 0) 
    {
      memcpy(last_buff, buff, 8);
      
      xQueueSend(signalQueue, buff, 0);
      Serial.println("Signal Changed -> Queued");
    }
  }
}

void commsTask(void *parameter) {
  uint8_t receivedBuff[8]{};
  for (;;) {
    if (xQueueReceive(signalQueue, receivedBuff, portMAX_DELAY) == pdTRUE) {
      print_signal(receivedBuff, &indicatorMsg);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(FOG_LIGHT, INPUT_PULLUP);
  pinMode(HAZARD, INPUT_PULLUP);
  pinMode(LOW_BEAM, INPUT_PULLUP);
  pinMode(HIGHT_BEAM, INPUT_PULLUP);
  pinMode(TURN_RIGHT, INPUT_PULLUP);
  pinMode(TURN_LEFT, INPUT_PULLUP);


  attachInterrupt(digitalPinToInterrupt(FOG_LIGHT), onSwitchChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(HAZARD), onSwitchChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LOW_BEAM), onSwitchChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(HIGHT_BEAM), onSwitchChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(TURN_RIGHT), onSwitchChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(TURN_LEFT), onSwitchChange, CHANGE);

  signalQueue = xQueueCreate(20,8);

  xTaskCreatePinnedToCore(inputTask, "InputTask", 4096, NULL, 2, &taskInputHandle, 1);
  xTaskCreatePinnedToCore(commsTask, "CommsTask", 4096, NULL, 1, &taskCommsHandle, 0);
}


void loop() {
  vTaskDelete(NULL);
}
