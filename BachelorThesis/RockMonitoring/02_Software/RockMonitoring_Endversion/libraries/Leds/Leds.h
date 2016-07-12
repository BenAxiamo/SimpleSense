#ifndef Leds_H
#define Leds_H
#include "WaspUSB.h"
#include <stdio.h>
#include "WaspPWR.h"
#include "WaspRTC.h"
#include <inttypes.h>
#include "printable.h"
#include "Waspmote.h"
#include "../Constants/Constants.h"

class Leds
{
public:
//methods
    Leds();
    void init();					//initializes Led GPIOs
	
    void LedGreenOn();				//Turns the green led on
    void LedGreenOff();				//Turns the green led off
    void LedButtonOn();				//Turns the led in the button on
    void LedButtonOff();			//Turns the led in the button off

    void LedGreenBlinkOnce();		//lets the green led blink once (blocks program for 300ms)
    void LedButtonBlinkOnce();		//ets the led on the button blink once (blocks program for 300ms)
    void BothBlinkOnce();			//lets both leds blink once  (blocks program for 300ms)
    void LedGreenBlink4seconds();	//the green Led blinks for 4 seconds (blocks program for 4 seconds)
    void BlinkAkku(uint8_t batLevel);//sets the button led on or in blink state for 5 seconds (depending on battery level) (blocks program for 5 seconds)

};

#endif // Leds_H
