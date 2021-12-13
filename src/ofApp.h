#pragma once

#include "ofMain.h"
#include <map>
#include "ofxGui.h"
#include <deque>
#include <vector>
#include "ofMath.h"
#include "essentiaSpectrum.h"

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

class ofApp : public ofBaseApp{
	
	public:

		//The most fundamental functions for an OF sketch
		void setup();
		void update();
		void draw();
		
		void mouseMoved(int x, int y);
		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);
		
		//These functions are called everytime an input/output buffer is filled
		void audioOut(ofSoundBuffer & buffer);
		void audioIn(ofSoundBuffer & buffer);

		//callback functions used by GUI panel
		void setBinRange(ofAbstractParameter& e);
		void toggleSound();


		int bufferSize; //Can be changed in setup()
		ofSoundStream soundStream;

		ofxPanel gui;
		ofParameter<float> measurementDensity;
		ofParameter<int> leftBin;
		ofParameter<int> binSpread;
		ofParameterGroup binParams;

		ofxButton soundToggle;
		bool isSoundRunning = true;

		const int MIN_BINS = 10; //This restricts the zoom level to show at least 10 bins

		essentiaSpectrum spec; //essentiaSpectrum implements the essentia algorithm AND handles drawing it to the OF window.

	
};
