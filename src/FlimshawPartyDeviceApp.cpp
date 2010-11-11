#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "ParticleController.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using std::list;

class FlimshawPartyDeviceApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	ParticleController mParticleController;

};

void FlimshawPartyDeviceApp::prepareSettings( Settings *settings )
{
	//settings->setWindowSize( 800, 600 );
	//settings->setFullScreen(true);
	//settings->setFrameRate( 60.0f );
}

void FlimshawPartyDeviceApp::setup()
{

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
