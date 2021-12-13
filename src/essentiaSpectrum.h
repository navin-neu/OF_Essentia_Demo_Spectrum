#pragma once

#include <essentia/algorithmfactory.h>
#include <essentia/essentiamath.h>
#include "essentia.h"
#include "ofSoundBuffer.h"
#include "ofMain.h"

/***************************************************/
/*! 
	Helper class to encapsulate the usage of Essentia's
    Spectrum algorithm along with the logic needed to draw it
    in the OF sketch.

    Importing OfMain allows us to draw directly to the OF sketch from
    this class instead of ofApp. This is useful since it negates the need
    to pass the vector containing our spectrum info back to ofApp.

	Navin K. 2021
*/
/***************************************************/


class essentiaSpectrum {

public:

    essentiaSpectrum();

    ~essentiaSpectrum();

    //called in ofApp.setup()
    void setup(int sr, int bufferSize, float scale=1.0f);

    //called by ofApp.update()
    void update();

    //called by ofApp.draw()
    void draw(int startingBin, int binsToDraw, float measurementDensity = 0.15f);

    //called by by ofApp.audioIn()
    void updateBufferContents(ofSoundBuffer &buffer);

    //called by ofApp.mouseMoved()
    void updateMouseCoordinates(int x, int y);

private:

//Our spectrum algo along with vectors for input and output
essentia::standard::Algorithm* spectrum;
std::vector<essentia::Real> specInput, specOutput;

int sampleRate; //these will be used by our draw function
float scaleFactor;

int mouseX = -1; //arbitrary init values for mouse position
int mouseY = -1;

};