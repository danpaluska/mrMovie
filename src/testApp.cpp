#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);

dirName = "/home/allasiabroadcaster/Videos/copies/";
	dir.listDir(dirName);
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
fileCounter = (int)dir.size();
fileNumber=0;
	frameByframe = false;
movieCurrent = dirName + "camTL-20110410-sndtrk.mp4";
//	fingerMovie.loadMovie("movies/fingers.mov");
//	fingerMovie.loadMovie("/home/allasiabroadcaster/Videos/copies/camTL-20110410-sndtrk.mp4");
string temp = dirName + "camTL-20110410-sndtrk.mp4";
	fingerMovie.loadMovie(temp);
	fingerMovie.play();
}

//--------------------------------------------------------------
void testApp::update(){
    fingerMovie.idleMovie();
}

//--------------------------------------------------------------
void testApp::draw(){

ofSetColor(ofColor::gray);

// wants to move to setup
	for(int i = 0; i < (int)dir.size(); i++){
       fileName[i] =dir.getName(i);
//		string fileInfo = "file " + ofToString(i + 1) + " = " + fileName[i];
		string fileInfo = "file " + ofToString(i + 1) + " = " + dir.getName(i);
		ofDrawBitmapString(fileInfo, 50,i * 20 + 50);
	}
 fileCounter = (int)dir.size();

	ofSetHexColor(0xFFFFFF);
    fingerMovie.draw(20,20);
    ofSetHexColor(0x000000);
    unsigned char * pixels = fingerMovie.getPixels();
    // let's move through the "RGB" char array
    // using the red pixel to control the size of a circle.
    //        for (int i = 4; i < 320; i+=8){
    //            for (int j = 4; j < 240; j+=8){
    //                unsigned char r = pixels[(j * 320 + i)*3];
    //                float val = 1 - ((float)r / 255.0f);
    //                ofCircle(400 + i,20+j,10*val);
    //            }
    //        }


    ofSetHexColor(0x000000);
	ofDrawBitmapString("press f to change",120,520);
    if(frameByframe) ofSetHexColor(0xCCCCCC);
    ofDrawBitmapString("mouse speed position",120,540);
    if(!frameByframe) ofSetHexColor(0xCCCCCC); else ofSetHexColor(0x000000);
    ofDrawBitmapString("keys <- -> frame by frame " ,190,540);
    ofSetHexColor(0x000000);

    ofDrawBitmapString("frame: " + ofToString(fingerMovie.getCurrentFrame()) + "/"+ofToString(fingerMovie.getTotalNumFrames()),20,580);
    ofDrawBitmapString("duration: " + ofToString(fingerMovie.getPosition()*fingerMovie.getDuration(),2) + "/"+ofToString(fingerMovie.getDuration(),2),20,600);
    ofDrawBitmapString("speed: " + ofToString(fingerMovie.getSpeed(),2),20,620);

    if(fingerMovie.getIsMovieDone()){
    fileNumber++;
    fingerMovie.closeMovie();
    fingerMovie.loadMovie(dirName+fileName[fileNumber]);
    fingerMovie.play();
    //        ofSetHexColor(0xFF0000);
    //      ofDrawBitmapString("end of movie",20,640);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    switch(key){
        case 'f':
            frameByframe=!frameByframe;
            fingerMovie.setPaused(frameByframe);
        break;
        case OF_KEY_LEFT:
            fingerMovie.previousFrame();
        break;
        case OF_KEY_RIGHT:
            fingerMovie.nextFrame();
        break;
        case '0':
            fingerMovie.firstFrame();
        break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
//	if(!frameByframe){
//        int width = ofGetWidth();
//        float pct = (float)x / (float)width;
//        float speed = (2 * pct - 1) * 5.0f;
//        fingerMovie.setSpeed(speed);
//	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
//	if(!frameByframe){
//        int width = ofGetWidth();
//        float pct = (float)x / (float)width;
//        fingerMovie.setPosition(pct);
//	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
//	if(!frameByframe){
//        fingerMovie.setPaused(true);
//	}
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
//	if(!frameByframe){
//        fingerMovie.setPaused(false);
//	}
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
