#include "mrmovie.h"

mrmovie::mrmovie(string path)
{
    this->path = path;
}

//--------------------------------------------------------------
void mrmovie::setup()
{
    currentFile=0;
    load();
}

void mrmovie::load()
{
    load(path);
}

void mrmovie::load(string path)
{
    movie.stop();
    if (!files.empty()) //if loaded, empty it
        files.clear();

	ofDirectory directory;
	directory.listDir(path);
	directory.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

    int nFiles = (int)directory.size();

	for(int i = 0; i < nFiles; i++)
	    files.push_back(path + directory.getName(i));

/*	for(int i = 0; i < files.size(); i++)
        cout<<files[i]<<endl;
*/

  //  currentFile = max(currentFile, nFiles-1);
    movie.loadMovie(files[currentFile]);
    movie.play();

    loadedAtS = ofGetElapsedTimef();
     cout << nFiles << " files loaded" << endl;
     cout << loadedAtS << " load time" << endl;
}

void mrmovie::next()
{
    set( (currentFile + 1) % files.size() );
}

void mrmovie::previous()
{
    set( (currentFile - 1 + files.size()) % files.size() );
}

void mrmovie::set(int i)
{
    currentFile = i;
    movie.closeMovie();
    movie.loadMovie(files[currentFile]);
    movie.play();
}

//--------------------------------------------------------------
void mrmovie::update()
{
    movie.idleMovie();

    if (movie.getIsMovieDone())
        next();

    if ((ofGetElapsedTimef()-loadedAtS) > 5000.0)
        load();
}

//--------------------------------------------------------------
void mrmovie::draw()
{
	ofBackground(155,155,155);
    ofSetColor(ofColor::gray);

    movie.draw(20, 20);

    ofDrawBitmapString(files[currentFile], 10,10);
}

//--------------------------------------------------------------
void mrmovie::keyPressed  (int key)
{
    switch(key)
    {
        case 'r':
            load(path);
            break;

        case OF_KEY_LEFT:
            previous();
            break;

        case OF_KEY_RIGHT:
            next();
            break;

        case '0':

            break;
    }
}
