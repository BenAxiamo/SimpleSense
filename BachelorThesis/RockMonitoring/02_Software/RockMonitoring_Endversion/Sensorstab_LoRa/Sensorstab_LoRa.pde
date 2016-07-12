/*  
Developed by:  Daniel Frei & Swen Bischof
Year:          2016
*/
 
// Put your libraries here (#include ...)
#include <Constants.h>
#include <Leds.h>
#include <SensorStab.h>
#include <lora_class.h>
#include <Wasp3G.h>
#include <FXLS8471Q.h>
#include <Connection3G.h>
#include <Sensors.h>
#include <WaspSX1272.h>
#include <lora_class.h>
#include <PCT2075.h>
#include <sdCard.h>
#include <Sensors.h>
     
//Creat the object
SensorStab mySensorStab = SensorStab();

void setup() {
    USB.ON();
    mySensorStab.init();
}
void loop() {
  mySensorStab.run();

}

