---
layout: post
title:  "OGRE"
date:   2013-09-20 12:00:00
categories: rendering
---

This page is part of the series that explores existing
[software for virtual world rendering]({{ site.url }}/rendering/2013/09/20/software-for-virtual-world-rendering.html).

Since 2001, 
[Object-Oriented Graphics Rendering Engine][ogre]
has grown to become one of the most popular open-source 
graphics rendering engines, and has been used in a 
large number of production projects, in such diverse 
areas as games, simulators, educational software, 
interactive art, scientific visualization, and others.
Here's [Wikipedia page](http://en.wikipedia.org/wiki/OGRE).

This was the way I thought I would head on. Open source, with 
lots of features, running on Windows, Mac and Linux. 
Unfortunately, the reality  is harsh: it is hard (and takes long)
to build the engine and, when you're done and you install it,
you find that you can't build the simplest of examples.

The excuse that it is a complex piece of software is just 
that: an excuse. The project has simply grown out of the
hands of the team that manages it. 
The [forum](http://www.ogre3d.org/forums/) shows 
[initiatives ](http://www.ogre3d.org/forums/viewtopic.php?f=25&t=78165)
to simplify it being discarded. The MinGW build (
[once you get it right](http://www.ogre3d.org/forums/viewtopic.php?f=2&t=76798)
) fails to link. Various 
[wiki pages](http://www.ogre3d.org/tikiwiki/tiki-index.php)
provide conflicting information at times and many are obsolete.


Here is a script that I thought I would use to help the users:

    @echo off

    REM =========    main variables    =========
    set CURRENT_DIR=%CD%
    set SOURCE_NAME=trunk
    set BUILD_NAME=build-%SOURCE_NAME%
    set INSTALL_NAME=install-%SOURCE_NAME%
    set VERSION=v2-0

    REM =========   derivate variables  =========
    set OGRE_SOURCE_DIR=%CURRENT_DIR%\%SOURCE_NAME%
    set OGRE_BUILD_DIR=%CURRENT_DIR%\%BUILD_NAME%
    set OGRE_INSTALL_DIR=%CURRENT_DIR%\%INSTALL_NAME%
    set OGRE_DEPENDS_DIR=%OGRE_SOURCE_DIR%\Dependencies

    REM =========   system variables  =========
    set BOOST_ROOT=C:\boost\boost_1_54_0
    set Boost_DIR=C:\boost\boost_1_54_0
    set MERCURIAL=D:\Program Files\TortoiseHg\hg.exe
    set CMAKE=D:\Program Files\Cmake\bin\cmake.exe
    set CMAKEGUI=D:\Program Files\Cmake\bin\cmake-gui.exe
    call ... set MinGW variables ...
    cd %CURRENT_DIR%


    echo =======================================================
    echo Get OGRE source
    echo =======================================================
    :get_source
    if not exist %OGRE_SOURCE_DIR% goto checkout_source
    :update_source

    pushd %OGRE_SOURCE_DIR%
    "%MERCURIAL%" pull && (echo pull success) || (goto final_error)
    "%MERCURIAL%" update %VERSION% && (echo update success) || (goto final_error)
    popd

    goto source_up_to_date
    :checkout_source

    "%MERCURIAL%" clone https://bitbucket.org/sinbad/ogre %OGRE_SOURCE_DIR% && (echo clone success) || (goto final_error)
    pushd %OGRE_SOURCE_DIR%
    "%MERCURIAL%" update %VERSION% && (echo update success) || (goto final_error)
    popd

    :source_up_to_date
    echo =======================================================


    echo =======================================================
    echo Get OGRE dependencies
    echo =======================================================
    :get_depends
    if not exist %OGRE_DEPENDS_DIR% goto checkout_depends
    :update_depends

    pushd %OGRE_DEPENDS_DIR%
    "%MERCURIAL%" pull && (echo pull success) || (goto final_error)
    "%MERCURIAL%" update && (echo update success) || (goto final_error)
    popd

    goto depends_up_to_date
    :checkout_depends

    pushd %OGRE_SOURCE_DIR%
    "%MERCURIAL%" clone https://bitbucket.org/cabalistic/ogredeps Dependencies && (echo clone success) || (goto final_error)
    popd

    :depends_up_to_date
    echo =======================================================


    echo =======================================================
    echo Create directories
    echo =======================================================
    :create_directories
    if exist %OGRE_BUILD_DIR% goto OGRE_BUILD_DIR_exists
    mkdir %OGRE_BUILD_DIR%
    :OGRE_BUILD_DIR_exists

    if exist %OGRE_INSTALL_DIR% goto OGRE_INSTALL_DIR_exists
    mkdir %OGRE_INSTALL_DIR%
    :OGRE_INSTALL_DIR_exists
    echo =======================================================


    echo =======================================================
    echo Run CMake
    echo =======================================================
    :run_cmake
    if exist %OGRE_BUILD_DIR%\CMakeCache.txt goto run_make
    pushd %OGRE_BUILD_DIR%
    "%CMAKE%" ..\%SOURCE_NAME% -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX="%OGRE_INSTALL_DIR%" && (echo cmake 1) || (goto final_error)
    "%CMAKE%" ..\%SOURCE_NAME% && (echo cmake 2) || (goto final_error)
    "%CMAKE%" ..\%SOURCE_NAME% && (echo cmake 3) || (goto final_error)
    "%CMAKEGUI%" ..\%SOURCE_NAME% && (echo cmake 4) || (goto final_error)
    popd
    echo =======================================================


    echo =======================================================
    echo Run make
    echo =======================================================
    :run_make
    pushd %OGRE_BUILD_DIR%
    mingw32-make.exe -j 4 && (echo make) || (goto final_error)
    mingw32-make.exe install && (echo make) || (goto final_error)
    popd
    echo =======================================================


    :final_ok
    echo %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    echo !!                                                   !!
    echo !!                       OK                          !!
    echo !!                                                   !!
    echo %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    @echo on
    pause
    exit /b 0


    :final_error
    echo %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    echo !!                  FATAL ERROR                      !!
    echo !!                                                   !!
    echo !!         Did not succeed in building OGRE          !!
    echo %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    @echo on
    pause
    exit /b 1



[ogre]: (http://www.ogre3d.org/)
