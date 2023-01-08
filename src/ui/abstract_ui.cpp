#include "abstract_ui.h"
#include "ui/uimanager.h"

void AbstractUI::handleEncoderLongPressToGoBack()
{
    auto io = IOManager::getInstance();

    if (io->btnEnc->isPressed() && io->btnEnc->currentDuration() > LONG_PRESS_DELAY)
    {
        UIManager::getInstance()->activateById(UI_MAIN_MENU);
    }
}
