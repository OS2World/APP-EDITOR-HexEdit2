/********************************************************************

              Copyright (C) 1994 Bennett C. Baker and B::Ware

                          All rights reserved

     Module:   H2SEARCH.C
     Author:   Bennett C. Baker
     CIS ID:   70274,3147
    Created:   14 March, 1994

Description:   Search and Replace functions for HexEdit/2

       $Log$

********************************************************************/
 
#if !defined(H2SEARCH_C)
#define H2SEARCH_C

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

#define  SEARCH_BUFFER_SIZE   257

/************  STATIC FUNCTION PROTOTYPES  *************************/

MRESULT EXPENTRY GoToDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );
MRESULT EXPENTRY SearchDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );
MRESULT EXPENTRY ReplaceDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );

BOOL  PromptAndReplace( HWND hwndWnd );
BOOL  QueryCharMatch( CHAR c1, CHAR c2, BOOL bCaseSens );
BOOL  SearchForString( CHAR *szBuffer, INT iBufferLength, INT iBufferOffset,
                       CHAR *szSearchString, INT iSearchLength,
                       BOOL bCaseSens, BOOL bSearchForward,
                       INT *piFindOffset );

/************  PUBLIC DATA *****************************************/

/************  PRIVATE DATA ****************************************/

INT      iGoToOffset;
INT      iOffsetType;

BOOL     bSearchForward;
BOOL     bCaseSens;

CHAR     szSearchBuffer[ SEARCH_BUFFER_SIZE ];
CHAR     szReplaceBuffer[ SEARCH_BUFFER_SIZE ];
INT      iSearchBufferLength;
INT      iReplaceBufferLength;

/************  INITIALIZATION CODE  ********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2search_init

Description:   Initialize Search and Replace variables

       Args:   void

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void h2search_init( void )
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2search_restore

Description:   Restore search settings from the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2search_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2search_save

Description:   Save search settings to the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2search_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
}

/************  SEARCH MANAGEMENT CODE ******************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   GoToOffset

Description:   Select an offset via dialog box and then move to that
               offset within the current file buffer.

       Args:   HWND - client window handle

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  GotoOffset( HWND hwndWnd )
{
   if ( WinDlgBox( HWND_DESKTOP, hwndWnd, GoToDlgProc, NULLHANDLE, IDD_GOTO_OFFSET, NULL ) )
   {
      if ( iOffsetType > 0 )
         MoveCursorToOffset( hwndWnd, QueryCursorOffset() + iGoToOffset );
      else if ( iOffsetType < 0 )
         MoveCursorToOffset( hwndWnd, QueryCursorOffset() - iGoToOffset );
      else
         MoveCursorToOffset( hwndWnd, iGoToOffset );
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   SearchForward

Description:   Search forward from the current cursor position for an
               ASCII or binary string.

       Args:   HWND - client window handle

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  SearchForward( HWND hwndWnd )
{
   INT   iCurrentOffset = QueryCursorOffset();


   if ( WinDlgBox( HWND_DESKTOP, hwndWnd, SearchDlgProc, NULLHANDLE, IDD_SEARCH, NULL ) )
   {
      bSearchForward = TRUE;
      if ( SearchForString( pbFileBuffer, iFileBytes, iCurrentOffset,
                            szSearchBuffer, iSearchBufferLength,
                            bCaseSens, bSearchForward, &iGoToOffset ) )
      {
         MoveCursorToOffset( hwndWnd, iGoToOffset );
      }
      else
      {
         DisplayMessageBox( IDS_SEARCHFAILED, IDS_SEARCH, MB_OK | MB_ICONEXCLAMATION );
      }
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   SearchBackward

Description:   Search backward from the current cursor position for an
               ASCII or binary string.

       Args:   HWND - client window handle

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  SearchBackward( HWND hwndWnd )
{
   INT   iCurrentOffset = QueryCursorOffset();


   if ( WinDlgBox( HWND_DESKTOP, hwndWnd, SearchDlgProc, NULLHANDLE, IDD_SEARCH, NULL ) )
   {
      bSearchForward = FALSE;
      if ( SearchForString( pbFileBuffer, iFileBytes, iCurrentOffset,
                            szSearchBuffer, iSearchBufferLength,
                            bCaseSens, bSearchForward, &iGoToOffset ) )
      {
         MoveCursorToOffset( hwndWnd, iGoToOffset );
      }
      else
      {
         DisplayMessageBox( IDS_SEARCHFAILED, IDS_SEARCH, MB_OK | MB_ICONEXCLAMATION );
      }
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   SearchAgain

Description:   Repeat the previous search command from the current cursor
               position.

       Args:   HWND - client window handle

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  SearchAgain( HWND hwndWnd )
{
   INT   iCurrentOffset = QueryCursorOffset();


   if ( bSearchForward )
      iCurrentOffset++;
   else
      iCurrentOffset--;

   if ( SearchForString( pbFileBuffer, iFileBytes, iCurrentOffset,
                         szSearchBuffer, iSearchBufferLength,
                         bCaseSens, bSearchForward, &iGoToOffset ) )
   {
      MoveCursorToOffset( hwndWnd, iGoToOffset );
   }
   else
   {
      DisplayMessageBox( IDS_SEARCHFAILED, IDS_SEARCH, MB_OK | MB_ICONEXCLAMATION );
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ReplaceForward

Description:

       Args:   HWND - client window handle

       Uses:

   Modifies:

    Returns:

-------------------------------------------------------------------*/

void  ReplaceForward( HWND hwndWnd )
{
   INT   iCurrentOffset = QueryCursorOffset();


   if ( WinDlgBox( HWND_DESKTOP, hwndWnd, ReplaceDlgProc, NULLHANDLE, IDD_REPLACE, NULL ) )
   {
      bSearchForward = TRUE;
      if ( SearchForString( pbFileBuffer, iFileBytes, iCurrentOffset,
                            szSearchBuffer, iSearchBufferLength,
                            bCaseSens, bSearchForward, &iGoToOffset ) )
      {
         PromptAndReplace( hwndWnd );
      }
      else
      {
         DisplayMessageBox( IDS_SEARCHREPLACEFAILED, IDS_REPLACE, MB_OK | MB_ICONEXCLAMATION );
      }
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ReplaceBackward

Description:

       Args:   HWND - client window handle

       Uses:

   Modifies:

    Returns:

-------------------------------------------------------------------*/

void  ReplaceBackward( HWND hwndWnd )
{
   INT   iCurrentOffset = QueryCursorOffset();


   if ( WinDlgBox( HWND_DESKTOP, hwndWnd, ReplaceDlgProc, NULLHANDLE, IDD_REPLACE, NULL ) )
   {
      bSearchForward = FALSE;
      if ( SearchForString( pbFileBuffer, iFileBytes, iCurrentOffset,
                            szSearchBuffer, iSearchBufferLength,
                            bCaseSens, bSearchForward, &iGoToOffset ) )
      {
         PromptAndReplace( hwndWnd );
      }
      else
      {
         DisplayMessageBox( IDS_SEARCHREPLACEFAILED, IDS_REPLACE, MB_OK | MB_ICONEXCLAMATION );
      }
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ReplaceAgain

Description:   Repeat the previous search and replace operation

       Args:   HWND - client window handle

       Uses:   bSearchForward, iGoToOffset

   Modifies:   iGoToOffset

    Returns:   void

-------------------------------------------------------------------*/

void  ReplaceAgain( HWND hwndWnd )
{
   INT   iCurrentOffset = QueryCursorOffset();


   if ( bSearchForward )
      iCurrentOffset++;
   else
      iCurrentOffset--;

   if ( SearchForString( pbFileBuffer, iFileBytes, iCurrentOffset,
                         szSearchBuffer, iSearchBufferLength,
                         bCaseSens, bSearchForward, &iGoToOffset ) )
   {
      PromptAndReplace( hwndWnd );
   }
   else
   {
      DisplayMessageBox( IDS_SEARCHREPLACEFAILED, IDS_REPLACE, MB_OK | MB_ICONEXCLAMATION );
   }
}

/************  DIALOG PROCEDURES ***********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   GotoDlgProc

Description:   Manage input from the SEARCH GO TO OFFSET dialog

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   nothing

    Returns:   MRESULT - usually FALSE

-------------------------------------------------------------------*/

MRESULT EXPENTRY GoToDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   static   CHAR *szDisplayMode = "%X";
   static   ULONG ulItemID = IDD_SEARCH_HEX;

   CHAR  szNewSize[ SEARCH_BUFFER_SIZE ];
   CHAR *pszOffset;
   INT   iNewSize;


   switch ( ulMsg )
   {
      case WM_INITDLG:
         WinSendDlgItemMsg( hwndDlg, ulItemID, BM_SETCHECK, MPFROMSHORT( 1 ), NULL );
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );

      case WM_COMMAND:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case DID_OK:
               iNewSize = 0;
               *szNewSize = '\0';
               WinQueryDlgItemText( hwndDlg, IDD_GOTO_OFFSET_OFFSET, SEARCH_BUFFER_SIZE, szNewSize );
               pszOffset = szNewSize;
               if ( *pszOffset == '-' )
               {
                  iOffsetType = -1;
                  pszOffset++;
               }
               else if ( *pszOffset == '+' )
               {
                  iOffsetType = 1;
                  pszOffset++;
               }
               else
               {
                  iOffsetType = 0;
               }
               if ( sscanf( pszOffset, szDisplayMode, &iGoToOffset ) )
                  WinDismissDlg( hwndDlg, TRUE );
               else
                  WinDismissDlg( hwndDlg, FALSE );
               break;

            case DID_CANCEL:
               WinDismissDlg( hwndDlg, FALSE );
               break;

            default:
               return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
         }
         break;

      case WM_CONTROL:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case IDD_SEARCH_OCTAL:
               szDisplayMode = "%o";
               ulItemID = IDD_SEARCH_OCTAL;
               break;

            case IDD_SEARCH_DECIMAL:
               szDisplayMode = "%d";
               ulItemID = IDD_SEARCH_DECIMAL;
               break;

            case IDD_SEARCH_HEX:
               szDisplayMode = "%X";
               ulItemID = IDD_SEARCH_HEX;
               break;

            default:
               break;
         }
         break;

      case WM_HELP:
         ProcessHelpMessage( hwndDlg, mpParm1, mpParm2 );
         break;

      default:
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
   }

   return MRFROMSHORT ( FALSE ) ;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   SearchDlgProc

Description:   Manage input from the SEARCH SEARCH dialog

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   nothing

    Returns:   MRESULT - usually FALSE

-------------------------------------------------------------------*/

MRESULT EXPENTRY SearchDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   static   CHAR *szDisplayMode = "%X";
   static   ULONG ulItemID = IDD_SEARCH_HEX;
   static   BOOL  bDlgCaseSens = FALSE;
   CHAR     szSearchText[ SEARCH_BUFFER_SIZE ];


   switch ( ulMsg )
   {
      case WM_INITDLG:
         WinSendDlgItemMsg( hwndDlg, ulItemID, BM_SETCHECK, MPFROMSHORT( 1 ), NULL );
         WinSendDlgItemMsg( hwndDlg, IDD_SEARCH_CASE_SENS, BM_SETCHECK, MPFROMSHORT( bDlgCaseSens ), NULL );
         if ( ulItemID == IDD_SEARCH_ASCII )
         {
            WinEnableControl( hwndDlg, IDD_SEARCH_CASE_SENS, TRUE );
         }
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );

      case WM_COMMAND:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case DID_OK:
               WinQueryDlgItemText( hwndDlg, IDD_SEARCH_OLD, SEARCH_BUFFER_SIZE, szSearchText );

               if ( ulItemID == IDD_SEARCH_ASCII )
               {                  
                  bCaseSens = WinQueryButtonCheckstate( hwndDlg, IDD_SEARCH_CASE_SENS );
                  bDlgCaseSens = bCaseSens;
                  strncpy( szSearchBuffer, szSearchText, SEARCH_BUFFER_SIZE );
                  iSearchBufferLength = strlen( szSearchBuffer );
               }
               else
               {
                  CHAR *token;

                  bCaseSens = TRUE;
                  token = strtok( szSearchText, " " );
                  for ( iSearchBufferLength = 0;
                        iSearchBufferLength < SEARCH_BUFFER_SIZE
                           && token != NULL
                           && sscanf( token, szDisplayMode, szSearchBuffer + iSearchBufferLength );
                        iSearchBufferLength++, token = strtok( NULL, " " ) )
                     ;
               }
               WinDismissDlg( hwndDlg, TRUE );
               break;

            case DID_CANCEL:
               WinDismissDlg( hwndDlg, FALSE );
               break;

            default:
               return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
         }
         break;

      case WM_CONTROL:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case IDD_SEARCH_OCTAL:
               szDisplayMode = "%o";
               ulItemID = IDD_SEARCH_OCTAL;
               WinEnableControl( hwndDlg, IDD_SEARCH_CASE_SENS, FALSE );
               break;

            case IDD_SEARCH_DECIMAL:
               szDisplayMode = "%d";
               ulItemID = IDD_SEARCH_DECIMAL;
               WinEnableControl( hwndDlg, IDD_SEARCH_CASE_SENS, FALSE );
               break;

            case IDD_SEARCH_HEX:
               szDisplayMode = "%X";
               ulItemID = IDD_SEARCH_HEX;
               WinEnableControl( hwndDlg, IDD_SEARCH_CASE_SENS, FALSE );
               break;

            case IDD_SEARCH_ASCII:
               szDisplayMode = "";
               ulItemID = IDD_SEARCH_ASCII;
               WinEnableControl( hwndDlg, IDD_SEARCH_CASE_SENS, TRUE );
               break;

            default:
               break;
         }
         break;

      case WM_HELP:
         ProcessHelpMessage( hwndDlg, mpParm1, mpParm2 );
         break;

      default:
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
   }

   return MRFROMSHORT ( FALSE ) ;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ReplaceDlgProc

Description:   Manage input from the SEARCH REPLACE dialog

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   nothing

    Returns:   MRESULT - usually FALSE

-------------------------------------------------------------------*/

MRESULT EXPENTRY ReplaceDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   static   CHAR *szDisplayMode = "%X";
   static   ULONG ulItemID = IDD_SEARCH_HEX;
   static   BOOL  bDlgCaseSens = FALSE;
   CHAR     szSearchText[ SEARCH_BUFFER_SIZE ];
   CHAR     szReplaceText[ SEARCH_BUFFER_SIZE ];


   switch ( ulMsg )
   {
      case WM_INITDLG:
         WinSendDlgItemMsg( hwndDlg, ulItemID, BM_SETCHECK, MPFROMSHORT( 1 ), NULL );
         WinSendDlgItemMsg( hwndDlg, IDD_SEARCH_CASE_SENS, BM_SETCHECK, MPFROMSHORT( bDlgCaseSens ), NULL );
         if ( ulItemID == IDD_SEARCH_ASCII )
         {
            WinEnableControl( hwndDlg, IDD_SEARCH_CASE_SENS, TRUE );
         }
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );

      case WM_COMMAND:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case DID_OK:
               WinQueryDlgItemText( hwndDlg, IDD_SEARCH_OLD, SEARCH_BUFFER_SIZE, szSearchText );
               WinQueryDlgItemText( hwndDlg, IDD_REPLACE_NEW, SEARCH_BUFFER_SIZE, szReplaceText );

               if ( ulItemID == IDD_SEARCH_ASCII )
               {                  
                  bCaseSens = WinQueryButtonCheckstate( hwndDlg, IDD_SEARCH_CASE_SENS );
                  bDlgCaseSens = bCaseSens;
                  strncpy( szSearchBuffer, szSearchText, SEARCH_BUFFER_SIZE );
                  iSearchBufferLength = strlen( szSearchBuffer );
                  strncpy( szReplaceBuffer, szReplaceText, SEARCH_BUFFER_SIZE );
                  iReplaceBufferLength = strlen( szReplaceBuffer );
               }
               else
               {
                  CHAR *token;


                  bCaseSens = TRUE;

                  token = strtok( szSearchText, " " );
                  for ( iSearchBufferLength = 0;
                        iSearchBufferLength < SEARCH_BUFFER_SIZE
                           && token != NULL
                           && sscanf( token, szDisplayMode, szSearchBuffer + iSearchBufferLength );
                        iSearchBufferLength++, token = strtok( NULL, " " ) )
                     ;

                  token = strtok( szReplaceText, " " );
                  for ( iReplaceBufferLength = 0;
                        iReplaceBufferLength < SEARCH_BUFFER_SIZE
                           && token != NULL
                           && sscanf( token, szDisplayMode, szReplaceBuffer + iReplaceBufferLength );
                        iReplaceBufferLength++, token = strtok( NULL, " " ) )
                     ;
               }

               WinDismissDlg( hwndDlg, TRUE );
               break;

            case DID_CANCEL:
               WinDismissDlg( hwndDlg, FALSE );
               break;

            default:
               return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
         }
         break;

      case WM_CONTROL:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case IDD_SEARCH_OCTAL:
               szDisplayMode = "%o";
               ulItemID = IDD_SEARCH_OCTAL;
               WinEnableControl( hwndDlg, IDD_SEARCH_CASE_SENS, FALSE );
               break;

            case IDD_SEARCH_DECIMAL:
               szDisplayMode = "%d";
               ulItemID = IDD_SEARCH_DECIMAL;
               WinEnableControl( hwndDlg, IDD_SEARCH_CASE_SENS, FALSE );
               break;

            case IDD_SEARCH_HEX:
               szDisplayMode = "%X";
               ulItemID = IDD_SEARCH_HEX;
               WinEnableControl( hwndDlg, IDD_SEARCH_CASE_SENS, FALSE );
               break;

            case IDD_SEARCH_ASCII:
               szDisplayMode = "";
               ulItemID = IDD_SEARCH_ASCII;
               WinEnableControl( hwndDlg, IDD_SEARCH_CASE_SENS, TRUE );
               break;

            default:
               break;
         }
         break;

      case WM_HELP:
         ProcessHelpMessage( hwndDlg, mpParm1, mpParm2 );
         break;

      default:
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
   }

   return MRFROMSHORT ( FALSE ) ;
}

/************  PRIVATE CODE  ***************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   PromptAndReplace

Description:   Prompt the user whether or not to perform the indicated
               replacement and then conditionally perform that replacement.

       Args:   HWND - client window handle

       Uses:

   Modifies:

    Returns:   BOOL - TRUE if replacement performed, else FALSE

-------------------------------------------------------------------*/

BOOL  PromptAndReplace( HWND hwndWnd )
{
   void *pbNewPtr;
   BYTE *pbCurrentPtr;
   INT   iNewSize;
   INT   iSizeDelta;


   /*
    * Move the cursor to the selected offset.  This call will
    * also update the display.
    */

   MoveCursorToOffset( hwndWnd, iGoToOffset );

   /*
    * Ask the user if he/she wants to replace this.
    */

   if ( MBID_YES == DisplayMessageBox( IDS_REPLACEQ,
                                       IDS_SEARCHREPLACE,
                                       MB_YESNO | MB_ICONQUESTION | MB_MOVEABLE ) )
   {
      iSizeDelta = iReplaceBufferLength - iSearchBufferLength;

      if ( iSizeDelta > 0 )
      {
         /*
          * The replace buffer is larger than the search buffer, so we
          * have to expand the allocation, move everything past the current
          * cursor offset to the end of the buffer, and then do the replace.
          */

         /*
          * Attempt the reallocation.  If it fails, return without
          * trashing the existing buffer.
          */

         iNewSize = iFileBytes + iSizeDelta;
         if ( ( pbNewPtr = realloc( pbFileBuffer, iNewSize ) ) == NULL )
         {
            DisplayMessageBox( IDS_SEARCHREPBADALLOC, IDS_REPLACE, MB_OK | MB_ICONHAND );
            return FALSE;
         }

         /*
          * Move everything from the current cursor up to the end of file to the
          * offset iSizeDelta bytes from the current cursor.
          *
          * WARNING: this code updates iFileBytes _after_ performing the
          * move.  Caveat maintainer.
          */

         pbFileBuffer = (BYTE *)pbNewPtr;
         pbCurrentPtr = pbFileBuffer + QueryCursorOffset();
         memmove( pbCurrentPtr + iReplaceBufferLength,
                  pbCurrentPtr + iSearchBufferLength,
                  iFileBytes - QueryCursorOffset() - iSearchBufferLength );
         memcpy( pbCurrentPtr, szReplaceBuffer, iReplaceBufferLength );
         iFileBytes = iNewSize;
      }
      else if ( iSizeDelta < 0 )
      {
         /*
          * The search buffer is larger than the replace buffer, so we do the
          * replace, then move everything from one beyond the end of the search
          * buffer to one beyond the end of the replace buffer, then shrink
          * the allocation.
          */

         iNewSize = iFileBytes + iSizeDelta;
         pbCurrentPtr = pbFileBuffer + QueryCursorOffset();
         memcpy( pbCurrentPtr, szReplaceBuffer, iReplaceBufferLength );
         memmove( pbCurrentPtr + iReplaceBufferLength,
                  pbCurrentPtr + iSearchBufferLength,
                  iFileBytes - QueryCursorOffset() - iSearchBufferLength );

         if ( ( pbNewPtr = realloc( pbFileBuffer, iNewSize ) ) == NULL )
         {
            DisplayMessageBox( IDS_SEARCHREPBADALLOC, IDS_REPLACE, MB_OK | MB_ICONHAND );
            return FALSE;
         }

         pbFileBuffer = (BYTE *)pbNewPtr;
         iFileBytes = iNewSize;
      }
      else
      {
         /*
          * The search and replace buffers are the same length, so we can use a simple
          * memcpy() without any buffer resizing.
          */

         memcpy( pbFileBuffer + QueryCursorOffset(), szReplaceBuffer, iReplaceBufferLength );
      }

      /*
       * Update the display again to show the changes, and set the "file has
       * been changed" flag.  The cursor has already been position correctly, so
       * there's no need to do it again.
       */

      UpdateDisplay( hwndWnd );
      bFileChanged = TRUE;
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   QueryCharMatch

Description:   Determine whether two chars match, taking into account
               case sensitivity.

       Args:   CHAR - first char
               CHAR - second char
               BOOL - TRUE for case-sensitive compare

       Uses:

   Modifies:

    Returns:   BOOL - TRUE if characters match

-------------------------------------------------------------------*/

BOOL  QueryCharMatch( CHAR c1, CHAR c2, BOOL bCaseSens )
{
   if ( bCaseSens )
      return ( c1 == c2 );
   else
      return ( toupper(c1) == toupper(c2) );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   SearchForString

Description:   Search a given buffer for a given string, using case
               sensitivity if called for.

       Args:   CHAR * - pointer to buffer to search
               INT    - length of buffer
               INT    - pointer to current offset within buffer to search
               CHAR * - pointer to string to search for
               INT    - length of search string
               BOOL   - TRUE to do a case-sensitive search
               BOOL   - TRUE to search forward, FALSE to search backward
               INT *  - pointer to where to put found offset

       Uses:

   Modifies:

    Returns:   BOOL - TRUE if found (offset in piFindOffset)

-------------------------------------------------------------------*/

BOOL  SearchForString( CHAR *szHaystack, INT iHaystackLength, INT iHaystackOffset,
                       CHAR *szNeedle, INT iNeedleLength,
                       BOOL bCaseSens, BOOL bSearchForward,
                       INT *piFindOffset )
{
   INT   j;
   BYTE *pHaystack, *pHaystackStart, *pHaystackEnd;
   BYTE *pNeedle, *pNeedleStart, *pNeedleEnd;


   /*
    * If the number of bytes to search is greater than the length
    * of the buffer remaining to search, fail.
    */

   if ( bSearchForward )
   {
      if ( iNeedleLength > ( iHaystackLength - iHaystackOffset ) )
         return FALSE;
   }
   else
   {
      if ( iNeedleLength > iHaystackLength )
         return FALSE;
   }

   /*
    * We can handle this one.
    */

   pNeedleStart = szNeedle;
   pNeedleEnd = pNeedleStart + iNeedleLength + 1;
   pNeedle = pNeedleStart;

   pHaystackStart = szHaystack;
   if ( bSearchForward )
      pHaystackEnd = szHaystack + iHaystackLength - iNeedleLength + 1;
   else
      pHaystackEnd = szHaystack + iHaystackLength;
   pHaystack = pHaystackStart + iHaystackOffset;

   if ( bSearchForward )
   {
      while ( pHaystack < pHaystackEnd )
      {
         if ( QueryCharMatch( *pHaystack, *pNeedle, bCaseSens ) )
         {
            j = 0;

            while ( QueryCharMatch( *pHaystack++, *pNeedle++, bCaseSens ) && ( pNeedle < pNeedleEnd ) )
               j++;

            if ( j == iNeedleLength )
            {
               *piFindOffset = (INT)( pHaystack - pHaystackStart - j - 1 );
               return TRUE;
            }
            else
            {
               pHaystack -= j;
               pNeedle = pNeedleStart;
            }
         }
         else
         {
            pHaystack++;
         }
      }
   }
   else
   {
      while ( pHaystack >= pHaystackStart )
      {
         if ( QueryCharMatch( *pHaystack, *pNeedle, bCaseSens ) )
         {
            j = 0;

            while ( QueryCharMatch( *pHaystack++, *pNeedle++, bCaseSens ) && ( pNeedle < pNeedleEnd ) )
               j++;

            if ( j == iNeedleLength )
            {
               *piFindOffset = (INT)( pHaystack - pHaystackStart - j - 1 );
               return TRUE;
            }
            else
            {
               pHaystack -= j + 2;
               pNeedle = pNeedleStart;
            }
         }
         else
         {
            pHaystack--;
         }
      }
   }

   return FALSE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:

Description:

       Args:

       Uses:

   Modifies:

    Returns:

-------------------------------------------------------------------*/

/************  END OF FILE  ****************************************/

#endif   /* if !defined(H2SEARCH_C) */
