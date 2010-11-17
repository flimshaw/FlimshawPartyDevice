/*
 *  ParticleController.cpp
 *  FlimshawPartyDevice
 *
 *  Created by Charles Hoey on 11/10/10.
 *  Copyright 2010 Flimshaw Industries. All rights reserved.
 *
 */
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "cinder/ImageIO.h"
#include "ParticleController.h"
#include "cinder/audio/Input.h"
#include "boost/filesystem.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/fstream.hpp"
#include <iostream>
#include "time.h"

using namespace ci;
using namespace ci::app;
using namespace boost::filesystem;
using namespace std;
using std::list;

ParticleController::ParticleController()
{
	if(exists("/flickr")) {
		directory_iterator end ;
		for( directory_iterator iter("/flickr") ; iter != end ; ++iter )
			if ( is_directory( *iter ) )
			{
				//cout << iter->leaf() << " (directory)\n" ;
			}
			else {
				//console() << iter->leaf();
				mImageFiles.push_back(iter->leaf());
				//cout << iter->leaf() << " (file)\n" ;
			}
	}
	
//	textureIterator = mImageFiles.begin();
	
	// set up our default values
	mParticleCount = 100;
	mGravityDir = Vec2f(0.0, 1.0);
	textureCounter = 0;
	mAudioScale = .1;
	mSmoothness = .1;
	mMinSize = .3f;
	mMaxSize = 1.0f;
	mDefaultScale = 1.0;
	mRandomParticleVectors = false;
	mAudioEnable = true;
	
	mInput = audio::Input();
	mInput.start();

}

// all our fun scaling functions
void ParticleController::setRandomParticleVectors(bool randomParticleVectors) {
	mRandomParticleVectors = randomParticleVectors;
	if(mRandomParticleVectors) {
		randomizeVectors();
	} else {
		setGravityDir(mGravityDir);
	}
}

void ParticleController::setSpeedScale(float speedScale) {
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ){
		p->setVelScale(speedScale);
		++p;
	}
}

void ParticleController::invertVelocity() {
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ){
		p->invertVelocity();
		++p;
	}
}

void ParticleController::setAudioScale(float audioScale) {
	mAudioScale = audioScale;
}

void ParticleController::setMinSize(float minSize) {
	mMinSize = minSize;
}

void ParticleController::setMaxSize(float maxSize) {
	mMaxSize = maxSize;
}

void ParticleController::setDefaultScale(float defaultScale) {
	mDefaultScale = defaultScale;
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ){
		p->setScale(mDefaultScale);
		++p;
	}
}

void ParticleController::setSmoothness(float smoothness) {
	mSmoothness = smoothness;
}


void ParticleController::setParticleMax(int particleCount) {
	if(particleCount > 0 && particleCount < 300) {
		mParticleCount = particleCount;
	}
}

void ParticleController::setGravityDir(Vec2f newGravityDir) {
	mGravityDir = newGravityDir;
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ){
		p->setGravityDir(mGravityDir);
		++p;
	}
}

void ParticleController::randomizeVectors() {
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ){
		p->setRandomVector();
		++p;
	}
}

void ParticleController::update()
{	
	mPcmBuffer = mInput.getPcmBuffer();
	if( ! mPcmBuffer ) {
		return;
	}
	
	uint16_t bandCount = 512;
	mFftDataRef = audio::calculateFft( mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT ), bandCount );
	
	// if we're running low on particles, add some more
	if(mParticles.size() < mParticleCount) {
		addParticles(mParticleCount - mParticles.size());
	}
	
	// if we have too many particles, remove some
	if(mParticles.size() > mParticleCount) {
		removeParticles(mParticles.size() - mParticleCount);
	}
	
	mPcmBuffer = mInput.getPcmBuffer();
	if( ! mPcmBuffer ) {
		return;
	}
	
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ){
		// clean up dead particles, and if they die birth new ones
		if( p->mIsDead ) {
			p = mParticles.erase( p );
			addParticles(1);
		} else {
			p->update();
			++p;
		}
	}
}

void ParticleController::draw()
{	
	float particleSize = 0.0f;
	uint16_t bandCount = 512;
	
	if( ! mFftDataRef ) {
		return;
	}
	
	// grab our FFT data and reduce it down to a basic level reading
	float * fftBuffer = mFftDataRef.get();
	float audioLevel = 0.0f;
	float filteredAudioLevel = 0.0f;
	for( int i = 0; i < ( bandCount ); i++ ) {
		audioLevel = audioLevel + fftBuffer[i];
	}
	audioLevel = audioLevel / bandCount;
	
	// run the filter:
	filteredAudioLevel = mSmoothness * audioLevel + (1-mSmoothness)*mLastAudioLevel;
	mLastAudioLevel = filteredAudioLevel;
	
	// scale our filtered audio level appropriately and make it our particle size
	particleSize = filteredAudioLevel * mAudioScale;
	
	// make sure we're within our boundaries
	if(particleSize > mMaxSize) {
		particleSize = mMaxSize;
	}
	
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p ){
		p->setAudioLevel(particleSize);
		p->draw();
	}
	

}

void ParticleController::addParticles( int amt )
{
	if(mImageFiles.size()) {
		for( int i=0; i<amt; i++ )
		{
			float x = Rand::randFloat( app::getWindowWidth() );
			float y = -500.0;
			
			int randomIndex = Rand::randInt(mImageFiles.size()) - 1;
			if(randomIndex < 0)
				randomIndex = 0;
			
			string nextImage = mImageFiles[randomIndex];
			while(nextImage == ".DS_Store") {
				int randomIndex = Rand::randInt(mImageFiles.size()) - 1;
				if(randomIndex < 0)
					randomIndex = 0;
				nextImage = mImageFiles[randomIndex];
			}
			if(nextImage != "") {
				Particle newParticle = Particle( Vec2f( x, y ), "/flickr/" + nextImage );
				mParticles.push_back( newParticle );
			}
		}
	}
}

void ParticleController::removeParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		mParticles.pop_back();
	}
}

