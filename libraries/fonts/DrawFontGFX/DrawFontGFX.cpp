/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/
 
#include "DrawFontGFX.h"
#include <../Adafruit_GFX/Adafruit_GFX.h>

DrawFontGFX::DrawFontGFX(Adafruit_GFX& disp)
	: display(disp)
{
}

void DrawFontGFX::setFont(const FONT_INFO& fontInfo, int textColor, int backColor)
{
	this->font = &fontInfo;
	this->textColor = textColor;
	this->backColor = backColor;
	
	heightPixels = pgm_read_byte_near(&font->heightPixels);
	startChar = pgm_read_byte_near(&font->startChar);
	chars = (FONT_CHAR_INFO*)(void*)pgm_read_word_near(&font->charInfo);
	pixels = (uint8_t*)pgm_read_word_near(&font->data);
}

int DrawFontGFX::drawChar(char charCode, int x, int y)
{
	uint8_t pos = ((uint8_t)charCode) - startChar;
	uint8_t charWidthBits = pgm_read_byte(&chars[pos].widthBits);
	uint16_t charOffset = pgm_read_word(&chars[pos].offset);
	
	/*Serial.print("======"); Serial.print(pos);
	Serial.print("======"); Serial.print((int)charCode);
	Serial.print("======"); Serial.println(charCode);
	for (int i = 0; i< 150; i++)
	{
		Serial.print(i);
		Serial.print(" ");
		Serial.print(pgm_read_byte(&chars[i].widthBits));
		Serial.print(" ");
		Serial.println(pgm_read_word(&chars[i].offset));
	}*/
	
	//charWidthBits = 9; charOffset = 1254;
	int wBytes = (charWidthBits+7)/8;
	int pX      = 0;
	int pY      = y;
	int color   = 0;
	byte line = 0;
	int gap = 2;

	for(int i=0; i<heightPixels; i++)
	{
	  if (backColor != TRANSPARENT) display.drawFastHLine(x, pY, charWidthBits+gap, backColor);
	  for (int k = 0;k < wBytes; k++)
	  { 
		line = pgm_read_byte(pixels + charOffset + wBytes*i+k);
		if(line)
		{
		  pX = x + k*8;
		  if(line & 0x80) display.drawPixel(pX, pY, textColor);
		  if(line & 0x40) display.drawPixel(pX+1, pY, textColor);
		  if(line & 0x20) display.drawPixel(pX+2, pY, textColor);
		  if(line & 0x10) display.drawPixel(pX+3, pY, textColor);
		  if(line & 0x08) display.drawPixel(pX+4, pY, textColor);
		  if(line & 0x04) display.drawPixel(pX+5, pY, textColor);
		  if(line & 0x02) display.drawPixel(pX+6, pY, textColor);
		  if(line & 0x01) display.drawPixel(pX+7, pY, textColor);
		}
	  }
	  pY++;
	}
	
	return charWidthBits+gap;
}

int DrawFontGFX::drawString(String text, int x, int y)
{
	int px = x;
	
	for (int i = 0; i < text.length(); i++)
		px += drawChar(text[i], px, y);
	
	//int gap = heightPixels / 4;
	//for(int i=0; i<heightPixels; i++)
	//	if (backColor != TRANSPARENT) display.drawFastHLine(px, y + i, gap, backColor);
	//px += gap;
	
	return px;
}

int DrawFontGFX::drawFloatValue(float value, int x, int y, int digAfterDecimal/* = 1*/)
{
	char buf[10];
	ftoa(buf, value, digAfterDecimal);
	return drawString(buf, x, y);
}

int DrawFontGFX::drawIntValue(float value, int x, int y)
{
	char buf[10];
	itoa(value, buf, 10);
	return drawString(buf, x, y);
}