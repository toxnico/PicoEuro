#ifndef ARPEGGIATOR_UI_H
#define ARPEGGIATOR_UI_H

#include "../abstract_ui.h"
#include <dmtimer.h>
#include "tools/delayed_executor.h"



/**
 * @brief Class for the quantizer's main interface.
 * 
 */
class ArpeggiatorUI : public AbstractUI
{
private:
    DMTimer _tmrNoteDuration;
    bool _isPlaying = false;

public:
    //Properties:
    int numSteps = 8;
    float arpVoltages[MAX_ARPEGGIATOR_STEPS];
    int arpDurations[MAX_ARPEGGIATOR_STEPS];

    int currentPosition = 0;


    //Methods:
    ArpeggiatorUI();
    void draw();
    
    void handleIO();
    
    void onExit();
    void onEnter();

    void handleGateIRQ(uint8_t channel, bool state);
    
    void playNote(uint8_t channel, float voltage, int duration_us);
    void runSequence();
};

static ArpeggiatorUI arpeggiatorUI;

#endif // ARPEGGIATOR_UI_H
