#include "Sensors.h"

/// \class Sensors
/// \brief Class that handles sensors
///
Sensors::Sensors()
{
    FXLS8471Q acc1;
    FXLS8471Q acc2;
    FXLS8471Q acc3;
}

/// \brief This method initializes the sensors
///
void Sensors::init()
{
    pinMode(ANALOG7, INPUT);    //Init ADC for potentiometer
    PWR.setSensorPower(SENS_3V3,SENS_ON); // Sets the 3,3V switch ON
	delay(2000);    //delay to allow the sensors to configure
    acc1.init(0);
    acc2.init(1);
    acc3.init(2);

    temp1.init(0);
    temp2.init(1);
    temp3.init(2);
    delay(1000);    //delay to allow the sensors to configure
}

/// \brief This method gives back the X, Y and Z - Values of all 3 Accelerometers
/// \param Sensors::structSensorsXYZ * sensors123, Pointer to struct which contains data of all 3 acc. sensors
///
/// \return uint8_t state, gives back 1 for Error and 0 for ok
///
uint8_t Sensors::getSensorAcc123(Sensors::structSensorsXYZ * sensors123)
{
    FXLS8471Q::structXYZ xyz;

    if(acc1.getAccXYZ(&xyz) == 0)
    {
       sensors123->sensor1 = xyz;
    }
    else
    {
        return RETURN_ERROR;   //Error
    }

    if(acc2.getAccXYZ(&xyz) == 0)
    {
        sensors123->sensor2 = xyz;
    }
    else
    {
        return RETURN_ERROR;   //Error
    }


    if(acc3.getAccXYZ(&xyz) == 0)
    {
        sensors123->sensor3 = xyz;
    }
    else
    {
        return RETURN_ERROR;   //Error
    }

    return RETURN_OK;   //ok
}

/// \brief This method reads the pitch and Roll Angle- Values of all 3 Accelerometers, all 3 temperatures and the extension \n
///  It reads the temperatures and accelerations as 16bit int but concluding 2 places after comma \n
///				the extension is given in 8 bit concluding the data in 1/10 mm
/// \param Sensors::structSensorsPitchRoll * sensors123, Pointer to struct which contains angle data of all 3 acc. sensors
///
/// \return uint8_t state, gives back 1 for Error with Accelerometer, 2 for Error with TempSensors and 0 for ok
///
uint8_t Sensors::getSensorValues(Sensors::structSensors * allsensors)
{
    uint8_t state = 0;

    FXLS8471Q::structPitchRoll pitchRoll;
    if(acc1.getAccPitchRoll(&pitchRoll) == 0)
    {
       allsensors->sensor1 = pitchRoll;
    }
    else
    {
        state = 1;   //Error reading Acceleration
    }
    if(acc2.getAccPitchRoll(&pitchRoll) == 0)
    {
        allsensors->sensor2 = pitchRoll;
    }
    else
    {
        state = 1;   //Error reading Acceleration
    }
    if(acc3.getAccPitchRoll(&pitchRoll) == 0)
    {
        allsensors->sensor3 = pitchRoll;
    }
    else
    {
        state = 1;   //Error reading Acceleration
    }

    int extension_;
    getExtension(extension_);
    allsensors->extension = extension_;
    allsensors->batteryLevel=PWR.getBatteryLevel();//Set batterylevel

    int8_t temp;
    if(temp1.getTemp(temp) == 0)
    {
       allsensors->sensTemp1 = temp;
    }
    else
    {
        state = 2;   //Error reading temperature
    }
    if(temp2.getTemp(temp) == 0)
    {
       allsensors->sensTemp2 = temp;
    }
    else
    {
        state = 2;   //Error reading temperature
    }
    if(temp3.getTemp(temp) == 0)
    {
       allsensors->sensTemp3 = temp;
    }
    else
    {
        state = 2;  //Error reading temperature
    }

    switch(state)
    {
        case 1:
            {
                return RETURN_ERROR_Acc;
            }
        break;
        case 2:
            {
                return RETURN_ERROR_Temp;
            }
        break;
        default:
            {
                return RETURN_OK;
            }
        break;
    }
}


/// \brief This method reads the X, Y and Z - Values of all 3 Accelerometers and the extensiomter and prints them to the serial Port
///
void Sensors::print()
{
    Sensors::structSensorsXYZ accsensors;
    int extension_;
    getExtension(extension_);
    if(getSensorAcc123(&accsensors) == 0)
    {
        USB.println(F(";"));
        USB.print(F("ACC1X_"));
        USB.print(accsensors.sensor1.x);
        USB.println(";");
        USB.print(F("ACC1Y_"));
        USB.print(accsensors.sensor1.y);
        USB.println(";");
        USB.print(F("ACC1Z_"));
        USB.print(accsensors.sensor1.z);
        USB.println(";");

        USB.print(F("ACC2X_"));
        USB.print(accsensors.sensor2.x);
        USB.println(";");
        USB.print(F("ACC2Y_"));
        USB.print(accsensors.sensor2.y);
        USB.println(";");
        USB.print(F("ACC2Z_"));
        USB.print(accsensors.sensor2.z);
        USB.println(";");

        USB.print(F("ACC3X_"));
        USB.print(accsensors.sensor3.x);
        USB.println(";");
        USB.print(F("ACC3Y_"));
        USB.print(accsensors.sensor3.y);
        USB.println(";");
        USB.print(F("ACC3Z_"));
        USB.print(accsensors.sensor3.z);
        USB.println(";");

        USB.print(F("POT_"));
        USB.print(extension_);
        USB.println(";");
    }
    else
    {
        USB.println(F("Error read sensors acc 1,2 and 3"));
    }
}


/// \brief This method reads all the sensors and prints them to the serial Port
///
void Sensors::printAll()
{
    Sensors::structSensors sensorsAll;
    if(getSensorValues(&sensorsAll) == RETURN_OK)
    {
        USB.print(F("ACC1pitch"));
        USB.print(sensorsAll.sensor1.pitch);
        USB.println(";");
        USB.print(F("ACC1roll"));
        USB.print(sensorsAll.sensor1.roll);
        USB.println(";");

        USB.print(F("ACC2pitch_"));
        USB.print(sensorsAll.sensor2.pitch);
        USB.println(";");
        USB.print(F("ACC2roll_"));
        USB.print(sensorsAll.sensor2.roll);
        USB.println(";");

        USB.print(F("ACC3pitch_"));
        USB.print(sensorsAll.sensor3.pitch);
        USB.println(";");
        USB.print(F("ACC3roll_"));
        USB.print(sensorsAll.sensor3.roll);
        USB.println(";");


        USB.print(F("POT_ "));
        USB.print(sensorsAll.extension);
        USB.println(";");

        USB.print(F("Temp1_ "));
        USB.print(sensorsAll.sensTemp1);
        USB.println(";");

        USB.print(F("Temp2_"));
        USB.print(sensorsAll.sensTemp2);
        USB.println(";");

        USB.print(F("Temp3_"));
        USB.print(sensorsAll.sensTemp3);
        USB.println(";");
    }
    else
    {
        USB.println(F("Error read sensors"));
    }
}

/// \brief This method reads the Extension of the potentiometer, it uses the 10Bit- ADC Converter \n
///	it calculates the extension in 1/10 mm with the median value of 30 measurements
/// \param int & extension, Reference to int which contains the extension value (from 0 - 1023)
///
void Sensors::getExtension(int & extension)
{
    int buffer [30];

    for(int i = 0;i<30;i++)
    {
        buffer[i] = analogRead(ANALOG7);
    }

    extension = (int)(0.02981 *100* (1023-median(30,buffer)));	//calculates the extension in 1/10 mm with median value of 30 measurements
}
/// \brief This method sets all sensors in Standby
///
void Sensors::goToSleep()
{
    acc1.goToSleep();
    acc2.goToSleep();
    acc3.goToSleep();
    temp1.goToSleep();
    temp2.goToSleep();
    temp3.goToSleep();
}
/// \brief This method is used to wake up the Sensors, it initializes them
///
void Sensors::wakeUp()
{
    init();
}

/// \brief This method calculates the median value of the elements in the array
/// \param int n, int x[], n = number of values in the array, x = array with values
///
/// \return int median, median value of the values in the array
///
int Sensors::median(int n, int x[]) {
    int temp;
    int i, j;
    // the following two loops sort the array x in ascending order
    for(i=0; i<n-1; i++)
    {
        for(j=i+1; j<n; j++)
        {
            if(x[j] < x[i])
            {
                // swap elements
                temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }
    if(n%2==0)
    {
        // if there is an even number of elements, return mean of the two elements in the middle
        return((x[n/2] + x[n/2 - 1]) / 2.0);
    } else
    {
        // else return the element in the middle
        return x[n/2];
    }
}
