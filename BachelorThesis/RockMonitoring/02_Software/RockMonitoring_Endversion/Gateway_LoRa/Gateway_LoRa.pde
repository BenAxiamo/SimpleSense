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
#include <Gateway.h>

//Creat the object
Gateway myGateway= Gateway();
void setup() {
    // put your setup code here, to run once:
    
    myGateway.init();
}


void loop() {
    // put your main code here, to run repeatedly:
  myGateway.run();
}

