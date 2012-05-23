===============================================================================
   The Toolkit for Building Voice Interaction Systems "MMDAgent" version 1.2
                           release December 25, 2011


The Toolkit for Building Voice Interaction Systems "MMDAgent"
(http://www.mmdagent.jp) has been developed by the MMDAgent project team (see
"Who we are" below).

*******************************************************************************
                                    Copying
*******************************************************************************

The MMDAgent is released under the Modified BSD license (see
http://www.opensource.org/). Using and distributing this software is free
(without restriction including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of this
work, and to permit persons to whom this work is furnished to do so) subject to
the conditions in the following license:

/* ----------------------------------------------------------------- */
/*           The Toolkit for Building Voice Interaction Systems      */
/*           "MMDAgent" developed by MMDAgent Project Team           */
/*           http://www.mmdagent.jp/                                 */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2011  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the MMDAgent project team nor the names of  */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

Although this software is free, we still offer no warranties and no
maintenance. We will continue to endeavor to fix bugs and answer queries when
can, but are not in a position to guarantee it. We will consider consultancy if
desired, please contacts us for details.

If you are using the MMDAgent in commercial environments, even though no
license is required, we would be grateful if you let us know as it helps
justify ourselves to our various sponsors. We also strongly encourage you to

 * refer to the use of MMDAgent in any publications that use this software
 * report bugs, where possible with bug fixes, that are found

See also "COPYRIGHT.txt" files as follows for details.

 .\COPYRIGHT.txt
 .\AppData\Julius\lang_m\COPYRIGHT.txt
 .\AppData\Julius\phone_m\COPYRIGHT.txt
 .\AppData\Open_JTalk\COPYRIGHT.txt

*******************************************************************************
                                  How to try
*******************************************************************************

Double click MMDAgent.exe file.

*******************************************************************************
                            Key and mouse bindings
*******************************************************************************

Key                          Action
-------------------------------------------------------------------------------
arrow keys                   rotate viewpoint
SHIFT + arrow keys           translate viewpoint
+, -                         zoom in, zoom out
ESC                          quit

X                            toggle self-shadowing
E, or SHIFT + E              change edge width
L, or SHIFT + L              toggle eye gaze control (follow mouse cursor)
P                            toggle physics simulation
W                            toggle wireframe view

F                            enable/disable full-screen
S                            show/hide FPS
D, or SHIFT + D              show/hide log
B                            show/hide bones
SHIFT + W                    show/hide rigid bodies
SHIFT + J                    show/hide log of speech recognition
SHIFT + F                    show/hide log of voice interaction manager

SHIFT + V                    enable/disable synchronization with VSync
SHIFT + X                    change rendering order of self-shadowing
CTRL + left, right           adjust latency of audio file play back
DELETE                       delete selected model(s) previously selected by
                             double click
H                            hold all motions

Mouse                        Action
-------------------------------------------------------------------------------
drag                         rotate viewpoint
SHIFT + drag                 translate viewpoint
wheel                        zoom in, zoom out
SHIFT + CTRL + drag          change light direction
CTRL + drag a model          translate the XZ position of the model
SHIFT + CTRL + drag a model  translate the XY position of the model
double click a model         select the model (see "File drop" below)

*******************************************************************************
                                   File drop
*******************************************************************************

File extention   Key         Action
-------------------------------------------------------------------------------
pmd              --          swap the target model, keeping motion(s)
pmd              CTRL        add a new model
vmd              --          give a motion to the target model
vmd              CTRL        give a motion to all models
xpmd             --          change the stage
bmp/tga/png/jpg  --          change the texture of the floor
bmp/tga/png/jpg  CTRL        change the texture of the background
mp3              --          play back audio

Note

 * the target model is a model at the dropped point, or you can pre-select by a
   double click
 * when "file.mp3" is dropped, "file.vmd" will be given to all models
 * the background and stage can be changed by a drag-and-drop of a pmd file,
   with the extention "xpmd"

*******************************************************************************
                               Acknowledgements
*******************************************************************************

Keiichi Tokuda
Akinobu Lee
Keiichiro Oura

*******************************************************************************
                                  Who we are
*******************************************************************************

The MMDAgent project team is a voluntary group for developing the Toolkit for
Building Voice Interaction System. Current members are

 Keiichi Tokuda      http://www.sp.nitech.ac.jp/~tokuda/
 Akinobu Lee         http://www.sp.nitech.ac.jp/~ri/
 Keiichiro Oura      http://www.sp.nitech.ac.jp/~uratec/

and the members are dynamically changing. The current formal contact address of
MMDAgent project team and a mailing list for MMDAgent users can be found at
http://www.mmdagent.jp/
===============================================================================
