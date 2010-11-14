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

class FlimshawPartyDeviceApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	ParticleController mParticleController;
	vector<Url>				mUrls;
	Vec2f mGravityDir;
	float particleSize;
	string textureDirectory;
	
	osc::Listener	listener;
	osc::Sender		sender;
	std::string host;
	int port;
};

void FlimshawPartyDeviceApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1024, 768 );
	//settings->setFullScreen(true);
	//settings->setFrameRate( 60.0f );
}

void FlimshawPartyDeviceApp::setup()
{
	mGravityDir = Vec2f(0.0, 1.0);
	listener.setup(3000);
	
	host = "10.0.1.76";
	port = 3002;
	sender.setup(host, port);
	

	
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

void FlimshawPartyDeviceApp::update()
{
	mParticleController.update();
	
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
		} else if(address == "/1/gravity_dir") {
			mGravityDir = Vec2f(message.getArgAsFloat(0) -1.0f, message.getArgAsFloat(1) -1.0f);
			mParticleController.setGravityDir(mGravityDir);
		} else if(address == "/1/multitoggle3/1/1") {
			setFullScreen(!isFullScreen());
		}
		
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

void FlimshawPartyDeviceApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	mParticleController.draw();
}


CINDER_APP_BASIC( FlimshawPartyDeviceApp, RendererGl )
