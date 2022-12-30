#ifndef MAIN_MENU_UI_H
#define MAIN_MENU_UI_H
#include "../abstract_ui.h"
#include "menu/menu_display.h"
#include "menu/menu_item.h"

class MainMenuUI : public AbstractUI
{
public:
    MenuDisplay* menu = NULL;
    
    MainMenuUI(Adafruit_SSD1306 *disp, PeacockState_t *state);
    MenuDisplay* buildMenu();
    
    
    void draw();
    void handleButtons();
    void onEnter();
    void onExit();

    
};

#endif // MAIN_MENU_UI_H