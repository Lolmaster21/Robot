/*
  This is the Robot code
*/
#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define LEFTMOTORSPEED A1 // Drive motors
#define RIGHTMOTORSPEED A0
#define LEFTCW 2 // Direction motors
#define LEFTCCW 3
#define RIGHTCW 4
#define RIGHTCCW 5

#define CLAW A2 // Servos
#define ARM A3

// All pins Caps please

const bool enableDrive = 1;
const bool enableServo = 1;

const int clawMax = 150; // Closed
const int clawMin = 90; // Open
const int armMax = 140;  // Up
const int armMin = 50; //Down

// Turn these into structs when possible

Servo claw; // initialize servo objects
Servo arm;

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
// Servo myservo; //Initialize Servo


const byte address[6] = "24769"; // Our id address

void setup() {
  // put your setup code here, to run once:
  pinMode(LEFTMOTORSPEED, OUTPUT);
  pinMode(LEFTCW, OUTPUT);
  pinMode(LEFTCCW, OUTPUT);
  pinMode(RIGHTMOTORSPEED, OUTPUT);
  pinMode(RIGHTCW, OUTPUT);
  pinMode(RIGHTCCW, OUTPUT);
  arm.attach(ARM);
  claw.attach(CLAW);
  radio.begin();//Tell the RF library to start radio communication
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, address);//set transmission address to 5-digit number above
  //Set module as transmitter
  radio.startListening();
  Serial.begin(9600);//you better know what this does
}

void loop() {
  // if(!radio.isChipConnected() ) {Serial.println("Radio Not connected");} else {Serial.println("Radio Connected");} // Debug Wires
  if (radio.available()) {//if a signal is available
    radio.read(&data, sizeof(DataPacket));
    delay(20);//20ms delay to unpack signal

    if (enableDrive) { //Disable drive control
    analogWrite(LEFTMOTORSPEED,data.leftMotorSpeed);
    digitalWrite(LEFTCW, data.leftMotorDirection);
    digitalWrite(LEFTCCW, !data.leftMotorDirection);
    analogWrite(RIGHTMOTORSPEED,data.rightMotorSpeed);
    digitalWrite(RIGHTCW, data.rightMotorDirection);
    digitalWrite(RIGHTCCW, !data.rightMotorDirection);
    }
    //for (int i = 90; i <= 150; i++){ claw.write(i); Serial.println(i); delay(200);} Tuning
    if (enableServo) {
    if (data.armState) {arm.write(armMax); Serial.println("Up");} else {arm.write(armMin); Serial.println("Down");}
    if (data.clawState) {claw.write(clawMax); Serial.println("Closed");} else {claw.write(clawMin); Serial.println("Open");}
    delay(20);
    }
    
    Serial.print("Left Speed: ");
    Serial.println(data.leftMotorSpeed);
    Serial.print("Left Direction: ");
    Serial.println(data.leftMotorDirection);
    Serial.print("Right Speed: ");
    Serial.println(data.rightMotorSpeed);
    Serial.print("Right Direction: ");
    Serial.println(data.rightMotorDirection);
    
    Serial.println("Radio Success");
  } else { 
    // Serial.println("Radio Fail"); 
  } // Radio Check
}
