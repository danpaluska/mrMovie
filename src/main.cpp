#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "mrmovie.h"

//========================================================================
int main( int argn, char * argv[] )
{
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
//	ofRunApp( new testApp());

    string directory = "";

    for (int i = 0; i < argn; i++)
    {
        if ( strcmp(argv[i], "-directory") == 0 && i + 1 < argn )
        {
            directory = argv[i+1];
            break;
        }
    }

    if (directory != "")
        ofRunApp( new mrmovie(directory) );
    else
        ofRunApp( new mrmovie() );

}
