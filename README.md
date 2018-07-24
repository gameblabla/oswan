Gameblabla's Oswan
=======================

Gameblabla's Oswan is a Wonderswan/Wonderswan color, based on OswanJ by bird_may_nike and Oswan by Alekmaul.

It can play most Wonderswan games properly.

It used to be based on Alekmaul's Oswan port but significant modifications were done to it.

This fork is also even more accurate than upstream, while still being faster and portable : 

Games are now playing at the proper speed, games like Klonoa and Beatmania now sound closer to real hardware.

A stripped-down version of Minizip is provided as well. Minizip is licensed under a 3-clause BSD license.

The source code (especially the front-end) is incredibly hacked-up : i miss the days where the source code was nice and clean...

COMPATIBILITY
=================

The only problematic games are Pocket Figher and the One Piece games : they have some graphical glitches.

Sound is far from accurate. (not even Mednafen is close to real hardware)

COMPILLING
=================

In all cases, i'm compilling the binaries on a GNU/Linux PC.

GCW0:

Make sure you have the precompiled SDK binaries installed somewhere and point its bin directory to PATH.

Then just do a make -f Makefile.gcw0

Wii:

You need to have the DevkitPPC binaries somewhere and point its bin directory to PATH.

Make sure you have libogc and LibSDL libraries !

Then just do : make -f Makefile.wii

Nspire:

Just install the ndless devkit (or compile the toolchain with their script).

Then do a make -f Makefile.nspire

MinGW:

Install the MingW packages provided by your distribution.

Make sure to compile libSDL with it and put them in MinGW paths.

Then do a make -f Makefile.mingw

CREDITS
===========

GUI comes from Alekmaul's Oswan.  (Itself based on Dmitry's Handy GCW0 port)

Mednafen, which i used for comparison.

I took some of their code as well, thanks ! (GPLv2 or later)

NomadSwan, the cpu core.

Alekmaul, for working on the initial version of Oswan. 

trap15, for his (more accurate) documentation. 

http://daifukkat.su/docs/wsman/
