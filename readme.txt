








                               H E X E D I T / 2

                               =================

                                  Version 1.04



                An OS/2 Presentation Manager binary file editor.





            Copyright (c) 1994 - 1996 Bennett C. Baker and B::Ware.

                              All rights reserved.















                                    B::Ware
                              75 Greenlodge Street
                               Dedham, MA  02026
                                  617-326-4719

                         Compuserve     70274,3147
                         Internet       bcb@bware.com


     INTRODUCTION

     Thank you for evaluating HexEdit/2, a shareware OS/2 Presentation
     Manager binary file editor from B::Ware.  With HexEdit/2, you can view
     and edit files in single or multibyte octal, decimal, or hexadecimal
     binary representation.  You can resize an existing file or allocate a
     new file from scratch, and perform search and replace functions
     forward or backward through the file.  HexEdit/2 provides a full help
     system as well as on-line documentation in the VIEW-compatible (.INF)
     format.

     LICENSE AGREEMENT

     HexEdit/2 is shareware.  You are free to use the program for a period
     of 21 days for the purpose of evaluation.  If you decide to continue
     using the program after the 30 day evaluation period, you must
     register it.  To register, send $20 (US funds, please) to:

          B::Ware
          75 Greenlodge Street
          Dedham, MA  02026
          USA

     You may make copies of this program and distribute them freely, as
     long as you include all of the files that you originally received with
     your shareware copy.  When you give a shareware copy of this program
     to another person, you agree to inform them that their copy is to be
     used for a time limited evaluation period, and that they must register
     their copy if they continue to use the program after the end of the
     evaluation period.  You may not sell or distribute this program for a
     fee in excess of media costs and/or shipping charges actually incurred
     in copying and distributing this program.

     DISCLAIMER

     IF USED IMPROPERLY, THIS PROGRAM IS CAPABLE OF DESTROYING ALL DATA ON
     YOUR COMPUTER SYSTEM.  THIS PROGRAM IS PROVIDED "AS-IS", WITH NO
     WARRANTY OF MERCHANTABILITY OR FITNESS OF PURPOSE.   IN NO EVENT SHALL
     B::WARE OR BENNETT C. BAKER BE LIABLE FOR ANY DAMAGES WHATSOEVER
     ARISING OUT OF USE OF THIS PROGRAM.

     INSTALLATION

     There are 7 files comprising the HexEdit/2 installation:

          HEXEDIT2.EXE   - the executable
          HEXEDIT2.HLP   - the on-line help file
          HEXEDIT2.INF   - the VIEWable primary documentation file
          README.TXT     - the README file (this file)
          REGISTER.TXT   - the registration form
          HISTORY.TXT    - the program's revision history
          FILE_ID.DIZ    - information for BBS systems

     These files can go just about anywhere, with the caveat that the
     HEXEDIT2.HLP file should be in either the HEXEDIT2.EXE directory or
     one of the directories listed in the HELP environment variable.



     ----------------------------------------------------------------------
     HexEdit/2 ver. 1.04           README.TXT                        Page 1


     GETTING STARTED

     The primary documentation file is HEXEDIT2.INF, readable with the OS/2
     VIEW command.

     HexEdit/2 can be opened from either the command line or as a WPS
     Program Object.  From the command line, it will accept a single
     argument indicating the file to display for edit (additional command
     line entries are ignored).  As a WPS Program Object, you can open it
     either by double-clicking on the Program Object icon or by dragging a
     Data File Object onto the icon (rudimentary drag-n-drop).

     If you don't want to edit an existing file, you can use the File
     Allocate and File Resize menus to create a new buffer, which can then
     be saved with the File Save As command.

     Buffer display has 3 fields: File Offset, Binary Data, and ASCII data.
     You can change the format of the File Offset field to octal, decimal,
     or hexadecimal.  You can use those same formats for the Binary Data
     field, and you can also specify how many bytes (1, 2, or 4) are
     contained in each entity of that field.  Multi-byte displays use
     little-endian format (like Intel and VAX processors).

     A buffer is normally displayed in Read-Only mode, meaning that you
     cannot edit it.  To edit a buffer, select Options/ReadOnly to toggle
     the Read-Only mode flag.

     You can edit a file in either its Binary or ASCII fields.  In the
     Binary field, you use the 0-9 and A-F characters appropriate to the
     selected display mode (for instance, you can't use the hexadecimal A-F
     characters if the display mode is octal or decimal).  In the ASCII
     field, any typeable character is an editing character.

     You can switch between Binary and ASCII fields with the TAB key, or by
     positioning the mouse cursor on any character and clicking button 1.

     You can perform Search or Search and Replace operations on data within
     the buffer.  Both Search and Replace strings can be ASCII strings or
     sequences of octal, decimal, or hex digits.

     You can also go to a specific file offset (specified in octal,
     decimal, or hex) within the editing buffer.

     HexEdit/2 allows you to use any of the fixed-pitch fonts available on
     your OS/2 system, and will save your font selection for subsequent
     use.

     SUPPORT

     Support for HexEdit/2 is provided on CompuServe in the OS2SHARE forum
     (section 1 of the OS2BVEN forum).  Address messages to Bennett C.
     Baker, 70274,3147.

     Internet users can send E-mail to 70274.3147@compuserve.com or
     bcb@bware.com.




     ----------------------------------------------------------------------
     HexEdit/2 ver. 1.04           README.TXT                        Page 2


     ACKNOWLEDGMENTS

     Thanks to:

     Larry Waibel, who first got me interested in writing a PM binary
     editor.

     Dave Levitt, who published an extensive feature set wish list (I'm
     getting there...).

     Guy Scharf, who answered my questions about becoming a vendor on
     OS2SHARE.

     Special thanks to Rhonda, who put up with me while I was wrestling
     with the innards of P.M. and the voluminous IBM documentation.

     ABOUT B::WARE

     B::Ware is a consulting firm with over 16 years experience in MS-DOS
     and OS/2 applications and system development, Novell NetWare NLM and
     driver development, real-time embedded systems hardware and software
     development, and general telecomm and ISDN protocols and
     implementations.  If you think we can help you solve a problem, please
     contact us at:

          B::Ware
          75 Greenlodge Street
          Dedham, MA  02026
          Voice/FAX  (617)-326-4719
          CompuServe 70274,3147
          Internet   bcb@bware.ultranet.com




























     ----------------------------------------------------------------------
     HexEdit/2 ver. 1.04           README.TXT                        Page 3