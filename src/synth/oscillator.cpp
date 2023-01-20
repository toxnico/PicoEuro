#include "oscillator.h"

Oscillator::Oscillator(float *samples, int sampleCount)
{
	this->samples = samples;
	this->sampleCount = sampleCount;
}

int Oscillator::next()
{
	this->position = (int)(this->position + this->increment) % this->sampleCount;
	
	return this->samples[this->position];
}

void Oscillator::setFrequency(float freq)
{
	this->frequency = freq;
	auto signalPeriod_us = 1000000.0 / freq;
	
	/**
	 * let's say that next() will be called every 50us (fixed by the main timer)
     * if we're generating a 1000 Hz signal (i.e. 1000 us period), we need to run over the whole samples
	 * array in a period of 1000us, skipping some samples if necessary.
	 * 
	 * How many samples are we going to play during a signal period (1000us) ?
	 * 1000 / 50 = 20 samples
	 * 
	 * So the increment value will be (2048 / 20 = ) 102.4
	 * 
	 * Let's write that down:
	 * 
	 */
	
	auto samplesPerSignalPeriod = signalPeriod_us / (double)this->timerPeriod_us;
	this->increment = this->sampleCount / samplesPerSignalPeriod;
	


}

