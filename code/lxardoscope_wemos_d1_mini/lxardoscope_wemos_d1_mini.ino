/*
 *
 * This file is a modification of the file that is part of the LXARDOSCOPE package.
 *
 * LXARDOSCOPE is an Arduino based oscilloscope for Linux, using the Xforms library.
 *
 * Copyright (C) 2013 Oskar Leuthold, Modified by Simon Bauer, 2023
 * 
 * LXARDOSCOPE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LXARDOSCOPE is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with LXARDOSCOPE; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 */
 
//   This program for the wemos d1 mini reads the one internal adc channel and sends the data
//   out through the serial port in 4 bytes.
//   
//   
//  
//
//
int sensorValue = 0;        // value read from the pot
byte lb;
byte hb;
boolean ac1=false;		// Channel 1 AC amplifier off
boolean ac2=false;		// Channel 2 AC amplifier off
boolean cal1=false;

void setup() {
  // initialize serial communications at 115200 bps:
  Serial.begin(115200);
}

void loop() {
//  if(transmit) {

	  
    sensorValue = (analogRead(A0)/2.046);
//  Read the internal adc, currently configured to say it is 5v if 1023 is read, modify the value 2.046 if you want it to be something else (display sceme is 500 is 5.00v as an example) 
    
    if(cal1)
    {sensorValue=0;                         
    cal1=false;}
//attempt at a calibration routine (we dont need this as we use the internal adc reference but the linux app wants a calibration so we give it one)
//        sensorValue = test;
// shift sample by 3 bits, and select higher byte  
      hb=highByte(sensorValue<<3); 
// set 3 most significant bits and send out
      Serial.write(hb|224); 
// select lower byte and clear 3 most significant bits
      lb=(lowByte(sensorValue))&31;
// set bits 5 and 6 and send out
      Serial.write(lb|96);
// 

      sensorValue = 0;
//make the other channel read 0 as there is only one channel to display anything
// shift sample by 3 bits, and select higher byte 
      hb=highByte(sensorValue<<3); 
// set bits 5 and 6 and send out
      Serial.write(hb|96); 
// select lower byte and clear 3 most significant bits
      lb=(lowByte(sensorValue))&31;
// set bits 5 and 6 and send out
      Serial.write(lb|96);
//      }
      if(Serial.available()) {
		char inChar = Serial.read();
		if (inChar == '1') {            // cal channel 1
			      // AC1 off
			cal1=true;
        }                
		}
 }

