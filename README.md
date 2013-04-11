# Kade

## Introduction

Kade is a virtual secretary and information system build off of [The Toolkit for Building Voice Interaction Systems (MMDAgent)](http://www.mmdagent.jp/) and custom buit dialog manager developed by the Language Technologies Institute at Carnegie Mellon University.

Kade uses the [Flite](http://www.speech.cs.cmu.edu/flite/) engine for TTS and [PocketSphinx](http://cmusphinx.sourceforge.net/) for speech recognition along with the [OAQA](https://mu.lti.cs.cmu.edu/trac/oaqa2.0/) System for question answering.

## Building Kade

Currently Kade is only officially supported on Ubuntu 12.04 although it should work well on any recent UNIX distro. It may work on other platorms (especially Windows), but support is not guaranteed. Also, if you have any of the dependencies please feel free to skip those steps.

### Dependencies

#### Packages

    apt-get install subversion git autoconf libtool automake bison g++ libftgl-dev libfreetype6-dev libxrandr-dev libasound2-dev python2.7-dev wine libpulse-dev

#### CMU Sphinx
1. Make a new directory to install cmusphinx, inside that directory checkout the following repo:

        svn checkout svn://svn.code.sf.net/p/cmusphinx/code/trunk cmusphinx-code

2. First go into the sphinxbase directory and install sphinxbase

        cd sphinxbase
        ./autogen.sh
        make
        sudo make install

3. Then go into the pocketsphinx directory and install pocketsphinx on top of sphinxbase

        cd pocketsphinx
        ./autogen.sh
        make
        sudo make install

#### Freetype
Freetype2 is not very smart and doesn't make a symlink where it should so one has to be created manualy

    sudo ln -s /usr/include/freetype2/freetype /usr/include/freetype

#### LWP
You need the LWP perl bundle to compile the grammar, so lets install that now

    perl -MCPAN -e 'install Bundle::LWP'

### Installing Kade

1.  First clone the repo:

        git clone https://github.com/minigeek/MMDAgent.git

2.  Then initialize the submodules

        git submodule init
        git submodule update

3. Inside the `Library_Phoneix` directory run `./configure`

4. Update the location of your MMDAgent folder in `Plugin_Kade/Kade_Thread.cpp` (around line 119, replace `home/robocep` with the location of your `MMDAgent` folder).

5.  Then try:

        make x11-utf-8

6.  Assuming all went well we will now build the grammar files (which don't build by default)

        ./Release/AppData/Logios/CompileLanguage.sh

7.  If everything went well so far then try running MMDAgent (you may still be missing some dependencies, this is still a work in progress).

        ./Release/MMDAgent

Pretty much, it's awful.

## Contributors

 - Alan Black
 - Chase Brownell
 - Shane Smith

## Copywrite

The Kade System is released under the -------- License, for more details see Copywrite.txt.
