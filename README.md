**FOR THE LATEST CODE, CHANGE THE "branch" DROPDOWN MENU ABOVE TO "Version_2", AND DOWNLOAD MY Version_2 CODE INSTEAD! ~GS 19 Feb. 2015**

Readme last updated: 13 May 2014

eRCaGuy_analogReadXXbit
Library webpage: http://electricrcaircraftguy.blogspot.com/2014/05/using-arduino-unos-built-in-16-bit-adc.html

**Instructions**
Download the code by clicking "Download ZIP" to the right.  Extract the files, and install the library to your Arduino IDE. Run the examples and view the output in your Arduino IDE's Serial Monitor.  The examples read from analog pin A0, so I recommend putting a potentiometer wiper on that pin.

**Description**
This library allows you to do oversampling in order to increase the Arduino ADC (Analog to Digital Converter) from only 10-bit, to up to 21-bit!!!
The tradeoff is speed.  Every additional bit, n, beyond 10-bits, requires 4^n 10-bit samples.  This means that every time you add 1 more bit of resolution, your max sampling rate decreases by a factor of 4.  However, this library still proves very useful to provide that extra resolution when needed!

**For other interesting articles and info, visit my website here: (http://electricrcaircraftguy.blogspot.com/)**

I hope you find this useful.
~Gabriel Staples
http://electricrcaircraftguy.blogspot.com/
