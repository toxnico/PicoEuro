#ifndef ABSTRACT_UI_H
#define ABSTRACT_UI_H

#include <Adafruit_SSD1306.h>
#include "peacock/peacock_state.h"
#include "io/iomanager.h"
// #include "ui/uimanager.h"

class AbstractUI
{
protected:
    Adafruit_SSD1306 *disp = NULL;
    PeacockState_t *state = NULL;

public:
    /**
     * @brief Basic constructor
     *
     * @param disp
     * @param state
     */
    AbstractUI(Adafruit_SSD1306 *disp, PeacockState_t *state)
    {
        this->disp = disp;
        this->state = state;
    }

    /**
     * @brief This is the UI identifier, an arbitrary string.
     * They are defined in config.h
     *
     */
    const char *id;

    /**
     * @brief This is the method that draws the UI. It can be relatively slow.
     * It's called by the second core of the Pico.
     *
     */
    virtual void draw() {}

    /**
     * @brief This is the update method called from the main loop.
     * Fast stuff. Typically for handling changes in the inputs and sending stuff to the outputs.
     * No display here !
     *
     */
    virtual void handleIO() {}

    /**
     * @brief Called when the UI is activated
     *
     */
    virtual void onEnter() {}

    /**
     * @brief Called when the UI is unloaded (or more accurately, when another UI is activated)
     *
     */
    virtual void onExit() {}

    void handleEncoderLongPressToGoBack();
};

#endif // ABSTRACT_UI_H