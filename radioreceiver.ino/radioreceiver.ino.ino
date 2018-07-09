// https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
// http://www.instructables.com/id/ESC-Programming-on-Arduino-Hobbyking-ESC/

// receiver
#include <RF24.h>
#include <Servo.h>

RF24 radio(7, 8); // CE, CSN
Servo rightServo;
Servo leftServo;
Servo motorEsc;
const byte address[6] = "00001";

int lastThrottle;
int lastLeftServo;
int lastRightServo;

const int servoDeadzone = 1;
const int servoSpeed = 2;

// the data that needs to be received by the plane
typedef struct {
  int throttle = 0;
  int leftServo = 90;
  int rightServo = 90;
} flightData;

flightData data;

void setup() {
  Serial.begin(9600);

  // init servos at 90deg
  rightServo.attach(4);
  rightServo.write(90);
  leftServo.attach(3);
  leftServo.write(90);

  //init motor at 0
  motorEsc.attach(A0);
  motorEsc.write(1000);

  delay(2000);

  radio.begin();
  radio.openReadingPipe(0, address);

  // ### USE HIGH WHEN NOT TESTING ###
  //  radio.setPALevel(RF24_PA_HIGH);
  radio.setPALevel(RF24_PA_LOW);
  
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    // receive flight data from transmitter
    radio.read(&data, sizeof(data));

    //    printData();

    int leftServoValue = leftServo.read();
    int rightServoValue = rightServo.read();

    // check that right servo values are kept in range
    if (data.rightServo - servoSpeed >= 45 && data.rightServo + servoSpeed <= 135) {
      // incrementing/decrementing by one at a time reduces chance of jitter
      if (rightServoValue < data.rightServo) {
        rightServo.write(rightServoValue + servoSpeed);
      } else if (rightServoValue > data.rightServo) {
        rightServo.write(rightServoValue - servoSpeed);
      }
    }

    // check that left servo values are kept in range
    if (data.leftServo - servoSpeed >= 45 && data.leftServo + servoSpeed <= 135) {
      if (leftServoValue < data.leftServo) {
        leftServo.write(leftServoValue + servoSpeed);
      } else if (leftServoValue > data.leftServo) {
        leftServo.write(leftServoValue - servoSpeed);
      }
    }

    if (data.throttle >= 1010 && data.throttle <= 2000) {
      motorEsc.write(data.throttle);
    } else {
      motorEsc.write(1000);
    }

    // delay to slow turns and such down
    delay(5);
  }
}

void printData() {
  Serial.print("L: ");
  Serial.print(data.leftServo);
  Serial.print(" | R: ");
  Serial.print(data.rightServo);
  Serial.print(" | T: ");
  Serial.println(data.throttle);
}
