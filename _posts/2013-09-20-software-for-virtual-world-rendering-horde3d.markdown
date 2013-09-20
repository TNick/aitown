---
layout: post
title:  "Horde3D"
date:   2013-09-20 14:30:00
categories: rendering
---

This page is part of the series that explores existing
[software for virtual world rendering]( {{ site.baseurl }}/rendering/2013/09/20/software-for-virtual-world-rendering.html ).

Horde3D is a small open source 3D rendering engine
written in C++. 
It is written in an effort to create a graphics 
engine that offers the stunning visual effects 
expected in next-generation games while at the 
same time being as lightweight and conceptually 
clean as possible. Horde3D has a simple and 
intuitive interface accessible from virtually 
any programming language and is particularly 
suitable for rendering large crowds of 
animated characters in next-generation quality. 
[Horde3D][h3d] requires a fully OpenGL 2.0 
compatible graphics card. In terms of DirectX that 
means a card supporting at least 
Shader Model 2.0 or higher.

Since Beta4, Horde3D is licensed under the 
Eclipse Public License v1.0 (EPL). It is a quite 
liberal license and has less restrictions 
than the popular LGPL. Basically it allows 
you to use Horde3D in free and commercial 
projects as long as you contribute improvements 
like bug fixes, optimizations and code 
refactorings back to the community. The 
EPL allows static linking and is not viral, 
hence it does not affect any other 
modules of your application.

The project uses [GLFW](http://www.glfw.org/) 
in samples, an Open Source, multi-platform library for 
creating windows with OpenGL contexts and managing 
input and events. It is easy to integrate into 
existing applications and does not lay claim to the main loop.

It has a [forum](http://horde3d.org/forums/) and a 
[wiki](http://horde3d.org/wiki/index.php5?title=Main_Page).
An [editor](http://horde3d.org/wiki/index.php5?title=Horde3D_Scene_Editor)
is also provided.

It is simply gorgeous, with a CMake based build system
that builds the engine in no time. The documentation
is lacking from what I can tell
(the [tutorial](http://horde3d.org/docs/html/_tutorial.html)
misses some important steps but the Sample directory
has ready to use applications).

I have yet to read [Usage Guide](http://horde3d.org/docs/html/_usageguide.html).
It lacks goodies like integrated audio 
and a physics engine, from what I can tell.
If I had more time this would have been my first option.
Bookmarked it for future use.


[h3d]: (http://horde3d.org/)
