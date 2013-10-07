#********************************************************************
#
#            Copyright (C) 1994 Bennett C. Baker and B::Ware
#
#                          All rights reserved
#
#      Module: MAKEFILE.MAK
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

all:        hexedit2  genreg

hexedit2:   hexedit2.exe   hexedit2.inf   hexedit2.hlp

genreg:     genreg.exe

#************  INCLUDE FILES  ***************************************

#************  MAKE DIRECTIVES  *************************************

.SUFFIXES:

.SUFFIXES: .OBJ .CPP .RC

#.path.obj   = zObj
#.path.exp   = zObj

#************  MACROS  **********************************************

!IFDEF NDEBUG
BTYPE = RELEASE
LFLAGS =
!ELSE
BTYPE = DEBUG
LFLAGS = /DEBUG
!ENDIF

#************  GENERAL RULES  ***************************************

#.PROLOG:
#      @%echo Building $(BTYPE) version of HEXEDIT/2

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

OBJLIST  =  zobj\h2main.obj    \
            zobj\h2cursor.obj  \
            zobj\h2edit.obj    \
            zobj\h2file.obj    \
            zobj\h2font.obj    \
            zobj\h2help.obj    \
            zobj\h2reg.obj     \
            zobj\h2screen.obj  \
            zobj\h2search.obj
   
zobj\h2main.obj:    h2main.cpp    hexedit2.h  h2resrc.h

zobj\h2cursor.obj:  h2cursor.cpp  hexedit2.h

zobj\h2edit.obj:    h2edit.cpp    hexedit2.h  h2resrc.h

zobj\h2file.obj:    h2file.cpp    hexedit2.h  h2resrc.h

zobj\h2font.obj:    h2font.cpp    hexedit2.h  h2resrc.h

zobj\h2help.obj:    h2help.cpp    hexedit2.h  h2resrc.h

zobj\h2reg.obj:     h2reg.cpp     hexedit2.h  h2resrc.h   h2reg.h

zobj\h2screen.obj:  h2screen.cpp  hexedit2.h  h2resrc.h

zobj\h2search.obj:  h2search.cpp  hexedit2.h  h2resrc.h

hexedit2.res:  $*.rc    $*.ico   $*.dlg   $*.h

hexedit2.hlp:  $*.ipf

hexedit2.inf:  $*.ipf

hexedit2.exe:: $(OBJLIST)  hexedit2.def   hexedit2.res
   link386.exe $(LFLAGS) @<<hexedit2.lnk
$(OBJLIST:.obj=.obj+
)
$@,
hexedit2.map,
,
hexedit2.DEF;
<<KEEP
   rc $*.res $@

###hexedit2.exe:: $*.res
###   rc $*.res $@

#********** The Registration Generator

genreg.obj:    genreg.cpp

genreg.exe:    genreg.obj
   link386.exe $(LFLAGS) @<<genreg.lnk
genreg.obj
$@,
$*.map,
,
$*.def;
<<KEEP

#************  END OF FILE  *****************************************
