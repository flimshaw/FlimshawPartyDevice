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
#include "cinder/gl/Texture.h"
#include <list>

class ParticleController {
public:
	ParticleController();
	void update();
	void draw();
	void addParticles( int amt );
	void removeParticles( int amt );

	std::list<Particle>	mParticles;
	uint particleMax;
	uint textureBufferMax;
};