#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    camWidth = 640;
    camHeight = 480;
    mirror = false;
    sepia = false;
    grayScale = false;
    darkGrayScale = false;
    varGrayScale = false;
    rare3D = false;
    canalDeviate = false;
    equalizer = false;
    info = true;
    blur = false;
    blurSh.setup(camWidth, camHeight);
    videoGrabber.setVerbose(true);
    videoGrabber.initGrabber(camWidth, camHeight);
    videoMirror = new unsigned char[camWidth*camHeight*3];
    mirrorTexture.allocate(camWidth, camHeight, GL_RGB);
    for(int i=0;i<24;i++)
        eq[i]=0;
    myFont.loadFont("Consolas.ttf", 10);
    ofSetFrameRate(30);
}

//--------------------------------------------------------------
void testApp::update(){
    ofBackground(0, 0, 0);
    videoGrabber.grabFrame();
    if (videoGrabber.isFrameNew()) {
        unsigned char * pixels = videoGrabber.getPixels();
        for (int i = 0; i < camHeight; i++) {
            for (int j = 0; j < camWidth*3; j+=3) {
                int pix1 = (i*camWidth*3) + j;
                int pix2 = (i*camWidth*3) + (j+1);
                int pix3 = (i*camWidth*3) + (j+2);
                videoMirror[pix1] = pixels[pix1];
                videoMirror[pix2] = pixels[pix2];
                videoMirror[pix3] = pixels[pix3];
            }
        }
        if(mirror){
            for (int i = 0; i < camHeight; i++) {
                for (int j = 0; j < camWidth*3; j+=3) {
                    // pixel number
                    int pix1 = (i*camWidth*3) + j;
                    int pix2 = (i*camWidth*3) + (j+1);
                    int pix3 = (i*camWidth*3) + (j+2);
                    // mirror pixel number
                    int mir1 = (i*camWidth*3)+1 * (camWidth*3 - j-3);
                    int mir2 = (i*camWidth*3)+1 * (camWidth*3 - j-2);
                    int mir3 = (i*camWidth*3)+1 * (camWidth*3 - j-1);
                    // swap pixels
                    videoMirror[pix1] = pixels[mir1];
                    videoMirror[pix2] = pixels[mir2];
                    videoMirror[pix3] = pixels[mir3];
                }
            }
        }
        if(sepia){
            for (int i = 0; i < camHeight; i++) {
                for (int j = 0; j < camWidth*3; j+=3) {
                    int r = (i*camWidth*3) + j;
                    int g = (i*camWidth*3) + (j+1);
                    int b = (i*camWidth*3) + (j+2);
                    int grayPixel = (11 * videoMirror[r] + 16 * videoMirror[g] + 5 * videoMirror[b]) / 32;
                    videoMirror[(i*camWidth*3) + j] = grayPixel;
                }
            }
        }
        if(grayScale){
            for (int i = 0; i < camHeight; i++) {
                for (int j = 0; j < camWidth*3; j+=3) {
                    int r = (i*camWidth*3) + j;
                    int g = (i*camWidth*3) + (j+1);
                    int b = (i*camWidth*3) + (j+2);
                    // mirror pixel number
                    int mir1 = (videoMirror[r] + videoMirror[g] + videoMirror[b]) / 3;
                    int mir2 = (videoMirror[r] + videoMirror[g] + videoMirror[b]) / 3;
                    int mir3 = (videoMirror[r] + videoMirror[g] + videoMirror[b]) / 3;
                    videoMirror[r] = mir1;
                    videoMirror[g] = mir2;
                    videoMirror[b] = mir3;
                }
            }
        }
        if(darkGrayScale){
            for (int i = 0; i < camHeight; i++) {
                for (int j = 0; j < camWidth*3; j+=3) {
                    int r = (i*camWidth*3) + j;
                    int g = (i*camWidth*3) + (j+1);
                    int b = (i*camWidth*3) + (j+2);
                    // mirror pixel number
                    int mir1 = (max(max(videoMirror[r], videoMirror[g]), videoMirror[b]) + min(videoMirror[r], videoMirror[g]), videoMirror[b]) / 2;
                    int mir2 = (max(max(videoMirror[r], videoMirror[g]), videoMirror[b]) + min(videoMirror[r], videoMirror[g]), videoMirror[b]) / 2;
                    int mir3 = (max(max(videoMirror[r], videoMirror[g]), videoMirror[b]) + min(videoMirror[r], videoMirror[g]), videoMirror[b]) / 2;
                    videoMirror[r] = mir1;
                    videoMirror[g] = mir2;
                    videoMirror[b] = mir3;
                }
            }
        }
        if(varGrayScale){
            for (int i = 0; i < camHeight; i++) {
                for (int j = 0; j < camWidth*3; j+=3) {
                    int r = (i*camWidth*3) + j;
                    int g = (i*camWidth*3) + (j+1);
                    int b = (i*camWidth*3) + (j+2);
                    // mirror pixel number
                    int mir1 = sqrt(videoMirror[r]*videoMirror[r] + videoMirror[g]*videoMirror[g] + videoMirror[b]*videoMirror[b]);
                    int mir2 = sqrt(videoMirror[r]*videoMirror[r] + videoMirror[g]*videoMirror[g] + videoMirror[b]*videoMirror[b]);
                    int mir3 = sqrt(videoMirror[r]*videoMirror[r] + videoMirror[g]*videoMirror[g] + videoMirror[b]*videoMirror[b]);
                    videoMirror[r] = mir1;
                    videoMirror[g] = mir2;
                    videoMirror[b] = mir3;
                }
            }
        }
        if(rare3D){
            for (int i = 0; i < camHeight; i++) {
                for (int j = 0; j < camWidth*3; j+=3) {
                    int r = (i*camWidth*3) + j;
                    int g = (i*camWidth*3) + (j+1);
                    int b = (i*camWidth*3) + (j+2);
                    int grayPixel = (11 * pixels[r] + 16 * pixels[g] + 5 * pixels[b]) / 32;
                    videoMirror[(i*camWidth*3) + j] = grayPixel;
                }
            }
        }
        if(canalDeviate){
            unsigned char* auxTex;
            auxTex = new unsigned char[camWidth*camHeight*3];
            //distance between circles and their size depends on mouse pos
            if((camWidth < mouseX && mouseX < (camWidth + camWidth)) && (0 < mouseY && mouseY < camHeight)){
                float size = ofMap(mouseX,camHeight,camWidth+camWidth,1,100);
                for (int i = 0; i < camHeight; i++) {
                    for (int j = 0; j < camWidth*3; j+=3) {
                        int r = (i*camWidth*3) + j;
                        int g = (i*camWidth*3) + (j+1);
                        int b = (i*camWidth*3) + (j+2);
                        // mirror pixel number
                        videoMirror[(r + 0*(int)size*3) % (camWidth*3)+i*camWidth*3] = pixels[r];
                        videoMirror[(g + 1*(int)size*3) % (camWidth*3)+i*camWidth*3] = pixels[g];
                        videoMirror[(b + 2*(int)size*3) % (camWidth*3)+i*camWidth*3] = pixels[b];
                    }
                }
            }
        }
        if(equalizer){
            for(int i=0;i<24;i++)
                eq[i]=0;
            for (int i = 0; i < camHeight; i++) {
                for (int j = 0; j < camWidth*3; j+=3) {
                    int r = (i*camWidth*3) + j;
                    int g = (i*camWidth*3) + (j+1);
                    int b = (i*camWidth*3) + (j+2);
                    if(max(max(pixels[r],pixels[g]),pixels[b])==pixels[b]){
                        if (96 <= pixels[b] && pixels[b]< 128) {
                            eq[0]++;
                        }
                        else if (128 <= pixels[b] && pixels[b]< 160) {
                            eq[1]++;
                        }
                        else if (160 <= pixels[b] && pixels[b]< 192) {
                            eq[2]++;
                        }
                        else if (192 <= pixels[b] && pixels[b]< 224) {
                            eq[3]++;
                        }
                        else if (224 <= pixels[b] && pixels[b]< 256) {
                            eq[4]++;
                        }
                        else if (0 <= pixels[b] && pixels[b]< 32) {
                            eq[21]++;
                        }
                        else if (32 <= pixels[b] && pixels[b]< 64) {
                            eq[22]++;
                        }
                        else if (64 <= pixels[b] && pixels[b]< 96) {
                            eq[23]++;
                        }
                    }
                    else if(max(max(pixels[r],pixels[g]),pixels[b])==pixels[r]){
                        if (0 <= pixels[r] && pixels[r]< 32) {
                            eq[5]++;
                        }
                        else if (32 <= pixels[r] && pixels[r]< 64) {
                            eq[6]++;
                        }
                        else if (64 <= pixels[r] && pixels[r]< 96) {
                            eq[7]++;
                        }
                        else if (96 <= pixels[r] && pixels[r]< 128) {
                            eq[8]++;
                        }
                        else if (128 <= pixels[r] && pixels[r]< 160) {
                            eq[9]++;
                        }
                        else if (160 <= pixels[r] && pixels[r]< 192) {
                            eq[10]++;
                        }
                        else if (192 <= pixels[r] && pixels[r]< 224) {
                            eq[11]++;
                        }
                        else if (224 <= pixels[r] && pixels[r]< 256) {
                            eq[12]++;
                        }
                    }
                    else if(max(max(pixels[r],pixels[g]),pixels[b])==pixels[g]){
                        if (0 <= pixels[g] && pixels[g]< 32) {
                            eq[13]++;
                        }
                        else if (32 <= pixels[g] && pixels[g]< 64) {
                            eq[14]++;
                        }
                        else if (64 <= pixels[g] && pixels[g]< 96) {
                            eq[15]++;
                        }
                        else if (96 <= pixels[g] && pixels[g]< 128) {
                            eq[16]++;
                        }
                        else if (128 <= pixels[g] && pixels[g]< 160) {
                            eq[17]++;
                        }
                        else if (160 <= pixels[g] && pixels[g]< 192) {
                            eq[18]++;
                        }
                        else if (192 <= pixels[g] && pixels[g]< 224) {
                            eq[19]++;
                        }
                        else if (224 <= pixels[g] && pixels[g]< 256) {
                            eq[20]++;
                        }
                    }
                }
            }
        }
        mirrorTexture.loadData(videoMirror, camWidth, camHeight, GL_RGB);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255, 255, 255);
    videoGrabber.draw(0, 0);
    mirrorTexture.draw(camWidth, 0, camWidth, camHeight);

    if(blur){
        if((camWidth < mouseX && mouseX < (camWidth + camWidth)) && (0 < mouseY && mouseY < camHeight)){
            ofSetColor(255, 255, 255);
            blurSh.begin();
            blurSh.amount = ofMap(mouseX,camHeight,camWidth+camWidth,1,10,true);
            blurSh.iterations = ofMap(mouseY,0,ofGetHeight(),1,10,true);
            videoGrabber.draw(camWidth, 0);
            blurSh.end();
            ofSetColor(255, 255, 255);
            myFont.drawString("amount: " + ofToString(blurSh.amount), camWidth + 10, camHeight+30);
            myFont.drawString("iterations: " + ofToString(blurSh.iterations), camWidth + 10, camHeight+45);
        }
    }
    if(points){
        //distance between circles and their size depends on mouse pos
        if((camWidth < mouseX && mouseX < (camWidth + camWidth)) && (0 < mouseY && mouseY < camHeight)){
            float size = ofMap(mouseX,camHeight,camWidth+camWidth,1,10)/3;
            float step = ofMap(mouseY,camHeight,camHeight+camHeight,10,3)/3;
            
            for (int y=0; y<camHeight; y+=step) {
                for (int x=0; x<camWidth; x+=step) {
                    
                    //get pixel color from camera
                    int i = (y*camWidth+x)*3;
                    int r = videoGrabber.getPixels()[i+0];
                    int g = videoGrabber.getPixels()[i+1];
                    int b = videoGrabber.getPixels()[i+2];
                    
                    //calculate brightness
                    float bri = (r+g+b) / 765.0;
                    
                    //draw black circles
                    ofSetColor(140,255,140);
                    if(mirror)
                    {
                        ofCircle((2*camWidth)-x,y,(1-bri)*(size));
                    }
                    if(!mirror)
                    {
                        ofCircle(x+camWidth,y,(1-bri)*(size));
                    }
                }
            }
        }
    }
    if(equalizer){
        ofSetColor(0,0,32);     //21
        ofRect(camWidth+(camWidth/24)*0, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[21]>j*3200)
                ofRect(camWidth+(camWidth/24)*0, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,0,64);     //22
        ofRect(camWidth+(camWidth/24)*1, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[22]>j*3200)
                ofRect(camWidth+(camWidth/24)*1, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,0,96);     //23
        ofRect(camWidth+(camWidth/24)*2, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[23]>j*3200)
                ofRect(camWidth+(camWidth/24)*2, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        
        
        ofSetColor(0,0,128);    //0
        ofRect(camWidth+(camWidth/24)*3, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[0]>j*3200)
                ofRect(camWidth+(camWidth/24)*3, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,0,160);    //1
        ofRect(camWidth+(camWidth/24)*4, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[1]>j*3200)
                ofRect(camWidth+(camWidth/24)*4, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,0,192);    //2
        ofRect(camWidth+(camWidth/24)*5, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[2]>j*3200)
                ofRect(camWidth+(camWidth/24)*5, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,0,224);    //3
        ofRect(camWidth+(camWidth/24)*6, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[3]>j*3200)
                ofRect(camWidth+(camWidth/24)*6, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,0,256);    //4
        ofRect(camWidth+(camWidth/24)*7, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[4]>j*3200)
                ofRect(camWidth+(camWidth/24)*7, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(32,0,0);     //5
        ofRect(camWidth+(camWidth/24)*8, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[5]>j*3200)
                ofRect(camWidth+(camWidth/24)*8, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(64,0,0);     //6
        ofRect(camWidth+(camWidth/24)*9, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[6]>j*3200)
                ofRect(camWidth+(camWidth/24)*9, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(96,0,0);     //7
        ofRect(camWidth+(camWidth/24)*10, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[7]>j*3200)
                ofRect(camWidth+(camWidth/24)*10, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(128,0,0);    //8
        ofRect(camWidth+(camWidth/24)*11, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[8]>j*3200)
                ofRect(camWidth+(camWidth/24)*11, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(160,0,0);    //9
        ofRect(camWidth+(camWidth/24)*12, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[9]>j*3200)
                ofRect(camWidth+(camWidth/24)*12, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(192,0,0);    //10
        ofRect(camWidth+(camWidth/24)*13, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[10]>j*3200)
                ofRect(camWidth+(camWidth/24)*13, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(224,0,0);    //11
        ofRect(camWidth+(camWidth/24)*14, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[11]>j*3200)
                ofRect(camWidth+(camWidth/24)*14, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(256,0,0);    //12
        ofRect(camWidth+(camWidth/24)*15, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[12]>j*3200)
                ofRect(camWidth+(camWidth/24)*15, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,32,0);     //13
        ofRect(camWidth+(camWidth/24)*16, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[13]>j*3200)
                ofRect(camWidth+(camWidth/24)*16, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,64,0);     //14
        ofRect(camWidth+(camWidth/24)*17, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[14]>j*3200)
                ofRect(camWidth+(camWidth/24)*17, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,96,0);     //15
        ofRect(camWidth+(camWidth/24)*18, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[15]>j*3200)
                ofRect(camWidth+(camWidth/24)*18, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,128,0);    //16
        ofRect(camWidth+(camWidth/24)*19, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[16]>j*3200)
                ofRect(camWidth+(camWidth/24)*19, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,160,0);    //17
        ofRect(camWidth+(camWidth/24)*20, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[17]>j*3200)
                ofRect(camWidth+(camWidth/24)*20, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,192,0);    //18
        ofRect(camWidth+(camWidth/24)*21, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[18]>j*3200)
                ofRect(camWidth+(camWidth/24)*21, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,224,0);    //19
        ofRect(camWidth+(camWidth/24)*22, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[19]>j*3200)
                ofRect(camWidth+(camWidth/24)*22, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
        ofSetColor(0,256,0);    //20
        ofRect(camWidth+(camWidth/24)*23, camHeight +10 , camWidth/24-3, 10);
        for(int j = 0; j<8;j++){
            if(eq[20]>j*3200)
                ofRect(camWidth+(camWidth/24)*23, (camHeight +10) +15 * (j+1), camWidth/24-3, 10);
        }
    }
    if(info){
        ofSetColor(255, 255, 255);
        myFont.drawString("Menu info", 10, camHeight+30);
        myFont.drawString("'i' Show/Hide menu", 10, camHeight+50);
        myFont.drawString("'0' Mirror on/off", 10, camHeight+65);
        myFont.drawString("'9' Equalizer on/off", 10, camHeight+80);
        myFont.drawString("'1' sepia Effect on/off", 10, camHeight+95);
        myFont.drawString("'2' B&W Effect on/off", 10, camHeight+110);
        myFont.drawString("'3' Dark B&W Effect on/off", 10, camHeight+125);
        myFont.drawString("'4' Var B&W Effect on/off", 10, camHeight+140);
        myFont.drawString("'5' Rare3D Effect on/off", 10, camHeight+155);
        myFont.drawString("'6' Color Deviate Effect on/off", 10, camHeight+170);
        myFont.drawString("'7' Points Mode on/off", 10, camHeight+185);
        myFont.drawString("'8' Blur on/off", 10, camHeight+200);
        myFont.drawString("'ESC' End App", 10, camHeight+215);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case'i':
            info = !info;
            break;
        case'0':
            mirror = !mirror;
            break;
        case '9':
            equalizer = !equalizer;
            break;
        case '1':
            sepia = !sepia;
            grayScale = false;
            darkGrayScale = false;
            varGrayScale = false;
            rare3D = false;
            canalDeviate = false;
            break;
        case '2':
            grayScale = !grayScale;
            darkGrayScale = false;
            varGrayScale = false;
            sepia = false;
            rare3D = false;
            canalDeviate = false;
            break;
        case '3':
            darkGrayScale = !darkGrayScale;
            grayScale = false;
            varGrayScale = false;
            sepia = false;
            rare3D = false;
            canalDeviate = false;
            break;
        case '4':
            varGrayScale = !varGrayScale;
            darkGrayScale = false;
            grayScale = false;
            sepia = false;
            rare3D = false;
            canalDeviate = false;
            break;
        case '5':
            rare3D = !rare3D;
            mirror = true;
            sepia = false;
            grayScale = false;
            darkGrayScale = false;
            varGrayScale = false;
            canalDeviate = false;
            break;
        case '6':
            rare3D = false;
            mirror = false;
            sepia = false;
            grayScale = false;
            darkGrayScale = false;
            varGrayScale = false;
            canalDeviate = !canalDeviate;
            break;
        case '7':
            points = !points;
            break;
        case '8':
            blur = !blur;
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}