#ifndef ARPEGGIATOR_UI_H
#define ARPEGGIATOR_UI_H

#include "../abstract_ui.h"
#include <dmtimer.h>
#include "tools/delayed_executor.h"
#include "ui/quantizer_ui/quantizer_ui.h"



/**
 * @brief Class for the Arpeggiator's main interface.
 * 
 */
class ArpeggiatorUI : public AbstractUI
{
private:
    bool _isPlaying = false;
    DelayedExecutor delayedExecGate;
    QuantizerUI *quantizer = NULL;

    DMTimer _tmrClock;

public:
    //Properties:
    int numSteps = 8;
    float arpPitchOffsets[MAX_ARPEGGIATOR_STEPS];
    float arpDurations[MAX_ARPEGGIATOR_STEPS];
    float bpm = 120;
    uint8_t outputGateIndex = 0;

    int currentPosition = 0;
    int currentEditPosition = 0;


    //Methods:
    ArpeggiatorUI();
    void draw();
    void drawBar(int stepIndex, int left, int topY, int barWidth, int barHeight);
    void handleIO();
    
    bool isAtEnd() { return currentPosition == numSteps - 1;}

    void onExit();
    void onEnter();

    void handleGateIRQ(uint8_t channel, bool state);
    void playNote();
    void playNote(uint8_t channel, float voltage, int duration_us);
    void runSequence();
    

    void changeStepVoltage(int position, int direction, int rpm);
    void changeStepDuration(int position, int direction);
};

static ArpeggiatorUI arpeggiatorUI;

#endif // ARPEGGIATOR_UI_H
