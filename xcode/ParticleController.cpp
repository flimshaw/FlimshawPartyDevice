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
	
	particleMax = 20;
	mGravityDir = Vec2f(0.0, 1.0);
	mInput = audio::Input();
	mInput.start();
	textureCounter = 0;
}

void ParticleController::setParticleMax(int newMax) {
	if(newMax > 0 && newMax < 500) {
		particleMax = newMax;
	}
}

void ParticleController::setGravityDir(Vec2f newGravityDir) {
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ){
		mGravityDir = newGravityDir;
		p->setGravityDir(newGravityDir);
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
	if(mParticles.size() < particleMax) {
		addParticles(particleMax - mParticles.size());
	}
	
	// if we have too many particles, remove some
	if(mParticles.size() > particleMax) {
		removeParticles(mParticles.size() - particleMax);
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
	
	uint16_t bandCount = 16;
	
	if( ! mFftDataRef ) {
		return;
	}
	
	float * fftBuffer = mFftDataRef.get();
	float audioLevel = 0.0f;
	for( int i = 0; i < ( bandCount ); i++ ) {
		audioLevel = audioLevel + fftBuffer[i];
	}
	audioLevel = audioLevel / bandCount;
	//audioLevel = (bufferScratch / bufferSamples) * 1000 + 50;
	
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p ){
		p->setAudioLevel(audioLevel);
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

