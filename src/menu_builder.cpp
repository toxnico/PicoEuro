#include "menu_builder.h"
#include "config.h"
//#include "inc/constants.h"
//#include "ui/preset/settings.h"


MenuDisplay *buildMainMenu(Adafruit_GFX *display)
{
    
    MenuDisplay *menu = new MenuDisplay(display);
/*
    char *buff = (char*)malloc(20);
    sprintf(buff, "PEACOCK %s", PEACOCK_VERSION);

    //MenuItem *root = new MenuItem("MARIE'S LFO");
    MenuItem *root = new MenuItem(buff);
    root->addChild(new MenuItem(MENU_MODE, ValueType::Choice, true));
    root->addChild(new MenuItem(MENU_SYNCHRO, ValueType::Choice, false));
    root->addChild(new MenuItem(MENU_ACCEPT_MIDI, ValueType::OnOff, true));
    root->addChild(new MenuItem(MENU_FREQUENCY_UNIT, ValueType::Choice, true));
    root->addChild(new MenuItem(MENU_FREQUENCY_INCREMENT, ValueType::Float, true));
    root->addChild(new MenuItem(MENU_SEGMENT_COUNT, ValueType::Int, false));
    //root->addChild(new MenuItem(MENU_VOLTAGE_RANGE, ValueType::Int, true));
    root->addChild(new MenuItem(MENU_ENV_CNT_1, ValueType::Int, true));
    root->addChild(new MenuItem(MENU_ENV_CNT_2, ValueType::Int, true));
    root->addChild(new MenuItem(MENU_ENV_CNT_3, ValueType::Int, true));
    root->addChild(new MenuItem(MENU_ENV_CNT_4, ValueType::Int, true));

    MenuItem *syncMenu = root->findByName(MENU_SYNCHRO);
    syncMenu->setValueInt(0);
    syncMenu->choicesCount = 4;
    strcpy(syncMenu->choices[0], "NONE");
    strcpy(syncMenu->choices[1], "MIDI");
    strcpy(syncMenu->choices[2], "TRIG");
    strcpy(syncMenu->choices[3], "TRIG-RESET");

    MenuItem *freqIncMenu = root->findByName(MENU_FREQUENCY_INCREMENT);
    freqIncMenu->setValueFloat(1.0);
    freqIncMenu->incrementCallback = [](void *in, void *out)
    {
        float fVal;
        memcpy(&fVal, in, sizeof(float));
        fVal *= 10;
        if (fVal > 1000)
            fVal = 1000;
        memcpy(out, &fVal, sizeof(float));
    };

    freqIncMenu->decrementCallback = [](void *in, void *out)
    {
        float fVal;
        memcpy(&fVal, in, sizeof(float));
        fVal /= 10;
        if (fVal < 0.001)
            fVal = 0.001;

        memcpy(out, &fVal, sizeof(float));
    };

    // freqIncMenu->setValueFloat(INITIAL_FREQUENCY_INCREMENT);

    MenuItem *modeMenu = root->findByName(MENU_MODE);
    strcpy(modeMenu->choices[0], "LFO");
    strcpy(modeMenu->choices[1], "ENVELOPES");
    strcpy(modeMenu->choices[2], "AUDIO");
    modeMenu->choicesCount = 3;
    modeMenu->setValueInt(INITIAL_MODE);

    MenuItem *freqUnit = root->findByName(MENU_FREQUENCY_UNIT);
    strcpy(freqUnit->choices[0], "Hz");
    strcpy(freqUnit->choices[1], "BPM");
    freqUnit->choicesCount = 2;

    MenuItem *segmentCount = root->findByName(MENU_SEGMENT_COUNT);
    segmentCount->minimum = 1;
    segmentCount->maximum = MAX_NUMBER_OF_BEZIER_SEGMENTS;
    segmentCount->setValueInt(INITIAL_NUMBER_OF_BEZIER_SEGMENTS);

    MenuItem *cnt = root->findByName(MENU_ENV_CNT_1);
    cnt->yOffset = 0;
    cnt->xOffset = 0;
    cnt->minimum = 1;
    cnt->maximum = 1000;
    cnt->setValueInt(1);

    cnt = root->findByName(MENU_ENV_CNT_2);
    cnt->yOffset = -8;
    cnt->xOffset = 64;
    cnt->minimum = 1;
    cnt->maximum = 1000;
    cnt->setValueInt(1);

    cnt = root->findByName(MENU_ENV_CNT_3);
    cnt->yOffset = -8;
    cnt->xOffset = 0;
    cnt->minimum = 1;
    cnt->maximum = 1000;
    cnt->setValueInt(1);

    cnt = root->findByName(MENU_ENV_CNT_4);
    cnt->yOffset = -16;
    cnt->xOffset = 64;
    cnt->minimum = 1;
    cnt->maximum = 1000;
    cnt->setValueInt(1);


    //load saved settings
    menu->setMenuItem(root);
    for (int i = 0; i < root->childrenCount; i++)
    {
        loadMenuSetting(root->children[i]);
    }
*/
    return menu;
}

/*
MenuDisplay *buildSystemParamsMenu(Adafruit_GFX *display, AbstractIOManager* iomanager)
{
    MenuDisplay *menu = new MenuDisplay(display);
    

    MenuItem *root = new MenuItem("SYSTEM");
    
    root->addChild(new MenuItem(MENU_REVERSE_ENCODERS, ValueType::OnOff, true));

    //fuck fuck fuck.
    root->findByName(MENU_REVERSE_ENCODERS)->changeCallback = [] (MenuItem* sender){
        //handleInvertEncoderChange(sender->getValueOnOff());
        //invertEncoders = sender->getValueOnOff();
        //Serial.printf("Changed : %d\n", invertEncoders);
    };


    menu->currentMenuItem = root;
    menu->selectedIndex = 0;


    //load saved settings
    menu->setMenuItem(root);
    for (int i = 0; i < root->childrenCount; i++)
    {
        loadMenuSetting(root->children[i]);
    }

    return menu;
}
*/