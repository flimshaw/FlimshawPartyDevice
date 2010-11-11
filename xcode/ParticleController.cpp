/*
 *  ParticleController.cpp
 *  FlimshawPartyDevice
 *
 *  Created by Charles Hoey on 11/10/10.
 *  Copyright 2010 Flimshaw Industries. All rights reserved.
 *
 */
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "cinder/ImageIO.h"
#include "cinder/Thread.h"
#include "ParticleController.h"

using namespace ci;
using std::list;

ParticleController::ParticleController()
{
}

void loadSurfaceUrl( Surface *destSurface, const Url &url )
{
	*destSurface = Surface( loadImage( loadUrl( url ) ) );
}

void ParticleController::update()
{	
	
	// if we're running low on particles, add some more
	if(mParticles.size() < 5) {
		addParticles(5 - mParticles.size());
	}
	
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ){
		// clean up dead particles, and if they die birth new ones
		if( p->mIsDead ) {
			p = mParticles.erase( p );
			addParticles(1);
		} else {
			p->update();
			++p;
		}
	}
}

void ParticleController::draw()
{
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p ){
		p->draw();
	}
}

void ParticleController::addParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		float x = Rand::randFloat( app::getWindowWidth() );
		float y = Rand::randFloat( app::getWindowHeight() );
		mParticles.push_back( Particle( Vec2f( x, y ) ) );
	}
}

void ParticleController::removeParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		mParticles.pop_back();
	}
}

