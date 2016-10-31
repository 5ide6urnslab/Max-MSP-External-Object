# Max-MSP-External-Object
This repo is the library for Max/MSP that runs between Devices(Arduino etc.) and Max/MSP 7. The External Object has developed by Max SDK 7.1.0.

## Description
##### dataCoupling
This dataCoupling is the library for Max/MSP that runs between Devices and Max/MSP. This library couple data of 2 byte, when Device is transmitted data of 2 byte by every other byte. You must use 1st data "0x7E" of Serial communication as prefix, so Max/MSP find out starting position of Serial data. And Serial data must be used Big endian for data coupling. (e.g.) Serial communication of Arduino is transmitted data every other byte. So data of 2 byte divide 1 byte to transmit.

##### convertBpm2Ms
This convertBpm2Ms is the library for Max/MSP that runs between Devices and Max/MSP. This library exchange BPM to Millisecond. (e.g.) Pulse Sensor etc.

## Installation
(1) To use the above library, first you need to download and install  
https://cycling74.com

(2) To make the External Object, you need to download  
https://cycling74.com/downloads/sdk/#.WBbdLBTbhEc

(3) To get a copy of the repository you can download the source from  
https://github.com/5ide6urnslab/Max-MSP-External-Object

(4) You compile the source and the addon should sit in **/Max/Library/** .

## Running the Example Project
An example project is provided in the **/example/** folder.
<br>
##### datacoupling
This example "DataCoupling" couple data of 2 byte, when Arduino UNO is transmitted data of 2 byte by every other byte. You must use 1st data "0x7E" of Serial communication(Serial, XBee ZB AT mode) as prefix, so Max/MSP find out starting position of Serial data. And Serial data must be used Big endian for data coupling.  

<img class="photo" src="https://github.com/5ide6urnslab/Max-MSP-External-Object/blob/master/resource/dataCoupling.png" width="400px" />
<br>

##### convertBpm2Ms
This example is exchanged BPM to Millisecond from Pulse Sensor (and Arduino).

<img class="photo" src="https://github.com/5ide6urnslab/Max-MSP-External-Object/blob/master/resource/PulseSensor.png" width="400px" />
<br>

## Copyright
© 1990- All Rights Reserved.  Max is a trademark of Cycling ’74/IRCAM, l’Institut de Recherche et Coo¨rdination Acoustique/Musique. https://cycling74.com

## License
##### About this manual.
<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />5ide6urns lab is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.

##### About this software. 
Released under the MIT license. http://opensource.org/licenses/mit-license.php

## Credit
Electronics:   Show Kawabata(5ide6urns lab) http://dum6sen5e.com    
