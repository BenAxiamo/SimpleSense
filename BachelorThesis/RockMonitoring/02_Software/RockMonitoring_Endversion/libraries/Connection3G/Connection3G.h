#ifndef CONNECTION3G_H
#define CONNECTION3G_H
#include "WaspUSB.h"
#include "../_3G/Wasp3G.h"
#include <stdio.h>
#include "WaspPWR.h"
#include "WaspRTC.h"
#include <inttypes.h>
#include "Printable.h"
#include "Waspmote.h"
#include "Sensors.h"
#include "../Constants/Constants.h"
//#include <Wasp3G.h>

class Connection3G
{
public:
//methods
    Connection3G();
    void  init();													//This method initializes the operator parameters that are necessary for 3G connection
    uint8_t sendData(Sensors::structSensors * sensors);				//This method sends all the sensor information from one struct structSensors to the webserver
    uint8_t sendData(Sensors::structSensors * data, uint8_t size);	//This method sends the information of the sensors in the array to the webserver
    uint8_t checkConnection();										//This method checks if a connection to the 3G network could be established

private:
//Attributes
    const char* simPin = "5858";
    const char* httpStreamAddress = "rockmonitoring.netpresence.ch";
};

#endif // CONNECTION3G_H
