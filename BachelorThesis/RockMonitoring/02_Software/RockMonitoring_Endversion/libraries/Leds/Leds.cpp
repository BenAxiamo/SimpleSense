#include "Leds.h"
/// \class Leds
/// \brief Class that handles the Leds
///
Leds::Leds()
{
}
/// \brief This method initializes the output pins for the leds and clears both led
void Leds::init()
{
   pinMode(DIGITAL1 ,OUTPUT);   //Led green
   pinMode(DIGITAL2 ,OUTPUT);   //Led green on Button

   //clear both Leds
   LedGreenOff();
   LedButtonOff();
}
/// \brief This method turns the green led on
void Leds::LedGreenOn()
{
    digitalWrite(DIGITAL1, HIGH);
}
/// \brief This method turns the green led off
void Leds::LedGreenOff()
{
    digitalWrite(DIGITAL1, LOW);
}
/// \brief This method turns the led on the button on
void Leds::LedButtonOn()
{
    digitalWrite(DIGITAL2, HIGH);
}
/// \brief This method turns the led on the button off
void Leds::LedButtonOff()
{
    digitalWrite(DIGITAL2, LOW);
}

/// \brief This method lets the green led blink once (blocks program for 300ms)
void Leds::LedGreenBlinkOnce()
{
    LedGreenOn();
    delay(300);
    LedGreenOff();
}
/// \brief This method lets the led on the button blink once (blocks program for 300ms)
void Leds::LedButtonBlinkOnce()
{
    LedButtonOn();
    delay(300);
    LedButtonOff();
}
/// \brief This method lets both leds blink once (blocks program for 300ms)
void Leds::BothBlinkOnce()
{
    LedGreenOn();
    LedButtonOn();
    delay(300);
    LedButtonOff();
    LedGreenOff();
}

/// \brief This method lets the green led blink for 4 seconds (blocks program for 4s)
void Leds::LedGreenBlink4seconds()
{

    for(uint8_t i = 0; i<4; i++)
    {
        LedGreenBlinkOnce();
        delay(700);
    }
}
/// \brief This method sets the button led on or in blink state for 5 seconds (depending on battery level) (blocks program for 5s)
/// \param uint8_t batLevel, battery level
void Leds::BlinkAkku(uint8_t batLevel)
{
  if(batLevel >= 80)
    {
        LedButtonOn();
        delay(5000);
        LedButtonOff();
    }
    else if(batLevel >=40)
    {
        for(uint8_t i = 0; i<5; i++)
        {
            LedButtonOn();
            delay(500);
            LedButtonOff();
            delay(500);
        }
    }
    else
    {
        for(uint8_t i = 0; i<5; i++)
        {
            LedButtonOn();
            delay(100);
            LedButtonOff();
            delay(900);
        }
    }


}

