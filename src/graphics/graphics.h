#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Adafruit_SSD1306.h>

class Graphics{
public:

	static uint16_t getTextWidth(const char* str, Adafruit_GFX* display);
    static uint16_t getTextWidth(char* str, Adafruit_GFX* display);

	static void printRightAligned(Adafruit_SSD1306 *disp, char* str, uint16_t x, uint16_t y);
    
	static void dumpScreenToSerial(Adafruit_GFX* display);
    
};
#endif //GRAPHICS_H