#ifndef SENSORSTAB_H
#define SENSORSTAB_H
#include "Connection3G.h"
#include "lora_class.h"
#include "Sensors.h"
#include "../Leds/Leds.h"
#include "../Constants/Constants.h"


class SensorStab
{
    public:
        SensorStab();
        int8_t init(void);
        uint8_t sendDataToGateway(void);
        uint8_t makeMeasurment(void);
        void convertTime(uint32_t time);    //safes the converted time in the member timeString
        uint8_t deepSleepCommand(void);
        void deepSleep(void);
        int8_t wakeUp(void);
        uint8_t measureBattery(void);
        void run(void);
    private:
        //attributs
        Sensors::structSensors mySensorStruct; //struct used to save the data which was measured and send
        char timeString[12];    //Safes the result of the method convertTime
        uint8_t notSleepCnt=0;
        //objects
        Lora myLora;
        Connection3G my3G;
        Sensors mySensors;
        Leds myLeds;


};

#endif // SENSORSTAB_H
