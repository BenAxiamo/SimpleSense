/*
 *  ------ [Ut_1] Waspmote Using EEPROM Example --------
 *
 *  Explanation: This example shows how to use the EEPROM memory of Waspmote
 *
 *  Copyright (C) 2012 Libelium Comunicaciones Distribuidas S.L. 
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
 *  
 *  Version:           0.1 
 *  Design:            David Gascón 
 *  Implementation:    Marcos Yarza
 */

// Variables

// address in EEPROM
int address = 1024; 
// value to write
int value =2;  
int aux = 0;
 
 
void setup()
{
  // Init USB
  USB.ON();
  // Writing in the EEPROM
  Utils.writeEEPROM(address,value);
  aux = Utils.readEEPROM(address);
  USB.print(F("Address EEPROM:  "));
  USB.print(address,DEC);
  USB.print(F(" -- Value: "));
  USB.println(aux,DEC);
}

void loop()
{


}

