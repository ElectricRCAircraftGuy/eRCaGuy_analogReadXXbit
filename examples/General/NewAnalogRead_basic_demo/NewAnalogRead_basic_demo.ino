/*
NewAnalogRead_basic_demo.ino
-This is a demo of this library: eRCaGuy_NewAnalogRead, which replaces eRCaGuy_analogReadXXbit
-Written for Library Version: 2.1
Library webpage: http://electricrcaircraftguy.blogspot.com/2014/05/using-arduino-unos-built-in-16-bit-adc.html
-This is a library to utilize oversampling, by the 10-bit Arduino ADC (Analog to Digital Converter), in
 order to obtain much higher resolutions.  You may command any resolution from 10-bit to 21-bit, though oversampling has
 its limiations and you may not see improvements beyond ~16-bit commands. There are a lot of variables, so you will
 just have to play around and find out. The tradeoff in commanded resolution is speed.  The higher the commanded resolution, 
 the longer it takes to get an ADC reading.  Each new bit of resolution decreases the max read speed by a factor of 4.  See the
 top of the .h file for much more info.
-Additionally, this library provides automatic averaging of multiple readings, at your desired
 resolution of course, in order to smooth incoming data.
-For the theory behind oversampling, see AVR121 Application Note: http://www.atmel.com/images/doc8003.pdf
--esp. take a look at Table 3-1, Resolution Options, on pg. 8
 
 By Gabriel Staples
 http://electricrcaircraftguy.blogspot.com/
 -My contact info is available by clicking the "Contact Me" tab at the top of my blog.
 Written: 17 Feb. 2015
 Last Updated: 17 Feb. 2015
*/

/*
===================================================================================================
  LICENSE & DISCLAIMER
  Copyright (C) 2015 Gabriel Staples.  All right reserved.
  
  ------------------------------------------------------------------------------------------------
  License: GNU Lesser General Public License Version 3 (LGPLv3) - https://www.gnu.org/licenses/lgpl.html
  ------------------------------------------------------------------------------------------------

  This file is part of eRCaGuy_NewAnalogRead.
  
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
#include <eRCaGuy_NewAnalogRead.h>

//Global variables
byte pin = A0;
byte bitsOfResolution = 14; //commanded oversampled resolution
unsigned long numSamplesToAvg = 1; //number of samples AT THE OVERSAMPLED RESOLUTION that you want to take and average
ADC_prescaler_t ADCSpeed = ADC_FAST;
/*Speed options to store into ADCSpeed are as follows:
  ADC_PRESCALER_128_CLOCK_125KHZ    
    ADC_DEFAULT (same as above)                       
    ADC_SLOW (same as above)                          
  ADC_PRESCALER_64_CLOCK_250KHZ     
  ADC_PRESCALER_32_CLOCK_500KHZ     
  ADC_PRESCALER_16_CLOCK_1MHZ       
    ADC_FAST (same as above)                          
  CAUTION_ADC_PRESCALER_8_CLOCK_2MHZ
  CAUTION_ADC_PRESCALER_4_CLOCK_4MHZ
  CAUTION_ADC_PRESCALER_2_CLOCK_8MHZ
NB: if you change the ADC clock speed, it doesn't just affect this library, it also affects the 
ADC sample rate when calling the standard core Arduino analogRead() function.
*/

void setup() 
{
  Serial.begin(115200);
  Serial.println(F("Oversampling example, commanding 10-bit to 21-bit resolution using a 10-bit ADC on an Arduino"));
  Serial.println(F("Basic demo"));
  Serial.println(""); //add a line space
  
  //print defaults
  Serial.println(F("Default adc values:"));
  printValues();
  
  //Configure the adc how you want it
  adc.setADCSpeed(ADC_FAST);
  adc.setBitsOfResolution(bitsOfResolution);
  adc.setNumSamplesToAvg(numSamplesToAvg);

  //Verify your changes
  Serial.println(F("New adc values:"));
  printValues();
}

void loop() 
{ 
  //local variables
  unsigned long analogReading;
  
  //take a reading on the analog pin
  //note: this is an overloaded function, so there are 4 ways to do it!
  //Method 1: just take a reading, providing the pin number only; the library will use other values as necessary 
  //          that you already set with the "adc.set..." functions above
  analogReading = adc.newAnalogRead(pin); //get the avg. of [num_samples] [bits_of_resolution]-bit readings 
  //Method 2: update what the library has stored for bitsOfResolution, AND take a new reading
//  analogReading = adc.newAnalogRead(pin,bitsOfResolution); //UNCOMMENT TO USE
  //Method 3: update what the library has stored for bitsOfResolution and numSamplesToAvg, AND take a new reading
//  analogReading = adc.newAnalogRead(pin,bitsOfResolution,numSamplesToAvg); //UNCOMMENT TO USE
  //Method 4: update what the library has stored for bitsOfResolution and numSamplesToAvg, then set the ADCSpeed, AND take a new reading
//  analogReading = adc.newAnalogRead(pin,bitsOfResolution,numSamplesToAvg,ADCSpeed); //UNCOMMENT TO USE
  
  //output results
  Serial.print("analogReading = "); Serial.print(analogReading);
  Serial.print(", Voltage (V) = "); Serial.println(5.0*analogReading/adc.getMaxPossibleReading(),5); //display up to 5 digits of precision
  
  //wait a bit before taking another reading, so as to not overburden the serial port
  delay(50);
}

//print adc settings
void printValues()
{
  Serial.print(F("bitsOfResolution = ")); Serial.println(adc.getBitsOfResolution());
  Serial.print(F("MaxPossibleReading at this res = ")); Serial.println(adc.getMaxPossibleReading());
  Serial.print(F("numSamplesToAvg = ")); Serial.println(adc.getNumSamplesToAvg());
  Serial.print(F("ADCSpeedSetting = ")); Serial.println(adc.getADCSpeedSetting());
  Serial.print(F("ADCClockFreq (Hz) = ")); Serial.println(adc.getADCClockFreq()); //Hz
  Serial.println(""); //new line
}
