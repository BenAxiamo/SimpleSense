/*  
 *  ------ Waspmote Pro Code Example -------- 
 *  
 *  Explanation: This is the basic Code for Waspmote Pro
 *  
 *  Copyright (C) 2013 Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify  
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation, either version 3 of the License, or  
 *  (at your option) any later version.  
 *   
 *  This program is distributed in the hope that it will be useful,  
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of  
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
 *  GNU General Public License for more details.  
 *   
 *  You should have received a copy of the GNU General Public License  
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
 */
     
// Put your libraries here (#include ...)

void setup() {
 USB.ON();
 
  USB.println(F("Power test"));

     PWR.setSensorPower(SENS_3V3,SENS_ON); // Sets the 3,3V switc
    RTC.ON();
  
  // Setting time [yy:mm:dd:dow:hh:mm:ss]
  RTC.setTime("12:07:17:02:17:25:00");

}


void loop() {

   PWR.setSensorPower(SENS_3V3,SENS_ON); // Sets the 3,3V switc
   

        // switches off depending on the option selected  
//	PWR.switchesOFF(ALL_OFF);

	
//	Wire.close();
/*        pinMode(I2C_SDA,OUTPUT);
	pinMode(I2C_SCL,OUTPUT);
	digitalWrite(I2C_SDA,LOW);
	digitalWrite(I2C_SCL,LOW);*/

//      customDeepSleep("00:00:01:00",RTC_OFFSET,RTC_ALM1_MODE1,ALL_OFF);
//      PWR.deepSleep("00:00:01:00",RTC_OFFSET,RTC_ALM1_MODE1,ALL_OFF);
      customDeepSleep2("00:00:01:00",RTC_OFFSET,RTC_ALM1_MODE1,ALL_OFF);


}

void customDeepSleep2(	const char* time2wake, 
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

void customDeepSleep(	const char* time2wake, 
							uint8_t offset, 
							uint8_t mode, 
							uint8_t option	)
{	
	uint8_t retries=0;
	  USB.println(F("DS!"));
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
        USB.println(F("read"));
	while(digitalRead(MUX_RX)==HIGH)
	{	
		// clear all detected interruption signals
		delay(1);
		PWR.clearInterruptionPin();
		retries++;
		if(retries>10)
		{
			return (void)0;
		}
	}

        USB.println(F("rtc ON"));
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
                USB.println(F("rtc err"));
		return (void)0;
	}

        USB.println(F("rtc OK"));
        RTC.OFF();                
        
        Wire.close();
        pinMode(I2C_SDA,OUTPUT);
	pinMode(I2C_SCL,OUTPUT);
	digitalWrite(I2C_SDA,LOW);
	digitalWrite(I2C_SCL,LOW);

        USB.println(F("rtc ON"));
        // switches off depending on the option selected  
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
