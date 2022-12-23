#include "linreg.h"

LinRegParams computeLinReg(Point *points, uint8_t count)
{
    float sumX = 0;
    float sumY = 0;
    float sumXsq = 0;
    float sumXY = 0;
    for (uint8_t i = 0; i < count; i++)
    {
        sumX = sumX + points[i].x;
        sumXsq = sumXsq + (points[i].x * points[i].x);
        sumY = sumY + points[i].y;
        sumXY = sumXY + (points[i].x * points[i].y);
    }

    LinRegParams prm;
    

    float d = count * sumXsq - sumX * sumX;
    prm.a = (count * sumXY - sumX * sumY) / d;
    prm.b = (sumY * sumXsq - sumX * sumXY) / d;

    return prm;
}