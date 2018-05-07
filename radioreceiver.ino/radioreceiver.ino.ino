// https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/

// receiver
#include <RF24.h>

// pretty sure these are unnecessary to include
//#include <SPI.h>
//#include <nRF24L01.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

// the data that needs to be received by the plane
typedef struct {
  int throttle;
  int leftServo;
  int rightServo;
} flightData;

flightData data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(radio.available()) {
//    char text[32] = "";
//    radio.read(&text, sizeof(text));
//    Serial.println(text);
    radio.read(&data, sizeof(data));
    Serial.print("Throttle: ");
    Serial.println(data.throttle);
    Serial.print("Joy X: ");
    Serial.println(data.leftServo);
    Serial.print("Joy Y: ");
    Serial.println(data.rightServo);
    Serial.println();
  }
}
