#include "vco_ui.h"

repeating_timer_t vco_clock_timer;

/**
 * Timer callback
 */
bool timer_cb(repeating_timer_t *tmr)
{
    auto vco = (VcoUI*)tmr->user_data;

    if(vco)
    {
        auto sample = vco->sine.getSample();
        float voltage = sample;

        if(vco->calibrations)
        {
            vco->io()->setCVOut(voltage, 0, vco->calibrations);
        }
    }

    return true;
}

VcoUI::VcoUI()
{
    //delay(2000);
    //Serial.println("VCO init");
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
}

void VcoUI::onExit()
{

}

void VcoUI::begin()
{
    int sampleRate = 20000;
    float freq = 1000;

    int interval = 1000000 / sampleRate;
    bool res = add_repeating_timer_us(-interval, timer_cb, this, &vco_clock_timer);

    sine.initLUT(1024);

    sine.setSampleRate(sampleRate);
    sine.setFrequency(freq);
}


void VcoUI::onEnter()
{
    
}
