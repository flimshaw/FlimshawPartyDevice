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
#include "ThreadedImage.h"

using namespace ci;
using namespace ci::app;

Particle::Particle()
{
}

Particle::Particle( Vec2f loc )
{
	mLoc	= loc;
	mDir	= Vec2f(0.0, 1.0);
	mVel	= Rand::randFloat( 5.0f );
	mRadius	= 500.0f;
	mColor = Color(Rand::randFloat( 1.0f ), Rand::randFloat( 1.0f ), Rand::randFloat( 1.0f ));
	mNewScale = mRadius;
	mIsDead	= false;
	
	mTexture = gl::Texture( loadImage( "/flickr/paul-and-charlie.jpg" ) );

	
	//mImage.loadUrl(Url("http://farm2.static.flickr.com/1146/5155365620_3194692a49.jpg"));
}

Particle::Particle( Vec2f loc, string texturePath )
{
	mLoc	= loc;
	mDir	= Vec2f(0.0, 1.0);
	mVel	= Rand::randFloat( 5.0f );
	mRadius	= 500.f;
	mColor = Color(Rand::randFloat( 1.0f ), Rand::randFloat( 1.0f ), Rand::randFloat( 1.0f ));
	mNewScale = mRadius;
	mIsDead = false;
	
	try {
		mTexture = gl::Texture( loadImage( texturePath ) );
	} catch (cinder::ImageIoExceptionFailedLoad) {
		console() << "bad image!";
		mTexture = gl::Texture( loadImage( "/flickr/default.jpg" ) );
	}
}

void Particle::setScale(float newScale) {
	mNewScale = newScale;
}

void Particle::setGravityDir(Vec2f newGravityDir) {
	mDir = newGravityDir;
}

void Particle::update()
{
	mLoc += mDir * mVel;
	if(mLoc.x < 0 || mLoc.x >= 1024 || mLoc.y >= 768) {
		mIsDead = true;
	}
	
}

void Particle::draw()
{
	Rectf rect( mLoc.x, mLoc.y, mLoc.x + mTexture.getWidth(), mLoc.y + mTexture.getHeight() );
	//gl::color(mColor);
	//gl::drawSolidRect( rect );
	gl::draw(mTexture, rect);
}