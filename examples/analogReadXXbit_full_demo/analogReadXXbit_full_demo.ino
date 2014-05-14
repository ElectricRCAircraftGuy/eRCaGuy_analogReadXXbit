/*
analogReadXXbit_full_demo.ino
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
 Written: 7 May 2014
 Last Updated: 12 May 2014
 
 Version: 1.0 - first release
 
 History (newest on top):
 20140512 - first version created
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
const unsigned int num_samples = 10;
const uint8_t pin = A0; //analogRead pin
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

//Global variables
float analog_reading; //the ADC reading
unsigned long dt; //us; the total microseconds (delta time) required for the ADC readings
float V; //Voltage calculated on the analog pin
float freq; //Hz; data collection frequency


void setup() 
{
  Serial.begin(115200);
  Serial.println(F("Oversampling example to get 10-bit to 21-bit resolution using a 10-bit ADC on an Arduino"));
  Serial.println("Full demo");
  Serial.println(""); //add a line space
}


void loop() 
{
  //local variables
  uint8_t bits_of_precision; //bits of precision for the ADC (Analog to Digital Converter)
  unsigned long t0; //us; start time
  
  //take readings at 10-16-bit resolutions, & compare the results
  //Note: this library can handle up to 21-bit ADC resolutions when reading from an analog pin on the Arduino; 
  //however, I only do examples (shown below) for 10-16 bit resolutions
  
  //10-bit ADC reading
  bits_of_precision = 10; //bits of precision for the ADC (Analog to Digital Converter)
  t0 = micros(); //us; start time
  analog_reading = adc.analogReadXXbit(pin,bits_of_precision,num_samples); //get the avg. of [num_samples] 10-bit readings
  dt = micros() - t0; //us; the time this took
  V = analog_reading/MAX_READING_10_bit*5.0; //voltage
  freq = 1.0/(dt/1000000.0); //Hz; sample frequency (of obtaining the final, avg reading)
  //Print the results
  Serial.print("10-bit ADC avg = ");
  printData();
  
  //11-bit ADC reading
  bits_of_precision = 11; //bits of precision for the ADC (Analog to Digital Converter)
  t0 = micros(); //us; start time
  analog_reading = adc.analogReadXXbit(pin,bits_of_precision,num_samples); //get the avg. of [num_samples] 11-bit readings
  dt = micros() - t0; //us; the time this took
  V = analog_reading/MAX_READING_11_bit*5.0; //voltage
  freq = 1.0/(dt/1000000.0); //Hz; sample frequency (of obtaining the final, avg reading)
  //Print the results
  Serial.print("11-bit ADC avg = ");
  printData();
  
  //12-bit ADC reading
  bits_of_precision = 12; //bits of precision for the ADC (Analog to Digital Converter)
  t0 = micros(); //us; start time
  analog_reading = adc.analogReadXXbit(pin,bits_of_precision,num_samples); //get the avg. of [num_samples] 12-bit readings
  dt = micros() - t0; //us; the time this took
  V = analog_reading/MAX_READING_12_bit*5.0; //voltage
  freq = 1.0/(dt/1000000.0); //Hz; sample frequency (of obtaining the final, avg reading)
  //Print the results
  Serial.print("12-bit ADC avg = ");
  printData();
  
  //13-bit ADC reading
  bits_of_precision = 13; //bits of precision for the ADC (Analog to Digital Converter)
  t0 = micros(); //us; start time
  analog_reading = adc.analogReadXXbit(pin,bits_of_precision,num_samples); //get the avg. of [num_samples] 13-bit readings
  dt = micros() - t0; //us; the time this took
  V = analog_reading/MAX_READING_13_bit*5.0; //voltage
  freq = 1.0/(dt/1000000.0); //Hz; sample frequency (of obtaining the final, avg reading)
  //Print the results
  Serial.print("13-bit ADC avg = ");
  printData();
  
  //14-bit ADC reading
  bits_of_precision = 14; //bits of precision for the ADC (Analog to Digital Converter)
  t0 = micros(); //us; start time
  analog_reading = adc.analogReadXXbit(pin,bits_of_precision,num_samples); //get the avg. of [num_samples] 14-bit readings
  dt = micros() - t0; //us; the time this took
  V = analog_reading/MAX_READING_14_bit*5.0; //voltage
  freq = 1.0/(dt/1000000.0); //Hz; sample frequency (of obtaining the final, avg reading)
  //Print the results
  Serial.print("14-bit ADC avg = ");
  printData();
  
  //15-bit ADC reading
  bits_of_precision = 15; //bits of precision for the ADC (Analog to Digital Converter)
  t0 = micros(); //us; start time
  analog_reading = adc.analogReadXXbit(pin,bits_of_precision,num_samples); //get the avg. of [num_samples] 15-bit readings
  dt = micros() - t0; //us; the time this took
  V = analog_reading/MAX_READING_15_bit*5.0; //voltage
  freq = 1.0/(dt/1000000.0); //Hz; sample frequency (of obtaining the final, avg reading)
  //Print the results
  Serial.print("15-bit ADC avg = ");
  printData();
  
  //16-bit ADC reading
  bits_of_precision = 16; //bits of precision for the ADC (Analog to Digital Converter)
  t0 = micros(); //us; start time
  analog_reading = adc.analogReadXXbit(pin,bits_of_precision,num_samples); //get the avg. of [num_samples] 16-bit readings
  dt = micros() - t0; //us; the time this took
  V = analog_reading/MAX_READING_16_bit*5.0; //voltage
  freq = 1.0/(dt/1000000.0); //Hz; sample frequency (of obtaining the final, avg reading)
  //Print the results
  Serial.print("16-bit ADC avg = ");
  printData();

  //add in one more line space
  Serial.println("");
}


void printData()
{
  Serial.print(analog_reading);
  Serial.print(", dt(us) = ");
  Serial.print(dt);
  Serial.print(", sample freq (for ");
  Serial.print(num_samples);
  Serial.print(" readings) = ");
  Serial.print(freq);
  Serial.print(" Hz, sample freq (per indiv reading @ this resolution) = ");
  Serial.print(freq*num_samples);
  Serial.print(" Hz, V = ");
  Serial.println(V,8);
}


