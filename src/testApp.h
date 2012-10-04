#pragma once

#include "ofMain.h"
#include "ofxBlurShader.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofVideoGrabber videoGrabber;    // capture video from the webcam
    ofTexture mirrorTexture;        // mirrored video to the screen
    unsigned char* videoMirror;     // temporary pixel values
    int camWidth;                   // width of the video to capture
    int camHeight;                  // height of the video to capture
    
    bool sepia;                     // sepia mode
    bool grayScale;                 // grayScale mode
    bool darkGrayScale;             // lightGrayScale mode
    bool varGrayScale;              // varGrayScale mode
    bool rare3D;                    // rare effect
    bool canalDeviate;              // color deviate effect
    bool mirror;                    // mirror effect
    
    bool blur;                      // blur effect
    bool points;                    // points effect
    bool equalizer;                 // draws 24 filter equalizer
    int eq[24];
    
    bool info;                      // Shows info menu
    ofTrueTypeFont myFont;          // Menu font
    
    ofxBlurShader blurSh;           // Blur Shader
};
