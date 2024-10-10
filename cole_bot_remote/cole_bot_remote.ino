#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define BUTTON 10

RF24 radio(9, 8); // CE, SSN

struct DataPacket {
  bool buttonActivated;
};

DataPacket data;

const byte address[6] = "24769";

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
  data.buttonActivated = digitalRead(BUTTON);
  // put your main code here, to run repeatedly:
  

  Serial.println(data.buttonActivated);
  radio.write(&data, sizeof(DataPacket));
  delay(50);
}
