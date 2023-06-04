#include "sine_generator.h"
int SineGenerator::getSample()
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
    this->lut = (int*) malloc(lutSize * sizeof(int));
    
    for(int i=0;i<lutSize;i++)
    {
        float angle = (float)i* TWO_PI / (float)lutSize;
        float rawSin = sin(angle); //between -1.0 and +1.0
        this->lut[i] = (rawSin + 1) * 4095 / 2;

        //Serial.println(this->lut[i]);
    }
}