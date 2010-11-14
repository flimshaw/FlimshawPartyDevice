/*
 *  ParticleController.h
 *  FlimshawPartyDevice
 *
 *  Created by Charles Hoey on 11/10/10.
 *  Copyright 2010 Flimshaw Industries. All rights reserved.
 *
 */
#pragma once
#include "Particle.h"
#if defined( CINDER_COCOA_TOUCH )
#include "cinder/app/AppCocoaTouch.h"
typedef ci::app::AppCocoaTouch AppBase;
#else
#include "cinder/app/AppBasic.h"
#include "cinder/audio/FftProcessor.h"
typedef ci::app::AppBasic AppBase;
#endif

#include "cinder/audio/Input.h"
#include "cinder/gl/Texture.h"
#include <list>
#include <string>

using namespace std;

class ParticleController {
public:
	ParticleController();
	void update();
	void draw();
	void addParticles( int amt );
	void removeParticles( int amt );
	void setParticleMax( int newMax );
	void setGravityDir(Vec2f newGravityDir);

	std::list<Particle>	mParticles;
	std::list<Url> mTextures;
	std::vector<string> mImageFiles;
	
	uint particleMax;
	uint textureBufferMax;
	
	uint textureCounter;
	list<int>::iterator textureIterator;
	
	Vec2f mGravityDir;
	
	audio::Input mInput;
	boost::shared_ptr<float> mFftDataRef;
	audio::PcmBuffer32fRef mPcmBuffer;
};