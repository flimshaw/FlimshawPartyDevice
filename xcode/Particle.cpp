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

Particle::Particle( Vec2f loc, string texturePath )
{
	mLoc	= loc;
	mDir	= Vec2f(0.0, 1.0);
	mVel	= Rand::randFloat( 5.0f ) + 1.0f;
	mVelScale = 1.0;
	mScale	= 1.0f;
	mColor = Color(Rand::randFloat( 1.0f ), Rand::randFloat( 1.0f ), Rand::randFloat( 1.0f ));
	mIsDead = false;
	
	try {
		mTexture = gl::Texture( loadImage( texturePath ) );
	} catch (cinder::ImageIoExceptionFailedLoad) {
		console() << "bad image!";
		mTexture = gl::Texture( loadImage( "/flickr/default.jpg" ) );
	}
}

void Particle::setScale(float newScale) {
	mScale = newScale;
}

void Particle::setGravityDir(Vec2f newGravityDir) {
	mDir = newGravityDir;
}

void Particle::setAudioLevel(float audioLevel) {
	mAudioLevel = audioLevel;
}

void Particle::setVelScale(float velScale) {
	mVelScale = velScale;
}

void Particle::reactToAudio() {
	--mScale;
	mScale = mScale + (mAudioLevel * .002);
	if(mScale < 1.0) {
		mScale = 1.0;
	} else if(mScale > 1.5) {
		mScale = 1.5;
	}
}

void Particle::update()
{
	mLoc += mDir * (mVel * mVelScale);
	reactToAudio();
	if(mLoc.x < -mTexture.getWidth() || mLoc.x >= app::getWindowWidth() || mLoc.y >= (app::getWindowHeight() + mTexture.getHeight())) {
		mIsDead = true;
	}
}

void Particle::draw()
{
	float centerX = mLoc.x - ((mScale * mTexture.getWidth()) / 2);
	float centerY = mLoc.y - ((mScale * mTexture.getHeight()) / 2);
	Rectf rect( centerX, centerY, centerX + (mScale * mTexture.getWidth()), centerY + (mScale * mTexture.getHeight()) );
	//gl::color(mColor);
	//gl::drawSolidRect( rect );
	gl::draw(mTexture, rect);
}