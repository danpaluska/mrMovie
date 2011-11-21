#pragma once

#include "ofMain.h"

class mrmovie: public ofBaseApp
{
	public:

        mrmovie(string director = "/home/allasiabroadcaster/Videos/copies/");
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

        string path;
        void load(string path);
        void load();
        vector<string> files;

        int currentFile;
        ofVideoPlayer 		movie;

        void next();
        void previous();
        void set(int i);

        long loadedAtS;
};

