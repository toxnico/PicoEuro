#include "graphics.h"
#include <Fonts/Picopixel.h>

uint16_t Graphics::getTextWidth(const char *str, Adafruit_GFX *display)
{
	int16_t cursorX = 1;
	int16_t cursorY = 10;
	int16_t ulX, ulY;
	uint16_t w, h;

	display->getTextBounds(str, cursorX, cursorY, &ulX, &ulY, &w, &h);
	// Serial.printf("Str : %s, w : %d\n", str, w);
	return w;
}

uint16_t Graphics::getTextWidth(char *str, Adafruit_GFX *display)
{
	int16_t cursorX = 1;
	int16_t cursorY = 10;
	int16_t ulX, ulY;
	uint16_t w, h;

	display->getTextBounds(str, cursorX, cursorY, &ulX, &ulY, &w, &h);
	// Serial.printf("Str : %s, w : %d\n", str, w);
	return w;
}

void Graphics::printRightAligned(Adafruit_SSD1306 *disp, char *str, uint16_t x, uint16_t y)
{
	auto w = getTextWidth(str, disp);
	auto left = x-w;
	disp->setCursor(left, y);
	disp->print(str);
}

void Graphics::dumpScreenToSerial(Adafruit_GFX *display)
{
	Adafruit_SSD1306 *oled = (Adafruit_SSD1306 *)display;
	// uint8_t *buff = oled->getBuffer();
	int16_t w = oled->width();
	int16_t h = oled->height();

	// SerialUSB.print("P1\n%d %d\n", w, h);

	for (int16_t y = 0; y < h; y++)
	{
		for (int16_t x = 0; x < w; x++)
		{
			uint8_t pixel = oled->getPixel(x, y); // buff[r * w + c];
												  // SerialUSB.printf("%d ", pixel ? 0 : 1);
		}
		// Serial.println("");
	}
}
