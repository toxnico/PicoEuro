#ifndef LINREG_H
#define LINREG_H
#include <Arduino.h>

typedef struct LinRegParams
{
    float a = 0;
    float b = 0;
} LinRegParams;

typedef struct Point
{
    float x = 0;
    float y = 0;
} Point;


LinRegParams computeLinReg(Point *points, uint8_t count);

#endif // LINREG_H