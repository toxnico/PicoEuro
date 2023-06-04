#include "square_generator.h"
int SquareGenerator::getSample()
{
    if(position > samplesPerCycle - 1)
        position = 0;

    float output = 0;

    int idx = map(position, 0, samplesPerCycle, 0, 1023);
    output = this->lut[idx];
    
    position++;
    return output;
/*
    return;
    if(position > samplesPerCycle - 1)
        position = 0;

    float output = 4095;

    if(position < samplesPerCycle / 2)
        output = 0;

    position++;
    return output;
*/
}

void SquareGenerator::initLUT(int size)
{
    this->lutSize = size;
    this->lut = (int*) malloc(lutSize * sizeof(int));
    
    for(int i=0;i<lutSize;i++)
    {
        this->lut[i] = i < lutSize / 2 ? 0:4095;

        //Serial.println(this->lut[i]);
    }
}