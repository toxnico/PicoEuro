#include "sine_generator.h"

int SineGenerator::getSampleSquare()
{
    int spc = samplesPerCycle();
    if(position > spc - 1)
        position = 0;

    int output = 0;
    if(position < spc / 2)
    {
        output = 0;
    }
    else
    {
        output = 4096;
    }

    position++;
    return output;
}

float SineGenerator::getSample()
{
    int spc = samplesPerCycle();

    if(position > spc - 1)
        position = 0;

    float output = 0;

    int idx = map(position, 0, spc, 0, 1023);
    output = this->sine[idx];
    //float angle = (float)position * TWO_PI / (float)spc;

    //output = sin(angle);

    position++;
    return output;
}