/*
eRCaGuy_analogReadXXbit
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

#ifndef eRCaGuy_analogReadXXbit_h
#define eRCaGuy_analogReadXXbit_h

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

class eRCaGuy_analogReadXXbit
{
  public:
    //declare class constructor method
    eRCaGuy_analogReadXXbit();
	//declare other public class methods (functions)
	//function to read the ADC at a specified resolution; note: bits_of_precision must be between 10-21
	float analogReadXXbit(uint8_t analogPin, uint8_t bits_of_precision, unsigned long num_samples_to_avg); //standard method
	
  private:
	//declare private class methods (functions)
	//N/A
    //variables accessible by this class only
	//N/A
};

#endif

