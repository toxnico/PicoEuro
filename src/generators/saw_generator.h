#ifndef SAW_GENERATOR_H
#define SAW_GENERATOR_H

#include <Arduino.h>
#include "generator.h"

class SawGenerator: public Generator
{
    public:
    float getSample();
    void  initLUT(int size);
};

#endif //SAW_GENERATOR_H