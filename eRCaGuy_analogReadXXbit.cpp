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
 Last Updated: 25 May 2014
 
 Version: 1.0 - first release
 
 History (newest on top):
 20140525 - added in integer rounding, instead of doing a pure bit-shift operation to get the oversampled value; see rounding notes at the very bottom of this file
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

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include "eRCaGuy_analogReadXXbit.h"

//define class constructor method
eRCaGuy_analogReadXXbit::eRCaGuy_analogReadXXbit()
{
}

//define other class methods (functions)

/*First, a few notes about doing a power function (ex: 4^1, 4^2, 4^3, etc):
There are 3 ways I know how to raise 4 to a power:
1) Use the C power function: pow(4.0,power)
2) Use bit-shifting: 1<<(2*power)
3) Use a for loop, where you repeatedly multiply 4 by itself, the correct # of iterations

I first believed that method 2 above would be best, but I wanted to try method 1 anyway, since it's more readable.  Here's what I found:
I tried: 
Method 1) [FAILED]
unsigned long oversample_num = pow(4.0,bits_of_precision-10.0);
-However, this did NOT work, as it truncates floats when casting to unsigned long: ex: 3.99999999 --> 3, 15.9999999 --> 15.
-Therefore, when I ran my library I was getting 4^1 = 3 (instead of 4), 4^2 = 15 (instead of 16), 4^3 = 63, 4^4 = 255, etc.
-FOR MORE EXPLANATION OF THE ABOVE PROBLEM WITH THE POW() FUNCTION ON THE ARDUINO, SEE HERE:   
 http://stackoverflow.com/questions/20233454/arduino-odd-exponent-behavior-with-powx-y-function; 
-Also see here: http://forum.arduino.cc/index.php/topic,3056.0.html 
I then tried:
Method 1) [WORKED!]
unsigned long oversample_num = round(pow(4.0,bits_of_precision-10.0));
-This worked perfectly! Note to self: don't forget to use round() when going from floats to integer data types, as otherwise it will truncate; ex: 3.99999 --> 3, rather than 4, unless round() is used before the cast from float to the integer data type.  
-However, it was slow.  The round() function seemed to slow the code down enough to visibly reduce my max
 sampling rates, and the pow() function is already much more complicated & slow than is necessary anyway, as its input parameters are
 both floats, and it outputs a double (in standard C, though in Arduino it outputs a float).
--FYI: round() seems to be a rather slow function, decreasing the overall speed of analogReadXXbit by ~4% or so, so definitely use the most complicated, but very fast, bit-shift method instead!
Method 2) [WORKS PERFECTLY, & OF COURSE IS FAST SINCE IT'S A SIMPLE BIT-SHIFT!]
unsigned long oversample_num = 1<<(2*(bits_of_precision-10));
Method 3) [I didn't even try it; no need since Method 2 works great]
*/

//generic function for 10-21 bit resolution analogReads
//the max resolution is 21-bits, because at that point, the number of samples required for a single reading will be so high that it
//is possible for the "inner_sum" variable to overflow.
float eRCaGuy_analogReadXXbit::analogReadXXbit(uint8_t analogPin, uint8_t bits_of_precision, unsigned long num_samples_to_avg)
{
  //unsigned long oversample_num = round(pow(4.0,bits_of_precision-10.0)); //note: an alternate method of getting (4^n), or "pow(4,n)"
																		   //is (1<<(2*n)), which is much faster, so I am using it instead
  uint8_t n = bits_of_precision - 10; //"rightshift" value, AKA: "n"
									  //see AVR121 Application Note: http://www.atmel.com/images/doc8003.pdf
									  //Also see my table here: http://electricrcaircraftguy.blogspot.com/2014/05/using-arduino-unos-built-in-16-bit-adc.html
  unsigned long oversample_num = 1<<(2*n); //4^n; best & fastest method to do 4 to a power (see my extensive notes in the paragraph above)
  uint8_t divisor = 1<<n; //same thing as 2^n
  
  //////////////FOR DEBUGGING/////////////
  //Serial.println(oversample_num);
  //Serial.println(n);
  ////////////////////////////////////////
  
  //outer loop: get the number of samples to avg
  unsigned long reading_sum = 0;
  for (unsigned long i=0; i<num_samples_to_avg; i++)
  {
    //inner loop: do oversampling, per AVR121 Application Note, in order to enhance resolution of 10-bit ADC
    unsigned long inner_sum = 0;
    for (unsigned long j=0; j<oversample_num; j++)
    {
      inner_sum += analogRead(analogPin); //take a 10-bit reading on the Arduino ADC
    }
    //Convert these many 10-bit samples to a single higher-resolution sample:
	//Standard Method:
	//unsigned int reading = inner_sum >> n; //See AVR121 Application Note
	//Rounding Method (to nearest integer):
	unsigned int reading = (inner_sum + (unsigned long)divisor/2UL) >> n; //See AVR121 Application Note; this converts the analogRead into the higher-bit resolution reading. Note, however, that I am also doing integer rounding. For rounding details, see my rounding notes in the paragraph below.  Also note that ((inner_sum + divisor/2) >> n) is the same thing as ((inner_sum + divisor/2)/divisor), where divisor = 2^n.
    reading_sum += reading;
  }
  float avg_reading = (float)reading_sum/(float)num_samples_to_avg;
  return avg_reading;
}

/*Integer math rounding notes:
To do rounding with integers, during division, use the following formula:
(dividend + divisor/2)/divisor.

For example, instead of doing a/b, doing (a + b/2)/b will give you the integer value of a/b, rounded to the nearest whole integer.  This only works perfectly for even values of b.  If b is odd, the rounding is imperfect, since b/2 will not yield a whole number.  

Don't believe me that this works?
Consider the following examples:

a = 1723; b = 16
a/b = 107.6875 --> truncated to 107
(a + b/2)/b = 108.1875 --> truncated to 108, which is the same thing as a/b rounded to the nearest whole integer

a = 1720; b = 16
a/b = 107.5 --> truncated to 107
(a + b/2)/b = 108 exactly, which is the same thing as a/b rounded to the nearest whole integer

a = 1719; b = 16
a/b = 107.4375 --> truncated to 107
(a + b/2)/b = 107.9375 --> truncated to 107, which is the same thing as a/b rounded to the nearest whole integer

Why does this work?
If you do the algebra, you will see that doing (a + b/2)/b is the same thing as doing a/b + 1/2, which will always force a value, when truncated, to truncate to the value that it otherwise would have rounded to.  So, this works perfectly!  The only problem is that 1/2 is not a valid integer (it truncates to 0), so you must instead do it in the order of (a + b/2)/b, in order to make it all work out!
*/

