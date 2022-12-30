#ifndef ABSTRACT_UI_H
#define ABSTRACT_UI_H

#include <Adafruit_SSD1306.h>
#include "peacock/peacock_state.h"

class AbstractUI
{
protected:
    Adafruit_SSD1306 *disp = NULL;
    PeacockState_t *state = NULL;

public:
    AbstractUI(Adafruit_SSD1306 *disp, PeacockState_t *state)
    {
        this->disp = disp;
        this->state = state;
    }

    const char *id;

    virtual void draw() {}
    virtual void handleButtons() {}

    virtual void onEnter() {}
    virtual void onExit() {}

    /*
        virtual void onBtnTopClick() {}
        virtual void onBtnBottomClick() {}
        virtual void onEncoderClick() {}
        virtual void onEncoderChange(int delta, int rpm) {}
    */
};

#endif // ABSTRACT_UI_H