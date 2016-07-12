#ifndef Sensors_H
#define Sensors_H

#include "FXLS8471Q.h"
#include "WaspPWR.h"
#include "WaspConstants.h"
#include "WaspUSB.h"
#include "printable.h"
#include "Waspmote.h"
#include "PCT2075.h"
#include "../Constants/Constants.h"
class Sensors
{
public:
//typedefs
        typedef struct      //Struct with all 3 sensors containing the X, Y and Z values
        {
            FXLS8471Q::structXYZ sensor1;
            FXLS8471Q::structXYZ sensor2;
            FXLS8471Q::structXYZ sensor3;
        } structSensorsXYZ;

        typedef struct      //Struct with all 3 sensors containing the pitch and roll values
        {
            FXLS8471Q::structPitchRoll sensor1;
            FXLS8471Q::structPitchRoll sensor2;
            FXLS8471Q::structPitchRoll sensor3;
        } structSensorsPitchRoll;

        typedef struct      //Struct with all sensors values
        {
            FXLS8471Q::structPitchRoll sensor1;
            FXLS8471Q::structPitchRoll sensor2;
            FXLS8471Q::structPitchRoll sensor3;
            int8_t sensTemp1;
            int8_t sensTemp2;
            int8_t sensTemp3;
            int16_t extension;
            uint8_t sensorNodeId;
            uint8_t batteryLevel;
        } structSensors;

//methods
        Sensors();		//constructor
        void init();	//inits all sensors
        uint8_t getSensorAcc123(Sensors::structSensorsXYZ * sensors123);	//Gets the Acceleration values X, Y and Z from the 3 Accelerometers
        uint8_t getSensorValues(Sensors::structSensors * allSensors);		//reads the pitch and Roll Angle- Values of all 3 Accelerometers, all 3 temperatures and the extension
        void print(); 		//reads the X, Y and Z - Values of all 3 Accelerometers, reads the extensiomter and prints all the received data
        void printAll();	//reads all sensor data and prints it

        void getExtension(int & extension);		//reads the extension in 1/10 mm
        void goToSleep();	//sets all sensors in Standby
        void wakeUp();		//wakes up all sensors

private:
//attributes
        FXLS8471Q  acc1;
        FXLS8471Q  acc2;
        FXLS8471Q  acc3;
        PCT2075 temp1;
        PCT2075 temp2;
        PCT2075 temp3;

//methods
        int median(int n, int x[]);	//calculates median value of numbers in array

};
#endif /* Sensors_H */

