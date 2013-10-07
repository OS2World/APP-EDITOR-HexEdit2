/********************************************************************

              Copyright (C) 1994 Bennett C. Baker and B::Ware

                          All rights reserved

     Module:   H2SCREEN.C
     Author:   Bennett C. Baker
     CIS ID:   70274,3147
    Created:   14 March, 1994

Description:   Screen control routines for Hexedit/2, an OS/2 Hex editor

       $Log$

********************************************************************/
 
#if !defined(H2SCREEN_C)
#define H2SCREEN_C

/************  INCLUDE FILES  **************************************/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>

#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hexedit2.h"
#include "h2resrc.h"

/************  DEFINES, TYPEDEFS AND DATA STRUCTURE DEFINITIONS  ***/

/************  STATIC FUNCTION PROTOTYPES  *************************/

void  DisplayLine( HWND hwndWnd, HPS hpsScreen, INT iStartOffset, INT iClientLine );
BOOL  EditASCIIData( HWND hwndWnd, CHAR chr );
BOOL  EditBinaryData( HWND hwndWnd, CHAR chr );
MRESULT EXPENTRY DisplayDlgProc ( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );

/************  PRIVATE DATA ****************************************/

SHORT       cxClient = 0;
SHORT       cyClient = 0;

INT         iClientLines = 0;
INT         iFileLines = 0;

INT         iClientColumns = 0;
INT         iDisplayColumns = 0;

INT         iCharsPerEntity;
INT         iBytesPerLine;
INT         iBytesPerEntity;

/************  PUBLIC DATA *****************************************/

INT         iRadix;
INT         iEntitiesPerLine;
INT         iOffsetChars;
CHAR       *szOffsetFormat;
CHAR       *szPrintfFormat;
CHAR       *szScanfFormat;
INT         iDashSpot;
ULONG       ulEntryLimit;

INT         iOffsetSpaces     = 3;
INT         iEntitySpaces     = 3;
BOOL        bSpacing          = TRUE;

ULONG       ulDefaultOffsetID = IDD_DISPLAY_OFFSET_HEX;
ULONG       ulDefaultEntityID = IDD_DISPLAY_ENTITY_HEX;
ULONG       ulDefaultLengthID = IDD_DISPLAY_1_BYTE;

ULONG       ulNewOffsetID;
ULONG       ulNewEntityID;
ULONG       ulNewLengthID;

/*
 * Profile keys
 */

const CHAR  szPrfKeyWinPos[]  = "WindowPosition";
const CHAR  szPrfKeyOffset[]  = "OffsetMode";
const CHAR  szPrfKeyEntity[]  = "EntityMode";
const CHAR  szPrfKeyLength[]  = "ByteCount";
const CHAR  szPrfKeyResize[]  = "AutoResize";

/*
 * NOTE - the following structures depends on the ordering of the IDD_MODE
 * definitions in HEXEDIT2.H.  If you change them, change this ordering.
 */

struct _OFFSET
{
   INT   iOffsetChars;
   CHAR *szOffsetFormat;
}
asOffsetStruct[] =
{
   { 11, "%11.11o"},    // Octal
   { 10, "%10d"   },    // Decimal
   {  8, "%8.8X"  }     // Hex
};

ULONG aulEntryLimit[] =
{
   0x000000FF,          // 1 Byte
   0x0000FFFF,          // 2 Bytes
   0xFFFFFFFF           // 4 Bytes
};

struct _DATADISPLAY
{
   INT   iRadix;
   INT   iBytesPerEntity;
   INT   iEntitiesPerLine;
   INT   iCharsPerEntity;
   CHAR *szPrintfFormat;
   CHAR *szScanfFormat;
   INT   iDashSpot;
};

struct _DATADISPLAY asOctalByteEntry[] =
{
   {   8,  1,  8,  3, "%.3o",  "%3o",  4 },     // 1 Byte  
   {   8,  2,  8,  6, "%.6o",  "%6o",  4 },     // 2 Bytes 
   {   8,  4,  4, 11, "%.11o", "%11o", 2 }      // 4 Bytes 
};

struct _DATADISPLAY asDecimalByteEntry[] =
{
   {  10,  1, 10,  3, "%.3u",  "%3u",  5 },     // 1 Byte  
   {  10,  2,  8,  5, "%.5u",  "%5u",  4 },     // 2 Bytes 
   {  10,  4,  4, 10, "%.10u", "%10u", 2 }      // 4 Bytes 
};

struct _DATADISPLAY asHexByteEntry[] =
{
   {  16,  1, 16,  2, "%.2X",  "%2X",  8 },     // 1 Byte  
   {  16,  2,  8,  4, "%.4X",  "%4X",  4 },     // 2 Bytes 
   {  16,  4,  4,  8, "%.8X",  "%8X",  2 }      // 4 Bytes 
};

struct _DATADISPLAY *pasLengthEntryStructs[] =
{
   asOctalByteEntry,
   asDecimalByteEntry,
   asHexByteEntry
};

/************  INITIALIZATION CODE  ********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2screen_init

Description:   Initialize screen management variables

       Args:   void

       Uses:   ulDefaultOffsetID, ulDefaultLengthID

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2screen_init( void )
{
   register ULONG ulOffsetIndex;
   register ULONG ulEntityIndex;
   register ULONG ulLengthIndex;


   ulOffsetIndex = ulDefaultOffsetID - IDD_DISPLAY_OFFSET_OCTAL;
   ulEntityIndex = ulDefaultEntityID - IDD_DISPLAY_ENTITY_OCTAL;
   ulLengthIndex = ulDefaultLengthID - IDD_DISPLAY_1_BYTE;

   ulNewOffsetID = ulDefaultOffsetID ;
   ulNewEntityID = ulDefaultEntityID ;
   ulNewLengthID = ulDefaultLengthID ;

   szOffsetFormat= asOffsetStruct[ ulOffsetIndex ].szOffsetFormat;
   iOffsetChars  = asOffsetStruct[ ulOffsetIndex ].iOffsetChars;

   ulEntryLimit      = aulEntryLimit[ ulDefaultLengthID - IDD_DISPLAY_1_BYTE ];

   iRadix            = pasLengthEntryStructs[ ulEntityIndex ][ ulLengthIndex ].iRadix;
   iEntitiesPerLine  = pasLengthEntryStructs[ ulEntityIndex ][ ulLengthIndex ].iEntitiesPerLine;
   iBytesPerEntity   = pasLengthEntryStructs[ ulEntityIndex ][ ulLengthIndex ].iBytesPerEntity;
   iCharsPerEntity   = pasLengthEntryStructs[ ulEntityIndex ][ ulLengthIndex ].iCharsPerEntity;
   szPrintfFormat    = pasLengthEntryStructs[ ulEntityIndex ][ ulLengthIndex ].szPrintfFormat;
   szScanfFormat     = pasLengthEntryStructs[ ulEntityIndex ][ ulLengthIndex ].szScanfFormat;
   iDashSpot         = pasLengthEntryStructs[ ulEntityIndex ][ ulLengthIndex ].iDashSpot;

   iBytesPerLine     = iBytesPerEntity * iEntitiesPerLine;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2screen_restore

Description:   Restore screen settings from the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2screen_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
   SWP   swp;
   ULONG ulDisplayEntry;
   ULONG ulReadSize;


   /*
    * Restore the frame window size and position
    */

   ulReadSize = sizeof( swp );
   if ( PrfQueryProfileData( hiniPrf, pszApp, szPrfKeyWinPos, &swp, &ulReadSize ) )
   {
      WinSetWindowPos( WinQueryWindow( hwndWnd, QW_PARENT ),
                       HWND_TOP,
                       swp.x, swp.y,
                       swp.cx, swp.cy,
                       swp.fl );
   }

   /*
    * Restore the offset display mode, entity display mode, and byte count
    */

   ulReadSize = sizeof( ulDisplayEntry );
   if ( PrfQueryProfileData( hiniPrf, pszApp, szPrfKeyOffset, &ulDisplayEntry, &ulReadSize ) )
      ulNewOffsetID = ulDisplayEntry;

   ulReadSize = sizeof( ulDisplayEntry );
   if ( PrfQueryProfileData( hiniPrf, pszApp, szPrfKeyEntity, &ulDisplayEntry, &ulReadSize ) )
      ulNewEntityID = ulDisplayEntry;

   ulReadSize = sizeof( ulDisplayEntry );
   if ( PrfQueryProfileData( hiniPrf, pszApp, szPrfKeyLength, &ulDisplayEntry, &ulReadSize ) )
      ulNewLengthID = ulDisplayEntry;

   /*
    * Restore the Auto-Resize flag
    */

   ulReadSize = sizeof( ulDisplayEntry );
   if ( PrfQueryProfileData( hiniPrf, pszApp, szPrfKeyResize, &ulDisplayEntry, &ulReadSize ) )
   {
      bAutoResize = ulDisplayEntry;
      WinSendMsg( hwndMenu, MM_SETITEMATTR,
                  MPFROM2SHORT( IDM_OPTIONS_AUTORESIZE, TRUE ),
                  MPFROM2SHORT( MIA_CHECKED, bAutoResize ? MIA_CHECKED : 0 ) );
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2screen_save

Description:   Save screen settings to the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2screen_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
   SWP   swp;


   /*
    * Save the frame window size and position
    */

   WinQueryWindowPos( WinQueryWindow( hwndWnd, QW_PARENT ), &swp );
   PrfWriteProfileData( hiniPrf, pszApp, szPrfKeyWinPos, &swp, sizeof( swp ) );

   /*
    * Save the offset display mode, entity display mode, and byte count
    */

   PrfWriteProfileData( hiniPrf, pszApp, szPrfKeyOffset, &ulDefaultOffsetID, sizeof( ulDefaultOffsetID ) );
   PrfWriteProfileData( hiniPrf, pszApp, szPrfKeyEntity, &ulDefaultEntityID, sizeof( ulDefaultEntityID ) );
   PrfWriteProfileData( hiniPrf, pszApp, szPrfKeyLength, &ulDefaultLengthID, sizeof( ulDefaultLengthID ) );

   /*
    * Save the Auto-Resize flag
    */

   PrfWriteProfileData( hiniPrf, pszApp, szPrfKeyResize, &bAutoResize, sizeof( bAutoResize ) );
}

/************  PUBLIC CODE *****************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   DisplayModeDialog

Description:   Pop up a dialog box allowing the user to select the
               line number, data type, and byte length representations
               for the in-memory data.

       Args:   HWND - handle to client window

       Uses:   DisplayDialogProc

   Modifies:   

    Returns:   void

-------------------------------------------------------------------*/

void DisplayModeDialog( HWND hwndWnd )
{
   if ( WinDlgBox ( HWND_DESKTOP, hwndWnd, DisplayDlgProc, NULLHANDLE, IDD_DISPLAY, NULL ) )
      UpdateScreenLimits( hwndWnd, FALSE );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   DisplayModeAccelKey

Description:   Select a display mode accelerator key and update the
               display accordingly.

       Args:   HWND - handle to client window
               SHORT - accelerator key

       Uses:   DisplayDialogProc

   Modifies:   

    Returns:   void

-------------------------------------------------------------------*/

void DisplayModeAccelKey( HWND hwndWnd, SHORT sAccelKey )
{
   switch ( sAccelKey )
   {
      case IDD_DISPLAY_OFFSET_OCTAL:
      case IDD_DISPLAY_OFFSET_DECIMAL:
      case IDD_DISPLAY_OFFSET_HEX:
         ulNewOffsetID = (ULONG)sAccelKey;
         UpdateScreenLimits( hwndWnd, FALSE );
         break;

      case IDD_DISPLAY_ENTITY_OCTAL:
      case IDD_DISPLAY_ENTITY_DECIMAL:
      case IDD_DISPLAY_ENTITY_HEX:
         ulNewEntityID = (ULONG)sAccelKey;
         UpdateScreenLimits( hwndWnd, FALSE );
         break;

      case IDD_DISPLAY_1_BYTE:
      case IDD_DISPLAY_2_BYTE:
      case IDD_DISPLAY_4_BYTE:
         ulNewLengthID = (ULONG)sAccelKey;
         UpdateScreenLimits( hwndWnd, FALSE );
         break;

      default:
         break;
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   PaintScreen

Description:   Paints the screen in response to a WM_PAINT command by
               displaying the data in pbFileBuffer in whatever mode is
               currently selected (as indicated by ulDefaultEntityID).

       Args:   HWND - window handle

       Uses:   pbFileBuffer
               ulFileBytes
               ulDefaultEntityID
               hpsClient

   Modifies:   client window display

    Returns:   void

-------------------------------------------------------------------*/

void PaintScreen( HWND hwndWnd )
{
   HPS      hpsEdit;
   INT      j;
   INT      iStartOffset;
   INT      iPaintBeg, iPaintEnd;
   RECTL    rclInvalid;


   /*
    * Get a presentation space handle via WinBeginPaint and erase the screen.
    */

   ShowCursor( hwndWnd, FALSE );
   hpsEdit = WinBeginPaint( hwndWnd, NULL, &rclInvalid );
   GpiErase( hpsEdit );

   /*
    * If we have a file, display each screen line
    */

   if ( pbFileBuffer != NULL )
   {
      iPaintBeg = max( 0, ( cyClient - rclInvalid.yTop ) / cyChar );
      iPaintEnd = min( iClientLines, ( cyClient - rclInvalid.yBottom ) / cyChar + 1 );
      iStartOffset = iBytesPerLine * ( iVScrollPos + iPaintBeg );

      for ( j = iPaintBeg; j < iPaintEnd; j++, iStartOffset += iBytesPerLine )
      {
         DisplayLine( hwndWnd, hpsEdit, iStartOffset, j );
      }
   }

   /*
    * Call WinEndPaint to release the presentation space handle, and
    * then re-display the cursor.
    */

   WinEndPaint( hpsEdit );
   ShowCursor( hwndWnd, TRUE );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ProcessCharMsg

Description:   Accept a WM_CHAR message and determine whether it contains
               a valid editing key.

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   file buffer

    Returns:   BOOL - TRUE if key was handled by this routine

-------------------------------------------------------------------*/

BOOL  ProcessCharMsg( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   BOOL     ret;
   CHAR     chr;
   HPS      hpsEdit;


   /*
    * If there's no current buffer, ignore the editing key
    */

   if ( !iFileBytes )
      return TRUE;

   chr = CHARMSG(&ulMsg)->chr;

   if ( chr == '\t' )
   {
      SetEditMode( hwndWnd, EM_TOGGLE );
      return TRUE;
   }

   if ( iEditMode == EM_BINARY )
   {
      ret = EditBinaryData( hwndWnd, chr );
   }
   else
   {
      ret = EditASCIIData( hwndWnd, chr );
   }

   if ( ret )
   {
      INT   iLineOffset = ( iVScrollPos * iBytesPerLine ) + ( yCursor * iBytesPerLine );

      ShowCursor( hwndWnd, FALSE );

      hpsEdit = WinGetPS( hwndWnd );
      GpiSetBackMix( hpsEdit, BM_OVERPAINT );
      DisplayLine( hwndWnd, hpsEdit, iLineOffset, yCursor );
      WinReleasePS( hpsEdit );

      MoveCursorRight( hwndWnd );
      ShowCursor( hwndWnd, TRUE );

      bFileChanged = TRUE;
   }

   return TRUE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   UpdateDisplay

Description:   Sets the the scroll bar position, calls WinInvalidateRect()
               and WinUpdateWindow() to update the display.

       Args:   HWND - window handle of caller (client)

       Uses:   nothing

   Modifies:   nothing

    Returns:   void

-------------------------------------------------------------------*/

void UpdateDisplay( HWND hwndWnd )
{
   /*
    * Update the scroll bar settings according to the new (screen) regime
    */

   UpdateScrollWindows( hwndWnd );

   /*
    * Invalidate the entire screen region.
    */

   WinInvalidateRect( hwndWnd, NULL, TRUE );

   /*
    * Force a WM_PAINT message to the client window procedure.
    */

   WinUpdateWindow( hwndWnd );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   UpdateScreenLimits

Description:   Update the screen and redisplay if necessary

       Args:   HWND - window handle

       Uses:   ulNewEntityID, ulDefaultEntityID

   Modifies:   all screen dimension items

    Returns:   void

-------------------------------------------------------------------*/

void UpdateScreenLimits( HWND hwndWnd, BOOL bForce )
{
   BOOL     bChanged = FALSE;
   RECTL    rcl;
   INT      j;
   INT      iIntegralHeight;
   INT      iMinWidth;
   INT      iEntityChars;


   /*
    * Update the dialog box settings.  If any have changed, set the bChanged flag to TRUE.
    */

   if ( ulDefaultOffsetID != ulNewOffsetID )
   {
      ulDefaultOffsetID      = ulNewOffsetID;
      szOffsetFormat   = asOffsetStruct[ ulDefaultOffsetID - IDD_DISPLAY_OFFSET_OCTAL ].szOffsetFormat;
      iOffsetChars     = asOffsetStruct[ ulDefaultOffsetID - IDD_DISPLAY_OFFSET_OCTAL ].iOffsetChars;
      bChanged = TRUE;
   }

   if ( ulDefaultEntityID != ulNewEntityID ||  ulDefaultLengthID != ulNewLengthID )
   {
      struct _DATADISPLAY *pdd;
      int   k;

      ulDefaultEntityID = ulNewEntityID;
      ulDefaultLengthID   = ulNewLengthID;

      pdd = pasLengthEntryStructs[ ulDefaultEntityID - IDD_DISPLAY_ENTITY_OCTAL ];
      k = ( ulDefaultLengthID - IDD_DISPLAY_1_BYTE );

      iRadix            = pdd[ k ].iRadix;
      iEntitiesPerLine  = pdd[ k ].iEntitiesPerLine;
      iBytesPerEntity   = pdd[ k ].iBytesPerEntity;
      iCharsPerEntity   = pdd[ k ].iCharsPerEntity;
      szPrintfFormat    = pdd[ k ].szPrintfFormat;
      szScanfFormat     = pdd[ k ].szScanfFormat;
      iDashSpot         = pdd[ k ].iDashSpot;

      iBytesPerLine     = iBytesPerEntity * iEntitiesPerLine;

      ulEntryLimit      = aulEntryLimit[ ulDefaultLengthID - IDD_DISPLAY_1_BYTE ];

      bChanged = TRUE;
   }

   /*
    * Calculate the number of columns on screen (iClientColumns), lines on screen (iClientLines)
    * and lines in the file (iFileLines).
    */

   iClientColumns = cxClient / cxChar;
   iClientLines = cyClient/cyChar;
   iFileLines = ( iFileBytes / iBytesPerLine ) + ( ( iFileBytes % iBytesPerLine ) ? 1 : 0 );

   /*
    * If we have insufficient width or non-integral height, call WinSetWindowPos()
    * to give us adequate width and height.
    */

   /*
    * First, calculate the required width for the current display mode and
    * save it in iDisplayColumns.
    */

   iEntityChars = ( ( iEntitiesPerLine * iCharsPerEntity ) + ( iEntitiesPerLine - 1 ) );
   iDisplayColumns = ( ( iOffsetChars + iOffsetSpaces )     // For offset
                   + ( iEntityChars + iEntitySpaces )       // For entities
                   + ( iBytesPerLine )                      // For ASCII display
                   + 2 );                                   // For space at either end
   iMinWidth = ( iDisplayColumns * cxChar );

   /*
    * Compute the closest (non-zero) integral height
    */

   if ( iClientLines > 0 )
      iIntegralHeight = iClientLines * cyChar;
   else
      iIntegralHeight = cyChar;

   /*
    * Get the current client window dimensions and resize the client window if necessary
    */

   if ( cxClient != iMinWidth || cyClient != iIntegralHeight )
   {
      RECTL rcl;
      SHORT cxFrame, cyFrame;
      INT   iXDiff, iYDiff;
      HWND  hwndFrame;

      hwndFrame = WinQueryWindow( hwndWnd, QW_PARENT);
      if ( bAutoResize )
         iXDiff = iMinWidth - cxClient;
      else
         iXDiff = 0;
      iYDiff = iIntegralHeight - cyClient;
      WinQueryWindowRect( hwndFrame, &rcl );
      cxFrame = rcl.xRight - rcl.xLeft;
      cyFrame = rcl.yTop - rcl.yBottom;
      WinSetWindowPos( hwndFrame, 0, 0, 0, cxFrame + iXDiff, cyFrame + iYDiff, SWP_SIZE );
      bChanged = TRUE;
   }

   /*
    * Update the scroll bar limits and shift factors
    */

   UpdateScrollLimits( hwndWnd );

   /*
    * Update the cursor limits
    */

   UpdateCursorLimits( hwndWnd,
                       iOffsetChars + iOffsetSpaces,
                       iEntityChars,
                       iOffsetChars + iOffsetSpaces + iEntityChars + iEntitySpaces,
                       iBytesPerLine );

   /*
    * Update the screen if necessary.
    */

   if ( bChanged || bForce )
   {
      UpdateDisplay( hwndWnd );
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   UpdateWindowSize

Description:   Update the size of the client window

       Args:   HWND - client window handle
               SHORT - cx of client window
               SHORT - cy of client window

       Uses:   nothing

   Modifies:   cxClient, cyClient
               calls UpdateScreenLimits()

    Returns:   void

-------------------------------------------------------------------*/

void UpdateWindowSize( HWND hwndWnd, SHORT cxWindow, SHORT cyWindow )
{
   cxClient = cxWindow;
   cyClient = cyWindow;

   UpdateScreenLimits( hwndWnd, TRUE );
}

/************  PRIVATE CODE ****************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   DisplayLine

Description:   Display a line from the date in pbFileBuffer, in the currently
               selected format, on the indicated line.

       Args:   HWND - window handle
               HPS - presentation space handle
               ULONG - offset into pbFileBuffer of start of line
               ULONG - client window line number (0-based)

       Uses:   pbFileBuffer
               ulFileBytes
               ulDefaultEntityID
               hpsClient

   Modifies:   client window display

    Returns:   void

-------------------------------------------------------------------*/

void DisplayLine( HWND hwndWnd, HPS hpsScreen, INT iStartOffset, INT iClientLine )
{
   INT      k;
   BYTE    *pb;
   BYTE    *pbMax;
   BYTE    *pbStart;
   CHAR     szLineBuffer[256];
   CHAR     szLNumBuf[64];
   CHAR     szBinaryBuf[128];
   CHAR     szASCIIBuf[64];
   CHAR     szEBuf[64];
   POINTL   ptl;
   INT      iMinDataLen;


   /*
    * Set up the start of the line to display and the end of the
    * file (to make sure we don't step over the line).
    */

   pbStart = pbFileBuffer + iStartOffset;
   pbMax = pbFileBuffer + iFileBytes;

   if ( pbStart < pbMax )
   {
      // Fill in the line number buffer
      sprintf( szLNumBuf, szOffsetFormat, iStartOffset );

      // Fill in the ASCII buffer
      pb = pbStart;
      memset( szASCIIBuf, '\0', sizeof( szASCIIBuf ) );
      for ( k = 0; k < iBytesPerLine && pb < pbMax; k ++, pb++ )
      {
         if ( *pb >= ' ' && *pb < 0x7F )
            szASCIIBuf[k] = *pb;
         else
            szASCIIBuf[k] = '.';
      }

      // Fill in the binary buffer
      pb = pbStart;
      szBinaryBuf[0] = '\0';
      for ( k = 0; k < iEntitiesPerLine && pb < pbMax; k ++ )
      {
         ULONG ulDummy = 0;
         INT   j;

         for ( j = 0; j < iBytesPerEntity && pb < pbMax; j++, pb++ )
         {
            ulDummy += *pb << ( j * 8 );
         }
         sprintf( szEBuf, szPrintfFormat, ulDummy );
         strcat( szBinaryBuf, szEBuf );

         if ( bSpacing )
         {
            if ( k < iEntitiesPerLine - 1 )
            {
               if ( k == iDashSpot - 1 )
                  strcat( szBinaryBuf, "-" );
               else
                  strcat( szBinaryBuf, " " );
            }
         }
      }

      // Pad the binary buffer to a full width
      iMinDataLen = ( iCharsPerEntity * iEntitiesPerLine ) + iEntitiesPerLine - 1;
      if ( strlen( szBinaryBuf ) < iMinDataLen )
      {
         memset( szBinaryBuf + strlen( szBinaryBuf ), ' ', iMinDataLen - strlen( szBinaryBuf ) );
         szBinaryBuf[ iMinDataLen ] = '\0';
      }

      // Concatenate the various buffers into the Line buffer
      szLineBuffer[0] = '\0';
      sprintf( szLineBuffer, "%s | %s | %s", szLNumBuf, szBinaryBuf, szASCIIBuf );

      // Display the Line buffer at the proper co-ordinates
      ptl.x = cxChar - ( sHScrollPos * cxChar );
      ptl.y = cyClient - cyChar * ( iClientLine + 1 ) + cyDesc;
      GpiCharStringAt( hpsScreen, &ptl, strlen( szLineBuffer ), szLineBuffer );
   }

   return;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   EditASCIIData

Description:   Accept characters to edit the ASCII display portion of
               the data display, translating them into their appropriate
               binary counterparts in their proper place in the byte
               stream.

       Args:   HWND - window handle
               CHAR - character from keyboard

       Uses:   pbFileBuffer
               ulFileBytes
               ulDefaultEntityID
               hpsClient

   Modifies:   client window display

    Returns:   BOOL - TRUE if valid editing character

-------------------------------------------------------------------*/

BOOL  EditASCIIData( HWND hwndWnd, CHAR chr )
{
   INT   iBufOffset;


   /*
    * Make sure that we accept only displayable characters.
    */

   if ( !bReadOnly && chr >= ' ' )
   {
      iBufOffset = ( iVScrollPos * iBytesPerLine ) + ( yCursor * iBytesPerLine ) + xCursorA;
      *(pbFileBuffer + iBufOffset) = chr;
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   EditBinaryData

Description:   Accept characters to edit the binary display portion of
               the data display.
               stream.

       Args:   HWND - window handle
               CHAR - character from keyboard

       Uses:   pbFileBuffer
               ulFileBytes
               ulDefaultEntityID
               hpsClient

   Modifies:   client window display

    Returns:   BOOL - TRUE if valid editing character

-------------------------------------------------------------------*/

BOOL  EditBinaryData( HWND hwndWnd, CHAR chr )
{
   INT      val = -1;
   INT      shift;
   INT      iCursorOffset;
   INT      iLineOffset;
   INT      j;
   INT      iEditOK;
   POINTL   ptl;
   CHAR     xchar;
   CHAR    *pb;
   CHAR    *pbMax;
   CHAR    *pbFirstEntityByte;
   ULONG    ulDummy;
   CHAR     szEditBuf[32];


   /*
    * Handle special case characters first.
    */

   if ( chr == ' ' )
   {
      MoveCursorRight( hwndWnd );
      return FALSE;
   }

   /*
    * Convert the character to upper case.  If it is not a valid character for
    * the current radix, discard it.
    */

   if ( chr >= '0' && chr <= '9' )
   {
      xchar = chr;
      val = xchar - '0';
   }
   else if ( chr >= 'A' && chr <= 'F' )
   {
      xchar = chr;
      val = xchar - 'A' + 10;
   }
   else if ( chr >= 'a' && chr <= 'f' )
   {
      xchar = chr - ( 'a' - 'A' );
      val = xchar - 'A' + 10;
   }
   else
      return FALSE;

   if ( val >= iRadix )
      return FALSE;

   /*
    * Now comes the fun part.  Create an ASCII representation of what the current entity
    * looks like on-screen, edit that ASCII representation, and then do a SCANF to determine
    * whether it's a valid number for the current radix and byte length.
    */

   /*
    * Get the start of the current entity and sprintf() it's value, taking into account
    * the currently selected byte count.
    */

   pbMax = pbFileBuffer + iFileBytes;
   iLineOffset = ( iVScrollPos * iBytesPerLine ) + ( yCursor * iBytesPerLine );
   pbFirstEntityByte = pbFileBuffer + iLineOffset + ( iBytesPerEntity * ( xCursorB / ( iCharsPerEntity + 1 ) ) );
   ulDummy = 0;

   for ( j = 0, pb = pbFirstEntityByte; j < iBytesPerEntity && pb < pbMax; j++, pb++ )
   {
      ulDummy += *pb << ( j * 8 );
   }

   sprintf( szEditBuf, szPrintfFormat, ulDummy );

   /*
    * Calculate the character offset of the cursor position relative to the
    * start of the current entity, and replace that character with the character
    * entered from the keyboard.  Do a scanf() into ulDummy to verify that the
    * string is still a valid number in the current scheme.
    */

   iCursorOffset = xCursorB % ( iCharsPerEntity + 1 );
   szEditBuf[ iCursorOffset ] = xchar;
   iEditOK = sscanf( szEditBuf, szScanfFormat, &ulDummy );

   /*
    * If the edit was valid, put the value back into the buffer and re-display the (changed) line
    */

   if ( !bReadOnly && iEditOK && ulDummy <= ulEntryLimit)
   {
      for ( j = 0, pb = pbFirstEntityByte; j < iBytesPerEntity && pb < pbMax; j++, pb++ )
      {
         *pb = ( ulDummy >> ( j * 8 ) ) & 0xFF;
      }

      return TRUE;
   }

   return FALSE;
}

/************  DIALOG PROCEDURES ***********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   DisplayDlgProc

Description:   Dialog Box procedure for DISPLAY dialog, which determines the
               type of line numbers (octal, decimal, or hex), the data display
               mode (octal, decimal, or hex) and the number of bytes per data
               entity.

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:

   Modifies:   ulDefaultEntityID - set to whatever mode is finally selected

    Returns:   MRESULT - usually FALSE

-------------------------------------------------------------------*/

MRESULT EXPENTRY DisplayDlgProc ( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   switch ( ulMsg )
   {
      case WM_INITDLG:
         WinSendDlgItemMsg( hwndDlg, ulDefaultOffsetID, BM_SETCHECK, MPFROMSHORT( 1 ), NULL );
         WinSendDlgItemMsg( hwndDlg, ulDefaultEntityID, BM_SETCHECK, MPFROMSHORT( 1 ), NULL );
         WinSendDlgItemMsg( hwndDlg, ulDefaultLengthID, BM_SETCHECK, MPFROMSHORT( 1 ), NULL );
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );

      case WM_CONTROL:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case IDD_DISPLAY_OFFSET_DECIMAL:
            case IDD_DISPLAY_OFFSET_HEX:
            case IDD_DISPLAY_OFFSET_OCTAL:
               ulNewOffsetID = (ULONG)SHORT1FROMMP( mpParm1 );
               break;

            case IDD_DISPLAY_ENTITY_DECIMAL:
            case IDD_DISPLAY_ENTITY_HEX:
            case IDD_DISPLAY_ENTITY_OCTAL:
               ulNewEntityID = (ULONG)SHORT1FROMMP( mpParm1 );
               break;

            case IDD_DISPLAY_1_BYTE:
            case IDD_DISPLAY_2_BYTE:
            case IDD_DISPLAY_4_BYTE:
               ulNewLengthID = (ULONG)SHORT1FROMMP( mpParm1 );
               break;
         }
         break;

      case WM_COMMAND:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case DID_OK:
               WinDismissDlg( hwndDlg, TRUE );
               break;

            case DID_CANCEL:
               ulNewOffsetID = ulDefaultOffsetID;
               ulNewEntityID = ulDefaultEntityID;
               ulNewLengthID = ulDefaultLengthID;
               WinDismissDlg( hwndDlg, FALSE );
               break;

            default:
               return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
         }
         break;

      case WM_HELP:
         /*
          * If F1 is pressed, the mpParm1 parameter will be 0x0000.  In that
          * case, determine which dialog is currently showing and use that
          * panel for display.
          */
         ProcessHelpMessage( hwndDlg, mpParm1, mpParm2 );
         break;

      default:
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
   }

   return MRFROMSHORT ( FALSE ) ;
}

/************  PRIVATE CODE  ***************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:

Description:

       Args:

       Uses:

   Modifies:

    Returns:

-------------------------------------------------------------------*/

/************  END OF FILE  ****************************************/

#endif   /* if !defined(H2SCREEN_C) */
