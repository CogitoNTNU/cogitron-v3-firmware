#include <SPI.h>                //SPI comms library
#include <mcp2515.h>            //CAN comms library

MCP2515 mcp2515(13);

// Defining a sender CAN frame and a receiver CAN frame
struct can_frame CANMsg;

// Defining motor limits
float limit0 = 1.0;
float limit1 = 1.0;

// Defining a position input;
float pos;

// Defining a float based map-function;
float mapFloat(float &x, const float &in_min, const float &in_max, const float &out_min, const float &out_max){
  return (x - in_min) * (out_max - out_min)/(in_max - in_min) + out_min;
}

//Defining speed limit function
void setLimits(float &lim0, float &lim1, can_frame &frame, MCP2515 &mcp_unit) {
  const float out_max = 1.0;
  const float out_min = 0.0;
  const float in_min = 0.0;
  const float in_max = 255.0;
  float res1 = mapFloat(lim0, in_min, in_max, out_min, out_max);
  float res2 = mapFloat(lim1, in_min, in_max, out_min, out_max);
  frame.can_id = 0x00F;
  frame.can_dlc = 4;
  frame.data[0] = 0;
  frame.data[1] = 4;
  frame.data[2] = static_cast<byte>(res1);
  frame.data[3] = static_cast<byte>(res2);
  mcp_unit.sendMessage(&frame);
}

void setPos(float pos, can_frame &frame, MCP2515 &mcp_unit){
  frame.can_id = 0x00C;
  frame.can_dlc = 4;
  frame.data[0] = 0;
  frame.data[1] = 4;
  frame.data[2] = 6;
  frame.data[3] = static_cast<byte>(pos);
  mcp_unit.sendMessage(&frame);
}

void setup() {
  while(!Serial);
  Serial.begin(9600);
  Serial.println("Beginning SPI communication!");
  SPI.begin();                  //Begins SPI communication
  Serial.println("SPI communication done...");
  Serial.println("Resetting CAN module");
  mcp2515.reset();
  Serial.println("CAN Module reset...");
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);        //Sets CAN at speed 500KBPS and Clock 8MHZ
  Serial.println("CAN Bitrate set to 500KBPS...");
  mcp2515.setNormalMode();
  Serial.println("CAN Module started!");
  setLimits(limit0, limit1, CANMsg, mcp2515);
  Serial.println("Motor limits set to 1 rotation for both axes...");
}

void loop() {
  if(Serial.available() > 0){
    Serial.println("Reading input");
    byte* input = Serial.read();
    Serial.println("Converting to position");
    float angle = float(int(input)/10);
    Serial.println("Setting new position...");
    setPos(pos, CANMsg, mcp2515);
  }
}
