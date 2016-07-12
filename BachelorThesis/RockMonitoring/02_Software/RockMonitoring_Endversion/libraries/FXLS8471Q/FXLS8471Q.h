#ifndef FXLS8471Q_H
#define FXLS8471Q_H

// FXLS8471Q internal register addresses
#define FXLS8471Q_STATUS 0x00
#define FXLS8471Q_WHOAMI 0x0D
#define FXLS8471Q_XYZ_DATA_CFG 0x0E
#define FXLS8471Q_CTRL_REG1 0x2A
#define FXLS8471Q_CTRL_REG2 0x2B
#define FXLS8471Q_CTRL_REG3 0X2C
#define FXLS8471Q_WHOAMI_VAL 0x6A

//constants
#define FXLS8471Q_READ_LEN 7			// number of bytes to be read from FXLS8471Q status plus 3 accelerometer channels
#define FXLS8471Q_Sensitivity 0.000244

//includes
#include <util/twi.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "Wire.h"
#include "WaspClasses.h"
#include "../Constants/Constants.h"

class FXLS8471Q
{
public:
//typedefs
	typedef struct  //Struct containing the X, Y and Z values
	{
		int16_t x;
		int16_t y;
		int16_t z;
	} structXYZ;

	typedef struct  //Struct containing the pitch and roll values
	{
		int16_t pitch;
		int16_t roll;
	} structPitchRoll;

//methods
    FXLS8471Q();	            	//Constructor

	void init(uint8_t slaveAddr);  	// Method that confiugres FXLS8471Q accelerometer sensor

    uint8_t getAccXYZ(FXLS8471Q::structXYZ * xyz);                      //Method that reads the accelerometer x, y and z values
    uint8_t getAccPitchRoll(FXLS8471Q::structPitchRoll * PitchRoll);    //Method that reads the accelerometer pitch and roll angle

    void goToSleep();           	//sets accelerometer to sleep mode

private:
//attributes
    uint8_t i2cSlaveAddr;
};

#endif /* FXLS8471Q_H */
