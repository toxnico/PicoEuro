#include "arpeggiator_ui.h"
#include "ui/quantizer_ui/quantizer_ui.h"
#include <Fonts/Org_01.h>

#include "graphics/graphics.h"
#include "io/iomanager.h"
#include "ui/uimanager.h"
#include "ui/quantizer_ui/note.h"
#include "ui/quantizer_menu_ui/quantizer_menu_ui.h"

ArpeggiatorUI::ArpeggiatorUI()
{
    this->id = UI_ARPEGGIATOR;

    // this->linkedMenuUI = UIManager::getInstance()->getUIById(UI_QUANTIZER_MENU);
}

void ArpeggiatorUI::drawBar(int stepIndex, int left, int topY, int barWidth, int barHeight)
{
    // draw the empty bar:
    //disp->drawRect(left, 63 - barHeight, barWidth, barHeight, WHITE);

    // map the voltage to the bar height:
    auto voltageHeight = map((int)(abs(arpPitchOffsets[stepIndex] * 100)), 0, 500, 0, barHeight - 1);
    // must always be visible I think:
    // if (voltageHeight == 0)
    //    voltageHeight = 1;

    if (arpPitchOffsets[stepIndex] >= 0)
    {
        auto top = topY + barHeight - voltageHeight;
        // Serial.printf("x: %d, y: %d, w: %d, h: %d\n",left, top, barWidth, voltageHeight);
        disp->fillRect(left, top, barWidth, voltageHeight, WHITE);
    }

    if (arpPitchOffsets[stepIndex] < 0)
    {
        auto top = topY;
        // Serial.printf("x: %d, y: %d, w: %d, h: %d\n",left, top, barWidth, voltageHeight);
        disp->fillRect(left, top, barWidth, voltageHeight, WHITE);
    }
    // disp->fillRect(left, 63 - barHeight + (barHeight/2) - voltageHeight, barWidth, voltageHeight, WHITE);

    // draw triangle over the selected bar:
    if (stepIndex == currentPosition)
    {
        auto barOffset = left + (barWidth / 2);
        disp->fillTriangle(left + (barWidth / 2), 63 - barHeight - 4, barOffset - 2, 63 - barHeight - 6, barOffset + 2, 63 - barHeight - 6, WHITE);
    }

    // draw a line over the edided bar:
    if (stepIndex == currentEditPosition)
    {
        disp->drawRect(left, 63 - barHeight - 2, barWidth, 2, WHITE);
    }
}

void ArpeggiatorUI::draw()
{
    disp->setFont(&Org_01);
    disp->setTextSize(1);
    disp->setCursor(0, 10);

    // disp->print("ARPEGGIATOR");

    // disp->setCursor(0, 17);
    disp->printf("VOLTAGE: %.3f\nDURATION: %.3f\n", arpPitchOffsets[currentEditPosition], (float)arpDurations[currentEditPosition]);
    disp->printf("FREQ : %.2f / %.2f", this->getFrequencyOnGate(0), this->getFrequencyOnGate(1));
    
    ////////////////////
    // draw the steps //
    ////////////////////
    int standardWidth = 8;
    int barMargin = 1;
    int barHeight = 30;
    int left = 0;
    for (uint8_t i = 0; i < stepCount; i++)
    {
        auto barWidth = standardWidth * arpDurations[i];
        drawBar(i, left, 32, barWidth, barHeight);
        left += barWidth + barMargin;
    }



    // draw the quantizer scale on the right
    auto inputVoltage = this->arpPitchOffsets[currentEditPosition];
    auto quantized = quantizer->rawVoltageToQuantizedVoltage(inputVoltage);
    quantizer->drawGauge(115, inputVoltage, quantized, -1);

    // draw the octave number:
    int octave = floor(abs(inputVoltage));
    if (octave != 0)
    {
        disp->setCursor(90, 60);
        disp->setTextSize(2);
        char sign = inputVoltage < 0 ? '-' : '+';
        disp->printf("%c%d", sign, octave);
        disp->setTextSize(1);
    }

    // print the note name:
    auto noteName = getNoteName(quantized);
    disp->setCursor(90, 48);
    disp->setTextSize(2);
    disp->printf("%s", noteName);
    disp->setTextSize(1);

    io()->setLedTopButton(io()->btnTop->isPressed());
    io()->setLedBottomButton(io()->btnBottom->isPressed());
}

/**
 * @brief Reset the playhead
 *
 */
void ArpeggiatorUI::reset()
{

    // TODO: handle reverse and random order
    auto options = (QuantizerMenuUI *)UIManager::getInstance()->getUIById(UI_QUANTIZER_MENU);
    int direction = options->menu->root->findByName("DIRECTION")->getValueInt();
    switch (direction)
    {

    case 0: // forward
        this->currentPosition = 0;
        break;
    case 1: // reverse
        this->currentPosition = this->stepCount - 1;
        break;
    case 2: // random
        this->currentPosition = random(this->stepCount - 1);
        break;

    default:
        this->currentPosition = 0;
    }

    this->playedStepsCount = 0;
    //Serial.println("Reset");
}

/**
 * @brief Go to the next position. Can be currentPosition +1, -1 or random
 * 0
 */
void ArpeggiatorUI::moveNext()
{
    
    auto previousPos = this->currentPosition;
    auto options = (QuantizerMenuUI *)UIManager::getInstance()->getUIById(UI_QUANTIZER_MENU);
    int direction = options->menu->root->findByName("DIRECTION")->getValueInt();
    switch (direction)
    {

    case 0: // forward
        this->currentPosition++;
        break;
    case 1: // reverse
        this->currentPosition--;
        break;
    case 2: // random
        this->currentPosition = random(this->stepCount - 1);
        break;

    default:
        this->currentPosition++;
    }

    this->playedStepsCount++;

    Serial.printf("Moving from %d to %d\n", previousPos, this->currentPosition);

    //Serial.println("Moved next");
}

/**
 * @brief Interrupt handler on the two input gates
 *
 * @param channel
 * @param state
 */
void ArpeggiatorUI::handleGateIRQ(uint8_t channel, bool state)
{
    // update stuff for the frequency calculation:
    if (state)
    {
        this->_lastIntervalOnGate[channel] = micros() - this->_lastTickTime[channel];
        this->_lastTickTime[channel] = micros();
    }

    // if we get a rising edge on gate 0, we reset the current position
    //  and relaunch the playing process:
    if (channel == 0 && state)
    {
        Serial.println("Got reset trigger ! Resetting.");
        reset();
        //finishNote(false);

    }

    // Rising edge on clock input ? Play note
    if (channel == 1 && state)
    {
        // sample the CV inputs

        this->cvIn[0] = io()->cvInVolts[0];
        this->cvIn[1] = io()->cvInVolts[1];

        // is gate already opened ? it means that a note is still playing.
        // let's finish the note and go the the next one
        auto durationUs = this->_lastIntervalOnGate[1] * arpDurations[currentPosition];
        durationUs = 500000;

        if(note.isPlaying())
        {
            Serial.printf("Clock, note is plaiyng -> move next");
            moveNext();
        }
        note.play(0, durationUs);

    }
}
/*
void ArpeggiatorUI::openGateFor(int duration_us)
{
    // set output gate low then high, so external envelopes can be retriggered :
    io()->setGateOut(outputGateIndex, 1);
    io()->setLedLeft(1);
    this->isGateOpen = true;

    Serial.printf("Gate opened for %d us. step %d / %d\n", duration_us, playedStepsCount, stepCount);

    //delayedExecGate.executeAfter(duration_us);
}
*/

void ArpeggiatorUI::changeStepVoltage(int position, int direction, int rpm)
{
    float increment = 0.025;

    if (rpm > 300)
        increment = 0.1;

    auto newVoltage = this->arpPitchOffsets[position] + (float)direction * increment;
    if (newVoltage < -5)
        newVoltage = -5;
    if (newVoltage > 5)
        newVoltage = 5;

    this->arpPitchOffsets[position] = newVoltage;
}

void ArpeggiatorUI::changeStepDuration(int position, int direction, int rpm)
{
    float newDuration = arpDurations[position] * (direction > 0 ? 2.0 : 0.5);
    newDuration = constrain(newDuration, 0.125, 4);

    arpDurations[position] = newDuration;

    /*
    auto increment = direction * 1000;
    if (rpm > 300)
        increment *= 50;
    this->arpDurations[position] += increment;
    */
}


void ArpeggiatorUI::handleIO()
{
    handleEncoderLongPressToGoBack();
/*
    // check if the output gate must be set to zero:
    delayedExecGate.update();
    if (delayedExecGate.isTimeElapsed())
    {
        finishNote(true);
    }
*/

    // encoder turning:
    auto dir = (int)io()->enc->getDirection() * ENCODER_DIRECTION;
    auto rpm = io()->enc->getRPM();

    // adjust the output pitch for all channels
    if (currentPosition < this->stepCount)
    {
        for (uint8_t channel = 0; channel < 2; channel++)
        {
            // if needed, we resample the CV inputs
            if (quantizer->quantificationMode == QuantificationMode_t::Continuous)
            {
                this->cvIn[0] = io()->cvInVolts[0];
                this->cvIn[1] = io()->cvInVolts[1];
            }
            auto rawVoltage = this->arpPitchOffsets[currentPosition] + cvIn[channel]; // io()->cvInVolts[0];
            auto quantizedVoltage = quantizer->rawVoltageToQuantizedVoltage(rawVoltage);
            io()->setCVOut(quantizedVoltage, channel, this->calibrations);
        }
    }

    while (true)
    {
        // turn the encoder with a button pressed
        if (io()->btnTop->isPressed() && dir != 0)
        {
            this->changeStepVoltage(this->currentEditPosition, dir, rpm);
            break;
        }

        if (io()->btnBottom->isPressed() && dir != 0)
        {
            this->changeStepDuration(this->currentEditPosition, dir, rpm);
            break;
        }

        /*
                if (io()->btnEnc->isPressed() && dir != 0)
                {
                    if (dir < 0 && this->stepCount > 1)
                        this->stepCount--;

                    if (dir > 0 && this->stepCount < MAX_ARPEGGIATOR_STEPS)
                        this->stepCount++;
                    // this->changeStepDuration(this->currentEditPosition, dir, rpm);
                    break;
                }
        */

        break;
    }

    bool noteEnded = note.update();
    if(noteEnded)
    {
        Serial.println("Note ended -> move next");
        this->moveNext();
    }
/*
    // TESTING:
    if (io()->btnTop->pressed())
    {
        handleGateIRQ(0, true);
        handleGateIRQ(0, false);
    }

    if (io()->btnBottom->pressed())
    {
         handleGateIRQ(1, true);
         handleGateIRQ(1, false);
         // reset position:
         // this->currentPosition = 0;
    }

    if(_tmrClock.isTimeReached())
    {
        //simulate a clock on gate 1 (clock input)
        handleGateIRQ(1, true);
        handleGateIRQ(1, false);
    }    
*/

    // read the current edited position:
    this->currentEditPosition = map(io()->potValue, 0, 4095, 0, this->stepCount - 1);
}

void ArpeggiatorUI::onExit()
{
}

void ArpeggiatorUI::onEnter()
{
    _tmrClock.setInterval(500000);
    // initialize the quantizer:
    this->quantizer = (QuantizerUI *)UIManager::getInstance()->getUIById(UI_QUANTIZER);
    this->quantizer->initVoltages(this->quantizer->currentScale());
    randomSeed(6000);
    this->stepCount = 8;
    // for testing : default arp notes & durations
    for (int i = 0; i < MAX_ARPEGGIATOR_STEPS; i++)
    {
        this->arpDurations[i] = 1.0;
        // this->arpPitchOffsets[i] = i * 0.3;
        this->arpPitchOffsets[i] = (float)random(0, 1500) / 1000.0;
    }

    reset();
}
