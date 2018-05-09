// https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/

// receiver
#include <RF24.h>
#include <Servo.h>

// pretty sure these are unnecessary to include
//#include <SPI.h>
//#include <nRF24L01.h>

RF24 radio(7, 8); // CE, CSN
Servo rightServo;
const byte address[6] = "00001";

int lastThrottle;
int lastLeftServo;
int lastRightServo;

const int servoDeadzone = 1;

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

  // init right servo at 90deg
  rightServo.attach(4);
  rightServo.write(90);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(radio.available()) {
      // receive flight data from transmitter
      radio.read(&data, sizeof(data));

    //if(data.rightServo > rightServo.read() + servoDeadZone || data.rightServo < rightServo.read() - servoDeadZone) {
    if(data.rightServo >= 45 && data.rightServo <= 135) {

      // incrementing/decrementing by one at a time reduces chance of jitter
        if(rightServo.read() < data.rightServo) {
          rightServo.write(rightServo.read() + 1);
        } else if(rightServo.read() > data.rightServo) {
          rightServo.write(rightServo.read() - 1);
        }  
      
      
      //Serial.println(data.rightServo);
      delay(3);
    } 

//      Serial.println("Received this:");
//      Serial.print("Throttle: ");
//      Serial.println(data.throttle);
//      Serial.print("Joy X: ");
//      Serial.println(data.rightServo);
//      Serial.print("Joy Y: ");
//      Serial.println(data.leftServo);
//      Serial.println();  
  }

}
