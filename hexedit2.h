/********************************************************************

             Copyright (C) 1994 - 1996 Bennett C. Baker and B::Ware

                          All rights reserved

     Module:   HEXEDIT2.H
     Author:   Bennett C. Baker
     CIS ID:   70274,3147
    Created:   14-Mar-1994

Description:   Header file for HEXEDIT/2 OS/2 Hex editor

       $Log$

********************************************************************/
 
#if !defined(HEXEDIT2_H)
#define HEXEDIT2_H

/************  INCLUDE FILES  **************************************/

/************  DEFINES, TYPEDEFS AND DATA STRUCTURE DEFINITIONS  ***/

/*
 * Version Numbers
 */

#define  VERSION_MAJOR              1
#define  VERSION_MINOR_TENTHS       0
#define  VERSION_MINOR_HUNDREDTHS   4

/*
 * Maximum string length
 */

#define  MAX_STRING_BYTES           128

/*
 * Edit mode flags.  Use EM_ASCII, EM_BINARY, or EM_TOGGLE as the
 * parameter in a SetEditMode() call; the iEditMode public will
 * take on either the EM_ASCII or EM_BINARY value.
 */

#define  EM_ASCII    0
#define  EM_BINARY   1
#define  EM_TOGGLE   2

/************  EXTERNAL DATA  **************************************/

// H2MAIN.CPP

extern   BOOL     bAutoResize;
extern   BOOL     bReadOnly;
extern   HAB      habAnchor;
extern   HWND     hwndHScroll;
extern   HWND     hwndVScroll;
extern   HWND     hwndMenu;

// H2CURSOR.CPP

extern   SHORT    sHScrollPos;
extern   INT      iVScrollPos;
extern   INT      iEditMode;
extern   SHORT    xCursorA, xCursorB, yCursor;

// H2FILE.CPP

extern   BOOL     bFileChanged;
extern   INT      iFileBytes;
extern   CHAR     szFileName[];
extern   BYTE    *pbFileBuffer;

// H2FONT.CPP

extern   SHORT    cxChar, cyChar, cxCaps, cyDesc;

// H2SCREEN.CPP

extern   SHORT    cxClient, cyClient;
extern   INT      iClientColumns;
extern   INT      iClientLines;
extern   INT      iDisplayColumns;
extern   INT      iFileLines;
extern   INT      iBytesPerEntity, iBytesPerLine, iCharsPerEntity;

/************  EXTERNAL CODE ***************************************/

// H2MAIN.CPP

USHORT   DisplayMessageBox( ULONG ulMessageID, ULONG ulTitleID, ULONG ulStyle );

// H2CURSOR.CPP (containing cursor and scroll control code)

void  h2cursor_init( void );
void  h2cursor_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  h2cursor_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  CreateCursor( HWND hwndWnd );
void  DestroyCursor( HWND hwndWnd );
void  MoveCursorUp( HWND hwndWnd );
void  MoveCursorDown( HWND hwndWnd );
void  MoveCursorLeft( HWND hwndWnd );
void  MoveCursorRight( HWND hwndWnd );
void  MoveCursorHome( HWND hwndWnd );
void  MoveCursorEnd( HWND hwndWnd );
BOOL  MoveCursorToOffset( HWND hwndWnd, INT iGoToOffset );
BOOL  MoveCursorToPointer( HWND hwndWnd, INT cxCursor, INT cyCursor );
BOOL  ProcessCursorMsg( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );
BOOL  ProcessHScrollMsg( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );
BOOL  ProcessVScrollMsg( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );
INT   QueryCursorOffset( void );
INT   SetEditMode( HWND hwndWnd, INT iNewMode );
void  ShowCursor( HWND hwndWnd, BOOL showflag );
void  UpdateCursorLimits( HWND hwndWnd, INT iBinaryOffset, INT iBinaryWidth,
                          INT iAsciiOffset, INT iAsciiWidth );
void  UpdateScrollLimits( HWND hwndWnd );
void  UpdateScrollWindows( HWND hwndWnd );

// H2EDIT.CPP

void  h2edit_init( void );
void  h2edit_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  h2edit_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
BOOL  EditCut( HWND hwndClient );
BOOL  EditCopy( HWND hwndClient );
BOOL  EditPaste( HWND hwndClient );
BOOL  EditInsert( HWND hwndClient );
BOOL  EditDelete( HWND hwndClient );

// H2FILE.CPP

void  h2file_init( void );
void  h2file_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  h2file_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  ClearFile( HWND hwndWnd );
BOOL  FileOpenDialog( HWND hwndWnd, CHAR *szOpenName );
BOOL  FileSaveDialog( HWND hwndWnd, CHAR *szSaveName );
BOOL  FileAllocateDialog( HWND hwndWnd );
BOOL  FileResizeDialog( HWND hwndWnd );

// H2FONT.CPP

void  h2font_init( void );
void  h2font_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  h2font_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  FontSelectDialog( HWND hwndWnd );
void  GetFontMetrics( HWND hwndWnd, BOOL fCopy );

// H2HELP.CPP

void  h2help_init( void );
void  h2help_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  h2help_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
BOOL  CreateHelpInstance( HAB hab, HWND hwndFrame, PSZ pszMainExecPath );
void  DestroyHelpInstance( void );
void  DisplayHelpPanel( HWND hwndWnd, SHORT sHelpID );
void  ProcessHelpMessage( HWND hwndWnd, MPARAM mpParm1, MPARAM mpParm2 );

// H2REG.CPP

void  h2reg_init( void );
void  h2reg_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  h2reg_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  DisplayAboutPanel( HWND hwndWnd, ULONG ulDisplaySecs );
void  DisplayRegistrationPanel( HWND hwndWnd );

// H2SCREEN.CPP

void  h2screen_init( void );
void  h2screen_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  h2screen_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  DisplayModeDialog( HWND hwndWnd );
void  DisplayModeAccelKey( HWND hwndWnd, SHORT sAccelKey );
void  PaintScreen( HWND hwndWnd );
BOOL  ProcessCharMsg( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );
void  UpdateDisplay( HWND hwndWnd );
void  UpdateScreenLimits( HWND hwndWnd, BOOL bForce );
void  UpdateWindowSize( HWND hwndWnd, SHORT cxWindow, SHORT cyWindow );

// H2SEARCH.CPP

void  h2search_init( void );
void  h2search_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  h2search_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp );
void  GotoOffset( HWND hwndWnd );
void  SearchForward( HWND hwndWnd );
void  SearchBackward( HWND hwndWnd );
void  SearchAgain( HWND hwndWnd );
void  ReplaceForward( HWND hwndWnd );
void  ReplaceBackward( HWND hwndWnd );
void  ReplaceAgain( HWND hwndWnd );

/************  END OF FILE  ****************************************/

#endif   /* if !defined(HEXEDIT2_H) */
