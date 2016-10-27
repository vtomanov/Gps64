# Gps64
GPS Calculations for Arduino

I have been always frustrated from the missing double (64bit) type in Arduino world and hence the problem with properly calculating GPS distance, bearing and destination point.

This library uses Float64 & Math64 libs and also adds some fast and very fast implementations of trigonometric functions and fast and regular versions of GPS distance, bearing and destination point.  

RUNNING/TESTING INSTRUCTIONS

1. downlaod Math64 and Float64 from :

https://github.com/mmoller2k

2. download GPS64 ( and optionally Thread64) from :

 https://github.com/vtomanov

3. create a folder:

C:\Program Files (x86)\Arduino\libraries\Math64

and copy the content of Math64 there

- do the same for Float64, Gps64  ( and optionally for Thread64)

4. create a  folder somewhere on your disk with name :  Gps64Example

5. copy the file Gps64Example.ino inside

6. restart your arduino environment ( to pickup the new libraries) 

7. navigate to the folder Gps64Example and open the file Gps64Example.ino from it.

8. compile - upload and run - open the serial interface monitor to see the results ( the example assumes you serial monitor is initialised with 115200 if different change the following line in the example :   Serial.begin(115200);

that should be all - no changes in includes etc. should be needed
