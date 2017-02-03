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
    RTC.ON();
     // Setting time [yy:mm:dd:dow:hh:mm:ss]
     RTC.setTime("12:07:17:02:17:25:00");

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
    _3G.OFF(); 
    USB.println("In deepsleep");
    USB.OFF();
    disableInterrupts(SENS_INT);
//DeepSleep does not seem to work on these board, consumes 20 ma
    customDeepSleep2("00:00:10:00",RTC_OFFSET,RTC_ALM1_MODE1,ALL_OFF);

  /*for (int i=0; i <= 8; i++){
	  PWR.sleep(WTD_8S, ALL_OFF);
   }*/
  

    if( intFlag & RTC_INT ){
      intFlag &= ~(RTC_INT);
    }
}

void SensorStab_GSM::customDeepSleep2(	const char* time2wake,
                                                        uint8_t offset,
                                                        uint8_t mode,
                                                        uint8_t option	)
{
        uint8_t retries=0;

        // Switch off both multiplexers in UART_0 and UART_1
        Utils.muxOFF();

        // set the XBee monitorization pin to zero
        pinMode(XBEE_MON,OUTPUT);
        digitalWrite(XBEE_MON,LOW);


        // mandatory delay to wait for MUX_RX stabilization
        // after switching off the sensor boards
        delay(100);

        // make sure interruption pin is LOW before entering a low power state
        // if not the interruption will never come
        while(digitalRead(MUX_RX)==HIGH)
        {
                // clear all detected interruption signals
                delay(1);
                PWR.clearInterruptionPin();
                retries++;
                if(retries>10)
                {
                        USB.println("interrupt fail");
                        return (void)0;
                }
        }

        // RTC ON
        RTC.ON();
        // set Alarm
        RTC.setAlarm1(time2wake,offset,mode);
        // get backup of selected Alarm
        uint8_t day_aux = RTC.day_alarm1;
        uint8_t hour_aux = RTC.hour_alarm1;
        uint8_t minute_aux = RTC.minute_alarm1;
        uint8_t second_aux = RTC.second_alarm1;
        // get Alarm
        RTC.getAlarm1();
        // check Alarm was correctly set
        if(	( day_aux != RTC.day_alarm1 )
        ||	( hour_aux != RTC.hour_alarm1 )
        ||	( minute_aux != RTC.minute_alarm1 )
        ||	( second_aux != RTC.second_alarm1 ) )
        {
                RTC.disableAlarm1();
                RTC.OFF();
                USB.println("RTC fail!");
                return (void)0;
        }
    RTC.OFF();
    PWR.switchesOFF(option);

        // set sleep mode
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_mode();
        sleep_disable();

        // in the case SENS_OFF was an option is mandatory to turn on the
        // sensor boards before setting up the I2C bus
        PWR.switchesON(option);

        // Switch on the RTC and clear the alarm signals
        // Disable RTC interruption after waking up
        RTC.ON();
        RTC.disableAlarm1();
        RTC.clearAlarmFlag();
        RTC.OFF();

        // Keep sensor supply powered down if selected
        if( option & SENS_OFF )
        {
                pinMode(SENS_PW_3V3,OUTPUT);
                digitalWrite(SENS_PW_3V3,LOW);
                pinMode(SENS_PW_5V,OUTPUT);
                digitalWrite(SENS_PW_5V,LOW);
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
