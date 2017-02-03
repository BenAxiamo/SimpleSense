#ifndef SENSORSTAB_GSM_H
#define SENSORSTAB_GSM_H
#include "Connection3G.h"
#include "lora_class.h"
#include "Sensors.h"
#include "../Leds/Leds.h"
#include "../Constants/Constants.h"


class SensorStab_GSM
{
    public:
        SensorStab_GSM();
        void init(void);
        uint8_t sendToServer(Sensors::structSensors *dataToSend);
        uint8_t makeMeasurment(void);
        void convertTime(uint32_t time);    //safes the converted time in the member timeString
        void deepSleep(void);
        void customDeepSleep2(const char* time2wake,uint8_t offset,uint8_t mode,uint8_t option);
        void wakeUp(void);
        uint8_t measureBattery(void);
        void run(void);
        Sensors::structSensors mySensorStruct; //struct used to save the data which was measured and send
        Sensors::structSensors mySensorStructArray[2]; //struct-array used to save the data which was send

    private:
        //Attributes
        enum State{wakingUP,measure,sendServer, sleep};
        State runState=measure;//used in run() method
        uint8_t gsmNodeId;
        uint8_t measureCnt=0;
        uint32_t sleepTime=60; //how long the Sensornode will sleep until the  next sending procedure
        char timeString[12];    //Safes the result of the method convertTime
        //objects
        Connection3G my3G;
        Sensors mySensors;
        Leds myLeds;


};

#endif // SENSORSTAB_GSM_H
