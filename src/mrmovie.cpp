#include "mrmovie.h"
#include "twitcurl.h"
#include <iostream>
#include <fstream>

mrmovie::mrmovie(string path)
{
    this->path = path;
}

//--------------------------------------------------------------
void mrmovie::setup()
{
    shortMsg="";
    currentFile=0;
    load(); // loads a movie
    twitSetup(); //sets up twitter oauth connection
    proFont.loadFont("ProFontWindows.ttf", 14);
    loadedImageAtS=0.0;
	alpha=65;
    ofBackground(0,0,0);
    initImages();
    xwalk=700;
    ywalk=5;
}

void mrmovie::initImages()
{
 //
//
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


   if ((ofGetElapsedTimef()-loadedImageAtS) > 15.0)
           loadImageLocal();



}

void mrmovie::loadImageLocal()
{
recentImages[imageCounter].loadImage("/tmp/motion/lastsnap.jpg");
imageCounter=(imageCounter+1)%5;
loadedImageAtS = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void mrmovie::draw()
{
	//ofBackground(0,0,0);
    ofSetColor(ofColor::white);

    movie.draw(05, 40);

    //ofDrawBitmapString(files[currentFile], 10,10);
    //proFont.drawString(" videos are uploaded daily to http://youtube.com/allasiatwvee", 10,10);
    ofDrawBitmapString(" videos are uploaded daily to http://youtube.com/allasiatwvee", 10,10);
    ofDrawBitmapString(" <    >  < > <> use the arrrow keys to jump to next/previous movie <> < >  <    >", 10,25);
    ofDrawBitmapString("type and press enter to send a tweet to http://twitter.com/allasiatwvee",15,580);
    drawMsg();

recentImages[max(1,imageCounter)].draw(680,5,400,300);
//recentImages[max(1,imageCounter)].draw(xwalk,ywalk,400,300);

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

    string s = shortMsg;
     if((key == 10) ||  (key == 13)){ // RETURN
    // send the message, clear the messafe
    poststatus(s);
    shortMsg="";
    }
    else if((key == 8) || (key == 127)) { // BACKSPACE
        if(s.size() > 0)
            shortMsg = s.substr(0, s.length() - 1);
    }
    else if((key >= 32) && (key <= 255)) { // NORMAL KEY
        s += (char)key;
        shortMsg=s;
    }


}


void mrmovie::twitSetup()
{

    XML.loadFile("settings.xml");

 /* Set twitter username and password */
    string userName = XML.getValue("keys:user","user",0);
    string passWord = XML.getValue("keys:pass","pass",0);

// cout << userName << "=user";
    twitterObj.setTwitterUsername( userName );
    twitterObj.setTwitterPassword( passWord );

    /* OAuth flow begins */
    /* Step 0: Set OAuth related params. These are got by registering your app at twitter.com */
    string consumerKey = XML.getValue("keys:ConsumerKey","user",0);
    string consumerSecret = XML.getValue("keys:ConsumerSecret","pass",0);
    twitterObj.getOAuth().setConsumerKey( consumerKey );
    twitterObj.getOAuth().setConsumerSecret( consumerSecret );

    string tokenKey = XML.getValue("keys:TokenKey","user",0);
    string tokenSecret = XML.getValue("keys:TokenSecret","pass",0);
    /* Step 1: Check if we alredy have OAuth access token from a previous run */
    std::string myOAuthAccessTokenKey(tokenKey);
    std::string myOAuthAccessTokenSecret(tokenSecret);
    std::ifstream oAuthTokenKeyIn;
    std::ifstream oAuthTokenSecretIn;

    oAuthTokenKeyIn.open( "twitterClient_token_key.txt" );
    oAuthTokenSecretIn.open( "twitterClient_token_secret.txt" );

 std::string tmpStr;
    std::string replyMsg;
    char tmpBuf[1024];
    int tmpVar = 0;

    memset( tmpBuf, 0, 1024 );
    oAuthTokenKeyIn >> tmpBuf;
    myOAuthAccessTokenKey = tmpBuf;

    memset( tmpBuf, 0, 1024 );
    oAuthTokenSecretIn >> tmpBuf;
    myOAuthAccessTokenSecret = tmpBuf;

    oAuthTokenKeyIn.close();
    oAuthTokenSecretIn.close();


    if( myOAuthAccessTokenKey.size() && myOAuthAccessTokenSecret.size() )
    {
        /* If we already have these keys, then no need to go through auth again */
        printf( "\nUsing:\nKey: %s\nSecret: %s\n\n", myOAuthAccessTokenKey.c_str(), myOAuthAccessTokenSecret.c_str() );

        twitterObj.getOAuth().setOAuthTokenKey( myOAuthAccessTokenKey );
        twitterObj.getOAuth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
    }
    else
    {
        /* Step 2: Get request token key and secret */
        twitterObj.oAuthRequestToken( tmpStr );

        /* Step 3: Get PIN  */
        memset( tmpBuf, 0, 1024 );
        printf( "\nDo you want to visit twitter.com for PIN (0 for no; 1 for yes): " );
        gets( tmpBuf );
        tmpVar = atoi( tmpBuf );

        if( tmpVar > 0 )
        {
            /* Ask user to visit twitter.com page and get PIN */
            memset( tmpBuf, 0, 1024 );
            printf( "\nPlease visit this link in web browser and authorize this application:\n%s", tmpStr.c_str() );
            printf( "\nEnter the PIN provided by twitter: " );
            gets( tmpBuf );
            tmpStr = tmpBuf;
            twitterObj.getOAuth().setOAuthPin( tmpStr );
        }
        else
        {
            /* Else, get it via twitcurl PIN handling */
            twitterObj.oAuthHandlePIN( tmpStr );
        }

        /* Step 4: Exchange request token with access token */
        twitterObj.oAuthAccessToken();

        /* Step 5: Now, save this access token key and secret for future use without PIN */
        twitterObj.getOAuth().getOAuthTokenKey( myOAuthAccessTokenKey );
        twitterObj.getOAuth().getOAuthTokenSecret( myOAuthAccessTokenSecret );

        /* Step 6: Save these keys in a file or wherever */
        std::ofstream oAuthTokenKeyOut;
        std::ofstream oAuthTokenSecretOut;

        oAuthTokenKeyOut.open( "twitterClient_token_key.txt" );
        oAuthTokenSecretOut.open( "twitterClient_token_secret.txt" );

        oAuthTokenKeyOut.clear();
        oAuthTokenSecretOut.clear();

        oAuthTokenKeyOut << myOAuthAccessTokenKey.c_str();
        oAuthTokenSecretOut << myOAuthAccessTokenSecret.c_str();

        oAuthTokenKeyOut.close();
        oAuthTokenSecretOut.close();
    }
    /* OAuth flow ends */

getUserTimeline();



}

void mrmovie::getUserTimeline()
{

  /* Get user timeline */
  std::string replyMsg;
    replyMsg = "";
    printf( "\nGetting user timeline\n" );
    if( twitterObj.timelineUserGet( false, false, 25 ) )
    {
        twitterObj.getLastWebResponse( replyMsg );
        printf( "\ntwitterClient:: twitCurl::timelineUserGet web response:\n%s\n", replyMsg.c_str() );
    }
    else
    {
        twitterObj.getLastCurlError( replyMsg );
        printf( "\ntwitterClient:: twitCurl::timelineUserGet error:\n%s\n", replyMsg.c_str() );
    }

    // write to file
    ofstream outfile ("data/tweetlist2.xml");

  // >> i/o operations here <<
    outfile << replyMsg.c_str();

    outfile.close();
XML.clear();
bool isFileLoaded = XML.loadFile("tweetlist2.xml");
cout << isFileLoaded << " that the file was loaded " << endl;


//bool isBufferLoaded = XML.loadFromBuffer(replyMsg.c_str());
//cout << isBufferLoaded << " that the string buffer was loaded ";
int numTags = XML.getNumTags("statuses:status");
cout << numTags << " num tags---" << endl;
XML.pushTag("statuses");
	for(int i = 0; i < 25; i++)
	{

	timeLine[i] = XML.getValue("status:created_at","--",i) + " " +XML.getValue("status:text","--",i);
	//ofDrawBitmapString(timeLine[i],15,680+15*i);
	cout << timeLine[i] << endl;
	}


}
void mrmovie::poststatus()
{
     std::string tmpStr;
    std::string replyMsg;
       /* Post a new status message */
    char statusMsg[1024];
    memset( statusMsg, 0, 1024 );
    printf( "\nEnter a new status message: " );
    gets( statusMsg );
    tmpStr = statusMsg;
    replyMsg = "";
    if( twitterObj.statusUpdate( tmpStr ) )
    {
        twitterObj.getLastWebResponse( replyMsg );
        printf( "\ntwitterClient:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
    }
    else
    {
        twitterObj.getLastCurlError( replyMsg );
        printf( "\ntwitterClient:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
    }



}
void mrmovie::poststatus(string statusMsg)
{
     std::string tmpStr;
    std::string replyMsg;
       /* Post a new status message */
//    char statusMsg[1024];
  //  memset( statusMsg, 0, 1024 );

    tmpStr = statusMsg;
    replyMsg = "";
    if( twitterObj.statusUpdate( tmpStr ) )
    {
        twitterObj.getLastWebResponse( replyMsg );
        printf( "\ntwitterClient:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
         ofSetColor(ofColor::white);

        ofDrawBitmapString("yeah, you just tweeted this msg:" +tmpStr,15,660);
    }
    else
    {
        twitterObj.getLastCurlError( replyMsg );
        printf( "\ntwitterClient:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
         ofSetColor(ofColor::white);

        ofDrawBitmapString("i can't tell you why buy that tweet failed to make it",15,660);
    }


getUserTimeline();



}


void mrmovie::drawMsg()
{
    ofSetColor(ofColor::gray);
    ofRect(15,585,640,40);
    ofSetColor(ofColor::white);
    if (shortMsg.length() > 80)
    {
    string s1 = shortMsg.substr(0,80);
    string s2 = shortMsg.substr(80,shortMsg.length());
     ofDrawBitmapString(s1,15,600);
      ofDrawBitmapString(s2,15,615);
    }
    else
    {
    ofDrawBitmapString(shortMsg,15,600);
    }
    int left = 140-shortMsg.length();

    string tmp = "you have " + ofToString(left) + " characters left.";
    ofDrawBitmapString(tmp,15,640);

     for(int i = 0; i < 20; i++)
	{
	ofDrawBitmapString(timeLine[i],15,680+15*i);
	}

}
