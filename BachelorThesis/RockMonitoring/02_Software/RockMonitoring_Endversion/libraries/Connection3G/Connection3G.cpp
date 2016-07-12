#include "Connection3G.h"
/// \class Connection3G
/// \brief Class that handles the communication over 3G
///
Connection3G::Connection3G()
{
}
/// \brief This method initializes the operator parameters that are necessary for 3G connection
void Connection3G::init()
{
    USB.println("3G init");

    // sets operator parameters
    _3G.set_APN("gprs.swisscom.ch", "", "");
    // And shows them
    USB.println("APN INFOS:");
    _3G.show_APN();
}

/// \brief This method sends all the sensor information from the struct to the webserver
/// \param Sensors::structSensors * data
///
/// \return uint8_t state, 0 when sent, 1 when send failed
///
uint8_t Connection3G::sendData(Sensors::structSensors * data)
{
    int8_t answer;
    char request_str[200];
    uint8_t state = 0;

    USB.println(F("Entering 3G SendData"));
    answer = _3G.ON();                      // activate the 3G module:
    USB.println(F("turned 3G module on"));
    if ((answer == 1) || (answer == -3))
    {
        USB.println("3G module ready...");

        USB.println(F("Setting PIN code...")); // set pin code:
        if (_3G.setPIN(simPin) == 1)
        {
            USB.println(F("PIN code accepted"));
        }
        else
        {
            USB.println(F("PIN code incorrect"));
        }


        answer = _3G.check(180);    // Wait for connection to the network
        if (answer == 1)
        {
            USB.println(F("3G module connected to the network..."));
            USB.print(F("Getting URL with GET method..."));
			char sensorName[20];
            sprintf (sensorName, "SensorID%d", data->sensorNodeId);
            sprintf(request_str,"GET /LogData.php?name=%s&battery=%u&pitch1=%d&roll1=%d&pitch2=%d&roll2=%d&pitch3=%d&roll3=%d&pot=%d&temp1=%d&temp2=%d&temp3=%d HTTP/1.1\r\nHost: %s\r\nContent-Length: 0\r\n\r\n", sensorName, data->batteryLevel,data->sensor1.pitch,data->sensor1.roll,data->sensor2.pitch,data->sensor2.roll,data->sensor3.pitch,data->sensor3.roll,data->extension,data->sensTemp1,data->sensTemp2,data->sensTemp3,httpStreamAddress);
            USB.println(request_str);
            answer = _3G.readURL(httpStreamAddress, 80, request_str);       //gets URL from the solicited URL

            if ( answer == 1)   // Checks the answer
            {
                USB.println(F("Done"));
                state = RETURN_OK;
                USB.println(_3G.buffer_3G);
            }
            else if (answer < -14)
            {
                USB.print(F("Failed. Error code: "));
                USB.println(answer, DEC);
                USB.print(F("CME error code: "));
                USB.println(_3G.CME_CMS_code, DEC);
                state = RETURN_ERROR;
            }
            else
            {
                USB.print(F("Failed. Error code: "));
                USB.println(answer, DEC);
                state = RETURN_ERROR;
            }
        }
        else
        {
            USB.println(F("3G module cannot connect to the network..."));
            state = RETURN_ERROR;
        }
    }
    else
    {
        // Problem with the communication with the 3G module
        USB.println(F("3G module not started"));
        state = RETURN_ERROR;
    }

    return state;
    _3G.OFF();      // Powers off the 3G module
}


/// \brief This method sends the information of the sensors in the array to the webserver
///        the parameter size tells the function how many sensors from the array should be uploaded
/// \param Sensors::structSensors * data , data of the sensors\n
///         uint8_t size, number of sensors to upload to webserver
///
/// \return uint8_t state, 0 when sent, 1 when send failed
///
uint8_t Connection3G::sendData(Sensors::structSensors * data , uint8_t size)
{
    int8_t answer;
    char request_str[200];
    uint8_t state = 0;

    USB.println(F("Entering 3G SendData"));
    answer = _3G.ON();                      // activate the 3G module:
    USB.println(F("turned 3G module on"));
    if ((answer == 1) || (answer == -3))
    {
        USB.println("3G module ready...");

        USB.println(F("Setting PIN code...")); // set pin code:
        if (_3G.setPIN(simPin) == 1)
        {
            USB.println(F("PIN code accepted"));
        }
        else
        {
            USB.println(F("PIN code incorrect"));
        }

        answer = _3G.check(180);    // Wait for connection to the network
        if (answer == 1)
        {
            USB.println(F("3G module connected to the network..."));

            for(uint8_t i = 0;i<size; i++)
            {
                if(data[i].sensorNodeId != 0)   //Sends data only if Gateway has received new Data over Lora from a sensor node
                {
                    USB.print(F("Getting URL with GET method..."));
                    char sensorName[20];
                    sprintf (sensorName, "SensorID%d", data[i].sensorNodeId);
                    sprintf(request_str,"GET /LogData.php?name=%s&battery=%u&pitch1=%d&roll1=%d&pitch2=%d&roll2=%d&pitch3=%d&roll3=%d&pot=%d&temp1=%d&temp2=%d&temp3=%d HTTP/1.1\r\nHost: %s\r\nContent-Length: 0\r\n\r\n", sensorName, data[i].batteryLevel,data[i].sensor1.pitch,data[i].sensor1.roll,data[i].sensor2.pitch,data[i].sensor2.roll,data[i].sensor3.pitch,data[i].sensor3.roll,data[i].extension,data[i].sensTemp1,data[i].sensTemp2,data[i].sensTemp3,httpStreamAddress);
                    USB.println(request_str);
                    answer = _3G.readURL(httpStreamAddress, 80, request_str);       //gets URL from the solicited URL

                    if ( answer == 1)   // Checks the answer
                    {
                        USB.println(F("Done"));
                        state = RETURN_OK;
                        USB.println(_3G.buffer_3G);
                    }
                    else if (answer < -14)
                    {
                        USB.print(F("Failed. Error code: "));
                        USB.println(answer, DEC);
                        USB.print(F("CME error code: "));
                        USB.println(_3G.CME_CMS_code, DEC);
                        state = RETURN_ERROR;
                    }
                    else
                    {
                        USB.print(F("Failed. Error code: "));
                        USB.println(answer, DEC);
                        state = RETURN_ERROR;
                    }
                }
            }
        }
        else
        {
            USB.println(F("3G module cannot connect to the network..."));
            state = RETURN_ERROR;
        }
    }
    else
    {
        // Problem with the communication with the 3G module
        USB.println(F("3G module not started"));
        state = RETURN_ERROR;
    }

    return state;
    _3G.OFF();      // Powers off the 3G module
}
/// \brief This method checks if a connection to the 3G network can be established
///
/// \return uint8_t state, 0 when 3G Connection could be established, 1 when connection could not be established
///
uint8_t Connection3G::checkConnection()
{
    int8_t answer;

    USB.println(F("Entering 3G CheckConnection"));
    answer = _3G.ON();                      // activate the 3G module:
    USB.println(F("turned 3G module on"));
    if ((answer == 1) || (answer == -3))
    {
        USB.println("3G module ready...");

        USB.println(F("Setting PIN code...")); // set pin code:
        if (_3G.setPIN(simPin) == 1)
        {
            USB.println(F("PIN code accepted"));
        }
        else
        {
            USB.println(F("PIN code incorrect"));
            return RETURN_ERROR;
        }


        answer = _3G.check(180);    // Wait for connection to the network
        if (answer == 1)
        {
            USB.println(F("3G module connected to the network..."));
            return RETURN_OK;
        }
        else
        {
            USB.println(F("3G module cannot connect to the network..."));
            return RETURN_ERROR;
        }
    }
    else
    {
        // Problem with the communication with the 3G module
        // USB.println(F("3G module not started"));
        return RETURN_ERROR;
    }

    _3G.OFF();      // Powers off the 3G module
}
