#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class FlimshawPartyDeviceApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void FlimshawPartyDeviceApp::setup()
{
}

void FlimshawPartyDeviceApp::mouseDown( MouseEvent event )
{
}

void FlimshawPartyDeviceApp::update()
{
}

void FlimshawPartyDeviceApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}


CINDER_APP_BASIC( FlimshawPartyDeviceApp, RendererGl )
