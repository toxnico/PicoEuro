#include "waves.h"
#include <Arduino.h>

void initSine(float *arr, int size)
{
	double increment = TWO_PI / (double)size;

	for (int i = 0; i < size; i++)
		arr[i] = sin(i * increment);
}

void initSawTooth(float *arr, int size)
{
	float amplitude = 2.0;
	float slope = amplitude / (float)size;

	for (int i = 0; i < size; i++)
		arr[i] = (float)i * slope - (amplitude / 2);
}

void initSquare(float *arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (i < (size / 2))
			arr[i] = 1.0;
		else
			arr[i] = 0;
	}
	
}

void initSquareWithDuty(float dutyPercent, float *arr, int size)
{
	int idx = (int)((float)size * dutyPercent);
	for (int i = 0; i < size; i++)
	{
		if (i < idx)
			arr[i] = 1.0;
		else
			arr[i] = 0;
	}
	
}