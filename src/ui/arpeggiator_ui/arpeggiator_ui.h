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
    

public:
    //Properties:

    //Methods:
    //ArpeggiatorUI();
    ArpeggiatorUI();
    void draw();
    
    void handleIO();
    
    void onExit();
    void onEnter();

    void handleGateIRQ(uint8_t channel, bool state);
    
    
};


#endif // ARPEGGIATOR_UI_H
