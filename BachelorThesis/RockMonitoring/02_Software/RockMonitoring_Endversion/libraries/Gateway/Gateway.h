#ifndef GATEWAY_H
#define GATEWAY_H
#include "Connection3G.h"
#include "lora_class.h"
#include "Sensors.h"
#include "../Leds/Leds.h"
#include "../Constants/Constants.h"

class Gateway
{
    public:
        Gateway();
        uint8_t init(void);
        void showStructContent(uint8_t node);
        uint8_t sendToServer(Sensors::structSensors *dataToSend);
        uint8_t sendSleep(uint8_t ID);
        uint8_t requestData(uint8_t ID);
        uint8_t receiveNodeData(void);
        void deepSleep(void);
        int8_t wakeUp(void);
        uint8_t measureBattery(void);
        void run(void);
    private:
        //Attributs
        Sensors::structSensors mySensorStruct; //struct used to save the data which was measured
        Sensors::structSensors mySensorStructArray[numberOfNodes]; //struct-array used to save the data which was send
        uint32_t sleepTime=60; //how long the Sensornode will sleep until the  next sending procedure
        char timeString[12];    //Safes the result of the method convertTime
        uint8_t sensorID[numberOfNodes]={3,4,5}; //used ID's for communication

        uint8_t notReceived=0;
        uint8_t notReceivedCnt=0;
        uint8_t notSleepCnt=0;
        uint8_t nothingReceived=0;
        uint8_t currentNode=0; //safes which node data has been requested from

        //objects
        Lora myLora;
        Connection3G my3G;
        Leds myLeds;

};

#endif // GATEWAY_H
