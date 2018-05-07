#include <Servo.h>

Servo servo;
int pos = 90; // starting position of servo
int x_0, y_0; // starting x, y of joy
int x,y;      // current x, y of joy

// old deadzone
//int deadzone = 10; // deadzone of joystick
int deadzone = 1; // deadzone of joystick

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // probably wouldn't use the switch in a airplane controller
//  pinMode(2, INPUT);      // JOY SWITCH
//  digitalWrite(2, HIGH); // if not using INPUT_PULLUP, need to send current to joy
  
  servo.attach(10);
  servo.write(pos);

  // get starting x, y
  x_0 = analogRead(1);
  y_0 = analogRead(0);
}

void loop() {
  // put your main code here, to run repeatedly:
    pos = getNewPos();

    if(servo.read() != pos){
      servo.write(pos);
    }
    delay(50); // default was 10, but with new system higher numbers might be fine
}

// Fine tuning needed for recalculating pos. Currently reaches
// the max/min values prematurely.
// 
// Look into using a capacitor on servos...
int getNewPos() {
    x = analogRead(1);
    y = analogRead(0);

    // calculate new position based on joystick value, relative to servo start at 90
    // with a maximum difference of 45 degrees
    double degdiff = (((double)x / x_0) * 45) - 45;

    int newPos = 90 + (int)degdiff;

    // if new position is different enough from the old one, then apply change
    if(pos + deadzone > newPos || pos - deadzone < newPos) {
      return newPos;
    }

//    print useful info
//    Serial.print("Joy:\t");
//    Serial.print(x);
//    Serial.print("\tDegdiff:\t");
//    Serial.print(degdiff);
//    Serial.print("\tPos:\t");
//    Serial.println(pos);

    return pos;
}

