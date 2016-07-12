#include "SensorStab.h"


enum State{wakingUP,measure,sendToGateway, sleep, sleepCommand};
     static State runState=measure;//used in run() method

enum communicationState{send,activityDetection,waitForRequest};
    static communicationState sendState=activityDetection;//used in sendDataToGateway

enum sleepState{activity,waitSleepCommand };
    static sleepState deepSleepState=activity;//used in deepSleepCommand

uint64_t numberOfTrys=0;
uint64_t numberOfSuccess=0;
uint64_t numberOfFails=0;

SensorStab::SensorStab():myLora(&mySensorStruct)
{
/// \brief creates an object from Lora, Sensors and Connection3G
///

    Sensors mySensors;
    Connection3G my3G;
}
int8_t SensorStab::init(void){
/// \brief This method initializes all the components of the sensornode
/// \return 0 all settings are done correctly
/// \return 1 error during settings
///
    int8_t error=0;
    myLeds.init();
    pinMode(DIGITAL4 ,INPUT);   //Fault  (LOW-Active)
    pinMode(DIGITAL5 ,INPUT);   //Charge (LOW-Active)
    pinMode(DIGITAL6 ,OUTPUT);   //Shutdown
    myLeds.BlinkAkku(measureBattery());
    mySensors.init();
    error = myLora.init();
    return error;
}
uint8_t SensorStab::makeMeasurment(void){
/// \brief Gets the values from all accelerometer and saves them in the struct
/// \return 0 measurement successful
/// \return 1 error with accelerometer
/// \return 2 error with tempSensors
///

        uint8_t state=0;
        state = mySensors.getSensorValues(&mySensorStruct);
        mySensorStruct.sensorNodeId=myLora.mySettings.nodeId;
        mySensors.printAll();
        myLora.convertStruct();
        return state;
}
uint8_t SensorStab::sendDataToGateway(void){
/// \brief This method waits for the DataRequest of the gateway and sends the requested data via LoRa back to the gateway
/// \return 0 Data has been sent and gateway has acknowledged
/// \return 1 Data has not been sent
///

    switch(sendState){
    case activityDetection:{
            delay(300);
            if(myLora.activityDetection()==RETURN_OK){
                sendState=waitForRequest;
                USB.println("\t\ activity detected");
            }
        }
        break;
        case waitForRequest:{
            if(myLora.waitForDataRequest()==RETURN_OK){
                sendState=send;
                USB.println("\t\ received dataRequest");
                myLeds.LedGreenOn();
            }
        }
        break;
        case send:{
            if(myLora.sendData()==RETURN_OK){
                USB.println("\t\ send Packet succesful");
                sendState=activityDetection;
                numberOfSuccess++;
                USB.println("*********************************************************");
                USB.println("Statists:");
                USB.print("Number of Succesful transmission");
                USB.println(numberOfSuccess,DEC);
                USB.print("Number of Fails transmission");
                USB.println(numberOfFails,DEC);
                USB.println("*********************************************************");
                myLeds.LedGreenBlink4seconds();
                return RETURN_OK;
            }
            else{
                numberOfTrys++;
                USB.print("\t\ Number of Fails transmission");
                USB.println(numberOfFails,DEC);
                if(numberOfTrys>=4){
                    numberOfTrys=0;
                    numberOfFails++;
                    sendState=waitForRequest;
                }
                return RETURN_ERROR;
            }
        }
        break;
    }
}
void SensorStab::deepSleep(void){
/// \brief Turns off all the sensors and modules used in order to safe energy
/// \param time is the number of seconds the sensornode should rest in deepSleep
///
    //myLora.deepSleep();
    mySensors.goToSleep();
    Utils.setLED(LED0, LED_OFF);
    Utils.setLED(LED1, LED_OFF);
    PWR.deepSleep("00:00:15:00",RTC_OFFSET,RTC_ALM1_MODE1,SOCKET0_OFF);
    if( intFlag & RTC_INT ){
      intFlag &= ~(RTC_INT);
    }
}
uint8_t SensorStab::deepSleepCommand(void){
/// \brief Waits for the deepSleepCommand from the gateway
/// \return 0 received a deepSleepCommand
/// \return 1 not received a deepSleepCommand
///
    switch(deepSleepState){
    case activity:{
            if(myLora.activityDetection()==RETURN_OK){
                deepSleepState=waitSleepCommand;
                USB.println("\t\ activity detected");
            }
        }
        break;
    case waitSleepCommand:{
            if(myLora.waitForDeepSleep()==RETURN_OK){
                USB.println("\t\ go to sleep");
                return RETURN_OK;
            }
            else{
                return RETURN_ERROR;
            }
        }
        break;
    }
}
int8_t SensorStab::wakeUp(void){
/// \brief Turns ON all the modules and reconfigures them if necessary
/// \return 0 all modules have woken up successfully
/// \return 1 not all modules have woken up successfully
///
    mySensors.wakeUp();
    return myLora.wakeUp();
}
void SensorStab::convertTime(uint32_t time){
/// \brief This method converts a time into a uint8_t array
/// \param time to be converted
///
    uint8_t days = time / (60 * 60 * 24);
    time -= days * (60 * 60 * 24);
    uint8_t hours = time / (60 * 60);
    time -= hours * (60 * 60);
    uint8_t minutes = time / 60;
    time-=minutes*60;
    uint8_t seconds=time;

    SensorStab::timeString[0]=(48+days/10); //day
    SensorStab::timeString[1]=(48+days%10); //day
    SensorStab::timeString[2]=(':');
    SensorStab::timeString[3]=(48+hours/10); //hour
    SensorStab::timeString[4]=(48+hours%10); //hour
    SensorStab::timeString[5]=(':');
    SensorStab::timeString[6]=(48+minutes/10); //min
    SensorStab::timeString[7]=(48+minutes%10); //min
    SensorStab::timeString[8]=(':');
    SensorStab::timeString[9]=(48+seconds/10); //seconds
    SensorStab::timeString[10]=(48+seconds%10); //seconds
    SensorStab::timeString[11]=('0');
}
uint8_t SensorStab::measureBattery(void){
/// \brief This method shuts down the battery charger and measures the battery level
/// \return returns the battery level
///
    uint8_t level=0;
    USB.OFF();
    digitalWrite(DIGITAL6, HIGH);//Shutdown pin activate
    delay(200);
    level=PWR.getBatteryLevel();
    digitalWrite(DIGITAL6, LOW);//Shutdown pin deactivate
    USB.ON();
    return level;
}
void SensorStab::run(void){
/// \brief structures the procedure of the sensor-node
///
    switch(runState){
    case measure:
        USB.println();
        USB.println("runstate = measure");
        USB.println();
        if(SensorStab::makeMeasurment()==RETURN_OK){
            runState=sendToGateway;
        }
        break;
    case sendToGateway:
        USB.println();
        USB.println("runstate = sendToGateway");
        USB.println();
        if(SensorStab::sendDataToGateway()==RETURN_OK){
            runState=sleepCommand;
        }
        break;
    case sleepCommand:
        if(deepSleepCommand()==RETURN_OK){
            runState=sleep;
        }
        break;
    case sleep:
        USB.println();
        USB.println("runstate = sleep");
        USB.println();
        SensorStab::deepSleep();
        runState=wakingUP;
        break;
    case wakingUP:
        USB.println();
        USB.println("runstate = wakeUP");
        USB.println();
        if(wakeUp() == RETURN_OK){
            runState=measure;
        }
        break;
    }
}

