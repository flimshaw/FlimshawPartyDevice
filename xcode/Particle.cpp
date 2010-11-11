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
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
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
	mRadius	= Rand::randFloat( 50.0f ) + 5;
	
	mIsDead	= false;
	
}

void Particle::update()
{
	mLoc += mDir * mVel;
	if(mLoc.x < 0 || mLoc.y < 0 || mLoc.x >= 640 || mLoc.y >= 480) {
		mIsDead = true;
	}
}

void Particle::draw()
{
	Rectf rect( mLoc.x, mLoc.y, mLoc.x + mRadius, mLoc.y + mRadius );
	gl::drawSolidRect( rect );

}