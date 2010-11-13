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
				cout << iter->leaf() << " (directory)\n" ;
			}
			else
				cout << iter->leaf() << " (file)\n" ;
	}
	particleMax = 1;
	mGravityDir = Vec2f(0.0, 1.0);
	mInput = audio::Input();
	mInput.start();
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
	if( ! mPcmBuffer ) {
		return;
	}
	uint32_t bufferSamples = mPcmBuffer->getSampleCount();
	audio::Buffer32fRef leftBuffer = mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT );
	float bufferScratch = 0.0;
	float audioLevel = 0.0;
	
	for(uint32_t i = 0; i < bufferSamples; i++) {
		bufferScratch = abs(leftBuffer->mData[i]) + bufferScratch;
	}
	
	audioLevel = (bufferScratch / bufferSamples) * 1000 + 50;
	
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p ){
		//p->setScale(audioLevel);
		p->draw();
	}
}

void ParticleController::addParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		float x = Rand::randFloat( app::getWindowWidth() );
		float y = -100.0;
		Particle newParticle = Particle( Vec2f( x, y ) );
		//newParticle->setGravityDir(mGravityDir);
		mParticles.push_back( newParticle );
	}
}

void ParticleController::removeParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		mParticles.pop_back();
	}
}

