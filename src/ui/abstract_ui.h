#ifndef ABSTRACT_UI_H
#define ABSTRACT_UI_H

#include "io/iomanager.h"
#include "peacock/peacock_state.h"
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

/**
 * @brief Base class for any UI class (aka screen, view or whatever.)
 *
 */
class AbstractUI
{
protected:
    // the display interface
    Adafruit_SSD1306 *disp = NULL;

    //PeacockState_t *state = NULL;

    // If this UI has a corresponding menu UI, this field
    //  allows direct access to it
    AbstractUI *linkedMenuUI = NULL;

public:
    /**
     * @brief Basic constructor
     *
     * @param disp
     * @param state
     */
    AbstractUI()
    {
    }

    void init(Adafruit_SSD1306 *disp)
    {
        this->disp = disp;
        //this->state = state;
    }

    /**
     * @brief the address in the EEPROM to save the persistent parameters of the app.
     *
     */
    int saveAddress = -1;

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

    virtual void handleGateIRQ(uint8_t channel, bool state) {}

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

    //
    void handleEncoderLongPressToGoBack();

    // save the app state to the EEPROM
    virtual void save() {}
    virtual void load() {}

    // Helper method to quickly get the IOManager singleton
    inline IOManager *io() { return IOManager::getInstance(); }
};

#endif // ABSTRACT_UI_H