INTRO

This is a small tool that you can use to view movie files augemented by an Rends Vorze
Cyclone SA under Linux. For now, it only works with the USB stick that came with your
Vorze; the specifics of the 'raw' BluetoothLE connection are unknown as of yet. The
dongle itself is a CP2104 USB->serial converter connected to a BVMCN5102 BTLE module
which uses a nRF51882 chip.

HOWTO

First of all, you'll need Mplayer. Grab it from http://mplayerhq.hu or maybe your distro has
it. You will need the -udp-master option: if you try and play a file with -udp-master
on the commmand line and it fails, please update your copy of MPlayer or download and 
compile one yourself.

You will also need the standard gcc stuff: make, gcc, ld. You'll probably have those installed
already.

For mpv_bindings, you will need the lua library, liblua5.2
If you get errors that the lua headers weren't found, then you might have to adjust LIBLUA_INCDIR in the makefile for your system.

COMPILING

Just run 'make' and you should end up with a 'vorzemplayer' executable and mpv_bindings.so

USING IT

- Turn on your Vorze
- Plug in the stick
- Start up a terminal for the rest of the commands
- Run 'load.sh'. It will ask for your sudo-password to load the driver for the
  chipset on the stick and bind it to the (non-standard) PID/VID it uses.
- Run the vorze tool with the csv file that goes with the movie you're about to watch.
  For example:
  ./vorzemplayer play faphero_v1.csv
- Run mplayer with the -udp-master option to play the movie:
  mplayer -udp-master faphero1.mp4
- Enjoy!

MPV

- Turn on your Vorze
- Plug in the stick
- Start up a terminal for the rest of the commands
- Run 'load.sh'. It will ask for your sudo-password to load the driver for the
  chipset on the stick and bind it to the (non-standard) PID/VID it uses.
- Run mpv with sudo, with the mpv_vorze.lua script.
  EX,  sudo mpv -script=mpv_vorze.lua /path/to/video/file
  - The csv file must be in the directory as the video file, and with the same name, except with .csv replacing the video file extension.
  - mpv_bindings.so must be either in the current working directory, or in a place mpv can find it, eg) /usr/local/lib/lua/5.2/mpv_bindings.so
- Once it's going, you can even drag and drop video files

MORE FUN

If you have a joystick, you can also try the 'test' and 'record' option instead of the
'play' option. It'll allow you to control your Vorze from the joystick, with the
'record' option giving you the opportunity to recrd your own csv files. This functionality
still is experimental though, and at the moment it seems there's a bottleneck with the
amount of data that can be sent to the Vorze in a short timespan.



