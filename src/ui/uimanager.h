#ifndef UIMANAGER_H
#define UIMANAGER_H
#include <Arduino.h>
#include "abstract_ui.h"

class UIManager
{
private:
    static UIManager *_instance;
    UIManager();

public:
    static UIManager *getInstance();

    void next();
    AbstractUI *uis[MAX_UI_COUNT];
    uint8_t uiCount = 0;
    uint8_t currentIndex = 0;

    inline AbstractUI *currentUI() { return uis[currentIndex]; }
};

#endif // UIMANAGER_H