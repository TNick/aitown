---
layout: post
title:  "PlaneShift"
date:   2013-09-20 19:30:00
categories: rendering
---

This page is part of the series that explores existing
[software for virtual world rendering][root].

[PlaneShift][pshift] is a Role Playing Game immersed into a 
3D virtual fantasy world which is FULLY FREE to play. 
PlaneShift is made by a group of RPG enthusiasts and 
not by a commercial company. PlaneShift is Open Source for 
the client and server code, so everyone can contribute 
to its development!

The project is hosted at [sourceforge](http://sourceforge.net/projects/planeshift/)
and a [PlaneShift Compiling Guide][guide] is available.

Based on initial evaluation ([1](http://www.youtube.com/watch?v=8GhBcCpEGt0),
[2](http://www.youtube.com/watch?v=jGVYk3DoP0g), 
[3](http://www.youtube.com/watch?v=i79VouVkrAg))
this looks like a good balance between the complexity of the
world (that we want to be high) and the resources used
(that we want to be low).

The package is based on [Crystal Space](http://www.crystalspace3d.org/main/Main_Page),
a mature, full-featured Software Development Kit (SDK) providing 
real-time 3D graphics for applications such as games and virtual 
reality. It is free (LGPL) and cross-platform (Windows, 
GNU/Linux, Mac OS X).

Attempting to get and compile the sources according to the [guide][guide]
in Ubuntu 13.04 64-bit
resulted in failure to build Crystal Space the first time (using jam):

	LinkPlugin xwin.so 
	ld error: linker defined: multiple definition of '_edata'
	ld ./out/linux/debug/plugins/video/canvas/xwindow/libgobject-2.0.so.0.stub.o: previous definition here
	ld error: linker defined: multiple definition of '__bss_start'
	ld ./out/linux/debug/plugins/video/canvas/xwindow/libgobject-2.0.so.0.stub.o: previous definition here
	ld error: linker defined: multiple definition of '_end'
	ld ./out/linux/debug/plugins/video/canvas/xwindow/libgobject-2.0.so.0.stub.o: previous definition here
	collect2: error: ld returned 1 exit status

	LinkApplication walktest 
	g++: error: ./out/linux/debug/apps/walktest/walkcd.o: No such file or directory

A second attempt using make ended up the same way. So I've set-up a
virtual machine using vagrant and lxc:

	Vagrant.configure("2") do |config|
	  config.vm.box = "raring64"
	  config.vm.hostname = "planeshift"
	end

I used the [LXC provider for Vagrant](https://github.com/fgrehm/vagrant-lxc)

	sudo apt-get install vagrant lxc lxc-templates cgroup-lite redir
	sudo apt-get update && sudo apt-get dist-upgrade
	vagrant plugin install vagrant-lxc
	vagrant box add raring64 http://bit.ly/vagrant-lxc-raring64-2013-07-12

and this time [Crystal Space](http://www.crystalspace3d.org/main/Main_Page)
was build


[root]: {{ site.baseurl }}/rendering/2013/09/20/software-for-virtual-world-rendering.html
[pshift]: http://g3d.sourceforge.net/
[guide]: https://svn.code.sf.net/p/planeshift/code/trunk/docs/compiling.html
