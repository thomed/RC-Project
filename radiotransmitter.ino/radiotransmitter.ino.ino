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
int joyX = A1; // middle ~= 510, far left = 1023, right = 0

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

  // init throttle potentiometer
  pinMode(throttlePin, INPUT);

  // init joy
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
 

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
  data.leftServo = analogRead(joyX);
  data.rightServo = analogRead(joyY);

//  Serial.print("send data size: ");
//  Serial.println(sizeof(data));
  Serial.print("Throttle = ");
  Serial.println(data.throttle);
  Serial.print("Joy X = ");
  Serial.println(data.leftServo);
  Serial.print("Joy Y = ");
  Serial.println(data.rightServo);
  Serial.println();

  radio.write(&data, sizeof(data));
  //const char text[] = "Hello World";
  //radio.write(&text, sizeof(text));
  delay(200);
}
