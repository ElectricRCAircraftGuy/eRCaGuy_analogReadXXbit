/*
eight_analog_reads.ino
-rapidly read A0 through A7, on an ATmega328 SMT chip (ex: Arduino Nano, Pro Mini, etc, but NOT Uno), and measure the time this takes
--you may set global variable "NUM_ANALOG_PINS" below to 6 instead of 8 if you want to use this code on an Arduino Uno
-do this at a higher resolution (ex: 12-bit, via oversampling)

By Gabriel Staples
Written: 18 Feb. 2015
Updated: 18 Feb. 2015

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

#include <eRCaGuy_NewAnalogRead.h> //for oversampling to increase ADC resolution
#include <eRCaGuy_Timer2_Counter.h> //for measuring time to a 0.5us resolution (instead of the 4us resolution that micros() has)

//Global Vars
const byte NUM_ANALOG_PINS = 8; //set to 6 if using an Arduino Uno or other ATmega328 that only has 6 analog pins; set to 8 for Nano, Pro Mini, etc.

void setup()
{
  Serial.begin(115200);
  timer2.setup();
  
  //Configure ADC
  adc.setBitsOfResolution(12);
  
  //****************THIS ADC SPEED RESULTS IN ~390us PER 12-BIT ANALOG SAMPLE (~2565 Hz sample rate)****************
//  adc.setADCSpeed(ADC_FAST); 
  
  //****************THIS ADC SPEED RESULTS IN ~275us PER 12-BIT ANALOG SAMPLE (~3636 Hz sample rate)****************
  adc.setADCSpeed(CAUTION_ADC_PRESCALER_8_CLOCK_2MHZ); //this ADC clock speed is 2x that of ADC_FAST, and should still produce good results too; 
                                                       //give it a try, though it may reduce actual analog resolution a touch
}

void loop()
{
  //local variables
  unsigned int readings[NUM_ANALOG_PINS]; //prepare array to receive 8 analog readings
  unsigned long times[NUM_ANALOG_PINS]; //units of 0.5us
  float times_f[NUM_ANALOG_PINS]; //us; floating point times
  
  //take readings as fast as possible
  byte i = 0;
  unsigned long t_start = timer2.get_count(); //count units of 0.5us each
  for (byte pin=A0; pin<A0+NUM_ANALOG_PINS; pin++) //for analog pins A0 through A5 (Uno) or A7 (Nano)
  {
    analogRead(pin); //take a single reading and throw it out, to let the ADC settle
    readings[i] = adc.newAnalogRead(pin);
    times[i] = timer2.get_count(); //units of 0.5us
    i++;
  }
  
  //do time calcs & print out results
  i = 0;
  for (byte pin=A0; pin<A0+NUM_ANALOG_PINS; pin++)
  {
    //zero-reference the times, so that each time value is an elapsed time since starting this data set
    times_f[i] = (float)(times[i] - t_start)/2.0; //us; the "/2.0" is to convert from 0.5us units to us
    //get time for just this one analog reading
    float dt; //us
    if (i>0)
    {
      dt = times_f[i] - times_f[i-1]; //us
    }
    else //i==0
    {
      dt = times_f[i]; //us
    }
    
    //print results
    Serial.print(F("pin = A")); Serial.print(pin-14); //subtract 14 since A0 is actually Arduino pin 14, but I want it to print out as "A0" instead of "14"
    Serial.print(F(", reading = ")); Serial.print(readings[i]); 
    Serial.print(F(", t_elapsed(us) = ")); Serial.print(times_f[i]);
    Serial.print(F(", dt(us) = ")); Serial.print(dt);
    Serial.print(F(", ")); Serial.print(adc.getBitsOfResolution()); Serial.print(F("-bit sample rate(Hz) = ")); Serial.println(1000000/dt);
    i++;
  }
  
  Serial.println(""); 
  
  //wait a bit between sets of readings
  delay(500);
}



