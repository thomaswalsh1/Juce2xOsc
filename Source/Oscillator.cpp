/*
  ==============================================================================

    Oscillator.cpp
    Created: 24 Oct 2026 11:14:46am
    Author:  Thomas

  ==============================================================================
*/

#include "Oscillator.h"

void Oscillator::prepare(double newSampleRate) {
    sampleRate = newSampleRate;

    noteFrequency.reset(sampleRate, 0.0);
    offsetRatio.reset(sampleRate, SynthConstants::detuneSmoothingSeconds);
    frequencyMultiplier.reset(sampleRate, SynthConstants::controlSmoothingSeconds);
}

void Oscillator::setDetune(int newCoarse, float newFine) {
    if (newCoarse == coarse && newFine == fine) {
        return;
    }
    coarse = newCoarse;
    fine = newFine;
    updateOffsetRatio(false);
}

void Oscillator::setBend(float bendSemitones) {
    if (bendSemitones == bend) {
        return;
    }
    bend = bendSemitones;
    updateOffsetRatio(false);
}

void Oscillator::startNote(float fromHz, float toHz, float glideSeconds) {
    noteFrequency.reset(sampleRate, glideSeconds);
    noteFrequency.setCurrentAndTargetValue(fromHz);
    noteFrequency.setTargetValue(toHz);

    updateOffsetRatio(true);
    frequencyMultiplier.setCurrentAndTargetValue(frequencyMultiplier.getTargetValue());

    // same starting phase every note, so fm attacks sound consistent
    phase = 0.0f;
}

void Oscillator::updateOffsetRatio(bool force) {
    const float semitones = coarse + fine / 100.0f + bend;
    const float ratio = std::pow(2.0f, semitones / 12.0f);

    if (force) {
        offsetRatio.setCurrentAndTargetValue(ratio); // jump straight there on a new note
    }
    else {
        offsetRatio.setTargetValue(ratio); // smooth knob/bend changes
    }
}
