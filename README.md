# Gps64
GPS Calculations for Arduino

I have been always frustrated from the missing double (64bit) type in Arduino world and hence the problem with properly calculating GPS distance, bearing and destination point.

This library uses Float64 & Math64 libs and also adds some fast and very fast implementations of trigonometric functions and fast and regular versions of GPS distance, bearing and destination point.  

RUNNING/TESTING INSTRUCTIONS

1. download Float64 from :

https://github.com/mmoller2k/Float64

2. download Math64 from :

https://github.com/JamesGKent/Math64

note that the Math64 from mmoller2k at the time of writing (commit [254ffb9](https://github.com/mmoller2k/Math64/commit/254ffb91d186e1642b407a0b8a35d8e3a09c5b4a)) has flaws in the atan264 function and in the asin64 function that make them unable to return negative numbers, the above fork fixes those issues.

3. download Gps64 ( and optionally Thread64) from :

 https://github.com/vtomanov

4. copy the contents of repository to:

C:\Program Files (x86)\Arduino\libraries\Math64

or the local libraries folder.

- do the same for Math64, Float64, Gps64  ( and optionally for Thread64)

5. restart your arduino environment ( to pickup the new libraries) 

6. select Gps64Example from file->examples->Gps64.

8. compile - upload and run - open the serial interface monitor to see the results ( the example assumes you serial monitor is initialised with 115200 if different change the following line in the example :   Serial.begin(115200);

that should be all - no changes in includes etc. should be needed
