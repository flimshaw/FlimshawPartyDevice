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
#include "cinder/Url.h"
#include "ThreadedImage.h"
#include <vector>
#include <string>

using namespace ci;
using namespace std;

class Particle {
public:
	Particle();
	Particle( ci::Vec2f );
	Particle( ci::Vec2f loc, string texturePath );
	
	void update();
	void draw();
	void setScale(float newScale);
	void setGravityDir(Vec2f newGravityDir);
	
	ci::Vec2f	mLoc;
	ci::Vec2f	mDir;
	ci::Color	mColor;
	float		mVel;
	float		mNewScale;
	
	gl::Texture mTexture;
	
	float		mRadius;
	bool		mIsDead;
	
};