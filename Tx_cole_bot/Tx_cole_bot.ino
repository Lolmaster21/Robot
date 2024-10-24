/*
  This is the Transmitter code
*/
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define ARM A0
#define CLAW A1
#define DRIVEY A2
#define DRIVEX A3

RF24 radio(9, 8); // CE, SSN

struct DataPacket { //Packet class
  int rightMotorSpeed;
  int leftMotorSpeed;
  bool rightMotorDirection;
  bool leftMotorDirection;
  bool clawState = 0; // open
  bool armState = 0; // down
};

DataPacket data;

const byte address[6] = "24769"; // Our id address

void setup() {
  radio.begin(); //Tell the RF library to start radio communication
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address); //set transmission address to 5-digit number above
  //Set module as transmitter
  radio.stopListening();
  Serial.begin(9600);
}


// Add functions/objects if there is time
void loop() {
  static int driveValY;
  static int driveValX;
  static int armVal;
  static int clawVal;

  driveValY = map(analogRead(DRIVEY),0,1024,-255,255); // Intermediate Joystick values
  driveValX = map(analogRead(DRIVEX),0,1024,-255,255);
  armVal = map(analogRead(ARM),0,1024,-255,255); 
  clawVal = map(analogRead(CLAW),0,1024,-255,255);

  if (armVal >= 200) {data.armState = 1;} else if (armVal <= -200) {data.armState = 0;} // UP is up, DOWN is down
  if (clawVal >= 200) {data.clawState = 0;} else if (clawVal <= -200) {data.clawState = 1;} // Right is open, Left is close

  data.leftMotorSpeed = driveValY + driveValX; // Single joystick tank drive
  data.rightMotorSpeed = driveValY - driveValX;
  
  if (data.leftMotorSpeed >= 0) {data.leftMotorDirection = 0;} else {data.leftMotorDirection = 1;} // Directions
  if (data.rightMotorSpeed >= 0) {data.rightMotorDirection = 0;} else {data.rightMotorDirection = 1;}
  //data.leftMotorDirection = map(data.leftMotorSpeed, -510, 510, 0 , 1); //Speed bools
  //data.rightMotorDirection = map(data.rightMotorSpeed, -510, 510, 0 , 1);
  data.leftMotorSpeed = abs(data.leftMotorSpeed); // absolute after getting direction
  data.rightMotorSpeed = abs(data.rightMotorSpeed);
  if (data.leftMotorSpeed <= 15) {data.leftMotorSpeed = 0;} // UnderSpeed check
  if (data.rightMotorSpeed <= 15) {data.rightMotorSpeed = 0;}
  if (data.leftMotorSpeed >= 255) {data.leftMotorSpeed = 255;} // OverSpeed check 
  if (data.rightMotorSpeed >= 255) {data.rightMotorSpeed = 255;}

  Serial.print("Left Speed: ");
  Serial.println(data.leftMotorSpeed);
  Serial.print("Left Direction: ");
  Serial.println(data.leftMotorDirection);
  Serial.print("Right Speed: ");
  Serial.println(data.rightMotorSpeed);
  Serial.print("Right Direction: ");
  Serial.println(data.rightMotorDirection);

  radio.write(&data, sizeof(DataPacket));
  // if(!radio.isChipConnected() ) {Serial.println("Radio Not connected");} else {Serial.println("Radio Connected");} 
  delay(50);
}
