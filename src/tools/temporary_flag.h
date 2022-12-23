#ifndef TEMPORARY_FLAG_H
#define TEMPORARY_FLAG_H
#include <Arduino.h>

class TemporaryFlag
{
public:
    unsigned long duration_us = 1000000;
    unsigned long expiration_time = 0;
    void set() { this->expiration_time = micros() + duration_us; }
    bool isUp() { return micros() < this->expiration_time; }
    // void update();
};

#endif // TEMPORARY_FLAG_H