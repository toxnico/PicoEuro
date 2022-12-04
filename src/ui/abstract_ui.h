#ifndef ABSTRACT_UI_H
#define ABSTRACT_UI_H

#include <Adafruit_SSD1306.h>
#include "picoeuro/picoeuro_state.h"

class AbstractUI
{
protected:
    Adafruit_SSD1306 *disp = NULL;
    PicoEuroState_t *state = NULL;

public:
    AbstractUI(Adafruit_SSD1306 *disp, PicoEuroState_t *state)
    {
        this->disp = disp;
        this->state = state;
    }

    virtual void draw() {}

/*
    virtual void onBtnTopClick() {}
    virtual void onBtnBottomClick() {}
    virtual void onEncoderClick() {}
    virtual void onEncoderChange(int delta, int rpm) {}
*/

};

#endif // ABSTRACT_UI_H