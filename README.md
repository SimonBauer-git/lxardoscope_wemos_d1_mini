# lxardoscope_wemos_d1_mini
this is a single channel low frequency oscilloscope using a wemos d1 minis internal adc. you can use it just with the board itself but i have made a little daughterboard which that makes it more like a real oscilloscope.
![Alt the little daughterboard on the wemos board (able to meassure ac)](https://raw.githubusercontent.com/SimonBauer-git/lxardoscope_wemos_d1_mini/main/PXL_20240302_200453836.MV.jpg)
this is version 2 of the daughterboard, it can meassure ac voltages with the use of a voltage devider. you need a 200k resistor depending on the voltage meassured.
## continous aquisition
with continous aquisition (i.e. reading one sample, then transmitting that), 45ksps is possible or 22.5khz (so anything in the audio frequency range). 62ksps is possible or 31khz if you change the cpu frequency to 160mhz (can be done in arduino IDE).
## buffered aquisition 
by reading a lot of samples (in this case 16384) and then transmitting them in bulk much greater sampling rates are achivable, how much I am not sure of myself as due to the pulsed nature of the datastream the lxardoscope application doesnt show it correctly, I tested it with a 1 microsecond pulse though and it showed up as a squarewave, that was ok in terms of stability, so atleast 500khz. I have fixed the timebase issue now, you need to devide the tspan variable by 6 in lxa_ard.c to get atleast semi accurate results (of course this isnt a precision instrument by any means).
# using it on x86 linux
just download the lxardoscope code for linux, specify 2000000 BAUD in the code and use it, no extra steps required
## using it on windows
either use a linux vm and then do the same as on x86 linux or use wsl. you need wsl-usb to be able to use the usb from the computer on wsl.
## using it on android
![Alt my scope running on an android tablet](https://raw.githubusercontent.com/SimonBauer-git/lxardoscope_wemos_d1_mini/main/PXL_20240212_120237860.MV.jpg)
### with proot-distro already setup with a desktop enviroment
just download the folder android from here. you need to install libforms-dev and ibforms-bin to use it and to compile it. you need the app tcp uart transparent bridge, specify it to be server in the app.
https://play.google.com/store/apps/details?id=com.hardcodedjoy.tcpuart
to use it open a terminal in the folder, type ./scopeconnect.sh (assuming you made it executable with chmod +x or in the gui at first) then type ./lxardoscope (if the programm crashes the first time, then repeat this step). and afterwards you can use it.
### simple setup, just the scope
if you only want the scope and nothing more, just install termux, termux x11 and tcp uart bridge. open termux, then do these steps

1. pkg update
2. pkg upgrade
3. pkg install x11-repo termux-x11-nightly
4. pkg install proot-distro
5. proot-distro install debian
6. wget https://raw.githubusercontent.com/SimonBauer-git/lxardoscope_wemos_d1_mini/main/android/simple-setup/scope.sh
7. proot-distro login debian
8. apt update
9. apt upgrade
10. apt install libforms-dev libforms-bin libx11-bin libx11-dev
11. wget https://raw.githubusercontent.com/SimonBauer-git/lxardoscope_wemos_d1_mini/main/android/lxardoscope 
12. chmod +x lxardoscope
13. wget https://raw.githubusercontent.com/SimonBauer-git/lxardoscope_wemos_d1_mini/main/android/simple-setup/proot-distro/scope.sh
14. chmod +x scope.sh
# Function Generator:
![function generator](https://raw.githubusercontent.com/SimonBauer-git/lxardoscope_wemos_d1_mini/main/PXL_20240302_200633422.MV.jpg)
A very simple function generator, using an audio jack output (Frequency limited to whatever the device can provide). daughterboard breaks out left and right channel along with ground to pins, no electronics involved, just a direct connection. you can use something like this to create the waveforms, no install required, just use it: https://cse.unl.edu/~jfalkinburg/husker-scope/app/page/FunctionGenerator
or if you want an android app that has a different interface and a bit more functions look here: https://play.google.com/store/apps/details?id=com.keuwl.functiongenerator
