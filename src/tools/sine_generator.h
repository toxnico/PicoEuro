#ifndef SINE_GENERATOR_H
#define SINE_GENERATOR_H

#include <Arduino.h>

class SineGenerator
{
    private:
    uint64_t lastSampleTime = 0;
    public:
    int sampleRate = 44100;
    float frequency = 10.0;
    //float samplesPerCycle = 4410;
    int position = 0;

    inline void setSampleRate(int val) { this->sampleRate = val; }
    inline void setFrequency(int val) { this->frequency = val; }

    //inline float sampleInterval_us() { return 1000000.0 / sampleRate; }
    inline int samplesPerCycle() { return sampleRate / frequency; }
    float getSample();
    int getSampleSquare();

    float sine[1024];

};

#endif //SINE_GENERATOR_H