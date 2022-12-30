#include "main_menu_ui.h"

MainMenuUI::MainMenuUI(Adafruit_SSD1306 *disp, PeacockState_t *state)
    :AbstractUI(disp, state)
{
    this->menu = buildMenu();
}

MenuDisplay *MainMenuUI::buildMenu()
{
    return NULL;
}
