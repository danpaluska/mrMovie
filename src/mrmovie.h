#pragma once

#include "ofMain.h"
//#include "ofxTwitter.h"
#include "twitcurl.h"
#include "ofxXmlSettings.h"

class mrmovie: public ofBaseApp
{
	public:

        mrmovie(string director = "/home/allasiabroadcaster/Videos/copies/");
		void setup();
		void update();
		void draw();


		void keyPressed(int key);
			void twitSetup();
			void poststatus();
			void poststatus(string shortMsg);
        void drawMsg();
            void loadImageLocal();
             void getUserTimeline();

        ofxXmlSettings XML;

        string path;
        void load(string path);
        void load();
        vector<string> files;
        string shortMsg;
        string timeLine[25];
        ofTrueTypeFont		proFont;

        int currentFile;
        ofVideoPlayer 		movie;
        twitCurl      twitterObj;
        void next();
        void previous();
        void set(int i);

        long loadedAtS;
        long loadedImageAtS;

        ofVideoGrabber 		vidGrabber;
		unsigned char * 	videoInverted;
		unsigned char * 	videoLast;
		unsigned char * 	videoNminus2;
		ofTexture			videoTexture;
		ofTexture			videoTexture2;
		ofTexture			videoTexture3;
        ofImage             recentImages[10];
        int                 imageCounter;

    int alpha; // 1 to 99

};

