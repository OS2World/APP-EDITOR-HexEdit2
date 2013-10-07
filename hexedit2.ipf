.********************************************************************
.*
.*            Copyright (C) 1994 Bennett C. Baker and B::Ware
.*
.*                          All rights reserved
.*
.*       Module:  HEXEDIT2.IPF
.*       Author:  Bennett C. Baker
.*       CIS ID:  70274,3147
.*      Created:  14 March, 1994
.*
.*  Description:  Help and INF source for HexEdit/2, an OS/2 PM Hex Editor
.*
.*        $Log$
.*
.********************************************************************

:userdoc.
:docprof toc=1234.
:title.HexEdit/2 Help

.********************************************************************

:h1 res=1000 name=IDP_MAIN_WIN.About HexEdit/2
:i1 id=main.About HexEdit/2
:p.
:lines align=center.
:font facename=Helv size=28x14.
:hp9.HexEdit/2:ehp9.
.***:font facename=Helv size=18x10.
.***:hp9.version 1.03:ehp9.
:font facename=default size=0x0.
:elines.
:p.is a shareware OS/2 Presentation Manager binary file editor from
:hp9.B&colon.&colon.Ware:ehp9..  It
allows you to edit an existing file or create a new one.  You can view and
edit the file in hexadecimal, decimal, or octal representation, as an array
of 1, 2, or 4-byte entities.  In addition, file offsets can also be presented
as hexadecimal, decimal, or octal numbers.
:p.
HexEdit/2 is copyright (c) 1994 - 1996 by Bennett C. Baker and
B&colon.&colon.Ware.  All rights are reserved.
:ul compact.
:li.:link reftype=hd res=1005.Using HexEdit/2 - an overview:elink.
:li.:link reftype=hd res=1001.About B&colon.&colon.Ware:elink.
:li.:link reftype=hd res=1002.HexEdit/2 Support:elink.
:li.:link reftype=hd res=1003.HexEdit/2 License Agreement:elink.
:li.:link reftype=hd res=1004.Registering HexEdit/2:elink.
:eul.

:h2 res=1001 name=IDP_BWARE_WIN.About B&colon.&colon.Ware
:i1 id=bware.About B&colon.&colon.Ware
:p.B&colon.&colon.Ware is a consulting firm with over 16 years experience in&colon.
:lm margin=4.
:ul compact.
:li.MS-DOS and OS/2 applications and system development
:li.Novell NetWare NLM and driver development
:li.Real-time embedded systems hardware and software development
:li.General telecomm and ISDN protocols and implementations
:eul.
:lm margin=1.
:p.Call, write, or e-mail us at:
:lm margin=4.
:lines.
B&colon.&colon.Ware
75 Greenlodge Street
Dedham, MA  02026
USA
Voice/FAX&colon. (617)-326-4719
Compuserve&colon. 70274,3147
Internet&colon. bcb@bware.com
:elines.
:lm margin=1.

:h2 res=1002 name=IDP_SUPPORT_INFO.HexEdit/2 Support
:i2 refid=main.HexEdit/2 Support
:p.HexEdit/2 is supported on CompuServe in the OS2SHARE forum
(section 1 of OS2BVEN).  Send messages to Bennett C. Baker,
user ID 70274,3147.
:p.Internet users can address e-mail messages to 70274.3147@compuserve.com
or to bcb@bware.com.

:h2 res=1003 name=IDP_LICENSE_INFO.HexEdit/2 License Agreement
:i2 refid=main.HexEdit/2 License Agreement
:p.
:hp2.Evaluation Period:ehp2.  You may use this shareware copy of this program
for an evaluation period of up to 30 days, in order to determine whether the
program meets your needs before purchasing it.  Once the evaluation period
ends, you agree to either register this Program, or to stop using it.
:p.
:hp2.Making and Distributing Shareware Copies:ehp2.  You may make copies
of this program and distribute them freely, as long as you include all of
the files that you originally received with your shareware copy.  When you
give a shareware copy of this program to another person, you agree to inform
them that their copy is to be used for a time limited evaluation period, and
that they must register their copy if they continue to use the program after
the end of the evaluation period.  You may not sell or distribute this program
for a fee in excess of media costs and/or shipping charges actually incurred in
selling or distributing this program.
:p.
:hp2.Limitations on Liability:ehp2.  IF USED IMPROPERLY, THIS PROGRAM IS
CAPABLE OF DESTROYING ALL DATA ON YOUR COMPUTER SYSTEM.  THIS SOFTWARE IS
PROVIDED "AS-IS", WITH NO WARRANTY OF MERCHANTABILITY OR FITNESS OF
PURPOSE.   IN NO EVENT SHALL B&colon.&colon.WARE OR BENNETT C. BAKER BE LIABLE FOR ANY
INCIDENTAL, CONSEQUENTIAL, OR PUNITIVE DAMAGES WHATSOEVER ARISING OUT OF USE
OF THIS PROGRAM.

:h2 res=1004 name=IDP_REG_INFO.Registering HexEdit/2
:i2 refid=main.Registering HexEdit/2
:p.HexEdit/2 is shareware.  If you find it useful, you should register it.
When you decide to register, please send U.S. $20.00 (U.S. funds, please!)
to:
:lm margin=4.
:lines.
B&colon.&colon.Ware
75 Greenlodge Street
Dedham, MA  02026
USA
:elines.
:lm margin=1.
:p.There is a registration form in the REGISTER.TXT file for you
to fill out when you register HexEdit/2.  Please print this form,
fill it out, and send it in with your registration.  When we receive
your registration, we will send you a unique registration key to
personalize your copy of HexEdit/2.
:p.When you receive your registration key, select the Options/Register
menu option to enter the registration data into the product.  Once the
registration data has been properly entered, this menu option will no
longer display.

:h2 res=1005.HexEdit/2 Usage Overview
:i2 refid=main.HexEdit/2 Usage Overview
:p.HexEdit/2 allows you to manipulate files at the binary representation level.
:p.The
:link reftype=hd res=2000.File Menu:elink.
allows you to read and write existing files or create new ones.
:p.The
:link reftype=hd res=3000.Edit Menu:elink.
allows you to insert a block of nulls or delete a block of characters
from the existing file.
:p.The
:link reftype=hd res=4000.Search Menu:elink.
allows you to perform Search or Search and Replace functions within the currently
edited file using binary or ASCII search criteria.
:p.The
:link reftype=hd res=5000.Options Menu:elink.
controls HexEdit/2 configuration.
:p.Finally, the
:link reftype=hd res=6001.Display Dialog:elink.
allows you to select the format of displayed binary data.
:p.HexEdit/2 allows you to use a combination of
:link reftype=hd res=9501.Keyboard:elink.
and
:link reftype=hd res=10000.Mouse:elink.
controls to facilitate data manipulation.

.********************************************************************

:h1 res=2000 name=IDP_FILE.File Menu
:i1 id=File.File Menu
:p.The File menu contains the following choices&colon.
:dl tsize=13.
:dt.:link reftype=hd res=2100.Open:elink.
:dd.Opens and reads in a new file
:dt.:link reftype=hd res=2200.Save:elink.
:dd.Saves the current buffer into the current file name
(runs Save As if no current file name).
:dt.:link reftype=hd res=2300.Save As:elink.
:dd.Saves the current buffer under as user-selected file name.
:dt.:link reftype=hd res=2400.Clear:elink.
:dd.Clear the current buffer and erase the current file name.
:dt.:link reftype=hd res=2500.Allocate:elink.
:dd.Allocate a block of memory for editing.
:dt.:link reftype=hd res=2600.Resize:elink.
:dd.Resize either the current file or an allocated block of memory.
:dt.:link reftype=hd res=2900.Exit:elink.
:dd.Exit from HexEdit/2 without saving anything.
:edl.

:h2 res=2100 name=IDP_FILE_OPEN.File Open
:i2 refid=File.Open
:p.Use File Open to bring up the
:link reftype=hd res=2101.File Open Dialog:elink.
to read in an existing file for editing.  The file can be any
length, up to the limit allowed by the operating system.

:h3 res=2101 name=IDP_FILE_OPEN_DLG.File Open Dialog
:i2 refid=File.File Open Dialog
:p.Use File Open to open a file, read it into the current buffer space, and
then close the file.  The current buffer will be identical to the file size.
:p.You can select a file with the File Open Dialog in several ways.  If you
already know the file's name and directory, you can enter that information
by typing directly into the
:hp2.Open Filename&colon.:ehp2.
field.  If you don't know the filename, you can browse for it by using the
:hp2.Type of File&colon.:ehp2.,
:hp2.Drive&colon.:ehp2.,
:hp2.Directory&colon.:ehp2.,
and
:hp2.File&colon.:ehp2.
fields of the dialog.
:p.When you see the file you want, either double-click on the entry in the
:hp2.File&colon.:ehp2.
field, or highlight that file name (still in the
:hp2.File&colon.:ehp2.
field) and click on the
:hp2.OK:ehp2.
button.
:p.You can cancel the file selection process by clicking on the
:hp2.Cancel:ehp2.
button (or by hitting the
:hp2.Esc:ehp2.
key).

:h2 res=2200 name=IDP_FILE_SAVE.File Save
:i2 refid=File.Save
:p.Use File Save to save the current buffer under the current file name.
If there is no current file name (for instance, if you have done a File Allocate)
this selection will bring up the File Save As menu instead.

:h2 res=2300 name=IDP_FILE_SAVE_AS.File Save As
:i2 refid=File.Save As
:p.Use File Save As to bring up the
:link reftype=hd res=2301.File Save Dialog:elink.
to save the current buffer under the current file name.

:h3 res=2301 name=IDP_FILE_SAVE_AS_DLG.File Save As Dialog
:i2 refid=File.File Save As Dialog
:p.Use File Save As to save the current buffer into a disk file.
:p.You can select a file with the File Save As Dialog in several ways.  If you
already know the name and directory, of the file you want to save, you can
enter that information directly into the
:hp2.Save as Filename&colon.:ehp2.
field.  Otherwise, you can use the
:hp2.Drive&colon.:ehp2.
and
:hp2.Directory&colon.:ehp2.
fields of the dialog to maneuver into a chosen
directory, and then either select an existing name from the
:hp2.File&colon.:ehp2.
field (to overwrite that file) or type in a new name
in the
:hp2.Save as Filename&colon.:ehp2.
field.
:p.When you have selected a file name to save to, click on the
:hp2.OK:ehp2.
button.
:p.You can cancel the file selection process by clicking on the
:hp2.Cancel:ehp2.
button (or by hitting the
:hp2.Esc:ehp2.
key).

:h2 res=2400 name=IDP_FILE_CLEAR.File Clear
:i2 refid=File.Clear
:p.Use File Clear to clear the current buffer and current file name (if any).

:h2 res=2500 name=IDP_FILE_ALLOCATE.File Allocate
:i2 refid=File.Allocate
:p.Use File Allocate to bring up the
:link reftype=hd res=2501.File Allocate Dialog:elink.,
which allows the user to allocate buffer space for a new file.

:h3 res=2501 name=IDP_FILE_ALLOCATE_DLG.File Allocate Dialog
:i2 refid=File.File Allocate Dialog
:p.Use File Allocate to allocate current buffer space for a new file.  The
allocation must be non-zero.
:p.Enter the allocation quantity in the
:link reftype=hd res=2502.Desired Size :elink.
field.  The size is interpreted according to the setting in the
:link reftype=hd res=2503.Display Mode:elink.
field, so make sure that you are entering a number valid for the
display mode (for instance, don't use the hex digits 'A' through
'F' if you are editing in decimal).

:h3 hide res=2502 name=IDP_FILE_ALLOCATE_ENTRY.Desired Size
:p.Enter the desired allocation (i.e. file) size in this field, in a format
consistent with what is selected in the
:link reftype=hd res=2503.Display Mode:elink.
field.

:h3 hide res=2503 name=IDP_FILE_ALLOCATE_MODE.Display Mode
:p.The Display Mode selector allows you to select the mode (hex, decimal,
or octal) in which to interpret the value entered in the Desired Size
field.  The value in the Desired Size field must be valid for the selected
Display Mode.

:h2 res=2600 name=IDP_FILE_RESIZE.File Resize
:i2 refid=File.Resize
:p.Use File Resize to bring up the
:link reftype=hd res=2601.File Resize Dialog:elink.,
which allows the user to resize the current buffer space.

:h3 res=2601 name=IDP_FILE_RESIZE_DLG.File Resize Dialog
:i2 refid=File.File Resize Dialog
:p.Use File Resize to resize the current buffer space.  The new size must be
non-zero.
:p.
The
:link reftype=hd res=2602.Initial Size:elink.
field displays the current buffer size, in units specified by the
:link reftype=hd res=2604.Display Mode:elink.
field.  This field cannot be edited&colon.
it is there for information only.  Enter the desired new size in the
:link reftype=hd res=2603.New Size:elink.
field, but make sure you enter the
new number in a format acceptable to the Display Mode field.

:h3 hide res=2602 name=IDP_FILE_RESIZE_OLD.Initial Size
:p.This is the current size of the buffer, displayed in the current
:link reftype=hd res=2604.Display Mode:elink.
format.  This field cannot be edited&colon.
it is provided for information only.

:h3 hide res=2603 name=IDP_FILE_RESIZE_NEW.New Size
:p.Enter the desired new allocation size in this file, in units compatible with the
current
:link reftype=hd res=2604.Display Mode:elink.
format.

:h3 hide res=2604 name=IDP_FILE_RESIZE_MODE.Display Mode
:p.The Display Mode selector allows you to select the mode (hex, decimal,
or octal) in which to display the
:link reftype=hd res=2602.Initial Size:elink.
field and interpret the value entered in the
:link reftype=hd res=2603.New Size:elink.
field.  The value in the New Size field must be valid for the
selected Display Mode.

:h2 res=2900 name=IDP_FILE_EXIT.File Exit
:i2 refid=File.Exit
:p.Clear the current buffer and exit HexEdit/2.

.********************************************************************

:h1 res=3000 name=IDP_EDIT.Edit Menu
:i1 id=Edit.Edit Menu
:p.The Edit menu contains the following choices&colon.
:dl tsize=20.
:dt.:link reftype=hd res=3400.Insert:elink.
:dd.Insert a specified number of bytes into the buffer at the current
cursor location.  The buffer is expanded by the number of inserted bytes.
:dt.:link reftype=hd res=3500.Delete:elink.
:dd.Delete a specified number of bytes from the buffer starting at the
current cursor location.  The buffer is shrunk by the number of deleted
bytes.
:edl.

:h2 res=3400 name=IDP_EDIT_INSERT.Edit Insert
:i2 refid=Edit.Edit Insert
:p.Select Edit Insert to bring up the
:link reftype=hd res=3401.Edit Insert Dialog:elink.,
which allows the user to specify the number of bytes to insert into
the buffer at the current cursor location.  The inserted bytes are
all set to 0.

:h3 res=3401 name=IDP_EDIT_INSERT_DLG.Edit Insert Dialog
:i2 refid=Edit.Edit Insert Dialog
:p.Edit Insert will insert the specified number of bytes of zero (0x00)
into the buffer at the current cursor point; the newly-inserted bytes
can then be edited.

:h2 res=3500 name=IDP_EDIT_DELETE.Edit Delete
:i2 refid=Edit.Edit Delete
:p.Select Edit Delete to bring up the
:link reftype=hd res=3401.Edit Delete Dialog:elink.,
which allows the user to specify the number of bytes to delete from
the buffer starting at the current cursor location.

:h3 res=3501 name=IDP_EDIT_DELETE_DLG.Edit Delete Dialog
:i2 refid=Edit.Edit Delete Dialog
:p.Edit Delete deletes the specified number of bytes from the buffer
starting at the current cursor point.

.********************************************************************

:h1 res=4000 name=IDP_SEARCH.Search Menu
:i1 id=Search.Search Menu
:p.The Search menu contains the following choices&colon.
:dl tsize=20.
:dt.:link reftype=hd res=4100.Search Forward:elink.
:dd.Search for a string forward from the current buffer position.
:dt.:link reftype=hd res=4100.Search Backward:elink.
:dd.Search for a string backward from the current buffer position.
:dt.:link reftype=hd res=4100.Search Again:elink.
:dd.Repeat the previous search (data and direction).
:dt.:link reftype=hd res=4400.Replace Forward:elink.
:dd.Search and Replace for a string forward from the current buffer position.
:dt.:link reftype=hd res=4400.Replace Backward:elink.
:dd.Search and Replace for a string backward from the current buffer position.
:dt.:link reftype=hd res=4400.Replace Again:elink.
:dd.Repeat the previous Replace (data and direction).
:dt.:link reftype=hd res=4700.Go To Offset:elink.
:dd.Go to a specific offset within the current buffer.
:edl.

:h2 res=4100 name=IDP_SEARCH_SELECTIONS.Search Selections
:i2 refid=Search.Search Selections
:p.HexEdit/2 allows you to search the current buffer for any sequence of
octal, decimal, hex, or ASCII characters.  Select
:hp2.Search Forward:ehp2.
to search from the current cursor position to the end of the buffer,
:hp2.Search Backward:ehp2.
to search backward from the current cursor position to the beginning
of the buffer, or
:hp2.Search Again:ehp2.
to repeat the last search (same search string and direction).  Selecting
either Search Forward or Search Backward brings up the
:link reftype=hd res=4101.Search Dialog:elink.,
which allows you to control the search parameters.

:h3 res=4101 name=IDP_SEARCH_DLG.Search Dialog
:p.The Search Dialog controls the parameters of the search.  Enter the
string to search for in the
:link reftype=hd res=4102.Search String:elink.
field, and select how that string is to be interpreted in the
:link reftype=hd res=4103.Search Type:elink.
selection.

:h3 hide res=4102 name=IDP_SEARCH_STRING.Search String
:p.The
:hp2.Search String:ehp2.
is the string to search for.  Enter either an ASCII string or a sequence
of bytes in the format indicated by the
:link reftype=hd res=4103.Search Type:elink.
field.

:h3 hide res=4103 name=IDP_SEARCH_TYPE.Search Type Selection
:p.The Search Type field determines how the string entered in the
:link reftype=hd res=4102.Search String:elink.
or
:link reftype=hd res=4402.Replace String:elink.
field is interpreted.
:p.For a non-ASCII search (octal, decimal, or hex),
make sure that the byte sequence is in a format acceptable to
the Search Type field; for instance, do not enter any hex digits unless
the Search Type field is Hex.
:p.For an ASCII search, the string is interpreted as an ordinary ASCII
character string.  The
:hp2.Case Sens.:ehp2.
checkbox performs a case-sensitive search ASCII search when checked; it
has no meaning for the Replace String (the Replace String data is replaced
as is - it is :hp2.always:ehp2. case sensitive).

:h2 res=4400 name=IDP_REPLACE_SELECTIONS.Replace Selections
:i2 refid=Search.Replace Selections
:p.HexEdit/2 allows you to search the current buffer for any sequence of
octal, decimal, hex, or ASCII characters and replace that sequence with
an alternate sequence.  Select
:hp2.Replace Forward:ehp2.
to search and replace from the current cursor position to the end of the buffer,
:hp2.Replace Backward:ehp2.
to search and replace backward from the current cursor position to the beginning
of the buffer, or
:hp2.Replace Again:ehp2.
to repeat the last search and replace (same search string, replace string and direction).  Selecting
either Replace Forward or Replace Backward brings up the
:link reftype=hd res=4401.Replace Dialog:elink.,
which allows you to control the search and replace parameters.

:h3 res=4401 name=IDP_REPLACE_DLG.Replace Dialog
:p.The Replace Dialog controls the parameters of the search and replace.  Enter the
string to search for in the
:link reftype=hd res=4102.Search String:elink.
field, enter the replacement string in the
:link reftype=hd res=4402.Replace String:elink.
field, and select how those strings are to be interpreted in the
:link reftype=hd res=4103.Search Type:elink.
selection.  Both the Search and Replace strings are interpreted
in the same manner by the Search Type selection.

:h3 hide res=4402 name=IDP_REPLACE_STRING.Replace String
:p.The
:hp2.Replace String:ehp2.
is the string to replace the
:link reftype=hd res=4102.Search String:elink.
with.  Enter either an ASCII string or a sequence
of bytes in the format indicated by the
:link reftype=hd res=4103.Search Type:elink.
field.

:h2 res=4700 name=IDP_GOTO_OFFSET.Go To Offset
:i2 refid=Search.Go To Offset
:p.Use Go To Offset to bring up the
:link reftype=hd res=4701.Go To Offset Dialog:elink.,
which allows you to enter the buffer offset at which to position
the cursor.

:h3 res=4701 name=IDP_GOTO_OFFSET_DLG.Go To Offset Dialog
:p.Use this dialog box to position the cursor to any offset
within the current buffer.  Enter the desired offset in the
:hp2.Desired Offset:ehp2.
field, in the format indicated by the
:hp2.Offset Mode:ehp2.
field.
:p.You can select
:hp2.relative:ehp2.
offset positioning by prefixing the desired offset with either
a + or - sign, indicating positive or negative relative movement,
respectively.

.********************************************************************

:h1 res=5000 name=IDP_OPTIONS.Options
:i1 id=Options.Options Menu
:p.The Options menu contains the following choices&colon.
:dl tsize=13.
:dt.:link reftype=hd res=5100.Font:elink.
:dd.Selects a font for use by the client window.
:dt.:link reftype=hd res=5200.Read Only:elink.
:dd.Toggles the buffer Read Only flag.
:dt.:link reftype=hd res=5300.Auto Resize:elink.
:dd.Toggles the Auto Resize flag.
:dt.:link reftype=hd res=5400.Register:elink.
:dd.Run the product Registration dialog.
:edl.

:h2 res=5100 name=IDP_OPTIONS_FONT.Options/Font
:i2 refid=Options.Font
:p.The Font menu selection brings up a standard
:link reftype=hd res=5101.Font Dialog Box:elink.,
allowing the user to select which fixed-pitch font to use for
display purposes.  Note that only the currently available
fixed-pitch fonts appear in the selection list.

:h3 res=5101 name=IDP_OPTIONS_FONT_DLG.Font Selection Dialog
:i2 refid=Options.Font Selection Dialog
:p.The Font Selection dialog allows you to choose the font for
information display within the HexEdit/2 window.  Only fixed
fonts can be used.
:p.The selected font will be saved at program exit and used again
when the program is next run.  The initial default font is 10
point System Monospaced.

:h2 res=5200 name=IDP_OPTIONS_READONLY.Options/Read Only
:i2 refid=Options.Read Only
:p.The Read Only flag toggles whether the buffer is
editable.  This options defaults to
:hp2.on:ehp2.
(checked), but can be un-checked to allow editing within
the buffer.
:p.If this option is checked, you will not be able to edit
the buffer.

:h2 res=5300 name=IDP_OPTIONS_AUTORESIZE.Options/Auto Resize
:i2 refid=Options.Auto Resize
:p.The Auto Resize flag toggles whether the client window will
automatically resize when the display options (offset mode, main
display mode, bytes-per-entity) are changed.  This option defaults to
:hp2.off:ehp2.
(unchecked), but can be checked to allow automatic window resizing.

:h2 res=5400 name=IDP_OPTIONS_REGISTER.Options/Register
:i2 refid=Options.Register
:p.This option allows you to register HexEdit/2.  It does not appear
in a properly registered version.

.********************************************************************

:h1 res=6000 name=IDP_DISPLAY.Display Help
:p.The Display menu selection brings up the
:link reftype=hd res=6001.Display Dialog:elink.,
which allows the user to specify the format of on-screen information.

:h1 res=6001 name=IDP_DISPLAY_DLG.Display Dialog
:i1 id=Display.Display Dialog
:p.Use this dialog to alter the way information is displayed on the screen.
Change the file offset format with
:link reftype=hd res=6002.File Offsets:elink.
selection, change the default data representation with the
:link reftype=hd res=6003.Data Format:elink.
selection, and chose the number of bytes per item with the
:link reftype=hd res=6004.Bytes Per Item:elink.
selection.

:h2 res=6002 name=IDP_DISPLAY_OFFSET.File Offsets
:i2 refid=Display.File Offsets
:p.Use this field to select the format of the file offset display; octal,
decimal, or hexadecimal.

:h2 res=6003 name=IDP_DISPLAY_DATA.Data Format
:i2 refid=Display.Data Format
:p.Use this field to select the format of the main data display;
octal, decimal, or hexedecimal.  This field interacts with the
:link reftype=hd res=6004.Bytes Per Item:elink.
field to control the actual number of bytes/entities displayed on each line.

:h2 res=6004 name=IDP_DISPLAY_BYTE.Bytes Per Item
:i2 refid=Display.Bytes Per Item
:p.Use this field to select the number of bytes displayed in each
entity in the main data display.  This field interacts with the
:link reftype=hd res=6003.Data Format:elink.
field to control the actual number of bytes/entities displayed on each line.
:nt.
Multi-byte entities are displayed in
:hp2.Little-Endian:ehp2.
format (used by VAXen and Intel), with the first (least significant)
physical byte appearing at the end of the entity.
:ent.

.********************************************************************

:h1 res=9000 name=IDP_HELP.Help
:i1 id=Help.Help
:p.There are several entries in the Help menu&colon.
:dl tsize=16.
:dt.:link reftype=hd res=9100.Help Contents:elink.
:dd.Displays the Help Table of Contents
:dt.:link reftype=hd res=9200.Help Index:elink.
:dd.Displays the Help Index
:dt.:link reftype=hd res=9300.General Help:elink.
:dd.Displays General Help for HexEdit/2
:dt.:link reftype=hd res=9400.Using Help:elink.
:dd.Displays a guide on how to use the OS/2 help system.
:dt.:link reftype=hd res=9500.Keys Help:elink.
:dd.Displays a list of special keys used by HexEdit/2.
:dt.:link reftype=hd res=9600.About:elink.
:dd.Displays the "About HexEdit/2" dialog box.
:edl.

:h2 res=9100 name=IDP_HELP_CONTENTS.Help Table of Contents
:i2 refid=Help.Table of Contents
:p.Select Table of Contents to bring up the Table of Contents of HexEdit/2 help topics.

:h2 res=9200 name=IDP_HELP_INDEX.Help Index
:i2 refid=Help.Index
:p.Select Index to bring up the index of HexEdit/2 help topics.

:h2 res=9300 name=IDP_HELP_GENERAL.General Help
:i2 refid=Help.General
:p.Select General Help to bring up the initial HexEdit/2 help window (or just
hit the F1 key).

:h2 res=9400 name=IDP_HELP_USING.Using Help
:i2 refid=Help.Using
:p.Select Using Help to display a guide on how to use the OS/2 help system.

:h2 res=9500 name=IDP_HELP_KEYS.Keys Help
:i2 refid=Help.Keys
:p.Select Keys Help for a list of
:link reftype=hd res=9501.special keys:elink.
used by HexEdit/2.

:h2 res=9501 name=IDP_KEYS_INFO.Key Information
:p.Hexedit/2 supports cursor movement, data editing,
and hot-keys display formatting.
:ul.
:li.:link reftype=hd res=9502.Cursor Movement Keys:elink.
:li.:link reftype=hd res=9503.Data Editing Keys:elink.
:li.:link reftype=hd res=9504.Hot Keys:elink.
:li.:link reftype=hd res=10000.Mouse Support:elink.
:eul.

:h3 res=9502 name=IDP_MOVEMENT_KEYS.Cursor Movement Keys
:i2 refid=Help.Cursor Movement Keys
:dl compact tsize=16.
:dt.Up Arrow
:dd.Moves the cursor up one line
:dt.Ctrl-Up
:dd.Scrolls the screen up one line
:dt.Down Arrow
:dd.Moves the cursor down one line
:dt.Ctrl-Down
:dd.Scrolls the screen down one line
:dt.Left Arrow
:dd.Moves the cursor left one character
:dt.Right Arrow
:dd.Moves the cursor right one character
:dt.PgUp
:dd.Moves up one screenful
:dt.PgDn
:dd.Moves down one screenful
:dt.Home
:dd.Moves to the beginning of the buffer
:dt.End
:dd.Moves to the end of the buffer
:dt.Tab
:dd.Switches between Binary edit and ASCII edit fields
:dt.Spacebar
:dd.Moves the cursor right one character (only when in the
Binary edit field).
:edl.
:nt.You cannot edit the File Offset field (only the Binary and
ASCII fields).
:ent.

:h3 res=9503 name=IDP_EDITING_KEYS.Data Editing Keys
:i2 refid=Help.Data Editing Keys
:p.In the Binary field, you can use the standard 0-9, A-F keys
 to edit the data display in place.  You must use the appropriate
keys for the current
:link reftype=hd res=6003.Data Format:elink.&colon.
for instance, you cannot use the A-F keys if
the current Display Format is Octal or Decimal.
:p.In the ASCII field, type any displayable character (one with
an ASCII code greater than 0x1F) to edit the field.
:p.Remember that you cannot edit the file offset field.

:h3 res=9504 name=IDP_HOT_KEYS.Hot Keys
:i2 refid=Help.Hot Keys
:p.HexEdit/2 supports the following hot keys&colon.
:lm margin=4.
:sl compact.
:li.:link reftype=hd res=6002.Display File Offset Format:elink. Control
:dl compact tsize=13.
:dt.Ctrl-T
:dd.Oc:hp5.t:ehp5.al
:dt.Ctrl-C
:dd.De:hp5.c:ehp5.imal
:dt.Ctrl-X
:dd.He:hp5.x:ehp5.adecimal
:edl.
:li.:link reftype=hd res=6003.Display Data Format:elink. Control
:dl compact tsize=13.
:dt.Ctrl-O
:dd.:hp5.O:ehp5.ctal
:dt.Ctrl-D
:dd.:hp5.D:ehp5.ecimal
:dt.Ctrl-H
:dd.:hp5.H:ehp5.exadecimal
:edl.
:li.:link reftype=hd res=6004.Display Byte Count:elink. Control
:dl compact tsize=13.
:dt.Ctrl-1
:dd.:hp5.1:ehp5. byte per entity
:dt.Ctrl-2
:dd.:hp5.2:ehp5. bytes per entity
:dt.Ctrl-4
:dd.:hp5.4:ehp5. bytes per entity
:edl.
:li.:link reftype=hd res=4100.Search Keys:elink.
:dl compact tsize=13.
:dt.F5
:dd.Search Forward from current cursor position
:dt.Alt-F5
:dd.Search Backward from current cursor position
:dt.Shift-F5
:dd.Repeat last search operation
:edl.
:li.:link reftype=hd res=4400.Replace Keys:elink.
:dl compact tsize=13.
:dt.F6
:dd.Replace forward from current cursor position
:dt.Alt-F6
:dd.Replace backward from current cursor position
:dt.Shift-F6
:dd.Repeat last replace operation
:edl.
:li.:link reftype=hd res=4700.Go To Key:elink.
:dl compact tsize=13.
:dt.Ctrl-G
:dd.Invoke the
:link reftype=hd res=4701.Go To Offset Dialog:elink.
:edl.
:esl.
:lm margin=1.

:h2 res=9600 name=IDP_HELP_ABOUT.About
:i2 refid=Help.Product Info
:p.Select Help About to display an "About HexEdit/2" screen.

.********************************************************************

:h1 res=10000 name=IDP_MOUSE.Mouse Support
:i1 id=Mouse.Mouse Support
:p.You can move the cursor to any character within the Binary
or ASCII fields by positioning the mouse cursor on that
character and clicking button 1 (usually the left button).

.********************************************************************

:euserdoc.
