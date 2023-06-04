#ifndef SINE_GENERATOR_H
#define SINE_GENERATOR_H

#include <Arduino.h>
#include "generator.h"

class SineGenerator: public Generator
{
    public:
    int getSample();
    void  initLUT(int size);
};

#endif //SINE_GENERATOR_H