// https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
//
// sending structs: 
// http://www.bajdi.com/sending-structs-with-nrf24l01-modules-and-the-rf24-library/

// transmitter
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001"; // address needs to be the same for sending/receiving

// joy pins
// orient pins to the left
// set a middle value on setup? ( or average between max/min? )
int joyY = A0; // middle ~= 494, top = 1023, bottom = 0
int joyYInit = 494;
int joyX = A1; // middle ~= 510, far left = 1023, right = 0
int joyXInit = 510;

// logarithmic
// header end = 0, opposite = 1023
// might need to connect other headers for full range of values
int throttlePin = A2;

// the data that needs to be sent to the plane's receiver
// interpreted/calculated by the inputs from the controller
typedef struct {
  int throttle;
  int leftServo;
  int rightServo;
} flightData;

flightData data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  // init joy
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
  
  // change initial x value if abnormal
  if(analogRead(joyX) > joyXInit + 2 || analogRead(joyX) < joyXInit - 2) {
    joyXInit = analogRead(joyX);
  }

  // change initial y value if abnormal
  if(analogRead(joyY) > joyYInit + 2 || analogRead(joyY) - joyYInit - 2) {
    joyYInit = analogRead(joyY);
  }

  // init throttle potentiometer
  // need to do max/min to calibrate ESC???
  pinMode(throttlePin, INPUT);

  // init radio for transmitting
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:

  // need to math this stuff and get real, workable values
  data.throttle = analogRead(throttlePin);

//  data.leftServo = analogRead(joyX);
//  data.rightServo = analogRead(joyY);

//  Serial.println("Sending this:");
//  Serial.print("Throttle = ");
//  Serial.println(data.throttle);
//  Serial.print("Joy X = ");
//  Serial.println(data.leftServo);
//  Serial.print("Joy Y = ");
//  Serial.println(data.rightServo);
//  Serial.println();

  // update servo values based on joystick position before sending
  updateJoys();
  
  radio.write(&data, sizeof(data));
}

void updateJoys() {
  int joyXValue = analogRead(joyX);
  int joyYValue = analogRead(joyY);
  
  // find what amount  between -45 and 45 degrees to move right servo
  double rightDegDiff = (((double) joyXValue / joyXInit) * 45) - 45;
  data.rightServo = 90 + (int)rightDegDiff;

  //Serial.println(data.rightServo);
}


