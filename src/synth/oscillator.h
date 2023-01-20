#ifndef OSCILLATOR_H
#define OSCILLATOR_H

class Oscillator
{
protected:
    int position = 0;
    float increment = 1.0;
    float frequency = 0;
    int timerPeriod_us = 1;
    int samplesPlayedInThisPeriod = 0;

public:
    float *samples;
    int sampleCount = 0;

    Oscillator(float *samples, int sampleCount);

    int next();
    void setFrequency(float freq);
    inline float getFrequency() { return frequency; }
    inline void setTimerPeriod_us(int period_us) { this->timerPeriod_us = period_us; }
};

#endif // OSCILLATOR_H
