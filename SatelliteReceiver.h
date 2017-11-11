/*
 * SpectrumReceiver.h
 *
 *  Created on: 9. jan. 2011
 *      Author: develop
 */

#ifndef SPECTRUMRECEIVER_H_
#define SPECTRUMRECEIVER_H_

#define RXCENTER 0
#define RXTRAVEL 15
#define RXMIN -700
#define RXMAX 700

#define DX_E 162
#define DSM_POWER_PIN 36
#define DSM_MAX_CHANNELS 12


#include "Arduino.h"

class SatelliteReceiver{
public:
	void setup(void);
	void reset(void);
	void getFrame(void);
	int getErrors();
	int getBindType();
	int getThro();
	int getAile();
	int getElev();
	int getRudd();
	int getMode();
	int getFlap();
	int getArm();
	bool isArmed();
	bool isConnected();

private:
	// int channels[8];
	// int prevByte;
	// int channel;
	// bool cByte;
	short bindType;
	short frameDrops;
	int channels[DSM_MAX_CHANNELS];
	int index;
	int inByte;
	bool insync;
	unsigned long timer;

	const int THROTTLE = 0;
	const int THROTTLE_CENTER = 1024;

	const int AILE = 1;
	const int AILE_CENTER = 1024;

	const int ELEVATOR = 2;
	const int ELEVATOR_CENTER = 1024;
	const int RUDDER = 3;
	const int RUDDER_CENTER = 1024;

	const int FLAP = 6;
	const int MODE = 4;

	// unused (DXe has 6 channels only)
	const int ARM = 5;

};


#endif /* SPECTRUMRECEIVER_H_ */
