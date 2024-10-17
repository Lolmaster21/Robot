#include <Servo.h>

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define MOTORSPEED 5
#define CW 3
#define CCW 4

RF24 radio(9, 8); // CE, SSN

struct DataPacket { //Packet class
  bool buttonState;
  int motorSpeed;
  bool motorDirection;
  int servoAngle;
};

DataPacket data;
Servo myservo; //Initialize Servo


const byte address[6] = "24769"; // Our id address

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTORSPEED, OUTPUT);
  pinMode(CW, OUTPUT);
  pinMode(CCW, OUTPUT);
  myservo.attach(7);
  radio.begin();//Tell the RF library to start radio communication
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, address);//set transmission address to 5-digit number above
  //Set module as transmitter
  radio.startListening();
  Serial.begin(9600);//you better know what this does
}

void loop() {
  if (radio.available()) {//if a signal is available
    radio.read(&data, sizeof(DataPacket));
    delay(20);//20ms delay to unpack signal

    //we can now use variables in this void loop(). SICK!
    analogWrite(MOTORSPEED,data.motorSpeed);
    digitalWrite(CW, data.motorDirection);
    digitalWrite(CCW, !data.motorDirection);
    myservo.write(data.servoAngle);

    Serial.print("Button State: ");
    Serial.println(data.buttonState);
    Serial.print("Motor Speed: ");
    Serial.println(data.motorSpeed);
    Serial.print("Motor Direction: ");
    Serial.println(data.motorDirection);

  }
}
