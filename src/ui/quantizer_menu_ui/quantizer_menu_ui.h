#ifndef QUANTIZER_MENU_UI_H
#define QUANTIZER_MENU_UI_H
#include "../abstract_ui.h"
#include "menu/menu_display.h"
#include "ui/quantizer_ui/quantizer_ui.h"
//#include "menu/menu_item.h"

/**
 * @brief Structure to save the quantizer parameters
 * 
 */
typedef struct QuantizerMenuUISave_t{
    uint8_t mode = 0;
    uint8_t defaultScale = 0;
    uint8_t RESERVED[8];
    //uint32_t delay_us = 0;
} QuantizerMenuUISave_t;


/**
 * @brief UI for the quantizer options menu
 * 
 */
class QuantizerMenuUI : public AbstractUI
{
private:
    const char* MENU_MODE = "MODE";
    const char* MENU_DEFAULT_SCALE = "SCALE";
    const char* MENU_DIRECTION = "DIRECTION";
    //const char* MENU_TRIGGER_DELAY = "TRIG. DELAY US";


public:
    MenuDisplay* menu = NULL;
    
    QuantizerMenuUI();
    void init(Adafruit_SSD1306 *disp, PeacockCalibrations_t *calibrations);
    MenuDisplay* buildMenu();
    
    
    void draw();
    void handleIO();
    void onEnter();
    void onExit();
    void save();
    void load();

    
};
static QuantizerMenuUI quantizerMenuUI;
#endif // QUANTIZER_MENU_UI_H