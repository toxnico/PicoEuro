#include "saw_generator.h"
float SawGenerator::getSample()
{
    if (position > samplesPerCycle - 1)
        position = 0;

    float output = 0;

    int idx = map(position, 0, samplesPerCycle, 0, 1023);
    output = this->lut[idx];

    position++;
    return output;
}

void SawGenerator::initLUT(int size)
{
    this->lutSize = size;
    this->lut = (float *)malloc(lutSize * sizeof(float));
    
    //y = a * x + b
    float a = 2.0 / (lutSize - 1);
    float b = -1.0;

    for (int i = 0; i < lutSize; i++)
    {
        this->lut[i] = a * i + b;
    }
}