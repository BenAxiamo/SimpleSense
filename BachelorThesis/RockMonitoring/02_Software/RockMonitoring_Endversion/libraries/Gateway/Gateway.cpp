#include "Gateway.h"

 enum State{wakingUP,dataRequest,receive,sendServer, sendSleepCommand,sleep};
     static State runState=dataRequest;

Gateway::Gateway():myLora(&mySensorStruct){
/// \brief This method creats object of Lora and Connection3G
///
    Connection3G my3G;
}
uint8_t Gateway::init(void){
/// \brief This method initializes all the components of the sensornode
/// \return 0 all settings are done correctly
/// \return 1 error during settings
///
    int8_t error=0;
    PWR.setSensorPower(SENS_3V3,SENS_ON); // Sets the 3,3V switch ON
    myLeds.init();
    pinMode(DIGITAL4 ,INPUT);   //Fault  (LOW-Active)
    pinMode(DIGITAL5 ,INPUT);   //Charge (LOW-Active)
    pinMode(DIGITAL6 ,OUTPUT);   //Shutdown
    myLeds.BlinkAkku(measureBattery());
    my3G.init();
    error = myLora.init();
    return error;
}
uint8_t Gateway::measureBattery(void){
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
void Gateway::showStructContent(uint8_t node){
/// \brief Shows the content of a received packet from a sensor-node
///
    USB.println(F("\t\-----------------------------------"));
    USB.print(F("\t\ Show Struct from Node:"));
    USB.println(mySensorStructArray[node].sensorNodeId, DEC);
    USB.print(F("\t\ BatteryLevel: "));
    USB.println(mySensorStructArray[node].batteryLevel, DEC);
    USB.print(F("\t\ extension: "));
    USB.println(mySensorStructArray[node].extension, DEC);
    USB.print(F("\t\ pitch1: "));
    USB.println(mySensorStructArray[node].sensor1.pitch, DEC);
    USB.print(F("\t\ roll1: "));
    USB.println(mySensorStructArray[node].sensor1.roll, DEC);
    USB.print(F("\t\ pitch2: "));
    USB.println(mySensorStructArray[node].sensor2.pitch, DEC);
    USB.print(F("\t\ roll2: "));
    USB.println(mySensorStructArray[node].sensor2.roll, DEC);
    USB.print(F("\t\ pitch3: "));
    USB.println(mySensorStructArray[node].sensor3.pitch, DEC);
    USB.print(F("\t\ roll3: "));
    USB.println(mySensorStructArray[node].sensor3.roll, DEC);
    USB.print(F("\t\ temp1: "));
    USB.println(mySensorStructArray[node].sensTemp1,DEC);
    USB.print(F("\t\ temp2: "));
    USB.println(mySensorStructArray[node].sensTemp2, DEC);
    USB.print(F("\t\ temp3: "));
    USB.println(mySensorStructArray[node].sensTemp3, DEC);
    USB.println(F("\t\-----------------------------------"));
}
uint8_t Gateway::requestData(uint8_t ID){
/// \brief Requests data from a sensor-node
/// \return 0 request was successful
/// \return 1 request was unsuccessful
///
    return myLora.dataRequest(ID);
}
uint8_t Gateway::receiveNodeData(void){
/// \brief Receives data from a sensor-node
/// \return 0 receiving was successful
/// \return 1 receiving was unsuccessful
///
    return myLora.receiveData();//Information are stored in mySensorStruct
}
uint8_t Gateway::sendToServer(Sensors::structSensors *dataToSend){
/// \brief Uploads all the received data to a web-server
/// \return 0 uploading was successful
/// \return 1 uploading was unsuccessful
///
    my3G.sendData(dataToSend);
}
uint8_t Gateway::sendSleep(uint8_t ID){
/// \brief Sends to all the sensor-nodes a sleep command
/// \return 0 command was successful
/// \return 1 command was unsuccessful
///
    return myLora.sendDeepSleep(ID);
}
void Gateway::deepSleep(void){
/// \brief Sets gateway into deepSleep mode
///
    Utils.setLED(LED0, LED_OFF);
    Utils.setLED(LED1, LED_OFF);
    PWR.deepSleep("00:00:15:20",RTC_OFFSET,RTC_ALM1_MODE1,SOCKET0_OFF);
    if( intFlag & RTC_INT ){
      intFlag &= ~(RTC_INT);
    }
}
int8_t Gateway::wakeUp(void){
/// \brief After deepSleep reconfigures all the modules
/// \return 0 reconfiguration was successful
/// \return 1 reconfiguration was unsuccessful
///
PWR.setSensorPower(SENS_3V3,SENS_ON); // Sets the 3,3V switch ON
    return myLora.wakeUp();
}
void Gateway::run(void){
/// \brief structures the procedure of the gateway
///
    switch(runState){
        case dataRequest:{
            USB.println();
            USB.println("runstate = SendDataRequest");
            USB.println();
            if(Gateway::requestData(sensorID[currentNode])==RETURN_OK){//checks if the currentNode has received the data request
                USB.print("\t\ datarequest has been ack from sensorstab ");
                USB.println(sensorID[currentNode],DEC);
                runState=receive;
            }
            else{
                USB.println("\t\ datarequest has NOT been ack from sensorstab ");
                if(notReceivedCnt>=1){//reached maximum number of tries --> sensor-node is not responding
                    USB.println("\t\ Reached maximum number of tries -> not responding -> overjump");
                    notReceivedCnt=0;
                    mySensorStructArray[currentNode].sensorNodeId=0; //sensorNodeId equal 0 means that the sensor could not be reached
                    if(currentNode != sizeof(sensorID)-1){//checks if it isn't the last node
                        USB.println("\t\ Not last sensor-node -> send data request to next sensor-node");
                        currentNode++;//jump to next node for data request, current node is not responding
                    }
                    else{
                        USB.println("\t\ Last Node -> change directly to runstate=sendServer");
                        runState=sendServer;
                    }
                }
                else{
                    USB.println("\t\ Is not responding");
                    notReceivedCnt++;
                }
            }
        }
        break;

    case receive:{

            USB.println();
            USB.println("runstate = receive");
            USB.println();
            if(receiveNodeData()==RETURN_OK){
                USB.println("\t\ data has been received");
                myLora.convertStructBack(&mySensorStructArray[currentNode]);//Saves the measurement of the different Sensor-nodes separately
                showStructContent(currentNode);
                if(currentNode == sizeof(sensorID)-1){//Check if all the Sensor-nodes have send there data
                    currentNode=0;//start again with the first sensor-node
                    myLeds.LedGreenOn();
                    runState=sendServer;
                }
                else{
                    currentNode++;
                    runState=dataRequest; //request data from next sensor-node
                }
            }
            else{
                if(nothingReceived==3){
                    nothingReceived=0;
                    mySensorStructArray[currentNode].sensorNodeId=0; //sensorNodeId equal 0 means that the sensor hasn't sent new data
                    if(currentNode != sizeof(sensorID)-1){//checks if it isn't the last node
                        currentNode++;//jump to next node for data request, current node is not responding
                        runState=dataRequest; //error handling
                        USB.println("\t\ received ACK for Datarequest but no new data");
                    }
                    else{
                        runState=sendServer;
                    }
                }
                else{
                    nothingReceived++;
                }
            }
        }
        break;

    case sendServer:{
            USB.println();
            USB.println("runstate = SendServer");
            USB.println();
            if(my3G.sendData(&mySensorStructArray[0],(uint8_t)numberOfNodes)==RETURN_OK){
                runState=sendSleepCommand;
            }
        }
        break;

    case sendSleepCommand:{
            USB.println();
            USB.println("runstate = SendSleepCommand");
            USB.println();
            myLeds.LedGreenBlink4seconds();//all data have been sent
            if(sendSleep(sensorID[currentNode])== RETURN_OK){
                if(currentNode == sizeof(sensorID)-1){//Check if all the Sensor-nodes have send there data
                    runState=sleep;
                    currentNode=0;//start again with the first sensor-node
                }
                else{
                    currentNode++;
                }
            }
            else{
                if(notSleepCnt>=4){
                    notSleepCnt=0;
                    if(currentNode != sizeof(sensorID)-1){//checks if it isn't the last node
                        currentNode++;//jump to next node for sleep command
                    }
                    else{
                        currentNode=0;
                        runState=sleep;
                    }
                }
                else{
                    notSleepCnt++;
                }
            }
        }
        break;

    case sleep:{
            USB.println();
            USB.println("runstate = Sleep");
            USB.println();
            deepSleep();
            runState=wakingUP;
        }
        break;

    case wakingUP:{
            USB.println();
            USB.println("runstate = WakingUp");
            USB.println();
            if(wakeUp()==RETURN_OK){
                runState=dataRequest;
            }
        }
        break;
    }
}
