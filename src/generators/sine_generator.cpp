#include "sine_generator.h"
float SineGenerator::getSample()
{
    if(position > samplesPerCycle - 1)
        position = 0;

    float output = 0;

    int idx = map(position, 0, samplesPerCycle, 0, 1023);
    output = this->lut[idx];
    
    position++;
    return output;
}

void SineGenerator::initLUT(int size)
{
    this->lutSize = size;
    this->lut = (float*) malloc(lutSize * sizeof(float));
    
    for(int i=0;i<lutSize;i++)
    {
        float angle = (float)i* TWO_PI / (float)lutSize;
        this->lut[i] = sin(angle);
    }
}