# lxardoscope_wemos_d1_mini
this is a single channel low frequency oscilloscope using a wemos d1 minis internal adc. you can use it just with the board itself but i have made a little daughterboard which contains a screw terminal, a capacitor for ac coupling, a switch to toggle between the ac and dc coupling and a bridge where you can add a resistor for meassuring higher voltages (it survives 5v, do this at your own risk as it is specified as 3.3v max). the code is still work in progress. you need to download lxardoscope+, and specify 2000000 Baud in the code, 45ksps is possible or 22.5khz
## using it on x86 linux
just download the lxardoscope code for linux, and use it, no extra steps required
## using it on windows
either use a linux vm and then do the same as on x86 linux or use wsl. you need wsl-usb to be able to use the usb from the computer on wsl.
## using it on android
follow my guide to install termux x11, then download the folder with _android in its name. you need to install libforms-dev and ibforms-bin to use it. you need the app tcp uart transparent bridge, specify it to be server in the app and give yourself a static ip in the network. https://play.google.com/store/apps/details?id=com.hardcodedjoy.tcpuart
