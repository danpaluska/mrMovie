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

        ofxXmlSettings XML;

        string path;
        void load(string path);
        void load();
        vector<string> files;
        string shortMsg;

        int currentFile;
        ofVideoPlayer 		movie;
        twitCurl      twitterObj;
        void next();
        void previous();
        void set(int i);

        long loadedAtS;
};

