#ifndef WAVES_H
#define WAVES_H

#define WAVE_SIZE 2048

static float sine_wave [WAVE_SIZE];
static float sawtooth_wave [WAVE_SIZE];
static float square_wave [WAVE_SIZE];


void initSine(float *arr, int size);
void initSawTooth(float *arr, int size);
void initSquare(float *arr, int size);
void initSquareWithDuty(float dutyPercent, float *arr, int size);

#endif //WAVES_H