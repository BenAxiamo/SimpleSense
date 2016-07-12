#ifndef LORA_CLASS_H
#define LORA_CLASS_H
//includes
#include "WaspSX1272.h"
#include "Sensors.h"
#include "../Constants/Constants.h"


/// \class Lora
/// \brief Class that is used to initialize and communicate between Gateway and Sensornode via LoRa

class Lora
{
private:
    uint8_t stringData[19];//has the size of the struct --> used to convert struct into a string
    Sensors::structSensors * localSensorStruct; //This Pointer is used to to access the struct in SensorStab and SensorStab_GSM
    uint8_t sendDataCommand=1;
    uint8_t sendCommandToSleep=2;
    uint8_t gatewayID=2;
public:
    struct loraSettings{    //LoraSettings is used to safe the settings of the Lora-module in order to restore them after deepSleep
        uint8_t channel=0;
        uint8_t mode=0;
        uint8_t nodeId=0;
        uint8_t packetLength=100;
        uint8_t header=0;
        char power='L';
        uint8_t crc=0;
    }mySettings;
    //Mehtod prototypes
    Lora(Sensors::structSensors * sensorstruct);
    int8_t init(void);
    uint8_t getID(void);
    uint8_t activityDetection(void);
    uint8_t dataRequest(uint8_t ID);
    uint8_t waitForDataRequest(void);
    void collectData(void);
    uint8_t sendData(void);
    uint8_t receiveData(void);
    uint8_t sendDeepSleep(uint8_t ID);
    uint8_t waitForDeepSleep(void);
    int8_t wakeUp(void);
    void convertStruct(void);
    void convertStructBack(Sensors::structSensors *dest);
};

#endif // LORA_CLASS_H
