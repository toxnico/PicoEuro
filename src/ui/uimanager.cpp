#include "uimanager.h"

UIManager *UIManager::_instance = NULL;

UIManager::UIManager()
{
    for (uint8_t i = 0; i < MAX_UI_COUNT; i++)
    {
        this->uis[i] = NULL;
    }
}

UIManager *UIManager::getInstance()
{
    if (!_instance)
        _instance = new UIManager();

    return _instance;
}

void UIManager::next()
{
    uint8_t nextIdx = this->currentIndex + 1;
    if (nextIdx > this->uiCount - 1)
        nextIdx = 0;

    this->currentUI()->onExit();

    this->currentIndex = nextIdx;

    this->currentUI()->onEnter();
}