---
layout: post
title:  "ioquake3"
date:   2013-09-20 14:00:00
categories: rendering
---

This page is part of the series that explores existing
[software for virtual world rendering][root].

[ioquake3][io3] is a free software (GPLv2+) first person shooter engine. 
ioquake3 is divided into two general parts, the client part and 
the server part, with a networking system that connects the two. 
The client mainly takes care of user interface. That is, it displays 
the 3D rendering of the game world and also collects keyboard, mouse, 
and joystick input events. The server is responsible for taking care of 
everything else: moving the user's position and view based on 
keyboard/mouse/joystick inputs, detecting collisions, processing 
bot AI, determining hits and frags, etc. 

The [github repository](https://github.com/ioquake/ioq3/) looks active.
It has a [wiki](http://wiki.ioquake3.org/) amd a 
[forum](http://www.ioquake.org/forums/).

From the description above it looks very similar to what we need.
Experimenting with it is planned for next days.

To build it from source on an Ubuntu 13.04 64-bit machine I used
the [guide](http://wiki.ioquake3.org/Building_ioquake3_on_Linux) in wiki.
It all went smoothly, and I ended up with two executables that
were refusing to start:
    "pak0.pk3" is missing. Please copy it from your legitimate Q3 CDROM
I had a suspicion that Quake3 should be installed before.


[root]: {{ site.baseurl }}/rendering/2013/09/20/software-for-virtual-world-rendering.html
[io3]: (http://ioquake3.org/)
