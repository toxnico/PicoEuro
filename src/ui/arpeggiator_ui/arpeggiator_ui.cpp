#include "arpeggiator_ui.h"

#include <Fonts/Org_01.h>

#include "graphics/graphics.h"
#include "io/iomanager.h"
#include "ui/uimanager.h"

ArpeggiatorUI::ArpeggiatorUI()
{
    this->id = UI_ARPEGGIATOR;

    // this->linkedMenuUI = UIManager::getInstance()->getUIById(UI_QUANTIZER_MENU);
}

void ArpeggiatorUI::draw()
{
    disp->setFont(&Org_01);
    disp->setTextSize(1);
    disp->setCursor(0, 10);

    disp->print("Arpeggiator");
}

void ArpeggiatorUI::handleGateIRQ(uint8_t channel, bool state)
{
    // are we in a rising edge? time to play a note !
    if (channel == 0 && state)
    {
        currentPosition = 0;

        runSequence();

        auto voltage = arpVoltages[currentPosition];
        auto duration = arpDurations[currentPosition];

        playNote(0, voltage, duration);

        currentPosition++;
        if(currentPosition > this->numSteps - 1)
            currentPosition = 0;

    }
}

void ArpeggiatorUI::runSequence()
{
    this->_isPlaying = true;
}

void ArpeggiatorUI::playNote(uint8_t channel, float voltage, int duration_us)
{
    io()->setCVOut(voltage, channel, this->calibrations);
}

void ArpeggiatorUI::handleIO()
{
    handleEncoderLongPressToGoBack();

    auto voltage = arpVoltages[currentPosition];
    auto duration = arpDurations[currentPosition];

    playNote(0, voltage, duration);

    currentPosition++;
}

void ArpeggiatorUI::onExit()
{
}

void ArpeggiatorUI::onEnter()
{
}
