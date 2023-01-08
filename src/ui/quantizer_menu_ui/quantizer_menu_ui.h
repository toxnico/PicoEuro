#ifndef QUANTIZER_MENU_UI_H
#define QUANTIZER_MENU_UI_H
#include "../abstract_ui.h"
#include "menu/menu_display.h"
//#include "menu/menu_item.h"

class QuantizerMenuUI : public AbstractUI
{
public:
    MenuDisplay* menu = NULL;
    
    QuantizerMenuUI(Adafruit_SSD1306 *disp, PeacockState_t *state);
    MenuDisplay* buildMenu();
    
    
    void draw();
    void handleIO();
    void onEnter();
    void onExit();

    
};

#endif // QUANTIZER_MENU_UI_H