/*
 *  Particle.h
 *  FlimshawPartyDevice
 *
 *  Created by Charles Hoey on 11/10/10.
 *  Copyright 2010 Flimshaw Industries. All rights reserved.
 *
 */
#pragma once
#include "cinder/Channel.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include <vector>

using namespace ci;

class Particle {
public:
	Particle();
	Particle( ci::Vec2f );
	void update();
	void draw();
	
	ci::Vec2f	mLoc;
	ci::Vec2f	mDir;
	float		mVel;
	
	float		mRadius;
	bool		mIsDead;
};