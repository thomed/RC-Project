// https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
// http://www.instructables.com/id/ESC-Programming-on-Arduino-Hobbyking-ESC/

// receiver
#include <RF24.h>
#include <Servo.h>

// pretty sure these are unnecessary to include
//#include <SPI.h>
//#include <nRF24L01.h>

RF24 radio(7, 8); // CE, CSN
Servo rightServo;
Servo leftServo;
Servo motorEsc;
const byte address[6] = "00001";

int lastThrottle;
int lastLeftServo;
int lastRightServo;

const int servoDeadzone = 1;
const int servoSpeed = 1;

// the data that needs to be received by the plane
typedef struct {
  int throttle = 0;
  int leftServo = 90;
  int rightServo = 90;
} flightData;

flightData data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // init servos at 90deg
  rightServo.attach(4);
  rightServo.write(90);
  leftServo.attach(3);
  leftServo.write(90);

  //init motor at 0
  motorEsc.attach(5);
  motorEsc.write(1000);

  delay(2000);
  
  radio.begin();
  radio.openReadingPipe(0, address);
//  radio.setPALevel(RF24_PA_HIGH);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(radio.available()) {
    // receive flight data from transmitter
    radio.read(&data, sizeof(data));

//    printData();

    int leftServoValue = leftServo.read();
    int rightServoValue = rightServo.read();
    
    //if(data.rightServo > rightServo.read() + servoDeadZone || data.rightServo < rightServo.read() - servoDeadZone) {
    if(data.rightServo >= 45 && data.rightServo <= 135) {  
        // incrementing/decrementing by one at a time reduces chance of jitter
        if(rightServoValue < data.rightServo) {
          rightServo.write(rightServoValue + servoSpeed);
        } else if(rightServoValue > data.rightServo) {
          rightServo.write(rightServoValue - servoSpeed);
        } 
    } 

    if(data.leftServo >= 45 && data.leftServo <= 135) {
        
        if(leftServoValue < data.leftServo) {
          leftServo.write(leftServoValue + servoSpeed);
        } else if(leftServoValue > data.leftServo) {
          leftServo.write(leftServoValue - servoSpeed);
        } 
    }

    if(data.throttle >= 1010 && data.throttle <= 2000) {
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
