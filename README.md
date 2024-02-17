# lxardoscope_wemos_d1_mini
this is a single channel low frequency oscilloscope using a wemos d1 minis internal adc. you can use it just with the board itself but i have made a little daughterboard which that makes it more like a real oscilloscope (described in detail further down)
![Alt the little daughterboard on the wemos board](https://raw.githubusercontent.com/SimonBauer-git/lxardoscope_wemos_d1_mini/main/schematic/image_1.jpg)
currently, 45ksps is possible or 22.5khz (so anything in the audio frequency range)
## using it on x86 linux
just download the lxardoscope code for linux, specify 2000000 BAUD in the code and use it, no extra steps required
## using it on windows
either use a linux vm and then do the same as on x86 linux or use wsl. you need wsl-usb to be able to use the usb from the computer on wsl.
## using it on android
![Alt my scope running on an android tablet](https://raw.githubusercontent.com/SimonBauer-git/lxardoscope_wemos_d1_mini/main/PXL_20240212_120237860.MV.jpg)
### with proot-distro setup and a desktop enviroment
just download the folder android from here. you need to install libforms-dev and ibforms-bin to use it and to compile it. you need the app tcp uart transparent bridge, specify it to be server in the app and give yourself a static ip in the network. https://play.google.com/store/apps/details?id=com.hardcodedjoy.tcpuart
to use it open a terminal in the folder, type ./scopeconnect.sh (assuming you made it executable with chmod +x or in the gui at first) then type ./lxardoscope (if the programm crashes the first time, then repeat this step). and afterwards you can use it.
### simple setup, just the scope

