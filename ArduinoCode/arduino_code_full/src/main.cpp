#include <Arduino.h>
#include <CAN.h>


void setup() {
  //Start Serial communication to PC/Raspberry
  Serial.begin(9600);

  while(!Serial);

  Serial.println("CAN Sender");

  if(!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while(1);
  }
}

void loop() {
  Serial.print("Sending packet ... ");

  CAN.beginPacket(0x12);
  CAN.write('h');
  CAN.write('e');
  CAN.write('l');
  CAN.write('o');
  CAN.endPacket();

  Serial.println("done");

  delay(1000);
}