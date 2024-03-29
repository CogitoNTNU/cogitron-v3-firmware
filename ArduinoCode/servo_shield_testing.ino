#include <Wire.h>
#include <string.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  72 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  540 // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;


int prevangle_1 = 81;
int prevangle_2 = 75;
int num_ops = 50;

void setup() {
  Serial.begin(9600);
  pwm.begin();

  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(10);
}

void set_angle(int servonum,int angle) {
  uint16_t pulselength = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servonum, 0, pulselength);
  delay(1);
}

void loop() {
  // Drive each servo one at a time using setPWM()
    int x;

  if(Serial.available()){
    String input = Serial.readStringUntil('\n');
    int index = input.indexOf(',');

    int angle1 = input.substring(0, index).toInt();
    int angle2 = input.substring(index +1).toInt();
    
    for (int i = 0; i < num_ops; i++) {
      int target1 = angle1 - prevangle_1;
      int temp = target1 * i / num_ops;
      target1 = prevangle_1 + temp;

      int target2 = angle2 - prevangle_2;
      temp = target2 * i / num_ops;
      target2 = prevangle_2 + temp;

      set_angle(0, target1);
      set_angle(1, target2);
    }

    prevangle_1 = angle1;
    prevangle_2 = angle2;

    delay(10);
  }

}
