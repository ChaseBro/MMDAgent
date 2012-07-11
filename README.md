# Kade

## Introduction

Kade is a virtual secretary and information system build off of [The Toolkit for Building Voice Interaction Systems (MMDAgent)](http://www.mmdagent.jp/) and custom buit dialog manager developed by the Language Technologies Institute at Carnegie Mellon University.

Kade uses the [Flite](http://www.speech.cs.cmu.edu/flite/) engine for TTS and [PocketSphinx](http://cmusphinx.sourceforge.net/) for speech recognition along with the [OAQA](https://mu.lti.cs.cmu.edu/trac/oaqa2.0/) System for question answering.

## Building Kade

Currently Kade is only officially supported on Ubuntu 12.04 although it should work well on any recent UNIX distro. It may work on other platorms (especially Windows), but support is not guaranteed.

1.  First, apt-get the following things:
        apt-get install libpulse-dev libftgl-dev libfreetype-dev

2.  Then try:
        make x11-utf-8
3.  Assuming all went well we will now build the grammar files (which don't build by default)
        ./Release/AppData/Logios/CompileLanguage.sh
4.  If everything went well so far then try running MMDAgent (you may still be missing some dependencies, this is still a work in progress).
        ./Release/MMDAgent

Pretty much, it's awful.

## Contributors

 - Alan Black
 - Chase Brownell

## Copywrite

The Kade System is released under the -------- License, for more details see Copywrite.txt.
