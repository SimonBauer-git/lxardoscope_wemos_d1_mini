# lxardoscope_wemos_d1_mini
this is a single channel low frequency oscilloscope using a wemos d1 minis internal adc. you can use it just with the board itself but i have made a little daughterboard which contains a screw terminal, a capacitor for ac coupling, a switch to toggle between the ac and dc coupling and a bridge where you can add a resistor for meassuring higher voltages (it survives 5v, do this at your own risk as it is specified as 3.3v max). the code is still work in progress. for faster sampling rate download
https://community.openenergymonitor.org/t/lxardoscope-recompiled-at-high-sample-rate/3755. if you want to go even faster you need to download lxardoscope+, and specify 2000000 Baud in the code, use the vfast code, 42ksps is possible or 21khz
