# lxardoscope_wemos_d1_mini
this is a single channel low frequency oscilloscope using a wemos d1 minis internal adc. you can use it just with the board itself but i have made a little daughterboard which that makes it more like a real oscilloscope.
![Alt the little daughterboard on the wemos board (v1,not able to meassure ac)](https://raw.githubusercontent.com/SimonBauer-git/lxardoscope_wemos_d1_mini/main/schematic/image_1.jpg)
this is version 1 of the daughterboard, it couldnt meassure ac voltages (i.e. negative voltages)
![Alt the little daughterboard on the wemos board (v1,not able to meassure ac)](https://raw.githubusercontent.com/SimonBauer-git/lxardoscope_wemos_d1_mini/main/schematic/image_1.jpg)
this is version 2 of the daughterboard, it can meassure ac voltages with the use of a voltage devider. you need a 200k resistor depending on the voltage meassured.
currently, 45ksps is possible or 22.5khz (so anything in the audio frequency range). 62ksps is possible or 31khz if you change the cpu frequency to 160mhz (can be done in arduino IDE).
## using it on x86 linux
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
