# Toy Train Speedometer
Current Firmware Version V0.8.1  
Current Hardware Revision Rev0.5.0

This project relies heavily on code from [Sparkfun Electronics’ Hot Wheels Speed Trap](https://edu.workbencheducation.com/cwists/preview/11064x) for calculating speeds and general structure, and is conceptually indebted to [Derek Bever’s Do-All-DRO](https://github.com/loansindi/Do-All-DRO). It is released under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html)

## Overview
This is a chronometer that can be used to measure the speed of toy trains for DCC speed matching, or whatever else you can come up with. It assumes that the object going through will be going less than 99 inches per second, and will throw an error if it is going faster than that.  

When the device boots up it will first calibrate itself to the environment. Once it is ready it will show “--” on the display until you run a train through it.   
After the train goes through the tunnel, the display will go dark for a second, then display the train’s speed in inches per second. This number will remain on the display until a train is run through the device again, or the device looses power.  
If the device shows the letter E, followed by a number, on the display it has encountered an error. The error code will remain on the display until another train goes through, or the device looses power. Error codes are described in the troubleshooting section below.  

## Troubleshooting
### Error Codes
#### Error 1 (E1)
The device is tired of waiting for the train! Error 1 means that the device saw something enter the chronometer but didn’t see anything exit. If the train is still in the chronometer when this error occurs it is moving too slowly for the chronometer to calculate it’s speed.

If the train exited the chronometer and the display didn’t go dark after it exited there is likely a problem with the second sensor system. Try wiggling and/or replacing the sensors – any IR emitter/receiver pair should work.

#### Error 2 (E2)
Slow down! This error means that the train was traveling through the chronometer faster than 99 inches per second. The display only has two digit places and therefore has no way of accurately measuring speeds above 99 inches per second. The code could be tweaked to accommodate a 4 digit display – if you build this make a pull request and I’ll include it in the repository.

### It won’t turn on!
Have you checked that it’s connected to power? If you’re powering it off a battery are you sure the battery is charged?  
If you’ve checked all of the above things there is an internal problem, check the power indicator on the micro controller (Likely a green or blue LED).  
If the power indicator is blinking that probably means there’s a short in the system somewhere and it’s shutting itself off to keep from being damaged. Look for stray pieces of metal touching wires or wires touching each other. Remove the metal and/or separate the wires and see if this fixes the issue.  
If the power indicator is steady there may be an issue with calibration, try moving the chronometer to a darker location.  
If the power indicator is dark the device is not receiving power, check that the supply is outputting power, and that the power device is attached correctly to the Microcontroller (the red/positive wire should go to VIN or VRAW and the black/negative wire should go to ground or GND.)

### The speed I’m getting can’t possibly be right!
There might be an issue with the detection system. Try moving the device to an area with a lower light level. You can also try wiggling and/or replacing the sensors – any IR emitter/receiver pair should work.

### I’ve read all the troubleshooting steps and it’s still broken!
I can’t solve problems I don’t know about. Please file an issue so I can look into it!

## Advanced Features - For Nerds
### Serial Port
The device uses the onboard serial port to transmit debugging and status information.
#### Boot
The serial port should print something like this on boot:  
```Toy_Train_Speedometer by Carlingr.tech
Firmware V0.9.1
-------------------
This software is available under GNU General Public License v3.0
Any modifications must be published under the same license.
-------------------
calibrating
508 , 513
-------------------
```
The firmware version is the current firmware version installed on the device. This can be updated by uploading the new code to the Arduino. Whole number firmware updates indicate a possible break in backwards compatibility. Proceed at your own risk.

The two numbers under calibrating are the threshold values calculated for the detectors. Values under 200 or over 800 tend to be a sign of a problem with the detection system. When submitting issues it’s super helpful if you can send me this information (along with anything else that comes out of the serial port when the issue presents).

#### Speed Readings 
Speed readings are sent via the serial port in the unit of `(inches per second) * 10`  
The output syntax is `speed = ##` where `##` is the speed in `(inches per second) * 10`.   
i.e. `speed = 85` means the train was going 8.5 inches per second, or  `speed = 151` means the train was going 15.1 inches per second. 

Theoretically this could be hooked up to a program like JMRI so that locomotives can speed match themselves. If you’re inclined to make this let me know and I’ll link to it here!

#### Error Codes
Error codes will also be printed to the serial port, With the syntax `Error#` where `#` is the Error code.  
i.e `Error1` is Error 1 and `Error2` is Error 2.

### Variables
There are some settings on the code that you may want to mess with if you are having issues. They are at the top of the file for your convenience. If you are having timeout problems (experiencing Error 1 a lot) you may want to increase the value of `const unsigned long TIMEOUT`, which is the time (in microseconds) the device will wait before displaying Error 1 and resetting for the next train. 

If you modify the device as to change the spacing between the emitters and detectors you will want to change `const int SPACING` which is the amount of space between the emitter/detector pairs in inches.

### The Detectors
The detectors are an infrared emitter detector pair. During the build process 99% of my problems came down to these. If you’re having problems, these being loose, backwards or otherwise faulty is a good place to start troubleshooting. Note that the data sheet for the detectors shows them from a bottom-up view.
