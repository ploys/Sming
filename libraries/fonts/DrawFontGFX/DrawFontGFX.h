/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/

#ifndef _DrawFontGFX_H_
#define _DrawFontGFX_H_

// Please use The Dot Factory to generate fonts: http://www.eran.io/the-dot-factory-an-lcd-font-and-image-generator/
// Big thanks to http://electro.olganet.com/2014/03/29/tiny-altimeter-oled/

#if ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "bitmap_db.h"
#include <stdlib.h>


// Color definitions
/*#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF*/
#define TRANSPARENT     -1


class Adafruit_GFX;

class DrawFontGFX
{
public:
	DrawFontGFX(Adafruit_GFX& disp);
	
	void setFont(const FONT_INFO& fontInfo, int textColor, int backColor);
	
	int drawChar(char charCode, int x, int y);
	int drawString(String text, int x, int y);
	int drawFloatValue(float value, int x, int y, int digAfterDecimal = 1);
	int drawIntValue(float value, int x, int y);
	
protected:
	// Thanx to skumlerud: http://forum.arduino.cc/index.php?topic=44262.msg320447#msg320447
	char *ftoa(char *a, double f, int precision)
	{
		long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};

		char *ret = a;
		long heiltal = (long)f;
		itoa(heiltal, a, 10);
		while (*a != '\0') a++;
		*a++ = '.';
		long desimal = abs((long)((f - heiltal) * p[precision]));
		itoa(desimal, a, 10);
		return ret;
	}

private:
	Adafruit_GFX& display;
	const FONT_INFO *font;
	int textColor;
	int backColor;
	
	// Font info
	uint8_t heightPixels;
	uint8_t startChar;
	FONT_CHAR_INFO* chars;
	uint8_t* pixels;
};

#endif