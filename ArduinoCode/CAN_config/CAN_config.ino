#include <SPI.h>                //SPI comms library
#include <mcp2515.h>            //CAN comms library

struct can_frame canMsg;

MCP2515 mcp2515(10);

void setup() {
  while(!Serial);
  Serial.begin(9600);

  SPI.begin();                  //Begins SPI communication

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);        //Sets CAN at speed 500KBPS and Clock 8MHZ
  mcp2515.setNormalMode();
}

void loop() {
  canMsg.can_id = 0x036;                      //CAN id can be changed
  canMsg.can_dlc = 8;                         //CAN data length chosen at 8
  canMsg.data[0] = 3;
  canMsg.data[1] = 0;                     //Update values in the CAN bus
  canMsg.data[2] = 0;
  canMsg.data[3] = 0;
  canMsg.data[4] = 0;
  canMsg.data[5] = 0;
  canMsg.data[6] = 0;
  canMsg.data[7] = 0;
  //......

  mcp2515.sendMessage(&canMsg);                   //Sends CAN message
  delay(1000);
}
