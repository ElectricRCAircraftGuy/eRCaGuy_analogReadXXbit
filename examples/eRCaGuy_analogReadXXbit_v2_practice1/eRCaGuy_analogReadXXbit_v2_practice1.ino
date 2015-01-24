/*
analogReadXXbit_ultra_basic_demo.ino
-This is a demo of this library: eRCaGuy_analogReadXXbit
Library webpage: http://electricrcaircraftguy.blogspot.com/2014/05/using-arduino-unos-built-in-16-bit-adc.html
-This is a library to utilize oversampling, by the 10-bit Arduino ADC (Analog to Digital Converter), in
 order to obtain much higher resolutions.  Resolutions from 10-bit to 21-bit are now possible, for reading the ADCs on
 an Arduino!  The tradeoff is speed.  The higher the resolution, the longer it takes to get an ADC reading.
 Each new bit of resolution decreases the max read speed by a factor of 4.
 Additionally, this library provides automatic averaging of multiple readings, at your desired
 resolution.
-See AVR121 Application Note: http://www.atmel.com/images/doc8003.pdf
--esp. take a look at Table 3-1, Resolution Options, on pg. 8
 
 By Gabriel Staples
 http://electricrcaircraftguy.blogspot.com/
 -My contact info is available by clicking the "Contact Me" tab at the top of my blog.
 Written: 24 Jan 2015
 Last Updated: 24 Jan 2015
 
 Library Version: 2.0
*/

/*
===================================================================================================
  LICENSE & DISCLAIMER
  Copyright (C) 2015 Gabriel Staples.  All right reserved.
  
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

void setup() 
{
  Serial.begin(115200);
  Serial.println(F("Oversampling example to get 10-bit to 21-bit resolution using a 10-bit ADC on an Arduino"));
  Serial.println(F("Basic demo"));
  Serial.println(""); //add a line space
  
  Serial.println(adc.getBitsOfResolution());
  Serial.println(adc.getNumSamplesToAvg());
  Serial.println(adc.getMaxPossibleReading());
  Serial.println(adc.getADCSpeedSetting());
  Serial.println(adc.getADCClockFreq()); //Hz
//  adc.setADCSpeed(ADC_PRESCALER_16_CLOCK_1MHZ);
  adc.setADCSpeed(ADC_FAST);
//  adc.setADCSpeed(CAUTION_ADC_PRESCALER_4_CLOCK_4MHZ); //still yields good results
//  adc.setADCSpeed(CAUTION_ADC_PRESCALER_2_CLOCK_8MHZ); //too fast, yields very poor results
  Serial.println(adc.getADCSpeedSetting());
  Serial.println(adc.getADCClockFreq()); //Hz
  Serial.println();
}

void loop() 
{
  //local variables
  byte pin = A0; //analogRead pin
  byte desiredBitsOfResolution = 21; //must be a value between 10 and 21, or else your input will be constrained to within these bounds
                              //Ex: if you want 14-bits of resolution, change "bits_of_resolution" parameter to 14
  unsigned int numSamplesToAvg = 1; //leave this at 1 to take only a single reading; make it >1 to return an avg. of this # of readings
  
  //take a reading on pin A0
  unsigned long analogReading = adc.newAnalogRead(pin,desiredBitsOfResolution,numSamplesToAvg); //get the avg. of [num_samples] [bits_of_resolution]-bit readings 
  
  //output results
  Serial.println("");
  Serial.print("analogReading = "); 
  Serial.println(analogReading);
  Serial.print("Voltage = ");
  Serial.print(5.0*analogReading/adc.getMaxPossibleReading(),5); //display up to 5 digits of precision
  Serial.println("V");
  Serial.print("bits = "); Serial.println(adc.getBitsOfResolution());
  
  //wait a bit before taking another reading
  delay(500);
}


