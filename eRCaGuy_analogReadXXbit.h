/*
eRCaGuy_analogReadXXbit
Written and Edited in Notepad++
Library webpage: http://electricrcaircraftguy.blogspot.com/2014/05/using-arduino-unos-built-in-16-bit-adc.html
-This is a library to utilize oversampling, by the 10-bit Arduino ADC (Analog to Digital Converter), in
 order to obtain much higher resolutions.  Resolutions from 10-bit to 21-bit are now possible, for reading the ADCs on
 an Arduino!  The tradeoff is speed.  The higher the resolution, the longer it takes to get an ADC reading.
 Each new bit of precision decreases the max read speed by a factor of 4.
 Additionally, this library provides automatic averaging of multiple samples, at your desired
 resolution.
-See AVR121 Application Note: http://www.atmel.com/images/doc8003.pdf
--esp. take a look at Table 3-1, Resolution Options, on pg. 8
 
 By Gabriel Staples
 http://electricrcaircraftguy.blogspot.com/
 -My contact info is available by clicking the "Contact Me" tab at the top of my blog.
 Written: 7 May 2014
 Last Updated: 23 Jan 2015
 
 Version History* (newest on top)
 V2.0 - 2nd release, major overhaul & addition of functionality
 V1.0 - first release, 20140512
 
 History (newest on top):
 20150123 - 2nd version created, V2.0; added a bunch of new features and functions
 20140512 - first version created, V1.0
 
 *Version Number Description:
  Format: A.B.C
  major.minor.subminor
  Ex: V2.0.1
  note: V means "version"
 Description:
   A = major version number 
	 -this number increments for major changes that significantly change the functionality of the library (ex: adding many new features), or that require significant modifications to your code to still be compatible with the library (ex: I change function names)
	 B = minor version number
	 -this number increments if I add some additional functionality to the code, or fundamentally change the ways that existing functions work, without requiring significant changes to your code to still use these functions
	 C = subminor version number 
	 -this number increments for minor code changes or additions, slight modifications to functions, variables, constants, etc
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

#ifndef eRCaGuy_analogReadXXbit_h
#define eRCaGuy_analogReadXXbit_h

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

//Macros
//clear all 3 ADPS bits; see datasheet pg 255-256
#define clearADPSbits()	ADCSRA &= ~(_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0))

//ADC speed constants
/*NOTES TO SELF:
-The below values are used to set the ADC prescaler, which in turn sets the ADC clock frequency, which determines the max samples/sec.  Since a normal ADC conversion (sample) takes 13 ADC clock cycles (see datasheet pg 245, Section 24.4), you can calculate the max theoretical sample rate as max_sample_freq = ADC_clock_freq/13.
-CAUTION: datasheet pg. 317 recommends max ADC clock frequency of 1Mhz, so I do not know whether higher frequencies could damage the ADC or not
-ADC references:
 -Simon Monk's book "Programming Arduino Next Steps" pgs. 81-83 (http://www.amazon.com/Programming-Arduino-Next-Steps-Sketches/dp/0071830251) 
 -datasheet Section 24 (Analog-to-Digital Converter, pgs. 242-258) & Table 29-20 (ADC Characteristics, pg. 317)
 -http://forum.arduino.cc/index.php?topic=6549.0
 -http://forum.arduino.cc/index.php?topic=8795.0
 -http://forum.arduino.cc/index.php/topic,7762.0.html
 -http://gabuku.com/scope/
 --http://gabuku.com/scope/scope.swf
*/
typedef enum
{																			//Max theoretical
																			//sample rate (Hz)
																			//(ADC_clock_freq/13)
	ADC_PRESCALER_128_CLOCK_125KHZ 			= 0,	//~9.6kHz (default Arduino ADC clock freq)
	ADC_DEFAULT														= 0,//alternate name for the above option
	ADC_PRESCALER_64_CLOCK_250KHZ 			= 1,	//~19.2kHz
	ADC_PRESCALER_32_CLOCK_500KHZ 			= 2,	//~38.5kHz
	ADC_PRESCALER_16_CLOCK_1MHZ 				= 3,	//~76.9kHz (max recommended ADC clock freq in datasheet)
	ADC_FAST															= 3,//alternate name for the above option
	CAUTION_ADC_PRESCALER_8_CLOCK_2MHZ	= 4,	//~154kHz (CAUTION: this ADC clock freq is higher than what the datasheet recommends)
	CAUTION_ADC_PRESCALER_4_CLOCK_4MHZ	= 5,	//~308kHz (CAUTION: this ADC clock freq is higher than what the datasheet recommends)
	CAUTION_ADC_PRESCALER_2_CLOCK_8MHZ	= 6,	//~615kHz (CAUTION: this ADC clock freq is higher than what the datasheet recommends)
} ADC_prescaler_t;

class eRCaGuy_analogReadXXbit
{
  public: //public API
		//class constructor method
    eRCaGuy_analogReadXXbit();
		//function to read the ADC at a specified resolution; note: bits_of_resolution must be between 10-21
		//I will overload this function by defining various numbers of input parameters, as shown below:
		float newAnalogRead(uint8_t analogPin, uint8_t bits_of_resolution, unsigned long num_samples_to_avg, ADC_prescaler_t ADC_speed); 
		float newAnalogRead(uint8_t analogPin, uint8_t bits_of_resolution, unsigned long num_samples_to_avg);
		float newAnalogRead(uint8_t analogPin, uint8_t bits_of_resolution);
		float newAnalogRead(uint8_t analogPin);
		//set functions
		void setBitsOfResolution(uint8_t bits_of_resolution);
		void setNumSamplesToAvg(unsigned long num_samples_to_avg);
		void setADCSpeed(ADC_prescaler_t ADC_speed);
		//get functions
		uint8_t getBitsOfResolution();
		unsigned long getNumSamplesToAvg();
		uint8_t getADCSpeedSetting();
		unsigned long getADCClockFreq(); //Hz
		unsigned long getMaxPossibleReading();
	
  private: //accessible by this class only
		//methods:
		float takeSamples(uint8_t analogPin);
		void updateADCSpeed();
		
		//variables:
		uint8_t _bits_of_resolution = 12;
		unsigned long _num_samples_to_avg = 1;
		ADC_prescaler_t _ADC_speed = ADC_PRESCALER_128_CLOCK_125KHZ; //use the Arduino default ADC clock speed
		unsigned long _ADCClockFreq; //Hz
};

extern eRCaGuy_analogReadXXbit adc; //preinstantiated object
#endif

