#ifndef GENERAL_STATE_UI_H
#define GENERAL_STATE_UI_H

#include "../abstract_ui.h"
#include "config.h"
#include "peacock/peacock_calibrations.h"
#include <MCP_DAC.h>

class GeneralStateUI : public AbstractUI
{
private:
    
public:
    GeneralStateUI();
    void draw();    
    void handleIO();
    

    ////////////////////
    // Event handlers //
    ////////////////////
    /*void onEncoderChange(int delta, int rpm);
    void onEncoderClick();
    void onBtnTopClick();
    void onBtnBottomClick();
    void onResetAllClick();
    */
};

#endif // GENERAL_STATE_UI_H