/*
 *  CharlieParticle.h
 *  FlimshawPartyDevice
 *
 *  Created by Charlie Hoey on 11/18/10.
 *  Copyright 2010 The Barbarian Group. All rights reserved.
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

class CharlieParticle {
public:
	CharlieParticle();
	void update();
	void draw();
	bool isDrawOn();
	void setDraw(bool drawOn);
	void setup();
	
	ci::Vec2f mLoc;
	gl::Texture mTexture;
	bool mDrawOn;
};