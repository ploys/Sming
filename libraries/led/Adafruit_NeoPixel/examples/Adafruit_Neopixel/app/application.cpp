/**
 * WARNING !
 * Ultrasonic modules usually work with 5v power and TTL levels,
 * so You need voltage divider or level shifter for ECHO pin.
 *
 * Trigger pin is tolerant for 3.3v and in my case work without problems
 *
 * By nik.sharky http://esp8266.ru/forum/members/sharky.396/
 */

#include <user_config.h>
#include <SmingCore/SmingCore.h>
//#include <Libraries/Ultrasonic/Ultrasonic.h>
#include <Libraries/Adafruit_Neopixel/Adafruit_Neopixel.h>

#define TRIG_PIN 4
#define ECHO_PIN 5

Timer procTimer;

//Ultrasonic* ultrasonic;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 14, NEO_GRB + NEO_KHZ800);
uint8_t r=0,g=50,b=50;

void measure()
{

	r++;
	if (r>255) {
		r=0;
		g++;
	}

	if (g>255) {
		g=0;
		b++;
	}

	if (b>255) {
		b=0;
	}
		pixels.setPixelColor(0, pixels.Color(r,g,b));
		pixels.show();
	//float dist_cm = ultrasonic->Ranging(CM);       // get distance
	Serial.println(r);                      // print the distance
}

void init()
{
	//pixels.setPin(2);
	pixels.begin();
	//pixels.setBrightness(80);
	//ultrasonic = new Ultrasonic(TRIG_PIN, ECHO_PIN);
	procTimer.initializeMs(10, measure).start();
}
