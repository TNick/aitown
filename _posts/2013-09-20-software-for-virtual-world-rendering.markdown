---
layout: post
title:  "Software for virtual world rendering"
date:   2013-09-20 08:38:00
categories: rendering
---

We will look at various open source projects that
may be used to accomplish our goals. Each project 
that comes close will have its own post referenced 
in this page.

The ideal project would have a client-server architecture,
with the server-side part being able to handle physics
and custom avatars. The client side should work
on popular OSs like Windows and Linux.

The server should maintain a scene graph that it sends to
clients or the interface should be simple enough to allow
writing custom code to do that in a reasonable time frame.
The server also needs to send streams of raw data
to be used by the actor (video, audio, touch, smell).
On the other hand, if there is no such project 
(that is likely) the server-side plug-in may decide what 
streams of data it sends, thus lowering the requirements of
the searched project.

The client needs to allow the user to view/hear the scene 
independent of the actor and as the actor sees it. What we're looking
for is a world as complex as possible using as few resources
as possible (because they need to be transmitted over the network, that
is slow).

-------------------------------------------------------------

Here is the list of pages exploring various projects:

* [Unity3D]({{ site.baseurl }}/rendering/2013/09/20/software-for-virtual-world-rendering-unity3d.html)
* [OGRE]({{ site.baseurl }}/rendering/2013/09/20/software-for-virtual-world-rendering-ogre.html)
* [ioquake3]({{ site.baseurl }}/rendering/2013/09/20/software-for-virtual-world-rendering-ioquake3.html)
* [Horde3D]({{ site.baseurl }}/rendering/2013/09/20/software-for-virtual-world-rendering-horde3d.html)
* [G3D]({{ site.baseurl }}/rendering/2013/09/20/software-for-virtual-world-rendering-g3d.html)
* [PlaneShift]({{ site.baseurl }}/rendering/2013/09/20/software-for-virtual-world-rendering-planeshift.html)
* [Delta3D]({{ site.baseurl }}/rendering/2013/09/20/software-for-virtual-world-rendering-delta3d.html)
* [other projects]({{ site.baseurl }}/rendering/2013/09/20/software-for-virtual-world-rendering-other.html)

Apart from [Google](https://www.google.ro), following pages helped 
this survey:

* [UC Santa Cruz](http://courses.soe.ucsc.edu/courses/cmps164/Spring12/01/pages/enginelist)
* [sourceforge MMORPG](http://sourceforge.net/directory/games/mmorpg/)

