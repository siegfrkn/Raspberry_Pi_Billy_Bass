/*
   Make two DC Motors Move to Sound.
   This example code is in the public domain.
   Created by Katrina Siegfried 2016, based on
   code by Donald Bell, Maker Project Lab (2016).
   Based on Sound to Servo by Cenk Özdemir (2012) 
   and DCMotorTest by Adafruit
*/

/* Things to add as of 11/29/2016:
   1. intregrate head/tail motor - need 12V power supply
   2. add filter to smooth out noise
   3. add billy bass speakers as audio output
*/




// include the Adafruit motor shield v2 library
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1 for mouth and M2 for tail
Adafruit_DCMotor *mouthMotor = AFMS.getMotor(1); // M1 is mouth motor, 64 kHz
Adafruit_DCMotor *headtailMotor = AFMS.getMotor(2); // M2 is head/tail motor, 64 kHz

// Some other Variables we need
int SoundInPin = A0;
//int LedPin = 12; //in case you want an LED to activate while mouth moves

void setup() // the setup routine runs once when you press reset:
{
  
  Serial.begin(9600); // set up Serial library baud rate at 9600 bps

  AFMS.begin();  // create motor library with the default frequency 1.6KHz

  // Set the speed to start, from 0 (off) to 255 (max speed)
  mouthMotor->setSpeed(0); // mouth motor not moving
  mouthMotor->run(FORWARD); // turn on mouth motor
  mouthMotor->run(RELEASE); // release mouth motor
  pinMode(SoundInPin, INPUT); // sound from the alexa coming into the jack is INPUT

  headtailMotor->setSpeed(0); // tail motor not moving
  headtailMotor->run(FORWARD); // turn on mouth motor
  headtailMotor->run(RELEASE); // release mouth motor
  pinMode(SoundInPin, INPUT);  // sound from the alexa coming into the jack is INPUT
}

void loop() // the loop routine runs over and over again forever:
{
  uint8_t i; // define i as an integer variable
  
  int sensorValue = analogRead(SoundInPin)*1.3; // read the input on analog pin A0 and add a gain of 30%

  sensorValue = map(sensorValue,0,512,0,180); // We Map it here down to the possible range of  movement.
  // note normally the 512 is 1023 because of analog reading should go so far, but I changed that to get better readings. -Donald Bell
  
  //int MoveDelayValue = map(sensorValue,0,255,0,sensorValue); // map the MoveDelayValue as the max value, not sure where this is used now*   

//  // Controlling head/tail motor movement TEST
//  delay(10);
//  headtailMotor->setSpeed(200);
//  delay(2000);
//  headtailMotor->run(FORWARD);
//  delay(2000);
//  headtailMotor->run(RELEASE);
//  delay(200);

//// Controlling head/tail motor movement
//  do
//  {
//  delay(1);
//  headtailMotor->setSpeed(200);
//  delay(1);
//  headtailMotor->run(FORWARD);
//  delay(1);
//  headtailMotor->run(RELEASE);
//  } while (sensorValue > 11);
//  headtailMotor->run(RELEASE);


   if (sensorValue > 13) // 11 is the threshold at which the head lifts
   {
     delay(1);
     headtailMotor->setSpeed(200); // set the head/tail motor speed to 200 out of range 0 to 255
     delay(1); // static delay to smooth things out
     headtailMotor->run(FORWARD); // move the head up (could be tail if you wired in opposite configuration)
     delay(2000); // static delay to smooth things out
     //headtailMotor->setSpeed(0); // trying to turn off the goddamend head
     //headtailMotor->run(RELEASE); // stop moving the head/tail motor
     delay(1); // static delay to smooth things out
   }
   

  // Controlling mouth motor movement
  if (sensorValue > 12) // to cut off some static readings, otherwise the jaw chatters from noise
  { 
   delay(1);  // a static delay to smooth things out
   mouthMotor->run(FORWARD); // now move the motor
   delay(1); // a static delay to smoothe things out
   
   for (i=140; i<255; i++) // control speed ramping, start at 140, count by 1, until 254 to speed up motor
  {
    mouthMotor->setSpeed(i);  // set the motor speed to whatever the value of the integer i is
  }
         
  //Release control of both motors  
  mouthMotor->run(RELEASE); // stop moving the mouth motor
  headtailMotor->run(RELEASE); // stop moving the head/tail motor
  delay(1); // static delay to smooth things out
  }

//headtailMotor->run(RELEASE); // trying to stop the head from being crazy

}
// Done

// Katrina Siegfried 2016
