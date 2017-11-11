/*
 * SatelliteReceiver.cpp
 *
 *  Created on: 9. jan. 2011
 *      Author: develop
 */
#include "SatelliteReceiver.h"

// The preamble byte vary between Rx
// The first byte is number of errors and maxs out at FF
// The second byte is the bind type
// 0x01 22ms 1024 DSM2
// 0x12 11ms 2048 DSM2
// 0xa2 22ms 2048 DSMS
// 0xb2 11ms 2048 DSMX

void SatelliteReceiver::setup(void){
  Serial3.begin(115200);
  pinMode(DSM_POWER_PIN, OUTPUT);
  digitalWrite(DSM_POWER_PIN, HIGH);
}

void SatelliteReceiver::reset(void){
  digitalWrite(DSM_POWER_PIN, LOW);
  delay(1);
  frameDrops = 0;
  digitalWrite(DSM_POWER_PIN, HIGH);
  delay(1);
}

void SatelliteReceiver::getFrame(void){
  if (not Serial3.available()) {
    if (millis() - timer > 1000) {
      // Serial.print("Reset after timeout. ");
      reset();
    }
    delay(1);
  }

  // if (Serial3.available() >= 32) {
  //   Serial.print("Dropping frames.\n");
  //   while(Serial3.available() >= 32) Serial3.read();
  // }

  while(Serial3.available() > 16)
  {
  	frameDrops = Serial3.read(); //
    bindType = Serial3.read();
    while (bindType != 0x12 && bindType != 0xa2 && bindType != 0xb2) {
      // Serial.print("Invalid bind type: ");
      // Serial.print(bindType);
      // Serial.print("\n");

      bindType = Serial3.read();
      return;
    }
    // Serial.print("Bind type: ");
    // Serial.print(bindType);
    // Serial.print("\n");

    for (int i = 0; i<7; ++i) {
      inByte = Serial3.read();
      // Serial.print(" ");
      // Serial.print(inByte);
      index = (inByte & 0b01111000) >> 3;
      if (index < DSM_MAX_CHANNELS) {
        channels[index] = (inByte & 0b00000111) * 256 + Serial3.read();
      } else {
        // Serial.print("Invalid channel. ");
        return;
      }
    }
    Serial.print("\n");
    timer = millis();
  }
}


int SatelliteReceiver::getErrors(){
	return frameDrops;
}
int SatelliteReceiver::getBindType(){
	return bindType;
}
int SatelliteReceiver::getAile(){
	return channels[AILE] - AILE_CENTER;
}
int SatelliteReceiver::getFlap(){

	return channels[FLAP] >> 8 & 0b00000011;
}
int SatelliteReceiver::getMode(){
	return channels[MODE] >> 8 & 0b00000011;
}
int SatelliteReceiver::getElev(){
	return channels[ELEVATOR] - ELEVATOR_CENTER;
}
int SatelliteReceiver::getArm(){
	return channels[ARM];
}
int SatelliteReceiver::getThro(){
	return channels[THROTTLE] - THROTTLE_CENTER;
}
int SatelliteReceiver::getRudd(){
	return channels[RUDDER] - RUDDER_CENTER;
}

bool SatelliteReceiver::isArmed(){
	const int ARMED_THRESHOLD = -800;
	return getThro() > ARMED_THRESHOLD;
}

bool SatelliteReceiver::isConnected(){
	return millis() - timer < 100;
}