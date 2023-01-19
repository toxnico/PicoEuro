#ifndef UIMANAGER_H
#define UIMANAGER_H
#include <Arduino.h>
#include "abstract_ui.h"

/**
 * @brief Singleton class for easy access to all the available UI instances, and activate them,
 * 
 */
class UIManager
{
private:
    static UIManager *_instance;
    UIManager();

public:
    static UIManager *getInstance();

    void next();
    void activateById(const char *id);

    AbstractUI *uis[MAX_UI_COUNT];
    uint8_t uiCount = 0;
    uint8_t currentIndex = 0;

    inline AbstractUI *currentUI() { return uis[currentIndex]; }

    AbstractUI *getUIById(const char *id);
    int getUIindexById(const char *id);
};

#endif // UIMANAGER_H