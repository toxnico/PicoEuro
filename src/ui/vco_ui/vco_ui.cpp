#include "vco_ui.h"
#include "synth/waves.h"

bool cb(repeating_timer_t tmr)
{
	Oscillator *osc = (Oscillator *)tmr.user_data;
	auto v = osc->next();
	float gain = 4.0;
	
	IOManager::getInstance()->setCVOut(v * gain, 0);
	return true;
}

VcoUI::VcoUI()
{
	osc = new Oscillator(sine_wave, WAVE_SIZE);
	osc->setFrequency(this->frequency);
	osc->setTimerPeriod_us(VCO_TIMER_PERIOD_US);

	repeating_timer_callback_t cb;
	repeating_timer_t tmr;

	
	add_repeating_timer_us(- VCO_TIMER_PERIOD_US, cb, &osc, &tmr);
}

void VcoUI::draw()
{
	disp->setCursor(0, 10);
	disp->print("VCO");
}

void VcoUI::handleIO()
{
	// read the input CV
	auto inputCV = io()->cvInVolts[0];

	auto baseFreq = 220;

	auto newFreq = baseFreq * pow(2, inputCV);

	//osc->setFrequency(newFreq);

	// retrieve the oscillator value (between -1.0 and 1.0)
	//auto v = osc->next();
	//float gain = 4.0;
	//io()->setCVOut(v * gain, 0);
}

void VcoUI::onExit()
{
}

void VcoUI::onEnter()
{
}