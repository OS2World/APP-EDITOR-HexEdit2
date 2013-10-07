/********************************************************************

              Copyright (C) 1994 Bennett C. Baker and B::Ware

                          All rights reserved

     Module:   H2FILE.C
     Author:   Bennett C. Baker
     CIS ID:   70274,3147
    Created:   14 March, 1994

Description:   File I/O for HexEdit/2

       $Log$

********************************************************************/
 
#if !defined(H2FILE_C)
#define H2FILE_C

/************  INCLUDE FILES  **************************************/

#define INCL_DOSERRORS
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

INT   ReadFile( HWND hwndWnd, CHAR *szReadFileName );
BOOL  WriteFile( HWND hwndWnd, CHAR *szWriteFileName );
void  ReadWriteError( HWND hwndWnd, ULONG ulStringID, CHAR *szFileName );

MRESULT EXPENTRY AllocateDlgProc( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );
MRESULT EXPENTRY ResizeDlgProc( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );

MRESULT EXPENTRY FileOpenDlgProc( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );
MRESULT EXPENTRY FileSaveAsDlgProc( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );

/************  PUBLIC DATA *****************************************/

BOOL        bFileChanged = FALSE;
INT         iFileBytes = 0;
CHAR        szFileName[CCHMAXPATH] = "";
BYTE       *pbFileBuffer = NULL;

/************  PRIVATE DATA ****************************************/

FILEDLG     fdFileDlg;
INT         iNewSize;

/************  INITIALIZATION CODE  ********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2file_init

Description:   Initialize file control variables

       Args:   void

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void h2file_init( void )
{
   /*
    * Initialize the File Dialog structure for later use.
    */

   memset( &fdFileDlg, 0, sizeof( FILEDLG ) );
   fdFileDlg.cbSize = sizeof( FILEDLG );
   strcpy( fdFileDlg.szFullFile,"*.*" );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2file_restore

Description:   Restore file settings from the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2file_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2file_save

Description:   Save file settings to the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2file_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
}

/************  FILE I/O CODE ***************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ClearFile

Description:   Clears out all allocated file resources and updates the
               screen display.

       Args:   HWND - window handle

       Uses:   pbFileBuffer

   Modifies:   szFileName
               ulFileBytes
               pbFileBuffer

    Returns:   void

-------------------------------------------------------------------*/

void  ClearFile( HWND hwndWnd )
{
   /*
    * Set all the file-name dependencies to the "no-file" condition.  If
    * any memory is currently allocated, free it.
    */

   ShowCursor( hwndWnd, FALSE );
   MoveCursorHome( hwndWnd );
   SetEditMode( hwndWnd, EM_BINARY );
   bFileChanged = FALSE;
   iFileBytes = 0;
   *szFileName = '\0';

   if ( pbFileBuffer != NULL )
   {
      free( pbFileBuffer );
      pbFileBuffer = NULL;
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   FileOpenDialog

Description:   Read the selected file name, or run standard File Open
               dialog and read in the selected file.  If the read is
               successful, set the global file variables.

       Args:   HWND - window handle
               CHAR * - pointer to file name string (NULL to use Read dialog)

       Uses:   File Dialog

   Modifies:   pbFileBuffer, szFileName, iFileBytes

    Returns:   BOOL - TRUE if read successful

-------------------------------------------------------------------*/

BOOL  FileOpenDialog( HWND hwndWnd, CHAR *szOpenName )
{
   BOOL  result = FALSE;

   if ( szOpenName == NULL )
   {
      fdFileDlg.fl = FDS_CENTER | FDS_OPEN_DIALOG | FDS_HELPBUTTON;
      fdFileDlg.pfnDlgProc = FileOpenDlgProc;
      WinFileDlg( HWND_DESKTOP, hwndWnd, (PFILEDLG)&fdFileDlg );
      if ( fdFileDlg.lReturn == DID_OK )
      {
         ClearFile( hwndWnd );
         if ( ( iFileBytes = ReadFile( hwndWnd, fdFileDlg.szFullFile ) ) > 0 )
         {
            strncpy( szFileName, fdFileDlg.szFullFile, CCHMAXPATH );
            result = TRUE;
         }
      }
   }
   else
   {
      ClearFile( hwndWnd );
      if ( ( iFileBytes = ReadFile( hwndWnd, szOpenName ) ) > 0 )
      {
         strncpy( szFileName, szOpenName, CCHMAXPATH );
      }
      result = TRUE;
   }

   return result;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   FileSaveDialog

Description:   Run standard File Save/Save As dialog, and write the
               current file data to the selected file name.  Set the
               global file name to the selected file name.

       Args:   HWND - window handle
               CHAR * - pointer to file name string (NULL to use Save As)

       Uses:   File dialog

   Modifies:

    Returns:   BOOL - TRUE if write successful

-------------------------------------------------------------------*/

BOOL  FileSaveDialog( HWND hwndWnd, CHAR *szSaveName )
{
   BOOL  result = FALSE;

   if ( szSaveName == NULL || *szSaveName == '\0' )
   {
      fdFileDlg.fl = FDS_CENTER | FDS_SAVEAS_DIALOG | FDS_HELPBUTTON;
      fdFileDlg.pfnDlgProc = FileSaveAsDlgProc;
      WinFileDlg( HWND_DESKTOP, hwndWnd, (PFILEDLG)&fdFileDlg );
      if ( fdFileDlg.lReturn == DID_OK )
      {
         strncpy( szFileName, fdFileDlg.szFullFile, CCHMAXPATH );
         result = WriteFile( hwndWnd, szFileName );
      }
   }
   else
   {
      result = WriteFile( hwndWnd, szFileName );
   }
   return result;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   FileAllocateDialog

Description:   Bring up a dialog which asks the user how many bytes to
               allocate (for when there is no file yet).

       Args:   HWND - client window handle

       Uses:   nothing

   Modifies:   pbFileBuffer, iFileBytes

    Returns:   BOOL - TRUE if allocation succeeded

-------------------------------------------------------------------*/

BOOL  FileAllocateDialog( HWND hwndWnd )
{
   void *pbNewPtr;


   if ( WinDlgBox( HWND_DESKTOP, hwndWnd, AllocateDlgProc, NULLHANDLE, IDD_FILE_ALLOCATE, NULL ) )
   {
      if ( ( pbNewPtr = calloc( iNewSize, sizeof(BYTE) ) ) == NULL )
      {
         ReadWriteError( hwndWnd, IDS_ERRMEMALLOC, szFileName );
         return FALSE;
      }
      else
      {
         ClearFile( hwndWnd );
         iFileBytes = iNewSize;
         pbFileBuffer = (BYTE *)pbNewPtr;
         return TRUE;
      }
   }

   return FALSE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   FileResizeDialog

Description:   Bring up a dialog which allows the user to resize the
               current allocation (not down to zero, mind you!).

       Args:   HWND - client window handle

       Uses:   szFileName

   Modifies:   pbFileBuffer, iFileBytes

    Returns:   BOOL - TRUE if resize succeeded

-------------------------------------------------------------------*/

BOOL  FileResizeDialog( HWND hwndWnd )
{
   void *pbNewPtr;


   if ( WinDlgBox( HWND_DESKTOP, hwndWnd, ResizeDlgProc, NULLHANDLE, IDD_FILE_RESIZE, NULL ) )
   {
      if ( ( pbNewPtr = realloc( pbFileBuffer, iNewSize ) ) == NULL )
      {
         ReadWriteError( hwndWnd, IDS_ERRMEMRESIZE, szFileName );
         return FALSE;
      }
      else
      {
         iFileBytes = iNewSize;
         pbFileBuffer = (BYTE *)pbNewPtr;
         return TRUE;
      }
   }

   return FALSE;
}

/************  LOW LEVEL FILE I/O CODE  ****************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ReadFile

Description:   Open the file named in szFileName, read it into a
               buffer (allocated by this routine and pointed to by
               pbFileBuffer), and close the file.

       Args:   HWND - window handle
               CHAR * - read file name

       Uses:   szFileName

   Modifies:   pbFileBuffer, iFileBytes

    Returns:   INT - iFileBytes (0 if error on file read)

-------------------------------------------------------------------*/

INT ReadFile( HWND hwndWnd, CHAR *szReadFileName )
#if 0
{
   FILE    *fp;
   INT      iBytesRead;

   /*
    * Open the file (if possible)
    */

   if ( ( fp = fopen( szReadFileName, "rb" ) ) == NULL )
   {
      ReadWriteError( hwndWnd, IDS_ERRREADOPEN, szReadFileName );
      return 0;
   }

   /*
    * Determine the file's length by seeking to the end, then seek back to the
    * beginning to read it.
    */

   fseek( fp, 0, SEEK_END );
   iBytesRead = ftell( fp );
   fseek( fp, 0, SEEK_SET );

   /*
    * Error exit if the file is zero-length (nothing to edit)
    */

   if ( iBytesRead == 0 )
   {
      ReadWriteError( hwndWnd, IDS_ERRZEROLENFILE, szReadFileName );
      fclose( fp );
      return 0;
   }

   /*
    * Do the necessary malloc() to read in the entire file
    */

   if ( ( pbFileBuffer = (BYTE *)malloc( iBytesRead ) ) == NULL )
   {
      ReadWriteError( hwndWnd, IDS_ERRMEMALLOC, szReadFileName );
      fclose( fp );
      return 0;
   }

   /*
    * Read in the file into the newly-allocated buffer.
    */

   if ( fread( pbFileBuffer, sizeof( BYTE ), iBytesRead, fp ) != iBytesRead )
   {
      ReadWriteError( hwndWnd, IDS_ERRFILEREAD, szReadFileName );
      fclose( fp );
      return 0;
   }

   /*
    * Close the file and return its byte count.
    */

   fclose( fp );
   return iBytesRead;
}
#else
{
   APIRET      ret;
   HFILE       fh;
   ULONG       ulBytesRead;
   ULONG       ulActionTaken;
   ULONG       ulFileSize        = 0;
   ULONG       ulFileAttribute   = 0;
   ULONG       ulOpenFlag        = OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS;
   ULONG       ulOpenMode        = OPEN_SHARE_DENYREADWRITE | OPEN_ACCESS_READONLY;
   PEAOP2      ppEABuf           = NULL;
   FILESTATUS3 FileInfoBuf;


   // Open the file

   if ( ( ret = DosOpen( szReadFileName, &fh, &ulActionTaken, ulFileSize, ulFileAttribute, ulOpenFlag, ulOpenMode, ppEABuf ) ) != NO_ERROR )
   {
      ReadWriteError( hwndWnd, IDS_ERRREADOPEN, szReadFileName );
      return 0;
   }

   // Get the file length, error out if it's zero-length

   DosQueryFileInfo( fh, FIL_STANDARD, &FileInfoBuf, sizeof(FileInfoBuf) );

   if ( FileInfoBuf.cbFile == 0 )
   {
      ReadWriteError( hwndWnd, IDS_ERRZEROLENFILE, szReadFileName );
      DosClose( fh );
      return 0;
   }

   // Alloc a buffer for the file data, error if unable to alloc

   if ( ( pbFileBuffer = (BYTE *)malloc( FileInfoBuf.cbFile ) ) == NULL )
   {
      ReadWriteError( hwndWnd, IDS_ERRMEMALLOC, szReadFileName );
      DosClose( fh );
      return 0;
   }

   // Read the file, error if unable to read or if not all bytes get read in

   if (
           ( ( ret = DosRead( fh, pbFileBuffer, FileInfoBuf.cbFile, &ulBytesRead ) ) != NO_ERROR )
        || ( ulBytesRead != FileInfoBuf.cbFile )
      )
   {
      ReadWriteError( hwndWnd, IDS_ERRFILEREAD, szReadFileName );
      DosClose( fh );
      return 0;
   }

   DosClose( fh );
   return FileInfoBuf.cbFile;
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   WriteFile

Description:   Write out the data currently in memory to the current
               file.

       Args:   HWND - window handle
               CHAR * - read file name

       Uses:   ulFileBytes
               pbFileBuffer

   Modifies:   nothing

    Returns:   BOOL - TRUE if write successful

-------------------------------------------------------------------*/

BOOL WriteFile( HWND hwndWnd, CHAR *szWriteFileName )
#if 0
{
   FILE *fp;


   if ( ( fp = fopen( szWriteFileName, "wb" ) ) == NULL )
   {
      ReadWriteError( hwndWnd, IDS_ERRWRITEOPEN, szWriteFileName );
      return FALSE;
   }

   if ( fwrite( pbFileBuffer, sizeof( BYTE ), iFileBytes, fp) != iFileBytes )
   {
      ReadWriteError( hwndWnd, IDS_ERRFILEWRITE, szWriteFileName );
      fclose( fp );
      return FALSE;
   }

   fclose( fp );

   bFileChanged = FALSE;

   return TRUE;
}
#else
{
   APIRET   ret;
   HFILE    fh;
   ULONG    ulBytesWritten;
   ULONG    ulActionTaken;
   ULONG    ulFileSize        = iFileBytes;
   ULONG    ulFileAttribute   = 0;
   ULONG    ulOpenFlag        = OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS;
   ULONG    ulOpenMode        = OPEN_SHARE_DENYREADWRITE | OPEN_ACCESS_READWRITE;
   PEAOP2   ppEABuf           = NULL;


   if ( ( ret = DosOpen( szWriteFileName, &fh, &ulActionTaken, ulFileSize, ulFileAttribute, ulOpenFlag, ulOpenMode, ppEABuf ) ) != NO_ERROR )
   {
      ReadWriteError( hwndWnd, IDS_ERRWRITEOPEN, szWriteFileName );
      return FALSE;
   }

   if ( 
           ( ( ret = DosSetFileSize( fh, 0 ) ) != NO_ERROR )
        || ( ( ret = DosWrite( fh, pbFileBuffer, (ULONG)iFileBytes, &ulBytesWritten) ) != NO_ERROR )
      )
   {
      ReadWriteError( hwndWnd, IDS_ERRFILEWRITE, szWriteFileName );
      DosClose( fh );
      return FALSE;
   }

   DosClose( fh );
   bFileChanged = FALSE;
   return TRUE;
}
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ReadWriteError

Description:   Display any errors encountered while reading or writing
               files.

       Args:   HWND - window handle
               CHAR * - type of error string

       Uses:   szFileName

   Modifies:   nothing

    Returns:   void

-------------------------------------------------------------------*/

void  ReadWriteError( HWND hwndWnd, ULONG ulStringID, CHAR *szFileName )
{
   CHAR  szVar[MAX_STRING_BYTES];
   CHAR  szCommon[MAX_STRING_BYTES];
   CHAR  szError[MAX_STRING_BYTES];
   CHAR  szUntitled[MAX_STRING_BYTES];
   CHAR  szErrorMsg[CCHMAXPATH + 80];


   WinLoadString( habAnchor, NULLHANDLE, ulStringID, sizeof( szVar ), szVar );
   WinLoadString( habAnchor, NULLHANDLE, IDS_ERRFILEMSG, sizeof( szCommon ), szCommon );
   WinLoadString( habAnchor, NULLHANDLE, IDS_ERRERROR, sizeof( szError ), szError );
   WinLoadString( habAnchor, NULLHANDLE, IDS_UNTITLED, sizeof( szUntitled ), szUntitled );

   sprintf( szErrorMsg, szCommon, szVar );

   if ( szFileName == NULL || *szFileName == '\0' )
      strcat( szErrorMsg, szUntitled );
   else
      strcat( szErrorMsg, szFileName );

   WinMessageBox( HWND_DESKTOP,
                  hwndWnd,
                  szErrorMsg,
                  szError,
                  0,
                  MB_ICONHAND | MB_OK );
}

/************  DIALOG PROCEDURES ***********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   AllocateDlgProc

Description:   Manage input from the FILE ALLOCATE dialog

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   nothing

    Returns:   MRESULT - usually FALSE

-------------------------------------------------------------------*/

MRESULT EXPENTRY AllocateDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   static   CHAR *szDisplayMode = "%X";
   static   ULONG ulItemID = IDD_DISPLAY_ENTITY_HEX;

   CHAR  szNewSize[ MAX_STRING_BYTES ];


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
               WinQueryDlgItemText( hwndDlg, IDD_FILE_ALLOCATE_ENTRY, MAX_STRING_BYTES, szNewSize );
               sscanf( szNewSize, szDisplayMode, &iNewSize );
               if ( iNewSize > 0 )
               {
                  WinDismissDlg( hwndDlg, TRUE );
               }
               else
               {
                  ReadWriteError( hwndDlg, IDS_ERRMEMALLOCSIZE, szFileName );
                  WinSetDlgItemText( hwndDlg, IDD_FILE_ALLOCATE_ENTRY, "" );
               }
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

    Routine:   ResizeDlgProc

Description:   Manage the FILE RESIZE dialog

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   nothing

    Returns:   MRESULT - usually FALSE

-------------------------------------------------------------------*/

MRESULT EXPENTRY ResizeDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   static   CHAR  szOldSize[ MAX_STRING_BYTES ];
   static   CHAR *szDisplayMode = "%X";
   static   ULONG ulItemID = IDD_DISPLAY_ENTITY_HEX;

   CHAR  szNewSize[ MAX_STRING_BYTES ];


   switch ( ulMsg )
   {
      case WM_INITDLG:
         WinSendDlgItemMsg( hwndDlg, ulItemID, BM_SETCHECK, MPFROMSHORT( 1 ), NULL );
         sprintf( szOldSize, szDisplayMode, iFileBytes );
         WinSetDlgItemText( hwndDlg, IDD_FILE_RESIZE_OLD, szOldSize );
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );

      case WM_COMMAND:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case DID_OK:
               iNewSize = 0;
               *szNewSize = '\0';
               WinQueryDlgItemText( hwndDlg, IDD_FILE_RESIZE_NEW, MAX_STRING_BYTES, szNewSize );
               sscanf( szNewSize, szDisplayMode, &iNewSize );
               if ( iNewSize > 0 )
               {
                  WinDismissDlg( hwndDlg, TRUE );
               }
               else
               {
                  ReadWriteError( hwndDlg, IDS_ERRMEMALLOCRESIZE, szFileName );
                  WinSetDlgItemText( hwndDlg, IDD_FILE_RESIZE_NEW, "" );
               }
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
               sprintf( szOldSize, szDisplayMode, iFileBytes );
               WinSetDlgItemText( hwndDlg, IDD_FILE_RESIZE_OLD, szOldSize );
               break;

            case IDD_DISPLAY_ENTITY_DECIMAL:
               szDisplayMode = "%d";
               ulItemID = IDD_DISPLAY_ENTITY_DECIMAL;
               sprintf( szOldSize, szDisplayMode, iFileBytes );
               WinSetDlgItemText( hwndDlg, IDD_FILE_RESIZE_OLD, szOldSize );
               break;

            case IDD_DISPLAY_ENTITY_HEX:
               szDisplayMode = "%X";
               ulItemID = IDD_DISPLAY_ENTITY_HEX;
               sprintf( szOldSize, szDisplayMode, iFileBytes );
               WinSetDlgItemText( hwndDlg, IDD_FILE_RESIZE_OLD, szOldSize );
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


/************  STANDARD DIALOG OVERRIDE PROCEDURES  ****************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   FileOpenDlgProc

Description:   Filtering dialog procedure for the File Open dialog.  All
               it does is look for WM_HELP messages and pass the appropriate
               help screen id to DisplayHelpScreen - everything else goes
               to the WinDefFileDlgProc.

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   nothing

    Returns:   MRESULT - usually result of WinDefFileDlgProc

-------------------------------------------------------------------*/

MRESULT EXPENTRY FileOpenDlgProc( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   if ( ulMsg == WM_HELP )
   {
      DisplayHelpPanel( hwndWnd, IDP_FILE_OPEN_DLG );
      return (MRESULT)FALSE ;
   }

   return WinDefFileDlgProc( hwndWnd, ulMsg, mpParm1, mpParm2 );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   FileSaveAsDlgProc

Description:   Filtering dialog procedure for the File Save As dialog.  All
               it does is look for WM_HELP messages and pass the appropriate
               help screen id to DisplayHelpScreen - everything else goes
               to the WinDefFileDlgProc.

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   nothing

    Returns:   MRESULT - usually result of WinDefFileDlgProc

-------------------------------------------------------------------*/

MRESULT EXPENTRY FileSaveAsDlgProc( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   if ( ulMsg == WM_HELP )
   {
      DisplayHelpPanel( hwndWnd, IDP_FILE_SAVE_AS_DLG );
      return (MRESULT)FALSE ;
   }

   return WinDefFileDlgProc( hwndWnd, ulMsg, mpParm1, mpParm2 );
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

#endif   /* if !defined(H2FILE_C) */