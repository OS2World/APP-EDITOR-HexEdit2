/********************************************************************

              Copyright (C) 1994 Bennett C. Baker and B::Ware

                          All rights reserved

     Module:   H2MAIN.C
     Author:   Bennett C. Baker
     CIS ID:   70274,3147
    Created:   14 March, 1994

Description:   Main routine for Hexedit/2, an OS/2 Hex editor

       $Log$

********************************************************************/
 
#if !defined(H2MAIN_C)
#define H2MAIN_C

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

MRESULT EXPENTRY ClientWndProc ( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );

MRESULT ProcessCommandMsg ( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );

BOOL  RestoreSettings( HWND hwndClient, HAB habAnchor );
BOOL  SaveSettings( HWND hwndClient, HAB habAnchor );
void  SetWindowTitle( HWND hwndWnd );
BOOL  QueryAbandonChanges( HWND hwndWnd );

INT   ReadFile( HWND hwndWnd, CHAR *szFileName );

/************  PUBLIC DATA *****************************************/

BOOL     bReadOnly = TRUE;
BOOL     bAutoResize = FALSE;

HAB      habAnchor;

HWND     hwndHScroll;
HWND     hwndVScroll;
HWND     hwndMenu;

/************  PRIVATE DATA ****************************************/

CHAR     szExecFile[ CCHMAXPATH ];
CHAR     szProfileFile[ CCHMAXPATH ];

const CHAR  szAppName[]       = "HexEdit/2";

const CHAR  szPrfBaseName[]   = "HEXEDIT2.INI";
const CHAR  szPrfAppName[]    = "B::Ware.HexEdit/2";

/************  MAIN ROUTINE ****************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   main

Description:   Main routine

       Args:   None

       Uses:   int - argc
               char ** - argv

   Modifies:

    Returns:

-------------------------------------------------------------------*/

INT main ( int argc, char **argv )
{
   ULONG    ulFlags;
   QMSG     qmMsg;
   HMQ      hmqQueue;
   HWND     hwndFrame;
   HWND     hwndClient;
   CHAR     szAppClass[MAX_STRING_BYTES];


   /*
    * Copy the fully-qualified executable filename into szExecFile.
    */

   strcpy( szExecFile, argv[0] );

   /*
    * Get the anchor block and initialize the message queue
    */

   habAnchor = WinInitialize ( 0 ) ;
   hmqQueue = WinCreateMsgQueue ( habAnchor, 0 ) ;

   /*
    * Run the initialization routines on all other modules.
    */

   h2cursor_init();
   h2edit_init();
   h2file_init();
   h2font_init();
   h2help_init();
   h2reg_init();
   h2screen_init();
   h2search_init();

   /*
    * Register the client window class name and window procedure.  Make the class
    * name the same as the application name.
    */

   strncpy( szAppClass, szAppName, MAX_STRING_BYTES );

   WinRegisterClass ( habAnchor,
                      szAppClass,
                      ClientWndProc,
                      CS_SIZEREDRAW,
                      0 ) ;

   /*
    * Create a standard window with vertical and horizontal scroll bars.  Keep it
    * invisible for now.
    */

   ulFlags = FCF_STANDARD | FCF_VERTSCROLL | FCF_HORZSCROLL;

   hwndFrame = WinCreateStdWindow ( HWND_DESKTOP,
                                    0,    /// WS_VISIBLE,
                                    &ulFlags,
                                    szAppClass,
                                    szAppName,
                                    0,
                                    NULLHANDLE,
                                    RES_CLIENT,
                                    &hwndClient ) ;

   /*
    * If frame creation was successful, proceed with the remainder
    * of the window startup tasks.
    */

   if ( hwndFrame != NULLHANDLE )
   {
      /*
       * Restore the window settings (if any) and display the window.
       */

      if ( RestoreSettings( hwndClient, habAnchor ) )
      {
         UpdateScreenLimits( hwndClient, TRUE );
      }

      DisplayAboutPanel( hwndClient, 2 );
      WinSetWindowPos( hwndFrame, HWND_TOP, 0, 0, 0, 0, SWP_ACTIVATE | SWP_SHOW );

      /*
       * Create a help instance.
       */

      CreateHelpInstance( habAnchor, hwndFrame, szExecFile );

      /*
       * If anything was specified on the command line, treat it as a file and
       * read it in.
       */

      if ( argc > 1 )
      {
         if ( FileOpenDialog( hwndClient, argv[1] ) )
         {
            SetWindowTitle( hwndClient );
            UpdateScreenLimits( hwndClient, TRUE );
         }
      }

      /*
       * Do the get message/dispatch message loop until done (WM_QUIT
       * posted to the message queue).
       */

      while ( WinGetMsg( habAnchor, &qmMsg, NULLHANDLE, 0, 0 ) )
         WinDispatchMsg ( habAnchor, &qmMsg );

      /*
       * Destroy the help instance.
       */

      DestroyHelpInstance();

      /*
       * And destroy the frame window (and all child windows).
       */

      WinDestroyWindow ( hwndFrame ) ;
   }

   /*
    * Cleanup - destroy the message queue, release the anchor block, and return.
    */

   WinDestroyMsgQueue ( hmqQueue ) ;
   WinTerminate ( habAnchor ) ;
   return 0 ;
}

/************  WINDOW PROCEDURE ************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ClientWndProc

Description:   Client window procedure (main procedure)

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   most globals

   Modifies:   most globals

    Returns:   MRESULT - usually FALSE

-------------------------------------------------------------------*/

MRESULT EXPENTRY ClientWndProc ( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   switch ( ulMsg )
   {
   // For HELP debugging
   #if 0
      case HM_ERROR:
         {
            CHAR     szTempBuffer[256];

            sprintf( szTempBuffer, "Received HM_ERROR message, mp1=0X%8.8X", (ULONG)mpParm1 );
            WinMessageBox ( HWND_DESKTOP, HWND_DESKTOP, szTempBuffer, "HM_ERROR", 0, MB_OK );
         }
         break;

      case HM_EXT_HELP_UNDEFINED:
         WinMessageBox ( HWND_DESKTOP, HWND_DESKTOP, "Received HM_EXT_HELP_UNDEFINED message", "HM_EXT_HELP_UNDEFINED", 0, MB_OK );
         break;

      case HM_HELPSUBITEM_NOT_FOUND:
         WinMessageBox ( HWND_DESKTOP, HWND_DESKTOP, "Received HM_HELPSUBITEM_NOT_FOUND message", "HM_HELPSUBITEM_NOT_FOUND", 0, MB_OK );
         break;
   #endif

      case HM_QUERY_KEYS_HELP:
         return (MRESULT)IDP_KEYS_INFO;
         break;

      case WM_BUTTON1CLICK:
         MoveCursorToPointer( hwndWnd, (*(POINTS *)&mpParm1).x, (*(POINTS *)&mpParm1).y );
         break;

   #if 0
      case WM_BUTTON2CLICK:
         {
            HPS      hpsClient;
            POINTL   ptl;
            INT      xC, yC;


            xC = ( ( (*(POINTS *)&mpParm1).x - cxChar) / cxChar );
            yC = ( ( cyClient - (*(POINTS *)&mpParm1).y ) / cyChar );

            hpsClient = WinGetPS( hwndWnd );
            GpiSetColor( hpsClient, CLR_PALEGRAY );
            if ( xC < 10 )
               GpiSetMix( hpsClient, FM_XOR );
            else if ( xC < 20 )
               GpiSetMix( hpsClient, FM_INVERT );
            else if ( xC < 30 )
               GpiSetMix( hpsClient, FM_NOTXORSRC );
            else if ( xC < 40 )
               GpiSetMix( hpsClient, FM_NOTMASKSRC );
            else if ( xC < 50 )
               GpiSetMix( hpsClient, FM_SUBTRACT );
///            GpiSetBackColor( hpsClient, CLR_PALEGRAY );
///            GpiSetBackMix( hpsClient, BM_OVERPAINT );
///            ptl.x = ( ( (*(POINTS *)&mpParm1).x - cxChar ) / cxChar ) * cxChar + cxChar;
///            ptl.y = ( ( (*(POINTS *)&mpParm1).y - cyClient ) / cyChar - 1 ) * cyChar + cyClient;
            ptl.x = cxChar + ( xC * cxChar );
            ptl.y = cyClient - ( ( yC + 1) * cyChar );
            GpiMove( hpsClient, &ptl );
            ptl.x += cxChar;
            ptl.y += cyChar;
            if ( yC < 5 )
               GpiBox( hpsClient, DRO_FILL, &ptl, 0, 0 );
            else
               GpiBox( hpsClient, DRO_OUTLINE, &ptl, 0, 0 );
///            GpiSetBackColor( hpsClient, CLR_BACKGROUND );
///            GpiSetMix( hpsClient, FM_DEFAULT );
            WinReleasePS( hpsClient );
         }
         break;
   #endif

      case WM_CHAR:
         if ( !( CHARMSG(&ulMsg)->fs & KC_KEYUP ) )
         {
            if ( CHARMSG(&ulMsg)->fs & KC_CHAR )
            {
               ProcessCharMsg( hwndWnd, ulMsg, mpParm1, mpParm2 );
            }
            else if ( CHARMSG(&ulMsg)->fs & KC_VIRTUALKEY )
            {
               if ( !ProcessCursorMsg( hwndWnd, ulMsg, mpParm1, mpParm2 ) )
               {
                  return WinDefWindowProc( hwndWnd, ulMsg, mpParm1, mpParm2 );
               }
            }
         }
         break;

      case WM_CLOSE:
         if ( QueryAbandonChanges( hwndWnd ) )
            return WinDefWindowProc ( hwndWnd, ulMsg, mpParm1, mpParm2 );
         break;

      case WM_COMMAND:
         return ProcessCommandMsg( hwndWnd, ulMsg, mpParm1, mpParm2 );
         break;

      case WM_CREATE:
         hwndMenu = WinWindowFromID( WinQueryWindow( hwndWnd, QW_PARENT ), FID_MENU );
         hwndHScroll = WinWindowFromID( WinQueryWindow( hwndWnd, QW_PARENT ), FID_HORZSCROLL );
         hwndVScroll = WinWindowFromID( WinQueryWindow( hwndWnd, QW_PARENT ), FID_VERTSCROLL );
         SetWindowTitle( hwndWnd );
         break;

      case WM_DESTROY:
         ClearFile( hwndWnd );
         break;

      case WM_ERASEBACKGROUND:
         return MRFROMSHORT ( TRUE ) ;

      case WM_HELP:
         ProcessHelpMessage( hwndWnd, mpParm1, mpParm2 );
         break;

      case WM_HSCROLL:
         if ( !ProcessHScrollMsg( hwndWnd, ulMsg, mpParm1, mpParm2 ) )
            return WinDefWindowProc( hwndWnd, ulMsg, mpParm1, mpParm2 );
         break;

      case WM_PAINT:
         PaintScreen( hwndWnd );
         break;

      case WM_SAVEAPPLICATION:
         SaveSettings( hwndWnd, habAnchor );
         return WinDefWindowProc( hwndWnd, ulMsg, mpParm1, mpParm2 );
         break;

      case WM_SETFOCUS:
         if ( SHORT1FROMMP( mpParm2 ) )
            CreateCursor( hwndWnd );
         else
            DestroyCursor( hwndWnd );
         break;

      case WM_SIZE:
         UpdateWindowSize( hwndWnd, SHORT1FROMMP( mpParm2), SHORT2FROMMP( mpParm2 ) );
         if ( hwndWnd == WinQueryFocus( HWND_DESKTOP ) )
         {
            DestroyCursor( hwndWnd );
            CreateCursor( hwndWnd );
         }
         break;

      case WM_VSCROLL:
         if ( !ProcessVScrollMsg( hwndWnd, ulMsg, mpParm1, mpParm2 ) )
            return WinDefWindowProc( hwndWnd, ulMsg, mpParm1, mpParm2 );
         break;

      default:
         return WinDefWindowProc( hwndWnd, ulMsg, mpParm1, mpParm2 );
         break;
   }

   return MRFROMSHORT ( FALSE ) ;
}

/************  PUBLIC CODE  ****************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   DisplayMessageBox

Description:   Call WinMessageBox with the appropriate parameters

       Args:   ULONG - string ID of the message to display
               ULONG - string ID of the window title
               ULONG - window style

       Uses:   habAnchor

   Modifies:

    Returns:   USHORT - return value of WinMessageBox

-------------------------------------------------------------------*/

USHORT   DisplayMessageBox( ULONG ulMessageID, ULONG ulTitleID, ULONG ulStyle )
{
   CHAR  szMessage[MAX_STRING_BYTES] = "";
   CHAR  szTitle[MAX_STRING_BYTES] = "";


   WinLoadString( habAnchor, NULLHANDLE, ulMessageID, sizeof( szMessage ), szMessage );
   WinLoadString( habAnchor, NULLHANDLE, ulTitleID, sizeof( szTitle ), szTitle );

   return WinMessageBox( HWND_DESKTOP,
                         HWND_DESKTOP,
                         szMessage,
                         szTitle,
                         0,
                         ulStyle );
}


/************  PRIVATE CODE  ***************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ProcessCommandMsg

Description:   Process WM_COMMAND messages for the main client window
               procedure.

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:

   Modifies:

    Returns:   MRESULT - result of operation (usually FALSE)

-------------------------------------------------------------------*/

MRESULT ProcessCommandMsg ( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   switch ( SHORT1FROMMP( mpParm1 ) )
   {
      /*
       * First, the menu selections.
       */

      case IDM_FILE_OPEN:
         if ( QueryAbandonChanges( hwndWnd ) )
         {
            if ( FileOpenDialog( hwndWnd, NULL ) )
            {
               SetWindowTitle( hwndWnd );
               UpdateScreenLimits( hwndWnd, TRUE );
            }
         }
         break;

      case IDM_FILE_SAVE:
         FileSaveDialog( hwndWnd, szFileName );
         SetWindowTitle( hwndWnd );
         break;

      case IDM_FILE_SAVE_AS:
         FileSaveDialog( hwndWnd, NULL );
         SetWindowTitle( hwndWnd );
         break;

      case IDM_FILE_CLEAR:
         if ( QueryAbandonChanges( hwndWnd ) )
         {
            ClearFile( hwndWnd );
            SetWindowTitle( hwndWnd );
            UpdateScreenLimits( hwndWnd, TRUE );
         }
         break;

      case IDM_FILE_ALLOCATE:
         if ( QueryAbandonChanges( hwndWnd ) )
         {
            if ( FileAllocateDialog( hwndWnd ) )
            {
               SetWindowTitle( hwndWnd );
               UpdateScreenLimits( hwndWnd, TRUE );
            }
         }
         break;

      case IDM_FILE_RESIZE:
         if ( FileResizeDialog( hwndWnd ) )
         {
            UpdateScreenLimits( hwndWnd, TRUE );
         }
         break;

      case IDM_FILE_EXIT:
         if ( QueryAbandonChanges( hwndWnd ) )
         {
            ClearFile( hwndWnd );
            WinPostMsg(hwndWnd, WM_QUIT, 0L, 0L);
         }
         break;

      case IDM_EDIT_CUT:
         if ( EditCut( hwndWnd ) )
         {
            SetWindowTitle( hwndWnd );
            UpdateScreenLimits( hwndWnd, TRUE );
         }
         break;

      case IDM_EDIT_COPY:
         if ( EditCopy( hwndWnd ) )
         {
            SetWindowTitle( hwndWnd );
            UpdateScreenLimits( hwndWnd, TRUE );
         }
         break;

      case IDM_EDIT_PASTE:
         if ( EditPaste( hwndWnd ) )
         {
            SetWindowTitle( hwndWnd );
            UpdateScreenLimits( hwndWnd, TRUE );
         }
         break;

      case IDM_EDIT_DELETE:
         if ( EditDelete( hwndWnd ) )
         {
            SetWindowTitle( hwndWnd );
            UpdateScreenLimits( hwndWnd, TRUE );
         }
         break;

      case IDM_EDIT_INSERT:
         if ( EditInsert( hwndWnd ) )
         {
            SetWindowTitle( hwndWnd );
            UpdateScreenLimits( hwndWnd, TRUE );
         }
         break;

      case IDM_SEARCH_FORWARD:
         SearchForward( hwndWnd );
         break;

      case IDM_SEARCH_BACKWARD:
         SearchBackward( hwndWnd );
         break;

      case IDM_SEARCH_AGAIN:
         SearchAgain( hwndWnd );
         break;

      case IDM_REPLACE_FORWARD:
         ReplaceForward( hwndWnd );
         break;

      case IDM_REPLACE_BACKWARD:
         ReplaceBackward( hwndWnd );
         break;

      case IDM_REPLACE_AGAIN:
         ReplaceAgain( hwndWnd );
         break;

      case IDM_GOTO_OFFSET:
         GotoOffset( hwndWnd );
         break;

      case IDM_OPTIONS_FONT:
         FontSelectDialog( hwndWnd );
         break;

      case IDM_OPTIONS_READONLY:
         bReadOnly ^= TRUE;
         WinSendMsg( hwndMenu, MM_SETITEMATTR,
                     MPFROM2SHORT( IDM_OPTIONS_READONLY, TRUE ),
                     MPFROM2SHORT( MIA_CHECKED, bReadOnly ? MIA_CHECKED : 0 ) );
         break;

      case IDM_OPTIONS_AUTORESIZE:
         bAutoResize ^= TRUE;
         WinSendMsg( hwndMenu, MM_SETITEMATTR,
                     MPFROM2SHORT( IDM_OPTIONS_AUTORESIZE, TRUE ),
                     MPFROM2SHORT( MIA_CHECKED, bAutoResize ? MIA_CHECKED : 0 ) );
         if ( bAutoResize )
            UpdateScreenLimits( hwndWnd, TRUE );
         break;

      case IDM_OPTIONS_REGISTER:
         DisplayRegistrationPanel( hwndWnd );
         break;

      case IDM_DISPLAY:
         DisplayModeDialog( hwndWnd );
         break;

      /*
       * Next, the DISPLAY dialog accelerator keys
       */

      case IDD_DISPLAY_OFFSET_OCTAL:
      case IDD_DISPLAY_OFFSET_DECIMAL:
      case IDD_DISPLAY_OFFSET_HEX:
      case IDD_DISPLAY_ENTITY_OCTAL:
      case IDD_DISPLAY_ENTITY_DECIMAL:
      case IDD_DISPLAY_ENTITY_HEX:
      case IDD_DISPLAY_1_BYTE:
      case IDD_DISPLAY_2_BYTE:
      case IDD_DISPLAY_4_BYTE:
         DisplayModeAccelKey( hwndWnd, SHORT1FROMMP( mpParm1 ) );
         break;

      /*
       * And the default
       */

      default:
         return WinDefWindowProc( hwndWnd, ulMsg, mpParm1, mpParm2 );
         break;
   }

   return MRFROMSHORT ( FALSE ) ;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   RestoreSettings

Description:   Restore settings from the HEXEDIT2.INI file

       Args:   HWND - client window handle
               HAB - anchor block handle

       Uses:   szPrfBaseName
               szExecFile

   Modifies:   szProfileFile

    Returns:   BOOL - TRUE if INI file readable

-------------------------------------------------------------------*/

BOOL  RestoreSettings( HWND hwndClient, HAB habAnchor )
{
   HINI  hiniPrf;
   CHAR *pLastSlash;


   /*
    * Construct a fully qualified profile file name (path and
    * file name) from the full path/name of the executable.
    */

   strncpy( szProfileFile, szExecFile, sizeof( szProfileFile ) );
   pLastSlash = strrchr( szProfileFile, '\\' );
   strcpy( pLastSlash + 1, szPrfBaseName );

   /*
    * Now open that profile file
    */

   hiniPrf = PrfOpenProfile( habAnchor, szProfileFile );

   /*
    * If the profile open was successful, restore the data therein
    */

   if ( hiniPrf != NULLHANDLE )
   {
      /*
       * Allow all modules to restore settings from the profile file
       */

      h2cursor_restore( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2edit_restore( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2file_restore( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2font_restore( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2help_restore( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2reg_restore( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2screen_restore( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2search_restore( hwndClient, hiniPrf, (PSZ)szPrfAppName );

      /*
       * Close the profile file and return TRUE
       */

      PrfCloseProfile( hiniPrf );
      return TRUE;
   }

   return FALSE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   SaveSettings

Description:   Construct the full path for the HEXEDIT2.INI file, and
               save settings to that file.

       Args:   HWND - client window handle
               HAB - anchor block handle

       Uses:   szProfileFile

   Modifies:   profile file contents

    Returns:   BOOL - TRUE if INI file was written

-------------------------------------------------------------------*/

BOOL  SaveSettings( HWND hwndClient, HAB habAnchor )
{
   HINI  hiniPrf;


   /*
    * Open the profile file
    */

   hiniPrf = PrfOpenProfile( habAnchor, szProfileFile );

   /*
    * If the profile open was successful, save what needs to be saved therein.
    */

   if ( hiniPrf != NULLHANDLE )
   {
      /*
       * Allow all modules to save settings to the profile file
       */

      h2cursor_save( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2edit_save( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2file_save( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2font_save( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2help_save( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2reg_save( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2screen_save( hwndClient, hiniPrf, (PSZ)szPrfAppName );
      h2search_save( hwndClient, hiniPrf, (PSZ)szPrfAppName );

      /*
       * Close the profile file and return TRUE
       */

      PrfCloseProfile( hiniPrf );
      return TRUE;
   }

   return FALSE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   SetWindowTitle

Description:   Sets the window title in the frame window with the
               program name and currently edited file name.

       Args:   HWND - handle of client window

       Uses:   szAppName, szFileName

   Modifies:   Application switch list

    Returns:   void

-------------------------------------------------------------------*/

void  SetWindowTitle( HWND hwndWnd )
{
   CHAR  szWinTitle[MAX_STRING_BYTES];
   CHAR  szUntitled[MAX_STRING_BYTES];


   /*
    * Use the application title as the first part of the window title
    */

   strncpy( szWinTitle, szAppName, MAX_STRING_BYTES );

   /*
    * If there is an allocated data buffer, concatenate a dash ( '-' ) and
    * either the name of the file or the string "(untitled)" to the window
    * title string.
    */

   if ( pbFileBuffer != NULL )
   {
      strncat( szWinTitle, " - ", MAX_STRING_BYTES );
      if ( *szFileName == '\0' )
      {
         WinLoadString( habAnchor, NULLHANDLE, IDS_UNTITLED, sizeof( szUntitled ), szUntitled );
         strncat( szWinTitle, szUntitled, MAX_STRING_BYTES );
      }
      else
      {
         strncat( szWinTitle, szFileName, MAX_STRING_BYTES );
      }
   }

   WinSetWindowText( WinQueryWindow( hwndWnd, QW_PARENT), szWinTitle );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   QueryAbandonChanges

Description:   Ask the user if he/she *really* wants to abandon his/her
               changes to the current buffer.

       Args:   HWND - client window handle

       Uses:   pbFileBuffer

   Modifies:   nothing

    Returns:   BOOL - TRUE if user selects YES (of if no file present)

-------------------------------------------------------------------*/

BOOL  QueryAbandonChanges( HWND hwndWnd )
{
   if ( bFileChanged )
   {
      return ( MBID_YES == DisplayMessageBox ( IDS_ABANDONQUERY,
                                               IDS_ABANDONTITLE,
                                               MB_YESNO | MB_ICONQUESTION | MB_MOVEABLE ) );
   }
   else
   {
      return TRUE;
   }
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

#endif   /* if !defined(H2MAIN_C) */
