/*
analogReadXXbit_ultra_basic_demo.ino
-This is a demo of this library: eRCaGuy_analogReadXXbit
Library webpage: http://electricrcaircraftguy.blogspot.com/2014/05/using-arduino-unos-built-in-16-bit-adc.html
-This is a library to utilize oversampling, by the 10-bit Arduino ADC (Analog to Digital Converter), in
 order to obtain much higher resolutions.  Resolutions from 10-bit to 21-bit are now possible, for reading the ADCs on
 an Arduino!  The tradeoff is speed.  The higher the resolution, the longer it takes to get an ADC reading.
 Each new bit of precision decreases the max read speed by a factor of 4.
 Additionally, this library provides automatic averaging of multiple readings, at your desired
 resolution.
-See AVR121 Application Note: http://www.atmel.com/images/doc8003.pdf
--esp. take a look at Table 3-1, Resolution Options, on pg. 8
 
 By Gabriel Staples
 http://electricrcaircraftguy.blogspot.com/
 -My contact info is available by clicking the "Contact Me" tab at the top of my blog.
 Written: 12 May 2014
 Last Updated: 13 May 2014
 
 Version: 1.0 - first release
 
 History (newest on top):
 20140513 - first version created
*/

/*
===================================================================================================
  LICENSE & DISCLAIMER
  Copyright (C) 2014 Gabriel Staples.  All right reserved.
  
  ------------------------------------------------------------------------------------------------
  License: GNU Lesser General Public License Version 3 (LGPLv3) - https://www.gnu.org/licenses/lgpl.html
  ------------------------------------------------------------------------------------------------

  This file is part of eRCaGuy_analogReadXXbit.
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see http://www.gnu.org/licenses/
===================================================================================================
*/

/*
Circuit:
We need to read an analog voltage on A0, so place a pot with the outer legs going to 5V and GND, respectively, and the wiper (middle leg) going to A0
-make sure to set your Serial Monitor to 115200 baud rate
*/

//include the library
#include <eRCaGuy_analogReadXXbit.h>

//instantiate an object of this library class; call it "adc"
eRCaGuy_analogReadXXbit adc;

//Global constants
//constants required to determine the voltage at the pin
const float MAX_READING_10_bit = 1023.0;
const float MAX_READING_11_bit = 2046.0;
const float MAX_READING_12_bit = 4092.0;
const float MAX_READING_13_bit = 8184.0;
const float MAX_READING_14_bit = 16368.0;
const float MAX_READING_15_bit = 32736.0;
const float MAX_READING_16_bit = 65472.0;
const float MAX_READING_17_bit = 130944.0;
const float MAX_READING_18_bit = 261888.0;
const float MAX_READING_19_bit = 523776.0;
const float MAX_READING_20_bit = 1047552.0;
const float MAX_READING_21_bit = 2095104.0;

void setup() 
{
  Serial.begin(115200);
  Serial.println(F("Oversampling example to get 10-bit to 21-bit resolution using a 10-bit ADC on an Arduino"));
  Serial.println("Ultra Basic demo");
  Serial.println(""); //add a line space
}

void loop() 
{
  //local variables
  int pin = A0; //analogRead pin
  int bits_of_precision = 16; //must be a value between 10 and 21
  int num_samples = 1;
  
  //take a reading on pin A0
  float analog_reading = adc.analogReadXXbit(pin,bits_of_precision,num_samples); //get the avg. of [num_samples] 16-bit readings 
  
  //output results
  Serial.print("analog_reading = ");
  Serial.println(analog_reading);
  Serial.print("Voltage = ");
  Serial.print(5.0*analog_reading/MAX_READING_16_bit,5); //display up to 5 digits of precision
  Serial.println("V");
  Serial.println("");
  
  //wait a bit before taking another reading
  delay(500);
}


