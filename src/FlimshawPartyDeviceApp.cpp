#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "ParticleController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class FlimshawPartyDeviceApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	ParticleController mParticleController;

};

void FlimshawPartyDeviceApp::setup()
{
	mParticleController.addParticles( 2500 );
}

void FlimshawPartyDeviceApp::mouseDown( MouseEvent event )
{
}

void FlimshawPartyDeviceApp::update()
{
	mParticleController.update();
}

void FlimshawPartyDeviceApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	mParticleController.draw();
}


CINDER_APP_BASIC( FlimshawPartyDeviceApp, RendererGl )
