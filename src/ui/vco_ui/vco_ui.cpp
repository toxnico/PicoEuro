#include "vco_ui.h"
#include "generators/sine_generator.h"
#include "generators/square_generator.h"
#include "generators/saw_generator.h"

repeating_timer_t vco_clock_timer;

/**
 * Timer callback
 */
bool timer_cb(repeating_timer_t *tmr)
{
    auto vco = (VcoUI *)tmr->user_data;

    if (vco)
    {
        auto sample = vco->wave->getSample();
        // float voltage = sample;
        vco->io()->setCVOutRaw(sample, 0);
    }

    return true;
}

VcoUI::VcoUI()
{
    // delay(2000);
    // Serial.println("VCO init");
    this->id = UI_VCO;
}

void VcoUI::draw()
{
    return;
    disp->setCursor(10, 10);
    disp->print("VCO");
}
void VcoUI::handleIO()
{
    handleEncoderLongPressToGoBack();

    auto cv0 = io()->cvInVolts[0];
    auto cv1 = io()->cvInVolts[1];

    //cv0 = (double)map(io()->potValue, 0, 4095, 0, 5000) / 1000.0;

    auto baseFreq = (double)map(io()->potValue, 0, 4095, 0, 4000);

    auto finalFreq = baseFreq * pow(2, cv0);

    wave->setFrequency(finalFreq);

    /*
        // benchmark

        int sampleCount = 1000000;

        auto start = micros();
        unsigned long end;

        for (int i = 0; i < sampleCount; i++)
        {
            auto sample = sine.getSample();

            this->io()->setCVOutRaw(sample, 0);
        }

        end = micros();

        auto samplesPerSecond = (float)sampleCount / (float)((end - start) / 1000000.0);

        Serial.printf("Benchmark result : %f\n", samplesPerSecond);
        */
}

void VcoUI::onExit()
{
}

void VcoUI::begin()
{
    wave = new SquareGenerator();
    //int sampleRate = 50000;
    int sampleRate = 40000;
    float freq = 5000;

    int interval = 1000000 / sampleRate;
    //bool res = add_repeating_timer_us(-interval, timer_cb, this, &vco_clock_timer);
    wave->initLUT(1024);

    wave->setSampleRate(sampleRate);
    wave->setFrequency(freq);
}

void VcoUI::onEnter()
{
}
