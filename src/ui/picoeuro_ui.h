#ifndef ABACUS_UI_H
#define ABACUS_UI_H

#include "abstract_ui.h"
#include "config.h"
#include "picoeuro/picoeuro_state.h"

class PicoEuroUI : public AbstractUI
{
private:
    
public:
    PicoEuroUI(Adafruit_SSD1306 *disp, PicoEuroState_t *state);
    void draw();    
    
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

#endif // ABACUS_UI_H