/*
eRCaGuy_analogReadXXbit
Written and Edited in Notepad++ (Tab settings: tab size 2, replace by space)
Library webpage: http://electricrcaircraftguy.blogspot.com/2014/05/using-arduino-unos-built-in-16-bit-adc.html
 
By Gabriel Staples
http://electricrcaircraftguy.blogspot.com/
-My contact info is available by clicking the "Contact Me" tab at the top of my blog.
 
**********************************************************************************************
***************SEE THE .H FILE FOR ADDITIONAL INFORMATION, VERSION HISTORY, ETC***************
**********************************************************************************************
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

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include "eRCaGuy_analogReadXXbit.h"

eRCaGuy_analogReadXXbit adc; //preinstantiation of object

//define class constructor method
eRCaGuy_analogReadXXbit::eRCaGuy_analogReadXXbit()
{
  updateADCSpeed(); //actually set the ADC speed via this private method
}

void eRCaGuy_analogReadXXbit::updateADCSpeed()
{
  switch (_ADC_speed) 
  {
    //Note: in order to set the ADC clock speed, you must properly configure the 3 ADPS bits, per the datasheet pg. 255-256
    case ADC_PRESCALER_128_CLOCK_125KHZ:
      _ADCClockFreq = F_CPU/128; //calc ADC clock freq
      ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); //set all 3 ADPS bits (ADPS2,ADPS1,ADPS0) (111)
      break;
    case ADC_PRESCALER_64_CLOCK_250KHZ:
      _ADCClockFreq =  F_CPU/64;
      clearADPSbits(); //clear all 3 ADPS bits
      ADCSRA |= _BV(ADPS2) | _BV(ADPS1); //set the proper bits (110)
      break;
    case ADC_PRESCALER_32_CLOCK_500KHZ:
      _ADCClockFreq =  F_CPU/32;
      clearADPSbits(); //clear all 3 ADPS bits
      ADCSRA |= _BV(ADPS2) | _BV(ADPS0); //set the proper bits (101)
      break;
    case ADC_PRESCALER_16_CLOCK_1MHZ:
      _ADCClockFreq =  F_CPU/16;
      clearADPSbits(); //clear all 3 ADPS bits
      ADCSRA |= _BV(ADPS2); //set the proper bits (100)
      break;
    case CAUTION_ADC_PRESCALER_8_CLOCK_2MHZ:
      _ADCClockFreq =  F_CPU/8;
      clearADPSbits(); //clear all 3 ADPS bits
      ADCSRA |= _BV(ADPS1) | _BV(ADPS0); //set the proper bits (011)
      break;
    case CAUTION_ADC_PRESCALER_4_CLOCK_4MHZ:
      _ADCClockFreq =  F_CPU/4;
      clearADPSbits(); //clear all 3 ADPS bits
      ADCSRA |= _BV(ADPS1); //set the proper bits (010)
      break;
    case CAUTION_ADC_PRESCALER_2_CLOCK_8MHZ:
      _ADCClockFreq =  F_CPU/2;
      clearADPSbits(); //clear all 3 ADPS bits
      ADCSRA |= _BV(ADPS0); //set the proper bits (001)
      break;
  }
}

//define other class methods (functions)

//NOTES TO SELF:
/*First, a few notes about doing a power function (ex: 4^1, 4^2, 4^3, etc):
There are 3 ways I know how to raise 4 to a power:
1) Use the C power function: pow(4.0,power)
2) Use bit-shifting: 1<<(2*power)
3) Use a for loop, where you repeatedly multiply 4 by itself, the correct # of iterations

I first believed that method 2 above would be best, but I wanted to try method 1 anyway, since it's more readable.  Here's what I found:
I tried: 
Method 1) [FAILED]
unsigned long oversample_num = pow(4.0,bits_of_resolution-10.0);
-However, this did NOT work, as it truncates floats when casting to unsigned long: ex: 3.99999999 --> 3, 15.9999999 --> 15.
-Therefore, when I ran my library I was getting 4^1 = 3 (instead of 4), 4^2 = 15 (instead of 16), 4^3 = 63, 4^4 = 255, etc.
-FOR MORE EXPLANATION OF THE ABOVE PROBLEM WITH THE POW() FUNCTION ON THE ARDUINO, SEE HERE:   
 http://stackoverflow.com/questions/20233454/arduino-odd-exponent-behavior-with-powx-y-function; 
-Also see here: http://forum.arduino.cc/index.php/topic,3056.0.html 
I then tried:
Method 1) [WORKED!]
unsigned long oversample_num = round(pow(4.0,bits_of_resolution-10.0));
-This worked perfectly! Note to self: don't forget to use round() when going from floats to integer data types, as otherwise it will truncate; ex: 3.99999 --> 3, rather than 4, unless round() is used before the cast from float to the integer data type.  
-However, it was slow.  The round() function seemed to slow the code down enough to visibly reduce my max
 sampling rates, and the pow() function is already much more complicated & slow than is necessary anyway, as its input parameters are
 both floats, and it outputs a double (in standard C, though in Arduino it outputs a float).
--FYI: round() seems to be a rather slow function, decreasing the overall speed of newAnalogRead by ~4% or so, so definitely use the most complicated, but very fast, bit-shift method instead!
Method 2) [WORKS PERFECTLY, & OF COURSE IS FAST SINCE IT'S A SIMPLE BIT-SHIFT!]
unsigned long oversample_num = 1<<(2*(bits_of_resolution-10));
Method 3) [I didn't even try it; no need since Method 2 works great]
*/

//generic function for 10-21 bit resolution analogReads
//the max resolution is 21-bits, because at that point, the number of samples required for a single reading will be so high that it
//is possible for the "inner_sum" variable to overflow.
float eRCaGuy_analogReadXXbit::takeSamples(uint8_t analogPin)
{
  //unsigned long oversample_num = round(pow(4.0,bits_of_resolution-10.0)); //note: an alternate method of getting (4^n), or "pow(4,n)"
                                                                            //is (1<<(2*n)), which is much faster, so I am using it instead
  uint8_t n = _bits_of_resolution - 10; //"rightshift" value, AKA: "n"
                                        //see AVR121 Application Note: http://www.atmel.com/images/doc8003.pdf
                                        //Also see my table here: http://electricrcaircraftguy.blogspot.com/2014/05/using-arduino-unos-built-in-16-bit-adc.html
  unsigned long oversample_num = 1UL<<(2*n); //4^n; best & fastest method to do 4 to a power (see my extensive notes in the paragraph above)
                                             //NB: be sure to use a 1UL value vs a 1byte value, or else the 1 will be shifted right off of the value, to the left, and you'll be left with oversample_num = 0 for any n>=8 (for some reason).
  unsigned int divisor = 1<<n; //same thing as 2^n
  
  //////////////FOR DEBUGGING/////////////
  Serial.print(F("n = ")); Serial.println(n);
  Serial.print(F("oversample_num = ")); Serial.println(oversample_num);
  Serial.print(F("divisor = ")); Serial.println(divisor);
  ////////////////////////////////////////
  
  //outer loop: get the number of samples to avg
  unsigned long reading_sum = 0;
  for (unsigned long i=0; i<_num_samples_to_avg; i++)
  {
    //inner loop: do oversampling, per AVR121 Application Note, in order to enhance resolution of 10-bit ADC
    unsigned long inner_sum = 0;
    for (unsigned long j=0; j<oversample_num; j++)
    {
      inner_sum += analogRead(analogPin); //take a 10-bit reading on the Arduino ADC
        //NB: I JUST DISCOVERED THAT I CAN ***NOT*** RE-USE THE ARDUINO CORE FUNCTION NAME "analogRead()" AS PART OF MY LIBRARY!!! [IE: "eRCaGuy_analogReadXXbit::analogRead(...)" IS BAD!!!]. THE REASON IS BECAUSE IF I DO THIS, THE LINE ABOVE WILL RECURSIVELY CALL MY *LIBRARY'S* analogRead() FUNCTION RATHER THAN THE *CORE* ARDUINO analogRead() FUNCTION, THEREBY GETTING STUCK IN AN INFINITELY RECURSIVE LOOP AND CRASHING THE ARDUINO!!!
    }
    //Convert these many 10-bit samples to a single higher-resolution sample:
    //Standard Method:
    //unsigned int reading = inner_sum >> n; //See AVR121 Application Note
    //Rounding Method (to nearest integer):
    unsigned long reading = (inner_sum + (unsigned long)divisor/2UL) >> n; //See AVR121 Application Note; this converts the analogRead into the higher-bit resolution reading. Note, however, that I am also doing integer rounding. For rounding details, see my rounding notes in the paragraph below.  Also note that ((inner_sum + divisor/2) >> n) is the same thing as ((inner_sum + divisor/2)/divisor), where divisor = 2^n.
    reading_sum += reading;
  }
  float avg_reading = (float)reading_sum/(float)_num_samples_to_avg;
  return avg_reading;
}

/*Integer math rounding notes:
To do rounding with integers, during division, use the following formula:
(dividend + divisor/2)/divisor.

For example, instead of doing a/b, doing (a + b/2)/b will give you the integer value of a/b, rounded to the nearest whole integer.  This only works perfectly for even values of b.  If b is odd, the rounding is imperfect, since b/2 will not yield a whole number.  

Examples:

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

float eRCaGuy_analogReadXXbit::newAnalogRead(uint8_t analogPin, uint8_t bits_of_resolution, unsigned long num_samples_to_avg, ADC_prescaler_t ADC_speed)
{
  _bits_of_resolution = constrain(bits_of_resolution,10,21);
  _num_samples_to_avg = num_samples_to_avg;
  if (_ADC_speed != ADC_speed)
  {
    _ADC_speed = ADC_speed;
    updateADCSpeed();
  }
  return takeSamples(analogPin);
}

float eRCaGuy_analogReadXXbit::newAnalogRead(uint8_t analogPin, uint8_t bits_of_resolution, unsigned long num_samples_to_avg)
{
  _bits_of_resolution = constrain(bits_of_resolution,10,21);
  _num_samples_to_avg = num_samples_to_avg;
  return takeSamples(analogPin);
}

float eRCaGuy_analogReadXXbit::newAnalogRead(uint8_t analogPin, uint8_t bits_of_resolution)
{
  _bits_of_resolution = constrain(bits_of_resolution,10,21);
  return takeSamples(analogPin);
}

float eRCaGuy_analogReadXXbit::newAnalogRead(uint8_t analogPin)
{
  return takeSamples(analogPin);
}

//"set" functions

void eRCaGuy_analogReadXXbit::setBitsOfResolution(uint8_t bits_of_resolution)
{
  _bits_of_resolution = constrain(bits_of_resolution,10,21);
}

void eRCaGuy_analogReadXXbit::setNumSamplesToAvg(unsigned long num_samples_to_avg)
{
  _num_samples_to_avg = num_samples_to_avg;
}

void eRCaGuy_analogReadXXbit::setADCSpeed(ADC_prescaler_t ADC_speed)
{
  if (_ADC_speed != ADC_speed)
  {
    _ADC_speed = ADC_speed;
    updateADCSpeed();
  }
}

//"get" functions

uint8_t eRCaGuy_analogReadXXbit::getBitsOfResolution()
{
  return _bits_of_resolution;
}

unsigned long eRCaGuy_analogReadXXbit::getNumSamplesToAvg()
{
  return _num_samples_to_avg;
}

uint8_t eRCaGuy_analogReadXXbit::getADCSpeedSetting()
{
  return (uint8_t)_ADC_speed;
}

unsigned long eRCaGuy_analogReadXXbit::getADCClockFreq() //Hz
{
  return _ADCClockFreq;
}

unsigned long eRCaGuy_analogReadXXbit::getMaxPossibleReading()
{
  return 1023UL*(unsigned long)(1<<(_bits_of_resolution - 10)); //maxPossibleReading = 1023*2^n, where n is the *additional* bits of resolution, beyond 10-bits; ie: maxPossibleReading = 1023*2^(_bits_of_resolution-10)
}



