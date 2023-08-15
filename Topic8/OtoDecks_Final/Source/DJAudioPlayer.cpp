/*
 ==============================================================================
 
 DJAudioPlayer.cpp
 Created: 1 Aug 2023 10:14:45pm
 Author:  Liu Junhua
 
 ==============================================================================
 */

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(){}
DJAudioPlayer::~DJAudioPlayer(){}


//==============================================================================
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
    formatManager.registerBasicFormats();
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {
    resampleSource.getNextAudioBlock(bufferToFill);
    
}
void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL){
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader,true));
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset (newSource.release());
    }
}

void DJAudioPlayer::setGain(double gain){
    if (gain < 0 || gain > 1.0){
        std::cout << "DJAudioPlayer::setGain: gain should be between 0 and 1" << std::endl;
    } else{
        transportSource.setGain(gain);
    }
}
void DJAudioPlayer::setSpeed(double ratio){
    if (ratio < 0 || ratio > 100.0){
        std::cout << "DJAudioPlayer::setSpeed: ratio should be between 0 and 100" << std::endl;
    } else{
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSec){
    transportSource.setPosition(posInSec);
}

void DJAudioPlayer::setPositionRelative(double pos){
    if (pos < 0 || pos > 1.0){
        std::cout << "DJAudioPlayer::setPositionRelative: relative pos should be between 0 and 1." << std::endl;
    } else{
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start(){
    transportSource.start();
}
void DJAudioPlayer::stop(){
    transportSource.stop();
}

