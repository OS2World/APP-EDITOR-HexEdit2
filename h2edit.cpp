/********************************************************************

              Copyright (C) 1994-1996 Bennett C. Baker and B::Ware

                          All rights reserved

     Module:   H2EDIT.C
     Author:   Bennett C. Baker
     CIS ID:   70274,3147
    Created:   19 June, 1994

Description:   Top menu Edit functions for HexEdit/2

       $Log$

********************************************************************/
 
#if !defined(H2EDIT_C)
#define H2EDIT_C

/************  INCLUDE FILES  **************************************/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hexedit2.h"
#include "h2resrc.h"

/************  DEFINES, TYPEDEFS AND DATA STRUCTURE DEFINITIONS  ***/

/************  STATIC FUNCTION PROTOTYPES  *************************/

MRESULT EXPENTRY DeleteDlgProc( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );
MRESULT EXPENTRY InsertDlgProc( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );

/************  PUBLIC DATA *****************************************/

/************  PRIVATE DATA ****************************************/

INT   iDeleteCount;
INT   iInsertCount;
LONG  lInsertValue;

/************  INITIALIZATION CODE  ********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2edit_init

Description:   Initialize file control variables

       Args:   void

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void h2edit_init( void )
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2edit_restore

Description:   Restore file settings from the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2edit_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2edit_save

Description:   Save file settings to the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2edit_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
}

/************  PUBLIC EDIT CODE ************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:

Description:

       Args:   HWND - window handle
       Args:

       Uses:

   Modifies:

    Returns:

-------------------------------------------------------------------*/

BOOL  EditCut( HWND hwndClient )
{
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

BOOL  EditCopy( HWND hwndClient )
{
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

BOOL  EditPaste( HWND hwndClient )
{
   return FALSE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   EditInsert

Description:   Bring up a dialog box inquiring how many bytes to insert
               at the current cursor position, and attempt to increase
               the current buffer allocation by that amount.

               To do an insert we have to expand the allocation, move
               everything past the current cursor offset to the end of
               the buffer, and then zero out what just got moved.

       Args:   HWND - window handle

       Uses:   iFileBytes
               pbFileBuffer

   Modifies:   iFileBytes
               pbFileBuffer

    Returns:   BOOL - TRUE if operation successful

-------------------------------------------------------------------*/

BOOL  EditInsert( HWND hwndClient )
{
   void *pbNewPtr;
   BYTE *pbCurrentPtr;
   INT   iNewSize;


   if ( WinDlgBox( HWND_DESKTOP, hwndClient, InsertDlgProc, NULLHANDLE, IDD_EDIT_INSERT, NULL ) )
   {
      /*
       * If we have a zero value for bytes to insert, tell the user and exit
       */

      if ( iInsertCount == 0 )
      {
         DisplayMessageBox( IDS_INSERTBADVALUE, IDS_EDIT, MB_OK | MB_ICONHAND );
         return FALSE;
      }

      /*
       * Set the new size.
       */

      iNewSize = iFileBytes + iInsertCount;

      /*
       * Attempt the reallocation.  If it fails, return without
       * trashing the existing buffer.
       */

      if ( ( pbNewPtr = realloc( pbFileBuffer, iNewSize ) ) == NULL )
      {
         DisplayMessageBox( IDS_INSERTBADREALLOC, IDS_EDIT, MB_OK | MB_ICONHAND );
         return FALSE;
      }

      /*
       * Update pbFileBuffer and iFileBytes
       */

      pbFileBuffer = (BYTE *)pbNewPtr;
      iFileBytes = iNewSize;

      /*
       * Move everything from the current cursor up to the end of file to the
       * offset iInsertCount bytes from the current cursor.
       */

      pbCurrentPtr = pbFileBuffer + QueryCursorOffset();
      memmove( pbCurrentPtr + iInsertCount,
               pbCurrentPtr,
               iFileBytes - QueryCursorOffset() - iInsertCount );

      /*
       * Fill in what was just moved with what's in lInsertValue (8 bits only, please!)
       */

      memset( pbCurrentPtr, lInsertValue & 0xFF, iInsertCount );

      /*
       * Mark the buffer as changed
       */

      bFileChanged = TRUE;

      /*
       * Tell the world we've succeeded
       */

      return TRUE;
   }

   /*
    * Default (failure) return
    */

   return FALSE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   EditDelete

Description:   Bring up a dialog box inquiring how many bytes to delete
               at the current cursor position, and attempt to decrease
               the current buffer allocation by that amount.

               To do a delete we move everything from one byte beyond the
               end of the delete range to the current cursor position, and
               then shrink the allocation.

       Args:   HWND - window handle

       Uses:   iFileBytes
               pbFileBuffer

   Modifies:   iFileBytes
               pbFileBuffer

    Returns:   BOOL - TRUE if operation successful

-------------------------------------------------------------------*/

BOOL  EditDelete( HWND hwndClient )
{
   void *pbNewPtr;
   BYTE *pbCurrentPtr;
   INT   iNewSize;


   if ( WinDlgBox( HWND_DESKTOP, hwndClient, DeleteDlgProc, NULLHANDLE, IDD_EDIT_DELETE, NULL ) )
   {
      /*
       * SPECIAL - if we're at the beginning of the buffer and the user wants
       * to delete all of the buffer bytes, use FileClear instead.
       */

      if ( iDeleteCount == iFileBytes && QueryCursorOffset() == 0 )
      {
         ClearFile( hwndClient );
         return TRUE;
      }

      /*
       * If we have a zero value for bytes to delete or if bytes to delete is
       * greater than the number of bytes in the buffer beyond the current
       * cursor position, tell the user and exit.
       */

      if ( iDeleteCount == 0 || iDeleteCount > iFileBytes - QueryCursorOffset() )
      {
         DisplayMessageBox( IDS_DELETEBADVALUE, IDS_EDIT, MB_OK | MB_ICONHAND );
         return FALSE;
      }

      /*
       * Set the new size.
       */

      iNewSize = iFileBytes - iDeleteCount;

      /*
       * Move everything past the delete range to the current
       * cursor position.
       */

      pbCurrentPtr = pbFileBuffer + QueryCursorOffset();
      memmove( pbCurrentPtr,
               pbCurrentPtr + iDeleteCount,
               iFileBytes - QueryCursorOffset() - iDeleteCount );

      /*
       * Attempt the reallocation.  It _should_ succeed (we're
       * shrinking, not growing), but pop up an error if it fails.
       */

      if ( ( pbNewPtr = realloc( pbFileBuffer, iNewSize ) ) == NULL )
      {
         DisplayMessageBox( IDS_DELETEBADREALLOC, IDS_EDIT, MB_OK | MB_ICONHAND );
         return FALSE;
      }

      /*
       * Update pbFileBuffer and iFileBytes
       */

      pbFileBuffer = (BYTE *)pbNewPtr;
      iFileBytes = iNewSize;

      /*
       * Mark the buffer as changed
       */

      bFileChanged = TRUE;

      /*
       * Tell the world we've succeeded
       */

      return TRUE;
   }

   /*
    * Default (failure) return
    */

   return FALSE;
}

/************  DIALOG PROCEDURES ***********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   InsertDlgProc

Description:   Manage input from the EDIT INSERT dialog

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   nothing

    Returns:   MRESULT - usually FALSE

-------------------------------------------------------------------*/

MRESULT EXPENTRY InsertDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   static   CHAR *szDisplayMode = "%X";
   static   ULONG ulItemID = IDD_DISPLAY_ENTITY_HEX;

   CHAR  szNewSize[ MAX_STRING_BYTES ];


   switch ( ulMsg )
   {
      case WM_INITDLG:
         WinSendDlgItemMsg( hwndDlg, ulItemID, BM_SETCHECK, MPFROMSHORT( 1 ), NULL );
         WinSendDlgItemMsg( hwndDlg, IDD_EDIT_INSERT_VALUE, SPBM_SETLIMITS, MPFROMLONG( 255 ), MPFROMLONG( 0 ) );
         WinSendDlgItemMsg( hwndDlg, IDD_EDIT_INSERT_VALUE, SPBM_SETCURRENTVALUE, MPFROMLONG( 0 ), NULL );
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );

      case WM_COMMAND:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case DID_OK:
               iInsertCount = 0;
               *szNewSize = '\0';
               WinQueryDlgItemText( hwndDlg, IDD_EDIT_INSERT_ENTRY, MAX_STRING_BYTES, szNewSize );
               sscanf( szNewSize, szDisplayMode, &iInsertCount );
               WinSendDlgItemMsg( hwndDlg, IDD_EDIT_INSERT_VALUE, SPBM_QUERYVALUE, MPFROMP( &lInsertValue ), MPFROM2SHORT( 0, SPBQ_UPDATEIFVALID ) );
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
            case IDD_DISPLAY_ENTITY_OCTAL:
               szDisplayMode = "%o";
               ulItemID = IDD_DISPLAY_ENTITY_OCTAL;
               break;

            case IDD_DISPLAY_ENTITY_DECIMAL:
               szDisplayMode = "%d";
               ulItemID = IDD_DISPLAY_ENTITY_DECIMAL;
               break;

            case IDD_DISPLAY_ENTITY_HEX:
               szDisplayMode = "%X";
               ulItemID = IDD_DISPLAY_ENTITY_HEX;
               break;

            default:
               break;
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

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   DeleteDlgProc

Description:   Manage input from the EDIT DELETE dialog

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   nothing

    Returns:   MRESULT - usually FALSE

-------------------------------------------------------------------*/

MRESULT EXPENTRY DeleteDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   static   CHAR *szDisplayMode = "%X";
   static   ULONG ulItemID = IDD_DISPLAY_ENTITY_HEX;

   CHAR  szNewSize[ MAX_STRING_BYTES ];
   CHAR  szInsertVal[ MAX_STRING_BYTES ];


   switch ( ulMsg )
   {
      case WM_INITDLG:
         WinSendDlgItemMsg( hwndDlg, ulItemID, BM_SETCHECK, MPFROMSHORT( 1 ), NULL );
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );

      case WM_COMMAND:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case DID_OK:
               iDeleteCount = 0;
               *szNewSize = '\0';
               WinQueryDlgItemText( hwndDlg, IDD_EDIT_DELETE_ENTRY, MAX_STRING_BYTES, szNewSize );
               sscanf( szNewSize, szDisplayMode, &iDeleteCount );
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
            case IDD_DISPLAY_ENTITY_OCTAL:
               szDisplayMode = "%o";
               ulItemID = IDD_DISPLAY_ENTITY_OCTAL;
               break;

            case IDD_DISPLAY_ENTITY_DECIMAL:
               szDisplayMode = "%d";
               ulItemID = IDD_DISPLAY_ENTITY_DECIMAL;
               break;

            case IDD_DISPLAY_ENTITY_HEX:
               szDisplayMode = "%X";
               ulItemID = IDD_DISPLAY_ENTITY_HEX;
               break;

            default:
               break;
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

#endif   /* if !defined(H2EDIT_C) */