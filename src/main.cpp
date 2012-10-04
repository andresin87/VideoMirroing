#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN or OF_GAME_MODE)
	ofSetupOpenGL(&window, 1280, 768, OF_WINDOW);
	ofRunApp(new testApp()); // start the app
}
