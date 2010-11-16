/*
 *  BpmClock.h
 *  FlimshawPartyDevice
 *
 *  Created by Charles Hoey on 11/15/10.
 *  Copyright 2010 Flimshaw Industries. All rights reserved.
 *
 */

#include "cinder/app/AppBasic.h"
#include "time.h"

class BpmClock {
public:
	BpmClock();
	void update();
	void setup();
	void bang();
	void bangOn();

	bool timerRunning; // flag to tell us if we're currently timing something
	uint mBpm; // current bpm count
	uint mBpmFrames; // conversion of bpm count to frame count
	
	clock_t mBpmTicks;
	clock_t mBpmRunningCounter; // variable for how many frames have passed so far for normal use
	clock_t mBpmTimerCounter; // variable for use when timing bpms for new settings
	
	clock_t mStartTime;
	
	
	uint cMinBpmFrames;
	uint cFps;
	
};