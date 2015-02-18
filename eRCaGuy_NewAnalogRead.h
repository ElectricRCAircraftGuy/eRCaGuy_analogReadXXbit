/*
eRCaGuy_NewAnalogRead (a replacement and continuation of eRCaGuy_analogReadXXbit)
Written and Edited in Notepad++ (Tab settings: tab size 2, replace by space)
Library webpage: http://electricrcaircraftguy.blogspot.com/2014/05/using-arduino-unos-built-in-16-bit-adc.html /////////////////////

By Gabriel Staples
http://electricrcaircraftguy.blogspot.com/
-My contact info is available by clicking the "Contact Me" tab at the top of my blog.
Written: 7 May 2014
Last Updated: 11 Feb 2015
*/

/*
Current Version: V2.1 alpha

Version History* (newest on top)
V2.1 alpha - class name change, comments & descriptions significantly improved
V2.0 alpha - 2nd release, major overhaul & addition of functionality
V1.0 - first release, 20140512

Edit History (newest on top):
20150212 - greater comments/explanation at top; moved private member variable assignments from .h to .cpp file
20150201 - changed function names
20150123 - 2nd version created, V2.0; added a bunch of new features and functions
20140512 - first version created, V1.0

*Version Number Description:
 Format: A.B.C
 major.minor.subminor
 Ex: V2.0.1
 note: V means "version"
Description:
  A = major version number 
  -this number increments for major changes that significantly change the functionality of the library (ex: adding many new features), or that require significant modifications to your code to still be compatible with the library (ex: I change many function names, or change function parameters)
  B = minor version number
  -this number increments if I add some additional functionality to the code, have major bug fixes, or fundamentally change the ways that existing functions work, without requiring significant changes to your code to still use these functions (ex: I change some function names)
  C = subminor version number 
  -this number increments for minor code changes or additions, minor bug fixes, slight modifications to functions, variables, constants, etc
*/

/*
Brief Library Description:
 -This library utilizes oversampling to achieve an ADC resolution of 12~16-bits or more. The tradeoff is sampling speed.  Higher commanded resolutions are slower. Sampling speed, however, can also be increased with this library, to substantially increase the max ADC sample rate.

Long Library Description:
 -This is a library to utilize oversampling, by the 10-bit Arduino ADC (Analog to Digital Converter), in order to obtain much higher resolutions. Each 10-bit ADC sample is acquired by calling the core Arduino function "analogRead()". Multiple 10-bit samples are rapidly acquired in succession in order to obtain higher resolutions via the oversampling technique outlined in the AVR121 Application Note (see link below). *Theoretical* resolutions from 10-bit to 21-bit are now possible, for reading the ADCs on an Arduino!  The *practical* resolution limit achievable via oversampling on an Arduino, however, may be only ~16-bit or so, though this may vary.  Actual resolutions achieved via oversampling can vary, and some level of natural noise is required on your analog sensor pin in order for oversampling to be effective (see AVR121 Application Note, link below). It is up to ***you***, as the user, to determine the practical limitations of this library for your particular situation. Though you may command an oversampled resolution of 21-bit, for instance, the values you read in may actually increment in such a way that they have 17-bit steps. This would indicate that the max actual resolution achievable for *your particular* test setup, and analog sensor used, is 17-bits. Other sensors you may use could achieve different results.  Again, the *actual* analog resolution you achieve with this library may *not* be what you command as the *desired, or theoretical* resolution.  
 
 Note also the tradeoff between commanded resolution and sampling speed. The higher the desired resolution, the longer it takes to get an ADC reading.
 Each new bit of commanded oversampled resolution decreases the max read speed by a factor of 4. This decrease in speed (Samples/sec) due to commanding a higher desired ADC resolution (12-bits, for example), however, can be compensated in part by increasing the ADC clock via the "setADCSpeed()" method I declare below. This method is specific to the ATmega328 microcontroller. The default Arduino ADC clock speed is only 125kHz, which allows a theoretical maximum 10-bit ADC sample rate of ~9.6kHz (note that a single ADC sample takes 13 ADC clock cycles, so 125kHz/13 ~9.6kHz). You can speed the ADC clock up by a factor of 8, however (to make it 1MHz), by calling "setADCSpeed(ADC_PRESCALER_16_CLOCK_1MHZ)", or "setADCSpeed(ADC_FAST)" (they are the same command). The actual improvement in 10-bit ADC samples may be only a factor of 5~6, due to some overhead in the analogRead function, but this is still a substantial improvement. The max *theoretical* ADC 10-bit sample rate with this ADC clock setting is ~76.9kHz.  To go back to the default ADC clock speed, with a max theoretical 10-bit sampling rate of ~9.6kHz, call "setADCSpeed(ADC_PRESCALER_128_CLOCK_125KHZ", or "setADCSpeed(ADC_DEFAULT)", or "setADCSpeed(ADC_SLOW)" (they are all the same command).    
 
 What is the downside of speeding up the ADC clock frequency, then?  Why not speed it up to 8Mhz, which would provide a theoretical ADC sampling rate of 615kHz?  The answer is: you *can*. However, you will be OUTSIDE the 1Mhz max ADC clock speed recommended in the datasheet (pg. 317, Table 29-20) and in the AVR120 Application Note (Section 2.8, pg 10). As you *increase* the ADC clock speed, you also *decrease* the ADC resolution, so beyond ~1Mhz, the ADC is no longer guaranteed to be 10-bit.  
 
 If you choose to increase the ADC clock beyond 1Mhz anyway, will you damage the ADC?  I don't think so, but I do not KNOW 100% for sure. I originally had 3 theories on why the ADC clock speed is recommended to be 1Mhz max, and I have now found sources to validate some of them:
 1) Beyond 1MHz ADC clock speed, the ADC will overheat and become damaged. (I don't think this is even a factor, but I still do not know 100% for sure).
 2) Atmel simply has not tested and characterized the ADC at higher clock speeds (AVR120 Application Note, Section 2.8, pg 10, states this is true by saying: "Operating the ADC with frequencies greater than 1 MHz is not characterized.").
 3) At ADC clock speeds >1MHz, the ADC is no longer guaranteed to be 10-bit. Rather, its resolution slightly *decreases.* (AVR120 Application Note, Section 2.8, pg 10, states this is true by saying: "For optimum performance, the ADC clock should not exceed 200 kHz. However, frequencies up to 1 MHz do not reduce the ADC resolution significantly."). 
 
 Therefore, it is my guess that the datasheet simply says to stop at 1MHz so that they can still say the ADC has a 10-bit resolution. If you want to examine the tradespace between super fast ADC clock speeds, and increasing ADC resolution via oversampling, however, I recommend you do so. It's your call. Test at your own risk, with the above understanding in mind. This is outside the datasheet recommendations.  
 
Primary Sources:
-See AVR121 Application Note: http://www.atmel.com/images/doc8003.pdf
--esp. take a look at Table 3-1, Resolution Options, on pg. 8
-Also see AVR120 Application Note: http://www.atmel.com/images/doc2559.pdf
--esp. see Sections 2.8 & 2.9 (pg 10) for info on the ADC clock speed and its effect on base resolution
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

#ifndef eRCaGuy_NewAnalogRead_h
#define eRCaGuy_NewAnalogRead_h

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

//Macros
//clear all 3 ADPS bits; see datasheet pg 255-256
#define clearADPSbits() ADCSRA &= ~(_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0))

//ADC speed constants
/*NOTES TO SELF:
-The below values are used to set the ADC prescaler, which in turn sets the ADC clock frequency, which determines the max samples/sec.  Since a normal ADC conversion (sample) takes 13 ADC clock cycles (see datasheet pg 245, Section 24.4), you can calculate the max theoretical sample rate as max_sample_freq = ADC_clock_freq/13.
-CAUTION: datasheet pg. 317 recommends max ADC clock frequency of 1Mhz, so I do not know whether higher frequencies could damage the ADC or not
-NB: if you change the ADC clock speed, it doesn't just affect this library, it also affects the ADC sample rate when calling the standard core Arduino analogRead() function.
-ADC references:
 - Simon Monk's book "Programming Arduino Next Steps" pgs. 81-83 (http://www.amazon.com/Programming-Arduino-Next-Steps-Sketches/dp/0071830251) 
 - datasheet Section 24 (Analog-to-Digital Converter, pgs. 242-258) & Table 29-20 (ADC Characteristics, pg. 317)
 - http://forum.arduino.cc/index.php?topic=6549.0
 - http://forum.arduino.cc/index.php?topic=8795.0
 - http://forum.arduino.cc/index.php/topic,7762.0.html
 - http://gabuku.com/scope/
 -- http://gabuku.com/scope/scope.swf
 - http://www.atmel.com/images/doc8003.pdf (AVR121 Application Note)
 - http://www.atmel.com/images/doc2559.pdf (AVR120 Application Note)
*/
typedef enum
{                                     //Max theoretical
                                      //sample rate (Hz)
                                      //(ADC_clock_freq/13)
  ADC_PRESCALER_128_CLOCK_125KHZ      = 0,  //~9.6kHz (default Arduino ADC clock freq)
  ADC_DEFAULT                           = 0,//alternate name for the above option
  ADC_SLOW                              = 0,//same as above
  ADC_PRESCALER_64_CLOCK_250KHZ       = 1,  //~19.2kHz
  ADC_PRESCALER_32_CLOCK_500KHZ       = 2,  //~38.5kHz
  ADC_PRESCALER_16_CLOCK_1MHZ         = 3,  //~76.9kHz (max recommended ADC clock freq in datasheet)
  ADC_FAST                              = 3,//alternate name for the above option
  CAUTION_ADC_PRESCALER_8_CLOCK_2MHZ  = 4,  //~154kHz (CAUTION: this ADC clock freq is higher than what the datasheet recommends)
  CAUTION_ADC_PRESCALER_4_CLOCK_4MHZ  = 5,  //~308kHz (CAUTION: this ADC clock freq is higher than what the datasheet recommends)
  CAUTION_ADC_PRESCALER_2_CLOCK_8MHZ  = 6,  //~615kHz (CAUTION: this ADC clock freq is higher than what the datasheet recommends)
} ADC_prescaler_t;

class eRCaGuy_NewAnalogRead
{
  public: //public API
    //class constructor method
    eRCaGuy_NewAnalogRead();
    //function to read the ADC at a specified resolution; note: bits_of_resolution must be between 10-21
    //I will overload this function by defining various numbers of input parameters, as shown below:
    float newAnalogRead(uint8_t analogPin, uint8_t bits_of_resolution, unsigned long num_samples_to_avg, ADC_prescaler_t ADC_speed); 
    float newAnalogRead(uint8_t analogPin, uint8_t bits_of_resolution, unsigned long num_samples_to_avg);
    float newAnalogRead(uint8_t analogPin, uint8_t bits_of_resolution);
    float newAnalogRead(uint8_t analogPin);
    //set functions
    void setBitsOfResolution(uint8_t bits_of_resolution);
    void setNumSamplesToAvg(unsigned long num_samples_to_avg);
    void setADCSpeed(ADC_prescaler_t ADC_speed); //NB: if you change the ADC clock speed, it doesn't just affect this library, it also affects the ADC sample rate when calling the standard core Arduino analogRead() function.
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
    uint8_t _bits_of_resolution;
    unsigned long _num_samples_to_avg;
    ADC_prescaler_t _ADC_speed;
    unsigned long _ADCClockFreq; //Hz
};

extern eRCaGuy_NewAnalogRead adc; //preinstantiated object
#endif

