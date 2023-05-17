#ifndef GENERATOR_H
#define GENERATOR_H

#include <Arduino.h>

class Generator
{

public:
    int sampleRate = 44100;
    float frequency = 10.0;
    int samplesPerCycle = sampleRate / frequency;
    int lutSize = 0;

    int position = 0;

    inline void setSampleRate(int val)
    {
        this->sampleRate = val;
        updateSamplesPerCycle();
    }
    inline void setFrequency(int val)
    {
        this->frequency = val;
        updateSamplesPerCycle();
    }

    inline void updateSamplesPerCycle() { samplesPerCycle = sampleRate / frequency; }
    //inline int samplesPerCycle() { return samplesPerCycle; }
    virtual float getSample();
    //int getSampleSquare();
    virtual void initLUT(int size);

    float *lut = NULL;

};

#endif // GENERATOR_H