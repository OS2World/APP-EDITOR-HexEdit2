/********************************************************************

              Copyright (C) 1994 Bennett C. Baker and B::Ware

                          All rights reserved

     Module:   H2CURSOR.C
     Author:   Bennett C. Baker
     CIS ID:   70274,3147
    Created:   14 March, 1994

Description:   Cursor and scroll control routines for Hexedit/2.

       $Log$

********************************************************************/
 
#if !defined(H2CURSOR_C)
#define H2CURSOR_C

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

/************  DEFINES, TYPEDEFS AND DATA STRUCTURE DEFINITIONS  ***/

#define  MovePrimaryCursor(hwnd, xoff, yoff)    \
         WinCreateCursor( hwnd,  cxChar + ( cxChar * ( xoff ) ) - ( sHScrollPos * cxChar ),  \
                          cyClient - cyChar * ( 1 + yoff ), 0, 0, CURSOR_SETPOS, NULL);

#define  VScrollLineUp(hwnd)     ProcessVScrollMsg( hwnd, WM_VSCROLL, 0, MPFROM2SHORT( 0, SB_LINEUP ) )
#define  VScrollLineDown(hwnd)   ProcessVScrollMsg( hwnd, WM_VSCROLL, 0, MPFROM2SHORT( 0, SB_LINEDOWN ) )
#define  VScrollPageUp(hwnd)     ProcessVScrollMsg( hwnd, WM_VSCROLL, 0, MPFROM2SHORT( 0, SB_PAGEUP ) )
#define  VScrollPageDown(hwnd)   ProcessVScrollMsg( hwnd, WM_VSCROLL, 0, MPFROM2SHORT( 0, SB_PAGEDOWN ) )

#define  ENABLE_CURSOR_UPDATE_POSITION    0

/************  STATIC FUNCTION PROTOTYPES  *************************/

void  SnapCursorToEntity( HWND hwndWnd );
void  ToggleSecondaryCursor( HWND hwndWnd, INT iXCursor, INT iYCursor );
BOOL  ValidateProposedOffset( INT xC, INT yC, BOOL bBinary );

/************  PUBLIC DATA *****************************************/

SHORT    sHScrollPos = 0;
INT      iVScrollPos = 0;
INT      iEditMode = EM_BINARY;
SHORT    xCursorA, xCursorB, yCursor;

/************  PRIVATE DATA ****************************************/

SHORT    xCursorAMin, xCursorAMax, xCursorAOffset;
SHORT    xCursorBMin, xCursorBMax, xCursorBOffset;
SHORT    yCursorMin, yCursorMax;
SHORT    cxCursor, cyCursor;

BOOL     bPrimaryCursorCreated = FALSE;
BOOL     bPrimaryCursorDisplayed = FALSE;
BOOL     bSecondaryCursorDisplayed = FALSE;

INT      iVScrollInc;
INT      iVScrollShift;
INT      iVScrollMax = 0;

SHORT    sHScrollMax;
SHORT    sVScrollMax;

/************  INITIALIZATION CODE  ********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2cursor_init

Description:   Initialize cursor management variables

       Args:   void

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2cursor_init( void )
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2cursor_restore

Description:   Restore cursor settings from the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2cursor_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2cursor_save

Description:   Save cursor settings to the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2cursor_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
}

/************  CURSOR CONTROL CODE  ********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ProcessCursorMsg

Description:   Process VK_xxx cursor control messages for the main client window
               procedure.

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:

   Modifies:

    Returns:   BOOL - TRUE if key was handled by this routine

-------------------------------------------------------------------*/

BOOL  ProcessCursorMsg ( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   BOOL  bProcessed = TRUE;


   /*
    * Switch on the virtual key code.  The message is guaranteed to have
    * a virtual key code because of the initial WM_CHAR processing in
    * the client window procedure.
    */

   ShowCursor( hwndWnd, FALSE );

   switch( CHARMSG(&ulMsg)->vkey )
   {
      case VK_PAGEUP:
         VScrollPageUp( hwndWnd );
         break;

      case VK_PAGEDOWN:
         VScrollPageDown( hwndWnd );
         break;

      case VK_UP:
         if ( CHARMSG(&ulMsg)->fs & KC_CTRL )
            VScrollLineUp( hwndWnd );
         else
            MoveCursorUp( hwndWnd );
         break;

      case VK_DOWN:
         if ( CHARMSG(&ulMsg)->fs & KC_CTRL )
            VScrollLineDown( hwndWnd );
         else
            MoveCursorDown( hwndWnd );
         break;

      case VK_LEFT:
         MoveCursorLeft( hwndWnd );
         break;

      case VK_RIGHT:
         MoveCursorRight( hwndWnd );
         break;

      case VK_HOME:
         MoveCursorHome( hwndWnd );
         break;

      case VK_END:
         MoveCursorEnd( hwndWnd );
         break;

      default:
         bProcessed = FALSE;
         break;
   }

   ShowCursor( hwndWnd, TRUE );
   return bProcessed;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   CreateCursor

Description:   Control cursor creation and display status

       Args:   HWND - client window handle

       Uses:   Cursor co-ordinates

   Modifies:   nothing

    Returns:   void

-------------------------------------------------------------------*/

void  CreateCursor( HWND hwndWnd )
{
   /*
    * Call WinCreateCursor() to create the cursor.  Note that the cursor
    * location co-ordinates are 0,0, because the subsequent call to ShowCursor()
    * will set the proper co-ordinates.  Declare a primary cursor created, and
    * set its displayed flag to false (a newly created cursor is invisible).
    */

   WinCreateCursor( hwndWnd, 0, 0, cxChar, cyChar, CURSOR_SOLID | CURSOR_FLASH, NULL );
   bPrimaryCursorCreated = TRUE;
   bPrimaryCursorDisplayed = FALSE;

   /*
    * Display the cursor (conditionally, of course).
    */

   ShowCursor( hwndWnd, TRUE );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   DestroyCursor

Description:   Control cursor creation and display status

       Args:   HWND - client window handle

       Uses:   nothing

   Modifies:   nothing

    Returns:   void

-------------------------------------------------------------------*/

void  DestroyCursor( HWND hwndWnd )
{
   /*
    * Hide the cursor, then destroy it.  Mark it as no longer created.
    */

   ShowCursor( hwndWnd, FALSE );
   WinDestroyCursor( hwndWnd );
   bPrimaryCursorCreated = FALSE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ShowCursor

Description:   Conditionally display or hide the cursor.  Show the
               cursor only if a data buffer exists; hide the cursor
               only if it is not already hidden.

       Args:   HWND - window handle of caller (client)
               BOOL - TRUE to display the cursor, FALSE to hide it

       Uses:   nothing

   Modifies:   nothing

    Returns:   void

-------------------------------------------------------------------*/

void  ShowCursor( HWND hwndWnd, BOOL showflag )
{
   if ( showflag )
   {
      if ( bPrimaryCursorCreated && !bPrimaryCursorDisplayed && iFileBytes )
      {
         if ( iEditMode == EM_BINARY )
         {
            MovePrimaryCursor( hwndWnd, xCursorB + xCursorBOffset, yCursor );
            if ( !bSecondaryCursorDisplayed )
               ToggleSecondaryCursor( hwndWnd, xCursorA + xCursorAOffset, yCursor );
         }
         else
         {
            MovePrimaryCursor( hwndWnd, xCursorA + xCursorAOffset, yCursor );
            if ( !bSecondaryCursorDisplayed )
               ToggleSecondaryCursor( hwndWnd, xCursorB + xCursorBOffset, yCursor );
         }

         bPrimaryCursorDisplayed = showflag;
         WinShowCursor( hwndWnd, showflag );
      }
   }
   else
   {
      if ( bPrimaryCursorCreated && bPrimaryCursorDisplayed )
      {
         if ( iEditMode == EM_BINARY )
         {
            if ( bSecondaryCursorDisplayed )
               ToggleSecondaryCursor( hwndWnd, xCursorA + xCursorAOffset, yCursor );
         }
         else
         {
            if ( bSecondaryCursorDisplayed )
               ToggleSecondaryCursor( hwndWnd, xCursorB + xCursorBOffset, yCursor );
         }

         bPrimaryCursorDisplayed = showflag;
         WinShowCursor( hwndWnd, showflag );
      }
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   MoveCursorUp
               MoveCursorDown
               MoveCursorLeft
               MoveCursorRight
               MoveCursorHome
               MoveCursorEnd

Description:   Position the cursor

       Args:   HWND - client window handle

       Uses:

   Modifies:   xCursorB, yCursor

    Returns:   void

-------------------------------------------------------------------*/

void  MoveCursorUp( HWND hwndWnd )
{
   if ( yCursor > yCursorMin )
   {
      yCursor--;
   }
   else
   {
      VScrollLineUp( hwndWnd );
      ShowCursor( hwndWnd, FALSE );
      if ( yCursor > yCursorMin )
         yCursor--;
   }
}


void  MoveCursorDown( HWND hwndWnd )
{
   if ( yCursor < yCursorMax )
   {
      if ( ValidateProposedOffset( xCursorB, yCursor + 1, TRUE ) )
         yCursor++;
   }
   else
   {
      VScrollLineDown( hwndWnd );
      ShowCursor( hwndWnd, FALSE );
      if ( ValidateProposedOffset( xCursorB, yCursor + 1, TRUE ) )
         yCursor++;
   }
}


void  MoveCursorLeft( HWND hwndWnd )
{
   if ( iEditMode == EM_BINARY )
   {
      if ( xCursorB > xCursorBMin )
      {
         xCursorB--;
         if ( xCursorB % ( iCharsPerEntity + 1 ) == iCharsPerEntity )
         {
            xCursorA -= iBytesPerEntity;
            xCursorB--;
         }
      }
      else if ( iVScrollPos != 0 || yCursor > yCursorMin )
      {
         xCursorA = xCursorAMax;
         xCursorB = xCursorBMax;
         if ( yCursor > yCursorMin )
         {
            yCursor--;
         }
         else
         {
            VScrollLineUp( hwndWnd );
            ShowCursor( hwndWnd, FALSE );
            yCursor--;
         }
      }
   }
   else
   {
      if ( xCursorA > xCursorAMin )
      {
         xCursorA--;
         xCursorB = ( xCursorA / iBytesPerEntity ) * ( iCharsPerEntity + 1 );
      }
      else if ( iVScrollPos != 0 || yCursor > yCursorMin )
      {
         xCursorA = xCursorAMax;
         xCursorB = ( xCursorA / iBytesPerEntity ) * ( iCharsPerEntity + 1 );
         if ( yCursor > yCursorMin )
         {
            yCursor--;
         }
         else
         {
            VScrollLineUp( hwndWnd );
            ShowCursor( hwndWnd, FALSE );
            yCursor--;
         }
      }
   }
}


void  MoveCursorRight( HWND hwndWnd )
{
   if ( iEditMode == EM_BINARY )
   {
      if ( xCursorB <  xCursorBMax )
      {
         if ( ValidateProposedOffset( xCursorB + 1, yCursor, TRUE ) )
         {
            xCursorB++;
            if ( xCursorB % ( iCharsPerEntity + 1 ) == iCharsPerEntity )
            {
               if ( ValidateProposedOffset( xCursorB + 1, yCursor, TRUE ) )
               {
                  xCursorA += iBytesPerEntity;
                  xCursorB++;
               }
               else
               {
                  xCursorB--;
               }
            }
         }
      }
      else if ( yCursor < yCursorMax || iVScrollPos < iVScrollMax )
      {
         xCursorA = xCursorAMin;
         xCursorB = xCursorBMin;
         if ( yCursor < yCursorMax )
         {
            if ( ValidateProposedOffset( xCursorB, yCursor + 1, TRUE ) )
               yCursor++;
         }
         else
         {
            VScrollLineDown( hwndWnd );
            ShowCursor( hwndWnd, FALSE );
            yCursor++;
         }
      }
   }
   else
   {
      if ( xCursorA <  xCursorAMax )
      {
         if ( ValidateProposedOffset( xCursorA + 1, yCursor, FALSE ) )
         {
            xCursorA++;
            xCursorB = ( xCursorA / iBytesPerEntity ) * ( iCharsPerEntity + 1 );
         }
      }
      else if ( yCursor < yCursorMax || iVScrollPos < iVScrollMax )
      {
         xCursorA = xCursorAMin;
         xCursorB = xCursorBMin;
         if ( yCursor < yCursorMax )
         {
            if ( ValidateProposedOffset( xCursorB, yCursor + 1, TRUE ) )
               yCursor++;
         }
         else
         {
            VScrollLineDown( hwndWnd );
            ShowCursor( hwndWnd, FALSE );
            yCursor++;
         }
      }
   }
}


void  MoveCursorHome( HWND hwndWnd )
{
   iVScrollPos = 0;
   UpdateDisplay( hwndWnd );
   ShowCursor( hwndWnd, FALSE );

   xCursorA = xCursorAMin;
   xCursorB = xCursorBMin;
   yCursor = yCursorMin;
}


void  MoveCursorEnd( HWND hwndWnd )
{
   INT   iBytesOnScreen;
   INT   iBytesOnLastLine;


   iVScrollPos = iVScrollMax;
   UpdateDisplay( hwndWnd );
   ShowCursor( hwndWnd, FALSE );
   
   iBytesOnScreen = iFileBytes - ( iBytesPerLine * iVScrollPos );
   yCursor = ( iBytesOnScreen - 1 ) / iBytesPerLine;
   iBytesOnLastLine = iBytesOnScreen - ( yCursor * iBytesPerLine );
   xCursorA = iBytesOnLastLine - 1;
   xCursorB = ( ( ( iBytesOnLastLine + iBytesPerEntity - 1 ) / iBytesPerEntity ) - 1 ) * ( iCharsPerEntity + 1 );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   MoveCursorToOffset

Description:   Position the cursor at a specific offset within the file

       Args:   HWND - client window handle
               INT - offset within the file

       Uses:

   Modifies:   xCursorA, xCursorB, yCursor

    Returns:   BOOL - TRUE if offset is valid, FALSE if not

-------------------------------------------------------------------*/

BOOL  MoveCursorToOffset( HWND hwndWnd, INT iGoToOffset )
{
   INT   iBytesToOffset;
   INT   iBytesOnLastLine;


   if ( ( iGoToOffset >= 0 ) && ( iGoToOffset < iFileBytes ) )
   {
      ShowCursor( hwndWnd, FALSE );

      iVScrollPos = iGoToOffset / iBytesPerLine;
      iVScrollPos = max( 0, iVScrollPos - ( ( iClientLines - 1 ) / 2 ) );
      UpdateDisplay( hwndWnd );
      ShowCursor( hwndWnd, FALSE );

      iBytesToOffset = iGoToOffset - ( iBytesPerLine * iVScrollPos ) + 1;
      yCursor = ( iBytesToOffset - 1 ) / iBytesPerLine;
      iBytesOnLastLine = iBytesToOffset - ( yCursor * iBytesPerLine );
      xCursorA = iBytesOnLastLine - 1;
      xCursorB = ( ( ( iBytesOnLastLine + iBytesPerEntity - 1 ) / iBytesPerEntity ) - 1 ) * ( iCharsPerEntity + 1 );

      ShowCursor( hwndWnd, TRUE );

      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   MoveCursorToPointer

Description:   Position the cursor to the current location of the
               mouse pointer

       Args:   HWND - client window handle
               INT - cxCursor
               INT - cyCursor

       Uses:

   Modifies:   xCursorB, yCursor

    Returns:   BOOL - TRUE if pointer offset is valid, FALSE if not

-------------------------------------------------------------------*/

BOOL  MoveCursorToPointer( HWND hwndWnd, INT cxCursor, INT cyCursor )
{
   INT   xC, yC;


   xC = ( ( cxCursor - cxChar) / cxChar ) + sHScrollPos;
   yC = ( ( cyClient - cyCursor ) / cyChar );

   if ( xC >= xCursorAOffset )
   {
      xC -= xCursorAOffset;

      if ( ( xC >= xCursorAMin && xC <= xCursorAMax && yC >= yCursorMin && yC <= yCursorMax )
        && ValidateProposedOffset( xC, yC, FALSE ) )
      {
         ShowCursor( hwndWnd, FALSE );
         xCursorA = xC;
         xCursorB = xC * ( iCharsPerEntity + 1 );
         yCursor = yC;
         iEditMode = EM_ASCII;
         ShowCursor( hwndWnd, TRUE );
         return TRUE;
      }
      else
      {
         return FALSE;
      }
   }
   else
   {
      xC -= xCursorBOffset;

      if ( ( xC >= xCursorBMin && xC <= xCursorBMax && yC >= yCursorMin && yC <= yCursorMax )
        && ( xC % ( iCharsPerEntity + 1 ) != iCharsPerEntity )
        && ValidateProposedOffset( xC, yC, TRUE ) )
      {
         ShowCursor( hwndWnd, FALSE );
         xCursorA = xC / ( iCharsPerEntity + 1 );
         xCursorB = xC;
         yCursor = yC;
         iEditMode = EM_BINARY;
         ShowCursor( hwndWnd, TRUE );
         return TRUE;
      }
      else
      {
         return FALSE;
      }
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   QueryCursorOffset

Description:   Returns the byte offset into the current file corresponding
               to the current cursor location on-screen.

       Args:   void

       Uses:   iVScrollPos
               iBytesPerLine, iBytesPerEntity
               xCursorB, yCursor

   Modifies:   nothing

    Returns:   INT - file offset (0 to iFileBytes - 1)

-------------------------------------------------------------------*/

INT   QueryCursorOffset( void )
{
   INT   iCursorOffset;


   iCursorOffset  = ( iVScrollPos * iBytesPerLine )
                  + ( yCursor * iBytesPerLine )
                  + xCursorA;

   return iCursorOffset;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   SetEditMode

Description:   Set the edit mode to the iNewMode value.  If iNewMode
               is EM_TOGGLE, toggle the current edit mode.

       Args:   HWND - client window handle
               INT - new edit mode (ASCII, Binary, or Toggle)

       Uses:   iEditMode

   Modifies:   iEditMode

    Returns:   INT - new edit mode

-------------------------------------------------------------------*/

INT   SetEditMode( HWND hwndWnd, INT iNewMode )
{
   ShowCursor( hwndWnd, FALSE );

   if ( iNewMode == EM_BINARY || iNewMode == EM_ASCII )
   {
      iEditMode = iNewMode;
   }
   else if ( iNewMode == EM_TOGGLE )
   {
      if ( iEditMode == EM_ASCII )
         iEditMode = EM_BINARY;
      else
         iEditMode = EM_ASCII;
   }

   ShowCursor( hwndWnd, TRUE );

   return iEditMode;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   SnapCursorToEntity

Description:   Move the cursor (left) to the nearest entity.

       Args:   HWND - client window handle

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  SnapCursorToEntity( HWND hwndWnd )
{
   ShowCursor( hwndWnd, FALSE );

   if ( iEditMode == EM_BINARY )
   {
      while ( ( !ValidateProposedOffset( xCursorB, yCursor, TRUE ) )
           || ( xCursorB % ( iCharsPerEntity + 1 ) == iCharsPerEntity ) )
         MoveCursorLeft( hwndWnd );
   }
   else
   {
      while ( !ValidateProposedOffset( xCursorA, yCursor, FALSE ) )
         MoveCursorLeft( hwndWnd );
   }

   ShowCursor( hwndWnd, TRUE );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ToggleSecondaryCursor

Description:   Toggles the secondary cursor (tracks the main cursor
               in the other field).

       Args:   HWND - client window handle
               INT - X-offset of cursor
               INT - Y-offset of cursor

       Uses:   cxChar, cxChar, cyChar, cyClient

   Modifies:   nothing

    Returns:   void

-------------------------------------------------------------------*/

void  ToggleSecondaryCursor( HWND hwndWnd, INT iXCursor, INT iYCursor )
{
   HPS      hpsClient;
   POINTL   ptl;


   hpsClient = WinGetPS( hwndWnd );
   GpiSetColor( hpsClient, CLR_PALEGRAY );
   GpiSetMix( hpsClient, FM_INVERT );
   ptl.x = cxChar + ( iXCursor * cxChar ) - ( sHScrollPos * cxChar );
   ptl.y = cyClient - ( ( iYCursor + 1) * cyChar );
   GpiMove( hpsClient, &ptl );
   ptl.x += cxChar;
   ptl.y += cyChar - 1;
   GpiBox( hpsClient, DRO_OUTLINE, &ptl, 0, 0 );
   WinReleasePS( hpsClient );

   bSecondaryCursorDisplayed ^= TRUE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   UpdateCursorLimits

Description:   Update the limits of cursor travel based on client
               window and file statistics.

       Args:   HWND - client window handle
               INT - offset of start of binary data (chars)
               INT - width of binary data (chars)
               INT - offset of start of ASCII data (chars)
               INT - width of ASCII data (chars)

       Uses:   iClientLines

   Modifies:   xCursorAMin, xCursorAMax, xCursorAOffset
               xCursorBMin, xCursorBMax, xCursorBOffset
               yCursorMin, yCursorMax

    Returns:   BOOL - TRUE if current cursor position is valid

-------------------------------------------------------------------*/

void  UpdateCursorLimits( HWND hwndWnd, INT iBinaryOffset, INT iBinaryWidth,
                          INT iAsciiOffset, INT iAsciiWidth )
{
   xCursorAMin = 0;
   xCursorAMax = iAsciiWidth - 1;

   xCursorAOffset = iAsciiOffset;

   xCursorBMin = 0;
   xCursorBMax = iBinaryWidth - 1;

   xCursorBOffset = iBinaryOffset;

   yCursorMin = 0;
   yCursorMax = iClientLines - 1;

   if ( xCursorA < xCursorAMin || xCursorB < xCursorBMin )
   {
      xCursorA = xCursorAMin;
      xCursorB = xCursorBMin;
   }

   if ( xCursorA > xCursorAMax || xCursorB > xCursorBMax )
   {
      xCursorA = xCursorAMax;
      xCursorB = xCursorBMax;
   }

   if ( yCursor < yCursorMin )
   {
      yCursor = yCursorMin;
   }

   if ( yCursor > yCursorMax )
   {
      yCursor = yCursorMax;
   }

   SnapCursorToEntity( hwndWnd );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ValidateProposedOffset

Description:   Validate whether the proposed cursor position will fall
               within the boundaries of the current file.  Does _NOT_
               verify whether the proposed cursor position will be
               between display entities.

       Args:   SHORT - proposed xCursorB
               SHORT - proposed yCursor
               BOOL  - TRUE to do a binary check, FALSE for ASCII

       Uses:   iVScrollPos
               iEntitiesPerLine
               iCharsPerEntity

   Modifies:   nothing

    Returns:   BOOL - TRUE if valid, FALSE if not

-------------------------------------------------------------------*/

BOOL ValidateProposedOffset( INT xC, INT yC, BOOL bBinary )
{
   INT   iFilePos;


   if ( bBinary )
      iFilePos = ( iVScrollPos * iBytesPerLine ) + ( yC * iBytesPerLine ) + ( iBytesPerEntity * ( xC / ( iCharsPerEntity + 1 ) ) );
   else
      iFilePos = ( iVScrollPos * iBytesPerLine ) + ( yC * iBytesPerLine ) + xC;

   if ( iFileBytes == 0 )
      return ( iFilePos <= iFileBytes );
   else
      return ( iFilePos < iFileBytes );
}

/************  SCROLL CONTROL CODE  ********************************/

BOOL  ProcessHScrollMsg( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   SHORT sHScrollInc;
   BOOL  bProcessed = TRUE;


   ShowCursor( hwndWnd, FALSE );

   switch ( SHORT2FROMMP( mpParm2 ) )
   {
      case SB_SLIDERTRACK:
         sHScrollInc = SHORT1FROMMP( mpParm2 ) - sHScrollPos;
         break;

      case SB_LINELEFT:
         sHScrollInc = -1;
         break;

      case SB_LINERIGHT:
         sHScrollInc = 1;
         break;

      case SB_PAGELEFT:
         sHScrollInc = -iClientColumns;
         break;

      case SB_PAGERIGHT:
         sHScrollInc = iClientColumns;
         break;

      default:
         bProcessed = FALSE;
         break;
   }

   if ( bProcessed )
   {

      sHScrollInc = max( -sHScrollPos, min( sHScrollInc, sHScrollMax - sHScrollPos ) );

      if ( sHScrollInc != 0 )
      {

         sHScrollPos += sHScrollInc;
        #if 01
         WinScrollWindow( hwndWnd, -(cxChar * sHScrollInc), 0,
                           NULL, NULL, NULL, NULL,
                           SW_INVALIDATERGN );
         UpdateScrollWindows( hwndWnd );
         WinUpdateWindow( hwndWnd );
        #else
         UpdateDisplay( hwndWnd );
        #endif
         SnapCursorToEntity( hwndWnd );
      }
   }

   ShowCursor( hwndWnd, TRUE );

   return bProcessed;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ProcessVScrollMsg

Description:   Process WM_VSCROLL messages for the main client window
               procedure.

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:

   Modifies:

    Returns:   BOOL - TRUE if message was handled by this routine

-------------------------------------------------------------------*/

BOOL  ProcessVScrollMsg ( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   SHORT sYCursorInc;
   SHORT sVScrollPos;
   BOOL  bProcessed = TRUE;


   ShowCursor( hwndWnd, FALSE );

   switch ( SHORT2FROMMP( mpParm2 ) )
   {
      /*
       * We treat SLIDERTRACK special due to an apparent bug in the PM
       * window scroll routines (they become un-synchronized if the cursor
       * moves outside of the client window while still "pushing" the
       * scroll bar slider).  Maybe it has to do with mouse capture or
       * something like that - all I know is that doing the screen update
       * the rigidly deterministic overkill way doesn't screw up.
       *
       * Other special treatment - if the slider position is equal to the
       * max slider position (the SHORT value) we set our internal slider
       * position to our internal max slider value.
       */

      case SB_SLIDERTRACK:
      #if 0
         iVScrollInc = ( SHORT1FROMMP( mpParm2 ) << iVScrollShift ) - iVScrollPos;
         sYCursorInc = 0;
      #else
         sVScrollPos = SHORT1FROMMP( mpParm2 );
         if ( sVScrollPos == sVScrollMax )
            iVScrollPos = iVScrollMax;
         else
            iVScrollPos = sVScrollPos << iVScrollShift;
         UpdateDisplay( hwndWnd );
         SnapCursorToEntity( hwndWnd );
         ShowCursor( hwndWnd, TRUE );
         return TRUE;
      #endif
         break;

      case SB_LINEUP:
         iVScrollInc = -1;
         sYCursorInc = 1;
         break;

      case SB_LINEDOWN:
         iVScrollInc = 1;
         sYCursorInc = -1;
         break;

      case SB_PAGEUP:
         iVScrollInc = -iClientLines;
         sYCursorInc = 0;
         break;

      case SB_PAGEDOWN:
         iVScrollInc = iClientLines;
         sYCursorInc = 0;
         break;

      default:
         bProcessed = FALSE;
         break;
   }

   if ( bProcessed )
   {

      iVScrollInc = max( -iVScrollPos, min( iVScrollInc, iVScrollMax - iVScrollPos ) );
      sYCursorInc = max( -yCursor, min( sYCursorInc, yCursorMax - yCursor ) );

      if ( iVScrollInc != 0 )
      {

         iVScrollPos += iVScrollInc;
         yCursor += sYCursorInc;
         WinScrollWindow( hwndWnd, 0, cyChar * iVScrollInc,
                           NULL, NULL, NULL, NULL,
                           SW_INVALIDATERGN );
         UpdateScrollWindows( hwndWnd );
         WinUpdateWindow( hwndWnd );
         SnapCursorToEntity( hwndWnd );
      }
   }

   ShowCursor( hwndWnd, TRUE );

   return bProcessed;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   UpdateScrollLimits

Description:   Update the limit variables used by the scroll routines

       Args:   HWND - client window handle
               INT  - client window lines
               INT  - file lines

       Uses:   iClientLines, iFileLines

   Modifies:   iVScrollMax, iVScrollPos, iVScrollShift

    Returns:   void

-------------------------------------------------------------------*/

void UpdateScrollLimits( HWND hwndWnd )
{
   /*
    * Compute the maximum value of the scroll sliders.
    */

   sHScrollMax = max ( 0, iDisplayColumns - iClientColumns );
   iVScrollMax = max ( 0, iFileLines - iClientLines );

   /*
    * Now compute the shift value necessary to scale the VScroll
    * max scroll bar slider down into a 16-bit signed quantity.
    */

   iVScrollShift = 0;
   if ( iVScrollMax > 32767 )
   {
      register INT it = iVScrollMax;

      while ( it > 32767 )
      {
         iVScrollShift++;
         it >>= 1;
      }
   }

   /*
    * Recompute the current scroll position based on the new
    * max position.
    */

   sHScrollPos = min( sHScrollPos, sHScrollMax );
   iVScrollPos = min( iVScrollPos, iVScrollMax );

   /*
    * Compute the 16-bit max VScroll position for use in other
    * functions.
    */

   sVScrollMax = (USHORT)( iVScrollMax >> iVScrollShift );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   UpdateScrollWindows

Description:   Sends an update to the vertical and horizontal scroll bars

       Args:   HWND - client window handle

       Uses:   sVScrollMax, iVScrollPos, iVScrollShift, sHScrollMax, sHScrollPos

   Modifies:   nothing

    Returns:   void

-------------------------------------------------------------------*/

void UpdateScrollWindows( HWND hwndWnd )
{
   SHORT       sVScrollPos;


   /*
    * Shift the 32-bit signed VScroll settings down into 16-bit
    * signed quantities.
    */

   sVScrollPos = (USHORT)( iVScrollPos >> iVScrollShift );

   /*
    * Special-case stuff: if the shifted values have hit either
    * limit while their unshifted counterparts have not, adjust
    * the shifted values so we don't get a scroll bar that refuses
    * to move to the first or last couple of lines.
    */

   if ( sVScrollPos == sVScrollMax && iVScrollPos < iVScrollMax )
      sVScrollPos--;
   else if ( sVScrollPos == 0 && iVScrollPos > 0 )
      sVScrollPos++;

   /*
    * Send the actual settings update message to the scrollbar windows.
    */

   WinSendMsg( hwndHScroll, SBM_SETSCROLLBAR, MPFROM2SHORT( sHScrollPos, 0), MPFROM2SHORT( 0, sHScrollMax ) );
   WinSendMsg( hwndVScroll, SBM_SETSCROLLBAR, MPFROM2SHORT( sVScrollPos, 0), MPFROM2SHORT( 0, sVScrollMax ) );

   /*
    * Set the scroll bar thumb size; this has the side effect of enabling the scroll
    * bar window if there is anything to scroll, allowing us to bypass a call to
    * WinEnableWindow().
    */

   WinSendMsg( hwndHScroll, SBM_SETTHUMBSIZE, MPFROM2SHORT( iClientColumns, iDisplayColumns ), 0 );
   WinSendMsg( hwndVScroll, SBM_SETTHUMBSIZE, MPFROM2SHORT( iClientLines, iFileLines >> iVScrollShift ), 0 );
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

#endif   /* if !defined(H2CURSOR_C) */
