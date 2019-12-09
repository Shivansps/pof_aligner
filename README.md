# pof_aligner
POF data alignment for FS2 Open

/********************************************************************/
	POF ALIGNER v0.1 - Cipolla Salvador shivanuo@hotmail.com
	  License? You can use it and do wharever you want.
/********************************************************************/

POF ALIGNER aligns the .pof files that Freespace 2 uses so they are
up to original Volition standarts and can be parsed by ARM cpus. 

The reason of this is that ModelView32 is removing space padding and 
PCS2 does not enforces it, this causes the pof files to end with
pointers offsets in unaligned position, that may end in a SEGBUS.

How to compile:
---------------
Codeblocks project file is provided.

How to use
-----------
Command line argument parsing is very basic, please respect all lowercases and arguments order.

Show POF Info: If you want to check files
-----------------------------------------
 
*To view a single file:
pof_aligner -view -pof example.pof

*To view a folder
This lists all files one after the other:
pof_aligner -view -fol_in d:\exampleFolder\exampleFolder2

This lists all files but pauses and clear screen after each file:
pof_aligner -view -fol_in d:\exampleFolder\exampleFolder2 -pause

*To view a VP file
This lists all files in a VP one after the other:
pof_aligner -view -vp_in d:\exampleVP.vp

This lists all files in a VP but pauses and clear screen after each file:
pof_aligner -view -vp_in d:\exampleVP.vp -pause



Align files : This fixes the file and create a new, aligned version of them
---------------------------------------------------------------------------
*Single file
pof_aligner -align -fil_in d:\example_in.pof -fil_out d:\example_out.pof

*Entire Folder
pof_aligner -align -fol_in d:\example_folder_in -fol_out d:\example_folder_out

*VP File
pof_aligner -align -vp_in d:\example.vp

This will create the new aligned files in the same folder the executable it is,
this is temporal, the code to directly create a new vp file is not working yet.



List VP Files:
--------------
pof_aligner -list -vp d:\example.vp

No much usefull yet, this is meant to check the output VP file contents,
this displays all files inside a vp.


TODO
-----
-Fix the code to create the VP file directly
-.net GUI

BUGS
----
Im interested in pof files that cant be processed or results in wrong data.
If you find a case like that please provide the pof file.

Features
--------
-POF chunk alignment of:
FUEL, GLOW, SPCL, TXTR, DOCK, PATH, SLDC, OBJ2, SOBJ.

-BSP DATA alignment of
DEFPOINTS.

-String Alignment
All lengths divisible by 4, if string is empty, length = 4 and 4 null terminators.
