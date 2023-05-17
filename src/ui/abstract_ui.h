#ifndef ABSTRACT_UI_H
#define ABSTRACT_UI_H

#include <Adafruit_SSD1306.h>
#include "peacock/peacock_calibrations.h"
#include "io/iomanager.h"
#include <EEPROM.h>

/**
 * @brief Base class for any UI class (aka screen, view or whatever.)
 * 
 */
class AbstractUI
{
protected:
    //the display interface
    Adafruit_SSD1306 *disp = NULL;
    

    // If this UI has a corresponding menu UI, this field
    //  allows direct access to it
    AbstractUI *linkedMenuUI = NULL;

public:
    PeacockCalibrations_t *calibrations = NULL;
    /**
     * @brief Basic constructor
     *
     * @param disp
     * @param state
     */
    AbstractUI()
    {
    }

    virtual void init(Adafruit_SSD1306 *disp, PeacockCalibrations_t *calibrations)
    {
        this->disp = disp;
        this->calibrations = calibrations;
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

    /**
     * @brief If the user presses the encoder for one second or so, this is called.
     * 
     */
    void handleEncoderLongPressToGoBack();

    /**
     * @brief Handler for interrupt requests on gates.
     * 
     * @param channel 
     * @param state 
     */
    virtual void handleGateIRQ(uint8_t channel, bool state) { }

    // save the app state to the EEPROM
    virtual void save() {}
    virtual void load() {}

    // Helper method to quickly get the IOManager singleton
    inline IOManager *io() { return IOManager::getInstance(); }
};

#endif // ABSTRACT_UI_H