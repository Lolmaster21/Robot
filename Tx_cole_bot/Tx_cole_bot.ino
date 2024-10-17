#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define BUTTON 2
#define POTENTIOMETER 2
#define SERVOP 0

RF24 radio(9, 8); // CE, SSN

struct DataPacket { //Packet class
  bool buttonState;
  int motorSpeed;
  bool motorDirection;
  int servoAngle;
};

DataPacket data;

const byte address[6] = "24769"; // Our id address

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON, INPUT_PULLUP);
  radio.begin();//Tell the RF library to start radio communication
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);//set transmission address to 5-digit number above
  //Set module as transmitter
  radio.stopListening();
  Serial.begin(9600);//you better know what this does
}

void loop() {
  data.buttonState = digitalRead(BUTTON);
  data.motorSpeed = abs(map(analogRead(POTENTIOMETER),0,1024,-255,255)); // speed
  data.motorDirection = map(analogRead(POTENTIOMETER),0,1024, 0,1); // direction
  data.servoAngle = map(analogRead(SERVOP), 0, 1024, 0, 180);
  if (map(analogRead(POTENTIOMETER),0,1024,-255,255) >= 0) {data.motorDirection = 0; }
  else {data.motorDirection = 1;}
  if (data.motorSpeed <= 10) {data.motorSpeed = 0;}
  // put your main code here, to run repeatedly:
  
  Serial.print("Button State: ");
  Serial.println(data.buttonState);
  Serial.print("Motor Speed: ");
  Serial.println(data.motorSpeed);
  Serial.print("Motor Direction: ");
  Serial.println(data.motorDirection);
  radio.write(&data, sizeof(DataPacket));
  delay(50);
}
