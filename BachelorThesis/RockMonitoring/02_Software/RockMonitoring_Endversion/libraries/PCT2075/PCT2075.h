#ifndef PCT2075_H
#define PCT2075_H

//#include "WaspClasses.h"

// PCT2075 internal register addresses
#define PCT2075_CONF 0x01
#define PCT2075_TEMP 0x00
#define PCT2075_TOS 0x03
#define PCT2075_THYST 0x02
#define PCT2075_TIDLE 0x04

// number of bytes to be read from PCT2075
#define PCT2075_READ_LEN 1

#include "WaspUSB.h"
#include <util/twi.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "Wire.h"
#include "printable.h"
#include "Waspmote.h"
#include "../Constants/Constants.h"

#define RETURN_ERROR 1
#define RETURN_OK 0

class PCT2075
{
public:
//methods
    PCT2075();	            		//Constructor
    void init(uint8_t slaveAddr);	//configures PCT2075 temperature sensor
    uint8_t getTemp(int8_t & Temp);	//gets the temperatur value of the temperature sensor PCT2075
    void goToSleep();				//sets the temperature Sensor in shutdown mode

private:
//attributes
        uint8_t i2cSlaveAddr;
};

#endif /* PCT2075_H */
