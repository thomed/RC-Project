// https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
//
// sending structs:
// http://www.bajdi.com/sending-structs-with-nrf24l01-modules-and-the-rf24-library/

// transmitter
#include <RF24.h>
#include <math.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001"; // address needs to be the same for sending/receiving

// joy pins
// orient pins to the left
// set a median value on setup? ( or average between max/min? )
int joyY = A0; // middle ~= 494, top = 1023, bottom = 0
int joyYInit = 494;
int joyX = A1; // middle ~= 510, far left = 1023, right = 0
int joyXInit = 510;

// logarithmic
// header end = 0, opposite = 1023
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
//
//  // change initial x value if abnormal
//  if (analogRead(joyX) > joyXInit + 1 || analogRead(joyX) < joyXInit - 1) {
//    joyXInit = analogRead(joyX);
//  }
//
//  // change initial y value if abnormal
//  if (analogRead(joyY) > joyYInit + 1 || analogRead(joyY) < joyYInit - 1) {
//    joyYInit = analogRead(joyY);
//  }

    // delay to stabilize current (fixed init and joy rest inconsistency)
    delay(500);
    joyXInit = analogRead(joyX);
    joyYInit = analogRead(joyY);

  // init throttle potentiometer
  // need to do max/min to calibrate ESC???
  pinMode(throttlePin, INPUT);

  // init radio for transmitting
  radio.begin();
  radio.openWritingPipe(address);
  //  radio.setPALevel(RF24_PA_HIGH);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

void loop() {
  int val = analogRead(throttlePin);
  val = map(val, 0, 1023, 1000, 2000);
  data.throttle = val;

  // update servo values based on joystick position before sending
  updateJoys();

  radio.write(&data, sizeof(data));
  //  printData();
  //delay(25);
}

/*
   Elevon actions:
   Roll right: Left up, right down
   Roll left: Right up, left down
   Pitch down: Both go up
   Pitch up: Both go down
*/
void updateJoys() {
  int joyXValue = analogRead(joyX);
  int joyYValue = analogRead(joyY);

  // find what amount  between -45 and 45 degrees to move right servo
  /*
   * Joy x movement % = joyXValue / joyXInit
   * 
   */
//  Serial.print("X%: ");
//  Serial.print((double)joyXValue / joyXInit);
//  Serial.print(" Y%: ");
//  Serial.println((double)joyYValue / joyYInit);

  double diffX = (((double)joyXValue / joyXInit) * 45) - 45;
  double diffY = (((double)joyYValue / joyYInit) * 45) - 45;
  double diff = (diffX + diffY) / 2;
  data.rightServo = 90 + (int)diff;
  data.leftServo = 90 - (int)diff;

  printRawData(((double)joyXValue / (double)joyXInit), ((double)joyYValue / (double)joyYInit));
//  printRawData(joyXValue, joyXInit);
  
//  double rightDegDiff = (((double) joyXValue / joyXInit) * 45) - 45;
//  data.rightServo = 90 + (int)rightDegDiff;
//
//  //  double leftDegDiff = (((double) joyYValue / joyYInit) * 45) - 45;
//  //  data.leftServo = 90 + (int)leftDegDiff;
//  data.leftServo = 90 - (int)rightDegDiff;

  // Serial.println(data.leftServo);
}

void printData() {
  Serial.print("L: ");
  Serial.print(data.leftServo);
  Serial.print(" | R: ");
  Serial.print(data.rightServo);
  Serial.print(" | T: ");
  Serial.println(data.throttle);
}

void printRawData(double x, double y) {
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.println(y);
}


