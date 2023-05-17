#include "square_generator.h"
float SquareGenerator::getSample()
{
    if(position > samplesPerCycle - 1)
        position = 0;

    float output = 1;

    if(position < samplesPerCycle)
        output = -1;

    position++;
    return output;
}

void SquareGenerator::initLUT(int size)
{
    /*
    this->lutSize = size;
    this->lut = (float*) malloc(lutSize * sizeof(float));
    
    for(int i=0;i<lutSize;i++)
    {
        float angle = (float)i* TWO_PI / (float)lutSize;
        this->lut[i] = sin(angle);
    }
    */
}