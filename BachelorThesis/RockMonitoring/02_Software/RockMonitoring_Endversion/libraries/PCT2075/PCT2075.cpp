#include "PCT2075.h"
/// \class PCT2075
/// \brief Class that handles the communication to the temperature sensor PCT2075
///
PCT2075::PCT2075()
{

}
/// \brief This method initializes the temperature sensor PCT2075
/// \param int slaveAddr, value from 0 to 3, sets the i2c slave address
///
void PCT2075::init(uint8_t slaveAddr)
{
    switch (slaveAddr)
    {
    case 0:
        {
            i2cSlaveAddr = 0x48; // with pins SA0=0, SA1=0
        }
        break;
    case 1:
        {
            i2cSlaveAddr = 0x49; // with pins SA0=1, SA1=0
        }
        break;
    case 2:
        {
            i2cSlaveAddr = 0x4A; // with pins SA0=0, SA1=1
        }
        break;
    case 3:
        {
            i2cSlaveAddr = 0x4B; // with pins SA0=1, SA1=1
        }
        break;
    default:
        {
            //i2c slave address not set
        }
    }

   uint8_t databyte;

   // write 0000 0000 = 0x00 to temperature sensor control register 1 to place it in active mode
   databyte = 0x00;    //active mode
   Wire.writeBytes( i2cSlaveAddr, 0x01, &databyte,(uint8_t) 1);

}

/// \brief This method gets the temperatur value of the temperature sensor PCT2075
/// \param int18_t & Temp, reference to data of temperature sensor
///
/// \return uint8_t state, gives back 1 for Error and 0 for ok
///
uint8_t PCT2075::getTemp(int8_t & Temp)
{
uint8_t databyte;

      uint8_t Buffer[2]; // read buffer
	  
      if (Wire.readBytes(i2cSlaveAddr, PCT2075_TEMP, Buffer, 1) == 1)
      {
         // Temp = ((Buffer[0] << 8) | Buffer[1])>>5;   //Temperature reading 2 Bytes
         Temp = Buffer[0];								//Temperature reading 1 Byte
      }
      else
      {
          USB.print(F("I2C error"));
          return RETURN_ERROR;
      }
      return RETURN_OK;
}

/// \brief This method sets the Temperature Sensor in shutdown mode
///
void PCT2075::goToSleep()
{
    uint8_t databyte;

   // write 0000 0001 = 0x01 to temperature sensor control register 1, to place it in shutdown mode
   databyte = 0x01;    //active mode
   Wire.writeBytes( i2cSlaveAddr, 0x01, &databyte,(uint8_t) 1);
}
