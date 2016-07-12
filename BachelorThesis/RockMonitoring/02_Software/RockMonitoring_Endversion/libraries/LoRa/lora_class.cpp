#include "lora_class.h"
#include "WaspUSB.h"
#include "WaspRTC.h"
#include "Waspmote.h"

//Variables
uint8_t initialize=1, rxFlag=0, txFlag=0, txSleepFlag=0, rxSleepFlag=0;


//Konstruktor
Lora::Lora(Sensors::structSensors * sensorstruct){
/// \brief Constructor sets all LoRa-paramets to default \n
/// \param TxPower is set to high \n
/// \param Mode is set to 3  \n
/// \param CRC and Header are on \n
/// \param Channel is set to Channel 17 \n
/// \retutn void
///

    localSensorStruct = sensorstruct;
    mySettings.channel=CH_17_868;
    mySettings.crc=1;
    mySettings.header=1;
    mySettings.mode=3;
    mySettings.power='H';
}
uint8_t Lora::getID(void){
/// \brief Takes the Input from the terminal to set the ID of the sensornode
/// \return An uint8_t containing the ID given in the terminal is returned
///
    USB.ON();
    USB.flush();
    int ID[3]={0,0,0};
    int i=0;
    int iOld;
    int myID=0;

    USB.print("Enter ID: ");
    while((i<3)&&(ID[iOld]!=13))
    {
        if (USB.available() > 0)
        {
           // USB.println("Byte received: ");
            ID[i] = USB.read();
            USB.print(ID[i],BYTE);
            iOld = i;
            i++;
        }
    }
    char ID2[3];
    for(i=0;i<3;i++){
        ID2[i]=(char)ID[i];
    }
    myID=atoi((const char*)ID2); //convert the String to
    //USB.println(F("Choosen ID is:"));
    USB.print(myID);
    USB.println();
    return (uint8_t)myID;
}
int8_t Lora::init(void){
/// \brief Sets the basic settings so that the sensor-node can communicate with the gateway when turned on
/// \brief Gets the SensorNodeID from the EEPROM and turns the RTC, the LoRa and the USB on
/// \return 0 settings done
/// \return 1 error during settings
///

//Config LoRa
    int8_t error=0;   //variable used to show the errors
    USB.ON();
    // init SX1272 module
    sx1272.ON();
    // Select channel
    error = error || sx1272.setChannel(CH_17_868);
    mySettings.channel = CH_17_868; //Safe the Settings
    USB.print(F("Setting Channel to\t'17'\t\t state "));
    USB.println(error);
    // Set Header mode: ON
    error = error || sx1272.setHeaderON();
    mySettings.header=1; //Safe the Setting
    USB.print(F("Setting Header  to\t'1' \t\t state "));
    USB.println(error);

    // Set Mode
    error = error || sx1272.setMode(3);//slowest mode with highest range
    mySettings.mode=3;
    USB.print(F("Setting Mode    to\t'3'\t\t state "));
    USB.println(error);

    // Set CRC mode
    error = error || sx1272.setCRC_ON();
    mySettings.crc=1;   //Safe the Settings
    USB.print(F("Setting CRC     to\t'1' \t\t state "));
    USB.println(error);

    // Set packet lengt
    error = error || sx1272.setPacketLength(19);
    mySettings.packetLength=19; //Safe the Settings
    USB.print(F("Setting Packet to '19'.\t\t state "));
    USB.println(error);

    // Select output power (Max, High or Low)
    error = error || sx1272.setPower('M');
    mySettings.power='M';   //Safe the Settings
    USB.print(F("Setting Power   to\t'L' \t\t state "));
    USB.println(error);

   // mySettings.nodeId = getID();//get the ID from Terminal
    mySettings.nodeId=Utils.readEEPROM(1024);
    error = error || sx1272.setNodeAddress(mySettings.nodeId);
    USB.print(F("Setting ID      to\t"));
    USB.print(mySettings.nodeId,DEC);
    USB.print(F("\t\t state "));
    //set retries
    sx1272.setRetries(5);
    //RTC config
    RTC.ON();
    RTC.setTime("00:00:00:00:00:00:0");
    USB.println(error);
    if(error==0){
        USB.println("settings succesful");
        Utils.setLED(LED0, LED_ON);
        return RETURN_OK;
    }
    else{
        USB.println("settings unsuccesful");
        return RETURN_ERROR;
    }
}
void Lora::convertStruct(void){
/// \brief Converts the struct containing the information to send into a uin8t_t array
///
    Lora::stringData[0]=(uint8_t)((localSensorStruct->sensor1.pitch>>8)&0xFF); //high byte
    Lora::stringData[1]=(uint8_t)((localSensorStruct->sensor1.pitch)&0xFF); //low byte
    Lora::stringData[2]=(uint8_t)((localSensorStruct->sensor2.pitch>>8)&0xFF); //high byte
    Lora::stringData[3]=(uint8_t)((localSensorStruct->sensor2.pitch)&0xFF); //low byte
    Lora::stringData[4]=(uint8_t)((localSensorStruct->sensor3.pitch>>8)&0xFF); //high byte
    Lora::stringData[5]=(uint8_t)((localSensorStruct->sensor3.pitch)&0xFF); //low byte
    Lora::stringData[6]=(uint8_t)((localSensorStruct->sensor1.roll>>8)&0xFF); //high byte
    Lora::stringData[7]=(uint8_t)((localSensorStruct->sensor1.roll)&0xFF); //low byte
    Lora::stringData[8]=(uint8_t)((localSensorStruct->sensor2.roll>>8)&0xFF); //high byte
    Lora::stringData[9]=(uint8_t)((localSensorStruct->sensor2.roll)&0xFF); //low byte
    Lora::stringData[10]=(uint8_t)((localSensorStruct->sensor3.roll>>8)&0xFF); //high byte
    Lora::stringData[11]=(uint8_t)((localSensorStruct->sensor3.roll)&0xFF); //low byte
    Lora::stringData[12]=(uint8_t)((localSensorStruct->extension>>8)&0xFF); //high byt
    Lora::stringData[13]=(uint8_t)((localSensorStruct->extension)&0xFF); //low byte
    Lora::stringData[14]=(uint8_t)(localSensorStruct->batteryLevel);
    Lora::stringData[15]=(uint8_t)(localSensorStruct->sensTemp1);
    Lora::stringData[16]=(uint8_t)(localSensorStruct->sensTemp2);
    Lora::stringData[17]=(uint8_t)(localSensorStruct->sensTemp3);
    Lora::stringData[18]=(uint8_t)(localSensorStruct->sensorNodeId);
}
void Lora::convertStructBack(Sensors::structSensors *dest){
/// \brief Converts the uin8t_t array back into a struct containing all the necessary information
/// \param dest is the destination of the conversion back into an struct
///
    dest->sensor1.pitch=(int16_t)((int16_t)(sx1272.packet_received.data[0]<<8)|(sx1272.packet_received.data[1]));
    dest->sensor2.pitch=(int16_t)((int16_t)(sx1272.packet_received.data[2]<<8)|(sx1272.packet_received.data[3]));
    dest->sensor3.pitch=(int16_t)((int16_t)(sx1272.packet_received.data[4]<<8)|(sx1272.packet_received.data[5]));
    dest->sensor1.roll=(int16_t)((int16_t)(sx1272.packet_received.data[6]<<8)|(sx1272.packet_received.data[7]));
    dest->sensor2.roll=(int16_t)((int16_t)(sx1272.packet_received.data[8]<<8)|(sx1272.packet_received.data[9]));
    dest->sensor3.roll=(int16_t)((int16_t)(sx1272.packet_received.data[10]<<8)|(sx1272.packet_received.data[11]));
    dest->extension=(int16_t)((int16_t)(sx1272.packet_received.data[12]<<8)|(sx1272.packet_received.data[13]));
    dest->batteryLevel=(uint8_t)sx1272.packet_received.data[14];
    dest->sensTemp1=(uint8_t)sx1272.packet_received.data[15];
    dest->sensTemp2=(uint8_t)sx1272.packet_received.data[16];
    dest->sensTemp3=(uint8_t)sx1272.packet_received.data[17];
    dest->sensorNodeId=(uint8_t)sx1272.packet_received.data[18];
}
uint8_t Lora::activityDetection(void){
/// \brief Detects any activity on the channel
/// \return 0 there has been an activity on the channel
/// \return 1 there hasn't been an activity on the channel
///
    if(sx1272.cadDetected()){
        USB.println(F("\t\t\ LoRa: Channel Activity Detected"));
        return RETURN_OK;
    }
    else{
        return RETURN_ERROR;
    }
}
uint8_t Lora::dataRequest(uint8_t ID){
/// \brief Gateway sends a request for measurement data to the sensornode
/// \param ID says which sensornode should send the data
/// \return 0 no error request has been received and acknowledged
/// \return 1 error request has not been received
///
    USB.println(F("\t\t\ LoRa:dataRequest"));
    USB.print(F("\t\t\ LoRa:sending Request to note"));
    USB.println(ID,DEC);
    txFlag=sx1272.sendPacketTimeoutACKRetries(ID,&sendDataCommand,1);

    if(txFlag==0){//sensornode has received the Data Request and sent ACK
        USB.println(F("\t\t LoRa: Request sent"));
        return RETURN_OK;
    }
    else{
        USB.print(F("\t\t\ LoRa: SEND state"));
        USB.println(txFlag, DEC);
        return RETURN_ERROR;
    }
    //delay(5000);
}
uint8_t Lora::waitForDataRequest(void){
/// \brief Sensor-node waits for the data-request from the gateway
/// \return 0 request has been received and acknowledged
/// \return 1 error request has not been received
///
    uint8_t rxFlag=0;
    USB.println(F("\t\t\ LoRa: dataRequest"));
    rxFlag=sx1272.receivePacketTimeoutACK(10000);
    if(sx1272.packet_received.data[0]==sendDataCommand){//checks if the received Packet is really a data request
        if(rxFlag==0){//sensornode has received the Data Request and send ACK
            USB.println(F("Packet Reiceived "));
            return RETURN_OK;
        }
        else{
            USB.print(F("\t\t\ LoRa: RECEIVE state "));
            USB.println(rxFlag, DEC);
            return RETURN_ERROR;
        }
    }
}
uint8_t Lora::sendData(void){
/// \brief Sensor-node sends the data requested by the gateway
/// \return 0 data has been send successful
/// \return 1 data has not been send successful
///
    USB.println(F("\t\t\  LoRa: sendData"));
    txFlag=sx1272.sendPacketTimeoutACKRetries(gatewayID ,stringData, sizeof(stringData));
    if(txFlag==0){
        USB.println(F("\t\t\ LoRa: Packet sent"));
        return RETURN_OK;
    }
    else{
        USB.print(F("\t\t\ LoRa: Sending failed: "));
        USB.println(txFlag, DEC);
        return RETURN_ERROR;
    }
}
uint8_t Lora::receiveData(void){
/// \brief Gateway receives the data sent by the sensor-node via Lora
/// \return 0-> no error Data has been received and acknowledged
/// \return 1-> error Data has not been received
///

    USB.println(F("receiveData"));
    rxFlag=sx1272.receivePacketTimeoutACK(10000);
    if(rxFlag == 0){ //received the packet and ACK is send
        return RETURN_OK;
    }
    else{
        USB.print(F("RECEIVE state"));
        USB.println(rxFlag, DEC);
        return RETURN_ERROR;
    }
}
int8_t Lora::wakeUp(void){
/// \brief After the deepSleep the LoRa-module needs to be reconfigured, takes all the settings saved in mySettings and restores them
/// \return 0 no error during reconfiguration
/// \return 1 error during reconfiguration
///
    int8_t error=0;
    USB.ON();
    sx1272.ON();
    delay(1000);
    // Select channel
    error =error|| sx1272.setChannel(CH_17_868);
    USB.print("set channel is ther an error: ");
    USB.println(error);
    // Set Header mode: ON
    if(mySettings.header==1){
        error =error || sx1272.setHeaderON();
    }
    if(mySettings.header==0){
        error = error || sx1272.setHeaderOFF();
    }
    // Set Mode
    error = error || sx1272.setMode( mySettings.mode);//slowest mode with highest range
    // Set CRC mode
    if(mySettings.crc==1){
        error = error || sx1272.setCRC_ON();
    }
    if(mySettings.crc==0){
        error = error || sx1272.setCRC_OFF();
    }
    // Set packet length
    error = error || sx1272.setPacketLength(mySettings.packetLength);
    // Select output power (Max, High or Low)
    error = error || sx1272.setPower(mySettings.power);
    //Set Node ID
    error = error || sx1272.setNodeAddress(mySettings.nodeId);
    //set number of retries
    sx1272.setRetries(5);
    //RTC config
    RTC.ON();
    RTC.setTime("00:00:00:00:00:00:0");

    if(error == 0){
        Utils.setLED(LED0, LED_ON);
        Utils.setLED(LED1, LED_ON);
        USB.println(F("\t\t\t LoRa: Settings done"));
        return RETURN_OK;
    }
    else{
        USB.print("\t\t\ LoRa: Settings error");
        USB.println(error);
        return RETURN_ERROR;
    }
}
uint8_t Lora::sendDeepSleep(uint8_t ID){
/// \brief After the gateway has finished uploading the information to the webserver it sends to command to the sensor-node to go to sleep
/// \param ID which sensor-node should go to sleep
/// \return 0 sensor-node received command and has acknowledged
/// \return 1 sensor-node has not received command
///
    USB.println(F("\t\t\ LoRa: sendDeepSleep command"));
    txSleepFlag=sx1272.sendPacketTimeoutACKRetries(ID,&sendCommandToSleep,1);
    if(txSleepFlag==0){
        USB.println(F("\t\t\  LoRa: deep sleep sent"));
        return RETURN_OK;
    }
    else{
        USB.print(F("\t\t\  LoRa: deep Sleep failed: "));
        USB.println(txSleepFlag, DEC);
        return RETURN_ERROR;
    }
}
uint8_t Lora::waitForDeepSleep(void){
/// \brief After the sensor-node has send the requested data to the gateway, he waits for the deepSleep command
/// \return 0 command received
/// \return 1 command not received
///
    USB.println(F("\t\t\ LoRa: Wait For DeepSleep command"));
    rxSleepFlag=sx1272.receivePacketTimeoutACK(10000);
    if(sx1272.packet_received.data[0]==sendCommandToSleep){//Checks if the packet received is a sleepCommand
        if(rxSleepFlag == 0){ //received the packet and ACK is send
            USB.println(F("\t\t DeepSleep command received"));
            return RETURN_OK;
        }
        else{
            USB.print(F("\t\t\ LoRa: DeepSleep command NOT received"));
            USB.println(rxSleepFlag, DEC);
            return RETURN_ERROR;
        }
    }
}
