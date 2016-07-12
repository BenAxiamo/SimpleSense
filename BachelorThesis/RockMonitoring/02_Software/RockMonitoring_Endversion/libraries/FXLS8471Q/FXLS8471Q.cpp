#include "FXLS8471Q.h"
#include "WaspUSB.h"

/// \class FXLS8471Q
/// \brief Class that handles the communication to the Accelerometer FXLS8471Q
///
FXLS8471Q::FXLS8471Q()
{
}
/// \brief This method initializes the Accelerometer FXLS8471Q
/// \param int slaveAddr, value from 0 to 3, sets the i2c slave address
void FXLS8471Q::init(uint8_t slaveAddr)
{
    switch (slaveAddr)
    {
    case 0:
        {
            i2cSlaveAddr = 0x1E; // with pins SA0=0, SA1=0
        }
        break;
    case 1:
        {
            i2cSlaveAddr = 0x1D; // with pins SA0=1, SA1=0
        }
        break;
    case 2:
        {
            i2cSlaveAddr = 0x1C; // with pins SA0=0, SA1=1
        }
        break;
    case 3:
        {
            i2cSlaveAddr = 0x1F; // with pins SA0=1, SA1=1
        }
        break;
    default:
        {
            //i2c slave address not set
        }
    }

	uint8_t databyte;

	// write 0000 0000 = 0x00 to accelerometer control register 1 to place FXLS8471Q into standby
	// [7-1] = 0000 000
	// [0]: active=0
	databyte = 0x00;
	Wire.writeBytes( i2cSlaveAddr, FXLS8471Q_CTRL_REG1, &databyte,(uint8_t) 1);

	// write 0000 0000= 0x00 to XYZ_DATA_CFG register
	// [7]: reserved
	// [6]: reserved
	// [5]: reserved
	// [4]: hpf_out=0
	// [3]: reserved
	// [2]: reserved
	// [1-0]: fs=00 for accelerometer range of +/-2g with 0.244mg/LSB
	databyte = 0x00;
	Wire.writeBytes(i2cSlaveAddr, FXLS8471Q_XYZ_DATA_CFG, &databyte, (uint8_t) 1);

	// write 0011 1001b = 0x39 to accelerometer control register 1
	// [7-6]: aslp_rate=00
	// [5-3]: dr=111 for 1.56Hz data rate
	// [2]: lnoise=0 for Reduced noise mode
	// [1]: f_read=0 for normal 16 bit reads
	// [0]: active=1 to take the part out of standby and enable sampling
	databyte = 0x39;
	Wire.writeBytes(i2cSlaveAddr, FXLS8471Q_CTRL_REG1, &databyte,(uint8_t) 1);

	// write 0001 0010b = 0x12 to accelerometer control register 2
	// [7]  : 0 self test disabled
	// [6]  : 0 reset disabled
	// [5]  : 0 -
	// [4-3]: 10 low noise low power in sleep mode
	// [2]  : 0 auto sleep not enabled
	// [1-0]: 10 low noise, low power
	databyte = 0x12;
	Wire.writeBytes(i2cSlaveAddr, FXLS8471Q_CTRL_REG2, &databyte,(uint8_t) 1);

	// write 0000 0011b = 0x03 to accelerometer control register 3
	// [7]  : 0 fifo_gate
	// [6]  : 0 wake_tran
	// [5]  : 0 wake_Indprt
	// [4]  : 0 wake_pulse
	// [3]  : 0 wake_ffmt
	// [2]  : 0 wake_en_a_vecm
	// [1]  : 1 ipol -> active high
	// [0]  : 0 pp_od ->open-drain
	databyte = 0x03;
	Wire.writeBytes(i2cSlaveAddr, FXLS8471Q_CTRL_REG3, &databyte,(uint8_t) 1);

    FXLS8471Q::structXYZ xyz;
	getAccXYZ(& xyz);
}

/// \brief This method returns the values of the X, Y and Z axis of the Accelerometer FXLS8471Q
/// \param FXLS8471Q::structXYZ * xyz, pointer to x,y and z data of one accelerometer
///
/// \return uint8_t state, gives back 1 for Error and 0 for ok
///
uint8_t FXLS8471Q::getAccXYZ(FXLS8471Q::structXYZ * xyz)
{
      uint8_t Buffer[FXLS8471Q_READ_LEN]; // read buffer
      // read FXLS8471Q Acceleration Data (Status plus 3 acceleration channels)
      if (Wire.readBytes(i2cSlaveAddr, FXLS8471Q_STATUS, Buffer, FXLS8471Q_READ_LEN) == FXLS8471Q_READ_LEN)
      {
          // copy the 14 bit accelerometer byte data into 16 bit words
          xyz->x = ((Buffer[1] << 8) | Buffer[2])>> 2;
          xyz->y = ((Buffer[3] << 8) | Buffer[4])>> 2;
          xyz->z = ((Buffer[5] << 8) | Buffer[6])>> 2;
      }
      else
      {
          return (RETURN_ERROR);
      }
      return (RETURN_OK);

}

/// \brief This method returns the values of the pitch and roll angles of the Accelerometer FXLS8471Q
/// \param FXLS8471Q::structPitchRoll * PitchRoll, pointer to pitch and roll angle data of one accelerometer
///
/// \return uint8_t state, gives back 1 for Error and 0 for ok
///
uint8_t FXLS8471Q::getAccPitchRoll(FXLS8471Q::structPitchRoll * PitchRoll)
{
      uint8_t Buffer[FXLS8471Q_READ_LEN]; // read buffer
      FXLS8471Q::structXYZ xyz;
      double roll, pitch;
      double X_G, Y_G, Z_G;
	  // read FXLS8471Q Acceleration Data (Status plus 3 acceleration channels)
      if (Wire.readBytes(i2cSlaveAddr, FXLS8471Q_STATUS, Buffer, FXLS8471Q_READ_LEN) == FXLS8471Q_READ_LEN)
      {
        // copy the 14 bit accelerometer byte data into 16 bit words
        xyz.x = ((Buffer[1] << 8) | Buffer[2])>> 2;
        xyz.y = ((Buffer[3] << 8) | Buffer[4])>> 2;
        xyz.z = ((Buffer[5] << 8) | Buffer[6])>> 2;
		
		//Calculate the acceleration in G for of each axis
        X_G = xyz.x*FXLS8471Q_Sensitivity;
        Y_G =xyz.y*FXLS8471Q_Sensitivity ;
        Z_G =xyz.z*FXLS8471Q_Sensitivity ;
		
		//Calculate pitch and roll angles 
        pitch = 57.2957795 * atan(X_G/(sqrt((Y_G*Y_G)+(Z_G*Z_G))));       //Calculation of Pitch Angle in Degrees
        roll  = 57.2957795 * atan(Y_G/(sqrt((X_G*X_G)+(Z_G*Z_G))));       //Calculation of Roll Angle in Degrees

		//set 2 digits after commas
        pitch = static_cast<int> (pitch*100+0.5) / 100.0;
        roll  = static_cast<int> (roll *100+0.5) / 100.0;
		
		//cast to int, but is stored 100 times higher (including 2 places after comma)
        PitchRoll->pitch = (int) (pitch*100);   
        PitchRoll->roll = (int) (roll*100);

      }
      else
      {
          return (RETURN_ERROR);
      }
      return (RETURN_OK);

}

/// \brief This method sets the Accelerometer in Standby
void FXLS8471Q::goToSleep()
{
	uint8_t databyte;

	// write 0000 0000 = 0x00 to accelerometer control register 1 to place FXLS8471Q into
	// standby
	// [7-1] = 0000 000
	// [0]: active=0
	databyte = 0x00;
	Wire.writeBytes( i2cSlaveAddr, FXLS8471Q_CTRL_REG1, &databyte,(uint8_t) 1);
}
