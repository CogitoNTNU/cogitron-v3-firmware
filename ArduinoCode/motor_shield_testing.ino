#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();


Adafruit_DCMotor *myMotor3 = AFMS.getMotor(3);
Adafruit_DCMotor *myMotor4 = AFMS.getMotor(4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AFMS.begin();
  myMotor3->setSpeed(100);
  myMotor3->run(BACKWARD);
  myMotor4->setSpeed(100);
  myMotor4->run(BACKWARD);
}

void loop() {
    // put your setup code here, to run once:
  if(Serial.available()){
    String input = Serial.readStringUntil('\n');
    int speed = input.toInt();
    if(speed == 1){
      myMotor3->run(FORWARD);
      myMotor4->run(FORWARD);
    }
    else if(speed == 2){
      myMotor3->run(BACKWARD);
      myMotor4->run(BACKWARD);
    }
    else if(speed == 3){
      myMotor3->run(RELEASE);
      myMotor4->run(RELEASE);
    }
  }

  

}
