# About

In this article we will setup a Particle Photon module to monitor compass baring through the MAG3110 Three-Axis Digital Magnetometer Electronic Compass I²C Mini Module.  Based on readings from this sensor we will turn relays on and off using the Relay Shield for Particle Photon I²C 8-Channel SPDT 10-Amp.

This Library is intended to be used in conjunction with the Control Anything article Compass_Relay.

The intention of this library is to make use of the MAG3110 Three-Axis Digital Magnetometer Electronic Compass I²C Mini Module and the Relay Shield for Particle Photon I²C 8-Channel SPDT 10-Amp with Particle development web IDE as simple as possible for users.

###Developer information
NCD has been designing and manufacturing computer control products since 1995.  We have specialized in hardware design and manufacturing of Relay controllers for 20 years.  We pride ourselves as being the industry leader of computer control relay products.  Our products are proven reliable and we are very excited to support Particle.  For more information on NCD please visit www.controlanything.com

###Requirements
- MAG3110 Three-Axis Digital Magnetometer Electronic Compass I²C Mini Module
- Relay Shield for Particle Photon I²C 8-Channel SPDT 10-Amp
- Particle Core/Photon module
- Knowledge base for developing and programming with Particle Core/Photon modules.

### Version
1.0.0

### How to use this library

This is a fully implemented Library and is intended to be used as is.  Users may modify and use the library in any way they see fit.  The intended use is to search Community Libraries under Particle's WEB IDE for Compass_Relay.  Once found make sure the application tab is selected, then click Use This Example button on the left.  Then flash the firmware into your Particle Module.  For more information see [Particle's documentation] [sparkIncludeLibrary].

### What it does

This Library monitors the direction of the compass sensor and then turns relays on and off on the 8 channel relay controller based on the current heading.

We publish 3 functions to the Particle cloud for this application("SetMinD", SetMaxD", and "Calibrate").

SetMinD will set the east most baring for our heading.
SetMaxD will set the west most baring for our heading.
Calibrate puts the controller into a calibration mode required to get accurate heading information from the compass module.

The West most direction set by the MaxD function will trigger Relay 1.  The East most direction set by the MinD function will trigger Relay 2.  This range from max to min is divided by 8 in order to calculate headings for controlling relays 2-7.  So if you wave the compass from the west to the east the relays will turn on/off in a sequence.

The firmware requires that the sensor be calibrated and the Minimum and Maximum barings are set.

In order to calibrate the compass sensor call the "Calibrate" function published to the Particle cloud and pass it the argument "Start".  You can monitor the serial output of the module to see when min or max values are updated.  Now move the sensor in all directions repeatedly.  The longer you do this the better calibrated the sensor is.  I recommend monitoring the serial output of the module and keep moving the sensor around untill there are no more prints to the console.  Once finished call the "Calibrate" function again passing it anything other than "Start" such as "Stop".  This calibration information is stored into EEPROM at this point as 3 different offsets as float values occupying EEPROM address locations 8-19. 

In order to set the Minimum and Maximum headings for the firmware you simply point the compass to the west most direction, then call the SetMaxD function with an argument of 0, then point the compass to the east most direction, then call the SetMinD function with an argument of 0.  These minimum and maximum headings are stored in EEPROM firmware as float values occupying EEPROM storage address's 0-7.  Always calibrate the sensor first before setting the min and max headings.

After the compass is calibrated and the minimum and maximum headings are set the firmware will start controlling the relays based on the compass heading.  Note that the minimum heading, maximum heading, and calibration values are stored in EEPROM so they persist across power cycles of the Particle Photon Module.

This firmware also publishes the current degree heading as a variable on Particle's server.

License
----

GNU
[sparkIncludeLibrary]:https://docs.particle.io/guide/getting-started/build/photon/
