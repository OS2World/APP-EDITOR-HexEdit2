#********************************************************************
#
#            Copyright (C) 1994 Bennett C. Baker and B::Ware
#
#                          All rights reserved
#
#      Module: HEXEDIT2.MAK
#      Author: Bennett C. Baker
#      CIS ID: 70274,3147
#     Created: 14 March, 1994
#
# Description: Makefile for HexEdit/2
#
#        $Log$
#
#********************************************************************

#************  PRIMARY TARGET  **************************************

all:  hexedit2.exe   hexedit2.inf   hexedit2.hlp   genreg.exe

#************  INCLUDE FILES  ***************************************

#************  MAKE DIRECTIVES  *************************************

.SUFFIXES:

.SUFFIXES: .OBJ .CPP .RC

.path.obj   = zObj
.path.exp   = zObj

#************  MACROS  **********************************************

%if %defined(NDEBUG)
      BTYPE = RELEASE
      LFLAGS =
%else
      BTYPE = DEBUG
      LFLAGS = /DEBUG
%endif

#************  GENERAL RULES  ***************************************

.PROLOG:
      @%echo Building $(BTYPE) version of HEXEDIT/2

{}.cpp{zObj}.obj:
      icc /C /Ti /Fo$@ $<
###      icc /C /Ti /Wall $<
###      sb -Q $<
###      sbd -d hexedit2 $*._sb
###      sbd -a hexedit2 $*._sb
###      del $*._sb

.rc.res:
      rc -r $< $@

.ipf.hlp:
      ipfc $<

.ipf.inf:
      ipfc /inf $<

#************  DEPENDENCIES  ****************************************

#********** The HexEdit/2 Application and associated files

OBJLIST  =  h2main.obj    \
            h2cursor.obj  \
            h2edit.obj    \
            h2file.obj    \
            h2font.obj    \
            h2help.obj    \
            h2reg.obj     \
            h2screen.obj  \
            h2search.obj
   
h2main.obj:    h2main.cpp    hexedit2.h  h2resrc.h
h2cursor.obj:  h2cursor.cpp  hexedit2.h
h2edit.obj:    h2edit.cpp    hexedit2.h  h2resrc.h
h2file.obj:    h2file.cpp    hexedit2.h  h2resrc.h
h2font.obj:    h2font.cpp    hexedit2.h  h2resrc.h
h2help.obj:    h2help.cpp    hexedit2.h  h2resrc.h
h2reg.obj:     h2reg.cpp     hexedit2.h  h2resrc.h   h2reg.h
h2screen.obj:  h2screen.cpp  hexedit2.h  h2resrc.h
h2search.obj:  h2search.cpp  hexedit2.h  h2resrc.h

hexedit2.res:  $*.rc    $*.ico   $*.dlg   $*.h

hexedit2.hlp:  $*.ipf
hexedit2.inf:  $*.ipf

hexedit2.exe:: $*.res
   rc $*.res $@

hexedit2.exe:: $(OBJLIST)  hexedit2.def   hexedit2.res
   link386.exe $(LFLAGS) <@<
$[s,"+\n",$[m,"*.obj",$**]],
$@,
hexedit2.map,

hexedit2.DEF;
<
   rc $*.res $@

#********** The Registration Generator

genreg.obj:    genreg.cpp

genreg.exe:    genreg.obj
   link386.exe $(LFLAGS) <@<
$[s,"+\n",$[m,"*.obj",$**]],
$@,
$*.map,

$*.def;
<

#************  END OF FILE  *****************************************
