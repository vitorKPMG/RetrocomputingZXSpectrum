# The TCP/IP Virtual Joystick for Android application is now Spectranet 8-bit Network File System compatible!
This folder contains some ZX Spectrum 48K games and a TCP/IP Virtual Joystick for Android test application compatible with a Spectranet 8-bit Network File System (I will try to add some more). They can be copied for instance into the NFS server that is included in the TCP/IP Virtual Joystick for Android application:

https://play.google.com/store/apps/details?id=com.iadvd.android.tcpipvirtualjoystick

The ones ending with an "A" and the "tcpipjoysticktest.tap" application will run only if the TCP/IP Virtual Joystick for Android app. is available.

1. "radeblunnere1.tap" and "radeblunnere2.tap" are our "Rade Blunner Episode 1" and "Rade Blunner Episode 2" games for Keyboard, Kempston and Sinclair joysticks only.

2. "radeblunnere1A.tap" and "radeblunnere2A.tap" are our "Rade Blunner Episode 1" and "Rade Blunner Episode 2" games also compatible with the TCP/IP Virtual Joystick for Android app.

3. "birdycantabile.tap" is our "Birdy Cantabile (a Musical Puzzle)" game for Keyboard, Kempston and Sinclair joysticks only.

4. "birdycantabileA.tap" is our "Birdy Cantabile (a Musical Puzzle)" game also compatible with the TCP/IP Virtual Joystick for Android app.

5. "moon.tap" is our "Moon's Fandom Festival" mini graphic adventure game for Keyboard, Kempston and Sinclair joysticks only.

6. "moonA.tap" is our "Moon's Fandom Festival" mini graphic adventure game also compatible with the TCP/IP Virtual Joystick for Android app.

7. "moonP.tap" is our "Moon and The Pirates" mini graphic adventure game for Keyboard, Kempston and Sinclair joysticks only (English version).

These .tap files are the same ones than here:

https://github.com/Iadvd/RetrocomputingZXSpectrum/tree/master/GameExamples

7. "tcpipjoysticktest.tap" is our tester application to verify that the TCP/IP Virtual Joystick for Android app. can be used from the ZX Spectrum machine. Exactly the same one that is here:

https://github.com/Iadvd/RetrocomputingZXSpectrum/tree/master/Basic%20ZX%20Spectrum%20TCPIP%20Server%20Project

8. The rest of games are Powered by The Mojon Twins La Churrera and also copyleft The Mojon Twins under a CC BY-NC-SA 3.0 license. So all credits go to them:

8.1 "perilsA.tap" https://github.com/Iadvd/RetrocomputingZXSpectrum/tree/master/GameExamples/perils

8.2 "tbA.tap" https://github.com/Iadvd/RetrocomputingZXSpectrum/tree/master/GameExamples/tb

8.3 "horaceA.tap" https://github.com/Iadvd/RetrocomputingZXSpectrum/tree/master/GameExamples/horace

8.4 "viajeA.tap" https://github.com/Iadvd/RetrocomputingZXSpectrum/tree/master/GameExamples/viaje

8.5 "ababolA.tap" https://github.com/Iadvd/RetrocomputingZXSpectrum/tree/master/GameExamples/ababol

8.6 "lalaA.tap" https://github.com/Iadvd/RetrocomputingZXSpectrum/tree/master/GameExamples/lala

8.7 "tmA.tap" https://github.com/Iadvd/RetrocomputingZXSpectrum/tree/master/GameExamples/tm

# How can I use these files?
The TCP/IP Virtual Joystick for Android application is compatible with the Spectranet 8-bit Network File System. Indeed it contains a file server that you can use in your local network! In the moment that the TCP/IP Virtual Joystick for Android app. is launched, the included local file server becomes active and it is possible to mount it as usual from a ZX Spectrum with a Spectranet peripheral via a %mount command. 

The file system is read-only, thus the permitted TNFS comands sent from the ZX Spectrum side are %mount, %umount, %cat, %loadsnap and %tapein. The file server has only a root folder. You can put for favourite ZX Spectrum programs, .tap or .sna files, in the root folder via your preferred Android File Manager application (so you can rename them, remove them, etc. via that File Manager directly on your Android device). We suggest the use of Google Files because it is free and ads-free:

https://play.google.com/store/apps/details?id=com.google.android.apps.nbu.files&hl=en

The server will be active until the TCP/IP Virtual Joystick is in use (and that happens when you press the OK button on the launch screen welcome message, from that moment the TNFS server will be off and the virtual joystick is active and searching for the Spectrum machine IP). 

If you want to rerun the server, just close the TCP/IP Virtual Joystick for Android application and launch it again. That is all!

# How can I load those applications into my Spectrum machine (or Fuse+Spectranet emulator)?
You need a Fuse emulator with a Spectranet peripheral emulation, or a real ZX Spectrum 48K with a Spectranet peripheral. In both cases, first you need to mount the network file server that is active in your Android device where your TCP/IP Virtual Joystick for Android app. has been launched and is showing the welcome screen. The file server IP is the one that appears at the top of the welcome message (AAA.BBB.CCC.DDD, check that the IP is not 0.0.0.0 because that means that you do not have WiFi connection and the server will not be available!). So you will write in your Spectrum as follows:

%mount 0,"AAA.BBB.CCC.DDD" (enter)

If everything goes well, the message back will be "0 OK, 0:1"

Now you can have a look at the contents, just write:

%cat (enter)

If everything goes well, the content of the root folder will be shown.

If you want to load a snap .sna file into your machine, write:

%loadsnap "nameofthesnapfile.sna" (enter)

It should take two or three seconds only, please be patient. 

If you want to load a tape file .tap into your machine, we suggest to write the following lines (we have found for some reason this solution more stable than the direct use of the %tapein command):

10%tapein"nameofthetapfile.tap" (enter)

20 PAUSE 30 (enter)

30 LOAD "" (enter)

RUN (enter)

This will load the .tap file into the memory. Again, it should take two or three seconds only, so please be patient. 

Sometimes the commands above are not enough to load correctly a game, and a R Tape loading error happens. In those cases the solution below usually works well:

RANDOMIZE USR 24000 (enter)

# Troubleshooting:

1. If by any chance the TNFS server or the TCP/IP Virtual Joystick for Android application turns unstable, reset the Spectrum machine, close the TCP/IP Virtual Joystick for Android app. and launch it again. Reseting both sides if enough to have a new connection to %mount. This does not happen very often but sometimes while using the %tapein or %loadsnap commands it might happen. 

2. If after %tapein a .tap file compatible with the TCP/IP Virtual Joystick for Android app., and clicking in the OK button of the Welcome Message of the TCP/IP Virtual Joystick for Android app. to start searching the Spectrum IP, the Android application is not able to find the Spectrum application, closing the Android application, launching it again and clicking the OK button is enough to get connected to the Spectrum game/application safely. 
