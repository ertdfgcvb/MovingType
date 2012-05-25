-------------------------------------------------------------------------------
MovingType

An animated alphabet.
Built and created for a two days workshop with first year students 
at SUPSI, Lugano
Adapted from a previous AS3 version
by
Andreas Gysin
2007-2012

-------------------------------------------------------------------------------
Purpose

This quite simple program permits to display image sequences associated 
to keystrokes. It permits – but is not limited to – building 
animated alphabets, hence the name.

-------------------------------------------------------------------------------
Examples

A few examples built by students are included with the application: 
press F1 to F12 to load different font sets (wait for loading).

-------------------------------------------------------------------------------
Extra keys

Command F   toggle fullscreen
Command X   reset
Command Q   quit
F1 - F12    load sets 1 to 12

-------------------------------------------------------------------------------
How to add a new set

Each set consists of a series of animations in the form of an image sequence
and a configuration file (below) all residing in an arbitrarily named folder.
The images for each animation have to reside in its own folder 
and will be displayed once the correspondent keystroke has been pressed.

Folder naming conventions:

 - space
 - lowercase_a
   lowercase_b
   to
   lowercase_z
 - number_1
   number_2
   to
   number_0
 - uppercase_a
   uppercase_b
   to
   uppercase_z
 - char_33   !
   char_44   ,
   char_46   .
   char_58   :
   char_63   ?
   etc.
	
The images in the folder will be loaded in alphabetical order.
It could happen that the sequence will played back in the wrong order:
   1.jpg
   11.jpg
   2.jpg
   3.jpg
   etc.

to avoid this use leading zeroes:
   01.jpg
   02.jpg
   03.jpg	  
   11.jgg
   etc.

-------------------------------------------------------------------------------
The configuration file

Each set needs a configuration file to work properly. 
The file has to be named 'config.xml' and needs to reside in the set folder.

Keys in 'config.xml' the file:

< set />         specify name, year and author of the set
< margin />      specify margins (in pixels)
< font />        height of the font, leading and spacing (in pixels)
< background />  background color of the canvas, color ranges from 0 to 1.0
                 black   r = 0.0   g = 0.0   b = 0.0	
                 white   r = 1.0   g = 1.0   b = 1.0
                 orange  r = 1.0   g = 0.5   b = 0.0
< animation />   set loop (0 = off, 1 = loop, 2 = ping pong) 
                 and speed measured in frames (1 = fastest, 10 = slower, etc)	
< system />      set textureMode = 1 for best performance 
                 set textureMode = 0 for better memory usage

-------------------------------------------------------------------------------
Additional notes on animations

 - Animations can have different lengths (number of frames)
 - On program start the first set found in the "data" folder is loaded
 - To avoid long loading times (and eventual memory overflows) 
   frames should not exceed a height of 256px.

-------------------------------------------------------------------------------
To do

 - Asynchronous image loader with progress indicator
 - A WebGL port is planned (maybe)

-------------------------------------------------------------------------------
Author: 
	Andreas Gysin
	@andreasgysin

	http://ertdfgcvb.com/movingtype
	http://github.com/ertdfgcvb/MovingType	

-------------------------------------------------------------------------------
Copyright (c) 2012 Andreas Gysin

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published 
by the Free Software Foundation, either version 3 of the License, 
or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty 
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.
If not, see <http://www.gnu.org/licenses/>.