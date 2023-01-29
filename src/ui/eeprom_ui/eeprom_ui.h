#ifndef EEPROM_UI_H
#define EEPROM_UI_H

#include "ui/abstract_ui.h"

class EepromUI : public AbstractUI
{
private:
    //
public:
    EepromUI();

    void draw();
    void handleIO();

};

static EepromUI eepromUI;
#endif //EEPROM_UI_H
