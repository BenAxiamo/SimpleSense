#include <SensorStab_GSM.h>

/*  
Developed by:  Daniel Frei & Swen Bischof
Year:          2016
*/
// Put your libraries here (#include ...)
#include <Constants.h>
#include <Leds.h>
#include <PCT2075.h>
#include <Wasp3G.h>
#include <WaspSX1272.h>
#include <FXLS8471Q.h>
#include <Sensors.h>
#include <lora_class.h>
#include <Connection3G.h>


//Creat the object
SensorStab_GSM mySensorStab_GSM = SensorStab_GSM();
void setup() {
    // put your setup code here, to run once:
    
    mySensorStab_GSM.init();
}


void loop() {
    // put your main code here, to run repeatedly:
  mySensorStab_GSM.run();
}

