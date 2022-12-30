#ifndef MENU_BUILDER_H
#define MENU_BUILDER_H

#include "menu/menu_display.h"



//#define MENU_NOISE_AMPLITUDE "NOISE AMP."
//#define MENU_NOISE_DENSITY "NOISE DENS."




MenuDisplay* buildMainMenu(Adafruit_GFX* display);
//MenuDisplay* buildSystemParamsMenu(Adafruit_GFX* display, IOManager* iomanager);

#endif