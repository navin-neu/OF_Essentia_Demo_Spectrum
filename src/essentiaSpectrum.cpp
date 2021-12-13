#include "essentiaSpectrum.h"

using namespace essentia::standard;

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

essentiaSpectrum::essentiaSpectrum()
{
    essentia::init();
}

essentiaSpectrum::~essentiaSpectrum()
{
    essentia::shutdown();
}

void essentiaSpectrum::setup(int sr, int bufferSize, float scale)
{
    //See essentia's "standard mode" documentation for explanations on this
    AlgorithmFactory& factory = essentia::standard::AlgorithmFactory::instance();
    spectrum = factory.create("Spectrum", "size", bufferSize);
    spectrum->input("frame").set(specInput);
    spectrum->output("spectrum").set(specOutput);

    scaleFactor = scale;
    sampleRate = sr;
}

void essentiaSpectrum::update()
{
    spectrum->compute(); //This will be called at framerate
}

void essentiaSpectrum::draw(int startingBin, int binsToDraw, float measurementDensity)
{
   
    float baseline = ofGetHeight() - 60.0f; //y position representing 0 amplitude.

    float lerpedWidth = (float) ofGetWidth()/(binsToDraw+1); //find how wide each bin should be on display
    float binFreqs = sampleRate * 0.5f / specOutput.size(); //The span of frequencies covered by 1 bin

    int indicatorPeriod = binsToDraw*measurementDensity; //how many bins should be between the pre-set frequency markers

    //Begin drawing the actual spectrum
    ofBeginShape();
	for (int i = 0; i < binsToDraw; i++){

        //find x position and place vertex according to amplitude
        float xPos = lerpedWidth*(i);
		ofVertex(xPos,baseline-specOutput.at(i+startingBin)*scaleFactor);

        //If this is a bin where we should draw a frequency marker,
        //calculate frequency value and draw the marker underneath the bin
        if (i % indicatorPeriod == 0) {
        ofDrawBitmapString(ofToString(binFreqs * (i+startingBin)),xPos, baseline + 50.0f);
        }
	}
	ofEndShape(false);

    //finally we draw a vertical line and frequency at mouse cursor x coordinate
    float freqAtMouse = binFreqs * (startingBin + binsToDraw * ((float) mouseX / ofGetWidth()));
    ofDrawBitmapString(ofToString(freqAtMouse), mouseX, baseline + 30.0f);
    ofDrawLine(mouseX, 0, mouseX, ofGetHeight());
}

void essentiaSpectrum::updateBufferContents(ofSoundBuffer &buffer)
{
    //Essentia includes a useful framecutter that might normally be used to parse audio into frames
    //However, in this case we can simply use each of the incoming buffers as a frame...

    int bufSize = buffer.getNumFrames();
    specInput.resize(bufSize);

    //We take in stereo audio and just sum it to mono
    for (int i = 0; i < bufSize; i++){
		specInput.at(i % bufSize) = buffer.getSample(i, 0) + buffer.getSample(i, 1);
	}

}

void essentiaSpectrum::updateMouseCoordinates(int x, int y)
{
    mouseX = x;
    mouseY = y;
}