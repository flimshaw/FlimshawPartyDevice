#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Xml.h"
#include "ParticleController.h"
#include <string>

using namespace ci;
using namespace ci::app;
using namespace std;
using std::list;

#include "OscListener.h"
#include "OscSender.h"
#include "BpmClock.h"
#include "CharlieParticle.h"

class FlimshawPartyDeviceApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void handleOSC();
	void handleBpmBang();
	
	ParticleController mParticleController;
	CharlieParticle mCharlieParticle;
	
	vector<Url>				mUrls;
	Vec2f mGravityDir;
	float particleSize;
	string textureDirectory;
	
	osc::Listener	listener;
	osc::Sender		sender;
	std::string host;
	int port;
	
	BpmClock mBpmClock;
};

void FlimshawPartyDeviceApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1024, 768 );
	//settings->setFullScreen(true);
	//settings->setFrameRate( 60.0f );
}

void FlimshawPartyDeviceApp::setup()
{

	// set up some default environment variables
	mGravityDir = Vec2f(0.0, 1.0);
	
	// set up our OSC stuff
	listener.setup(3000);
	host = "10.0.0.2";
	port = 3002;
	sender.setup(host, port);
	//mCharlieParticle.setup();

	// the corpse of the old flickr xml loader
	/*XmlDocument doc( loadUrlStream( "http://api.flickr.com/services/feeds/groups_pool.gne?id=1423039@N24&lang=en-us&format=rss_200" ) );	
	vector<XmlElement> items( doc.xpath( "/rss/channel/item" ) );
	for( vector<XmlElement>::iterator itemIter = items.begin(); itemIter != items.end(); ++itemIter ) {
		//std::cout << itemIter->firstChildElement().firstChild().value() << std::endl;
		//std::cout << itemIter->findChildNode("media:title").firstChild().value() << std::endl;
		map<string, string> attrs( itemIter->findChild("media:content").attributes() );
		console() <<  "URL:" << attrs["url"] << std::endl;
		
		mUrls.push_back( Url( attrs["url"] ) );
	}
	*/
	
}

void FlimshawPartyDeviceApp::mouseDown( MouseEvent event )
{
}

void FlimshawPartyDeviceApp::handleBpmBang() {
	
}

void FlimshawPartyDeviceApp::handleOSC()
{
	// loop through all queued OSC messages
	while (listener.hasWaitingMessages()) {
		osc::Message message;
		osc::Message sendMessage;
		
		listener.getNextMessage(&message);
		string address = message.getAddress();
		
		if(address == "/1/particle_count") {
			mParticleController.setParticleMax((int)(message.getArgAsFloat(0) * 100));
			sendMessage.addIntArg((int)(message.getArgAsFloat(0) * 100));
			sendMessage.setAddress("/1/label_particle_count");
			sendMessage.setRemoteEndpoint(host, port);
			sender.sendMessage(sendMessage);
		} else if(address == "/1/audio_scale") {
			mParticleController.setAudioScale(message.getArgAsFloat(0));
		} else if(address == "/1/audio_smoothness") {
			mParticleController.setSmoothness(message.getArgAsFloat(0));
		} else if(address == "/1/audio_min_size") {
			mParticleController.setMinSize(message.getArgAsFloat(0));
		} else if(address == "/1/audio_max_size") {
			mParticleController.setMaxSize(message.getArgAsFloat(0));
		} else if(address == "/1/particle_speed_scale") {
			mParticleController.setSpeedScale(message.getArgAsFloat(0));
		} else if(address == "/1/particle_default_scale") {
			mParticleController.setDefaultScale(message.getArgAsFloat(0));
		} else if(address == "/1/gravity_dir") {
			mGravityDir = Vec2f(message.getArgAsFloat(0) -1.0f, message.getArgAsFloat(1) -1.0f);
			mParticleController.setGravityDir(mGravityDir);
		} else if(address == "/3/multitoggle1/1/1") {
			bool fullScreenFlag = (bool)message.getArgAsFloat(0);
			setFullScreen(fullScreenFlag);
		} else if(address == "/3/multitoggle1/1/3") {
			mParticleController.invertVelocity();
		} else if(address == "/3/multitoggle1/1/4") {
			mParticleController.setRandomParticleVectors((bool)message.getArgAsFloat(0));
		} else if(address == "/1/bpm_button") {
			uint mBpmButton = message.getArgAsFloat(0);
			if(mBpmButton == 1) {
				mBpmClock.bang();
			}
		} else if(address == "/2/config_toggle/1/1") {
			setFullScreen(!isFullScreen());
		}
		
		// debug functions for OSC info
		console() << "New message received" << std::endl;
		console() << "Address: " << message.getAddress() << std::endl;
		console() << "Num Arg: " << message.getNumArgs() << std::endl;
		for (int i = 0; i < message.getNumArgs(); i++) {
			console() << "-- Argument " << i << std::endl;
			console() << "---- type: " << message.getArgTypeName(i) << std::endl;
			if (message.getArgType(i) == osc::TYPE_INT32){
				try {
					console() << "------ value: "<< message.getArgAsInt32(i) << std::endl;
				}
				catch (...) {
					console() << "Exception reading argument as int32" << std::endl;
				}
				
			}else if (message.getArgType(i) == osc::TYPE_FLOAT){
				try {
					console() << "------ value: " << message.getArgAsFloat(i) << std::endl;
				}
				catch (...) {
					console() << "Exception reading argument as float" << std::endl;
				}
				
				//particleSize = message.getArgAsFloat(0);
			}else if (message.getArgType(i) == osc::TYPE_STRING){
				try {
					console() << "------ value: " << message.getArgAsString(i).c_str() << std::endl;
				}
				catch (...) {
					console() << "Exception reading argument as string" << std::endl;
				}
				
			}
		}
		
	}
}

void FlimshawPartyDeviceApp::update()
{
	mBpmClock.update();
	handleOSC(); // read/write all pending OSC calls
	mParticleController.update();
	
}

void FlimshawPartyDeviceApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	gl::enableAlphaBlending();
	mParticleController.draw();
}


CINDER_APP_BASIC( FlimshawPartyDeviceApp, RendererGl )
