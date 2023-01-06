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

  // ui = generalStateUI;
  // ui = inputCalibrationUI;
    auto id = this->uis[nextIdx]->id;
    activateById(id);
}

void UIManager::activateById(const char *id)
{
    auto idx = getUIindexById(id);
    if(idx < 0)
        return;
    
    this->currentUI()->onExit();

    this->currentIndex = idx;

    this->currentUI()->onEnter();

}

/**
 * @brief finds a UI by its id.
 * Returns NULL if not found
 *
 * @param id
 * @return AbstractUI*
 */
AbstractUI *UIManager::getUIById(const char *id)
{
    auto idx = getUIindexById(id);
    if(idx < 0)
        return NULL;

    return this->uis[idx];
    
}

int UIManager::getUIindexById(const char *id)
{
    for (uint8_t i = 0; i < this->uiCount; i++)
    {
        if (!strcmp(this->uis[i]->id, id))
            return i;
    }
    return -1;
}