#include "SensorStab_GSM.h"


SensorStab_GSM::SensorStab_GSM()
{
/// \brief creates an object from Sensors and Connection3G
///
    Sensors mySensors;
    Connection3G my3G;
}
void SensorStab_GSM::init(void){
/// \brief This method initializes all the components of the sensornode
/// \brief The Sensornode ID is read from the EEPROM
///

    myLeds.init();
    pinMode(DIGITAL4 ,INPUT);   //Fault  (LOW-Active)
    pinMode(DIGITAL5 ,INPUT);   //Charge (LOW-Active)
    pinMode(DIGITAL6 ,OUTPUT);   //Shutdown
    myLeds.BlinkAkku(measureBattery());
    gsmNodeId=Utils.readEEPROM(1024);
    mySensors.init();
    my3G.init();
    gsmNodeId==Utils.readEEPROM(1024);
}
uint8_t SensorStab_GSM::sendToServer(Sensors::structSensors *dataToSend){
/// \brief Uploads all the received data to a web-server
/// \return 0 uploading was successful
/// \return 1 uploading was unsuccessful
///
    my3G.sendData(dataToSend);
}
uint8_t SensorStab_GSM::makeMeasurment(void){
/// \brief Gets the values from all accelerometer and saves them in the struct
/// \return 0 measurement successful
/// \return 1 error with accelerometer
/// \return 2 error with tempSensors
///

        uint8_t state=0;
        state = mySensors.getSensorValues(&mySensorStruct);
        mySensorStruct.sensorNodeId=gsmNodeId;
        mySensors.printAll();
        return state;
}
void SensorStab_GSM::deepSleep(void){
/// \brief Turns off all the sensors and modules used in order to safe energy
/// \param time is the number of seconds the sensornode should rest in deepSleep
///
    //myLora.deepSleep();
    mySensors.goToSleep();
    Utils.setLED(LED0, LED_OFF);
    Utils.setLED(LED1, LED_OFF);
    PWR.deepSleep("00:00:01:00",RTC_OFFSET,RTC_ALM1_MODE1,SOCKET0_OFF);
    if( intFlag & RTC_INT ){
      intFlag &= ~(RTC_INT);
    }
}
void SensorStab_GSM::wakeUp(void){
/// \brief Turns ON all the modules and reconfigures them if necessary
/// \return 0 all modules have woken up successfully
/// \return 1 not all modules have woken up successfully
///
    mySensors.wakeUp();
}
void SensorStab_GSM::convertTime(uint32_t time){
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

    SensorStab_GSM::timeString[0]=(48+days/10); //day
    SensorStab_GSM::timeString[1]=(48+days%10); //day
    SensorStab_GSM::timeString[2]=(':');
    SensorStab_GSM::timeString[3]=(48+hours/10); //hour
    SensorStab_GSM::timeString[4]=(48+hours%10); //hour
    SensorStab_GSM::timeString[5]=(':');
    SensorStab_GSM::timeString[6]=(48+minutes/10); //min
    SensorStab_GSM::timeString[7]=(48+minutes%10); //min
    SensorStab_GSM::timeString[8]=(':');
    SensorStab_GSM::timeString[9]=(48+seconds/10); //seconds
    SensorStab_GSM::timeString[10]=(48+seconds%10); //seconds
    SensorStab_GSM::timeString[11]=('0');
}
uint8_t SensorStab_GSM::measureBattery(void){
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
void SensorStab_GSM::run(void){
/// \brief structures the procedure of the sensor-node
///
    switch(runState){
    case measure:
        USB.println();
        USB.println("runstate = measure");
        USB.println();
        if(makeMeasurment()==RETURN_OK){
            runState=sendServer;
        }
        else{
            if(measureCnt>=5){
                measureCnt=0;
                runState=sleep;
            }
            else{
                measureCnt++;
            }
        }
        break;
    case sendServer:{
        if(my3G.sendData(&mySensorStruct)==RETURN_OK){
            runState=sleep;
        }}
        break;
    case sleep:
        USB.println();
        USB.println("runstate = sleep");
        USB.println();
        deepSleep();
        runState=wakingUP;
        break;
    case wakingUP:
        USB.println();
        USB.println("runstate = wakeUP");
        USB.println();
        wakeUp();
        runState=measure;
        break;
    }
}
