eRCaGuy_NewAnalogRead Readme  
Readme Written: 13 May 2014  
Readme Updated: 15 Feb 2015  

**Library:** eRCaGuy_NewAnalogRead (a replacement and continuation of eRCaGuy_analogReadXXbit)  
**Version:** 2.1 alpha  
-For additional version history and information, refer to the top of the .h file.  
Library webpage:   http://electricrcaircraftguy.blogspot.com/2014/05/using-arduino-unos-built-in-16-bit-adc.html  

**Instructions**  
Download the code by clicking "Download ZIP" to the right.  Extract the files, and install the library to your Arduino IDE. Run the examples and view the output in your Arduino IDE's Serial Monitor.  The examples read from analog pin A0, unless stated otherwise, so I recommend putting a potentiometer wiper on that pin.  
Ie: 5V---potentiometer leg 1, wiper (to A0), leg 3---GND  

**Description**  
This library allows you to do oversampling in order to increase the *commanded* theoretical Arduino ADC (Analog to Digital Converter) resolution from only 10-bit, to up to 21-bit! Note that the actual resolution, despite your commanded resolution, depending on your particular setup, noise characteristics, etc, may only increase up to 16-bits or so. This is up to you as the user to determine, since test setups can vary, and oversampling depends upon many factors and is not 100% guaranteed.  

As far as *commanded* resolution goes, the tradeoff is speed.  Every additional bit, n, beyond 10-bits, requires 4^n 10-bit samples.  This means that every time you add 1 more bit of resolution, your max sampling rate decreases by a factor of 4. However, this library also allows you to easily change the ADC clock prescaler, thus easily increasing the max 10-bit ADC sample rate up to 50+kHz.  

**FOR A GREAT DEAL MORE INFO, REFER TO THE TOP OF THE .H FILE.**  

**For other interesting articles and info, visit my website here: (http://electricrcaircraftguy.blogspot.com/)**  

I hope you find this useful.  
Sincerely,  
~Gabriel Staples  
http://electricrcaircraftguy.blogspot.com/  
