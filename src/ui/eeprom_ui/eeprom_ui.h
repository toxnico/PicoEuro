#ifndef EEPROM_UI_H
#define EEPROM_UI_H

#include "ui/abstract_ui.h"

class EepromUI : public AbstractUI
{
private:
    //
public:
    EepromUI(Adafruit_SSD1306 *disp, PeacockCalibrations_t *state);

    void draw();
    void handleIO();

};

#endif //EEPROM_UI_H
