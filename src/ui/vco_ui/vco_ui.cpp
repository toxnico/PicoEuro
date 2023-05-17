#include "vco_ui.h"

repeating_timer_t vco_clock_timer;
bool sts = false;

bool timer_cb(repeating_timer_t *tmr)
{
    auto vco = (VcoUI*)tmr->user_data;

    if(vco)
    {
        auto sample = vco->sine.getSample();
        float voltage = sample;// == 0 ? 0 : 5;
        //vco->io()->setLedLeft(sample > 0);
        
        if(vco->calibrations)
        {
            //Serial.printf("Calibrations : %x", vco->calibrations);
            //while(true);
            vco->io()->setCVOut(voltage, 0, vco->calibrations);
            //vco->io()->setGateOut0(sample > 0);
        }
    }
    //Serial.println("Tick!");
    
    //auto val = vco->io()->getGateOut0();
    sts = !sts;
    //vco->io()->setGateOut0(sts);
    //vco->io()->setLedLeft(0);

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

    for(int i=0;i<sine_wave_length;i++)
    {
        disp->drawPixel(i, sine_wave[i], WHITE);
    }
}
void VcoUI::handleIO()
{
    handleEncoderLongPressToGoBack();

    io()->setCVOut(0, 1, this->calibrations);
    io()->setCVOut(3, 1, this->calibrations);
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

    for(int i=0;i<1024;i++)
    {
        float angle = (float)i* TWO_PI / 1024.0;
        sine.sine[i] = sin(angle);
        //Serial.println(sine.sine[i]);
    }

    //while(true);

    sine.setSampleRate(sampleRate);
    sine.setFrequency(freq);
}


void VcoUI::onEnter()
{
    for(int i=0;i< sine_wave_length;i++)
    {
        float angle = (float)i * TWO_PI / (float)sine_wave_length;
        sine_wave[i] = 64-(sin(angle) * 32 + 32);
    }
}
