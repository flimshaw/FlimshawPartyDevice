/*
 *  Particle.cpp
 *  FlimshawPartyDevice
 *
 *  Created by Charles Hoey on 11/10/10.
 *  Copyright 2010 Flimshaw Industries. All rights reserved.
 *
 */
#include "Particle.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;

Particle::Particle()
{
}

Particle::Particle( Vec2f loc )
{
	mLoc	= loc;
	mDir	= Rand::randVec2f();
	mVel	= Rand::randFloat( 5.0f );
	mRadius	= 3.0f;
	
	mIsDead	= false;
}

void Particle::update()
{
	mLoc += mDir * mVel;
	if(mLoc.x >= 320.0f) {
		mIsDead = true;
	}
}

void Particle::draw()
{
	gl::drawSolidCircle( mLoc, mRadius );
}