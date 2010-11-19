/*
 *  CharlieParticle.cpp
 *  FlimshawPartyDevice
 *
 *  Created by Charlie Hoey on 11/18/10.
 *  Copyright 2010 The Barbarian Group. All rights reserved.
 *
 */

#include "CharlieParticle.h"
#include "cinder/Rand.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/AppBasic.h"

using namespace ci;
using namespace ci::app;

CharlieParticle::CharlieParticle() {
	mDrawOn = false;
	mLoc = Vec2f(Rand::randFloat(5.0f), Rand::randFloat(5.0f));
}

bool CharlieParticle::isDrawOn() {
	return mDrawOn;
}

void CharlieParticle::setDraw(bool drawOn) {
	mDrawOn = drawOn;
}

void CharlieParticle::setup() {
	mTexture = gl::Texture( loadImage( "/flickr/charlie/charlie-1.png" ) );	
}

void CharlieParticle::draw() {
	if(mDrawOn) {
		float centerX = mLoc.x - (mTexture.getWidth() / 2);
		float centerY = mLoc.y - (mTexture.getHeight() / 2);
		Rectf rect( centerX, centerY, centerX + mTexture.getWidth(), centerY + mTexture.getHeight() );
		gl::draw(mTexture, rect);
	}
}