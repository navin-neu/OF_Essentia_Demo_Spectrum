#include "ofApp.h"

/***************************************************/
/*! 
	Basic OpenFrameworks Sketch implementing a spectrum analyzer.
	The GUI panel allows the user to specify which bin the want
	the view to start at, as well as the number of bins to display.
	Together these act as a horizontal scroll and zoom level.
	
	This sketch demos the animating capabilities of OpenFrameworks,
	as well as the use of addons like ofxGui. It also demonstrates
	the use of callback functions such as mouseMoved(). Finally,
	it shows how shapes can be drawn by classes apart from ofApp.

	Navin K. 2021
*/
/***************************************************/

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(255,255,255);
	ofSetColor(0,0,0);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofNoFill();

	bufferSize = 2048;

	//OfSoundStream will allow us to use real time audio
    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.setInListener(this);
    settings.numOutputChannels = 2;
    settings.numInputChannels = 2;
    settings.numBuffers = 4;
    settings.bufferSize = bufferSize;
	settings.sampleRate = 48000;
	soundStream.setup(settings);

	//Pass required data to initizialize spectrum
	spec.setup(soundStream.getSampleRate(), settings.bufferSize);

	//Setup listeners
	soundToggle.addListener(this, &ofApp::toggleSound); //whenever soundToggle is pressed, toggleSound() is called

	//Construct GUI panel
	ofxGuiSetFont("Questrial-Regular.ttf",14,true,true);
	gui.setup("Controls");
	gui.add(leftBin.set("Starting Bin", 0, 0, bufferSize/2 - MIN_BINS));
	gui.add(binSpread.set("# of Bins", bufferSize/2, MIN_BINS, bufferSize/2));
	gui.add(measurementDensity.set("Grid Density", 0.15f, 0.0f, 0.75f));
	gui.add(soundToggle.setup("On/Off"));

	//define a parameter group so that all members of the group call the same function
	binParams.setName("binParams");
	binParams.add(leftBin);
	binParams.add(binSpread);
	ofAddListener(binParams.parameterChangedE(), this, &ofApp::setBinRange);

}

//--------------------------------------------------------------
void ofApp::update(){
	//only thing to do is update the spectrum to be displayed
	spec.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	//initial setup for scene
	ofBackgroundGradient(ofColor::white, ofColor::darkGray);
	ofSetColor(ofColor::black);
	ofSetLineWidth(15);
	ofNoFill();

	//draw spectrum
	if (leftBin >= 0 && leftBin + binSpread <= bufferSize/2) //just here as an extra safety net...
	{
		spec.draw(leftBin, binSpread, measurementDensity);
	}

	gui.draw(); //dont forget to draw the panel too!
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){ 
	//We send location of mouse pointer to essentiaSpectrum to draw the vertical line 
	spec.updateMouseCoordinates(x,y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){ 
	//This could be used to make the scroll and zoom features controllable via keyboard as well.
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){
	//No audio output in this app, but the buffer works quite similarly as audioIn()
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & buffer){
	//Only thing to do with audio is pass it to Spectrum algo
	spec.updateBufferContents(buffer); 
}

//--------------------------------------------------------------
void ofApp::setBinRange(ofAbstractParameter& e)
{
	//This dynamically adjusts the range of the GUI parameters to specifying bins that don't actually exist.
	binSpread.setMax(bufferSize/2 - leftBin);
	if (binSpread + leftBin > bufferSize/2) { binSpread = binSpread.getMax(); }
}

//---------------------------------------------------------------
void ofApp::toggleSound()
{
	if (isSoundRunning) { 
		soundStream.stop();
		isSoundRunning = false;
		}
	else { 
		soundStream.start(); 
		isSoundRunning = true;
		}
}
