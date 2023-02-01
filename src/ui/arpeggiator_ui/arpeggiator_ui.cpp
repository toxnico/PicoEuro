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

    disp->setCursor(0, 25);
    disp->printf("POS: %d\nVOLTAGE: %.3f", currentPosition, arpVoltages[currentPosition]);

    //draw the steps:
    int stepWidth = 6;
    int stepMargin = 3;
    int stepHeight = 20;
    for(uint8_t i = 0;i<numSteps;i++)
    {
        auto left = i*(stepWidth + stepMargin);

        disp->drawRect(left, 63-stepHeight, stepWidth, 20, WHITE);

        //map the voltage to a screen height:
        
        auto voltageHeight = map((int)(arpVoltages[i]*100), 0, 500, 0, stepHeight);
        disp->fillRect(left, 63-voltageHeight, stepWidth, 20, WHITE);


        //selected step:
        if(i == currentPosition)
        {

            auto stepOffset = left + (stepWidth / 2);
            disp->fillTriangle(left + (stepWidth/2), 63-stepHeight - 2, stepOffset-2, 63-stepHeight - 6, stepOffset + 2, 63-stepHeight - 6, WHITE);
        }
    }
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
    }
}

void ArpeggiatorUI::runSequence()
{
    this->_isPlaying = true;
}

void ArpeggiatorUI::playAndNext()
{
    auto voltage = this->arpVoltages[currentPosition];
    playNote(voltage, 0, 500000);
}

void ArpeggiatorUI::playNote(uint8_t channel, float voltage, int duration_us)
{
    // send the voltage to the analog output
    io()->setCVOut(voltage, channel, this->calibrations);

    // set output gate high:
    io()->setGateOut(outputGateIndex, 1);
    io()->setLedLeft(1);
    delayedExecGate.executeAfter(duration_us);
}

void ArpeggiatorUI::handleIO()
{
    handleEncoderLongPressToGoBack();

    // auto voltage = arpVoltages[currentPosition];
    // auto duration = arpDurations[currentPosition];

    // playNote(0, voltage, duration);

    // currentPosition++;

    if (io()->btnTop->pressed())
    {
        Serial.println("P01");
        playNote(0, arpVoltages[currentPosition], arpDurations[currentPosition]);
    }
    if(io()->btnBottom->pressed())
    {
        //reset position:
        this->currentPosition = 0;
    }

    delayedExecGate.update();
    if (delayedExecGate.isTimeElapsed())
    {
        io()->setGateOut(outputGateIndex, false);
        io()->setLedLeft(0);

        // Increment the current position after the note is played.
        // Using a temp variable to avoid bad stuff while drawing
        // (don't forget that the variable is shared between 2 cores)
        auto newPos = currentPosition + 1;
        if (newPos > this->numSteps - 1)
            newPos = 0;
        currentPosition = newPos;
    }
}

void ArpeggiatorUI::onExit()
{
}

void ArpeggiatorUI::onEnter()
{
    for (int i = 0; i < MAX_ARPEGGIATOR_STEPS; i++)
    {
        this->arpDurations[i] = 500000;
        this->arpVoltages[i] = i * 0.5;
    }
}
