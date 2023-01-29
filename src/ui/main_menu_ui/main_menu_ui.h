#ifndef MAIN_MENU_UI_H
#define MAIN_MENU_UI_H
#include "../abstract_ui.h"
#include "menu/menu_display.h"
//#include "menu/menu_item.h"

class MainMenuUI : public AbstractUI
{
private:
    //menu names:
    const char* _quantizer = "QUANTIZER";
    const char* _quantizer_opts = "QUANTIZER OPTIONS";
    const char* _general_state = "GENERAL STATE";
    const char* _input_calibration = "INPUT CALIBRATION";
    const char* _output_calibration = "OUTPUT CALIBRATION";
    const char* _arpeggiator = "ARPEGGIATOR";

public:

    MenuDisplay* menu = NULL;
    
    MainMenuUI();
    MenuDisplay* buildMenu();
    
    void handleEncoderRotation();
    void handleEncoderClick();
    
    void init(Adafruit_SSD1306 *disp, PeacockCalibrations_t *calibrations);

    void draw();
    void handleIO();
    void onEnter();
    void onExit();

    
};

static MainMenuUI mainMenuUI;
#endif // MAIN_MENU_UI_H