#ifndef SQUARE_GENERATOR_H
#define SQUARE_GENERATOR_H

#include <Arduino.h>
#include "generator.h"

class SquareGenerator: public Generator
{
    public:
    int getSample();
    void  initLUT(int size);
};

#endif //SQUARE_GENERATOR_H