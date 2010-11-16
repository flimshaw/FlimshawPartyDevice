/*
 *  BpmClock.cpp
 *  FlimshawPartyDevice
 *
 *  Created by Charles Hoey on 11/15/10.
 *  Copyright 2010 Flimshaw Industries. All rights reserved.
 *
 */

#include "BpmClock.h"
#include "cinder/app/AppBasic.h"

using namespace ci;
using namespace ci::app;

BpmClock::BpmClock()
{
	timerRunning = false; // start off not running a timer
	mBpmTicks = (CLOCKS_PER_SEC * 60) / 120; // set us to 120bpm by default
	mStartTime = clock();
	mBpmFrames = 15;
	cMinBpmFrames = 4; // max BPM frame count
	cFps = 30; // our frames per second
	
}

void BpmClock::setup() {

}

void BpmClock::update()
{
	if(timerRunning) { // if we're currently timing bpms
		mBpmTimerCounter = clock(); // then increment our frame counter
	}
	
	mBpmRunningCounter = clock();
	if(mBpmRunningCounter >= (mBpmTicks + mStartTime)) { // if we've reached our timer
		mBpmRunningCounter = clock(); // reset it
		mStartTime = clock(); // and reset our start time marker
		bangOn(); // and bang on
	}
}

// here's where we receive hits from our BPM button
void BpmClock::bang()
{
	if(timerRunning) { // if we're currently timing something
		timerRunning = false; // then stop doing that
		mBpmFrames = mBpmTimerCounter; // set our frame varible to our timer counter
		mBpmRunningCounter = 0;
		bangOn();
		if(mBpmFrames < cMinBpmFrames) { // if we're under our limit for timing
			mBpmFrames = cMinBpmFrames; // set us to our limit
		}
	} else {
		mBpmTimerCounter = 0; // reset our timer counter
		timerRunning = true; // start our timer
	}
}

// sent out a bang to everybody
void BpmClock::bangOn() {
	console() << "bang on!";
}