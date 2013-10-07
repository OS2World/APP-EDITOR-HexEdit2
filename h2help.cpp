/********************************************************************

              Copyright (C) 1994 Bennett C. Baker and B::Ware

                          All rights reserved

     Module:   H2HELP.C
     Author:   Bennett C. Baker
     CIS ID:   70274,3147
    Created:   14 March, 1994

Description:   Help routines for Hexedit/2, an OS/2 Hex editor

       $Log$

********************************************************************/
 
#if !defined(H2HELP_C)
#define H2HELP_C

/************  INCLUDE FILES  **************************************/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hexedit2.h"
#include "h2resrc.h"

/************  DEFINES, TYPEDEFS AND DATA STRUCTURE DEFINITIONS  ***/

///#define  PANELDEBUG

/************  STATIC FUNCTION PROTOTYPES  *************************/

/************  PUBLIC DATA *****************************************/

/************  PRIVATE DATA ****************************************/

CHAR  szHelpBaseName[]  = "HEXEDIT2.HLP";

HWND  hwndHelpInstance  = NULLHANDLE;

/************  INITIALIZATION CODE  ********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2help_init

Description:   Initialize dialog variables

       Args:   void

       Uses:

   Modifies:   ulDefaultXxxId and ulNewXxxId

    Returns:   void

-------------------------------------------------------------------*/

void h2help_init( void )
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2help_restore

Description:   Restore help settings from the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2help_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2help_save

Description:   Save help settings to the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2help_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
}

/************  PUBLIC CODE *****************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   CreateHelpInstance

Description:   Create a help instance for the application

       Args:   HAB - handle to original anchor block
               HWND - frame window handle.
               CHAR * - pointer to the EXECed routine (argv[0])

       Uses:

   Modifies:

    Returns:   BOOL - TRUE if help instance created.

-------------------------------------------------------------------*/

BOOL  CreateHelpInstance( HAB hab, HWND hwndFrame, CHAR *pszExecFile )
{
   HELPINIT    hini;
   CHAR        szWindowTitle[128];
   CHAR        szHelpFile[ CCHMAXPATH ];
   CHAR       *pLastSlash;


   /*
    * Initialize the help init structure
    */

   hini.cb = sizeof(HELPINIT);
   hini.pszHelpLibraryName = szHelpFile;
   hini.ulReturnCode = 0;
   hini.pszTutorialName = NULL;
   hini.phtHelpTable = (PHELPTABLE)MAKELONG(RES_CLIENT, 0xFFFF);
   hini.hmodHelpTableModule = 0;
   hini.hmodAccelActionBarModule = 0;
   hini.idAccelTable = 0;
   hini.idActionBar = 0;

   WinLoadString( hab, NULLHANDLE, IDS_HELPWINDOWTITLE, sizeof( szWindowTitle ), szWindowTitle );
   hini.pszHelpWindowTitle = szWindowTitle;

   /*
    * If debugging, show panel ids
    */

#ifdef PANELDEBUG
   hini.fShowPanelId = CMIC_SHOW_PANEL_ID;
#else
   hini.fShowPanelId = CMIC_HIDE_PANEL_ID;
#endif

   /*
    * Create the help instance using the Helpfile base name.  If that
    * doesn't work, try using a fully qualified file name in the
    * executable directory.  If that doesn't work, give up.
    */

   strcpy( szHelpFile, szHelpBaseName );
   hwndHelpInstance = WinCreateHelpInstance( hab, &hini );

   if ( hwndHelpInstance == NULLHANDLE || hini.ulReturnCode != 0 )
   {
      strncpy( szHelpFile, pszExecFile, sizeof( szHelpFile ) );
      pLastSlash = strrchr( szHelpFile, '\\' );
      strcpy( pLastSlash + 1, szHelpBaseName );
      hwndHelpInstance = WinCreateHelpInstance( hab, &hini );
   }

   if ( hwndHelpInstance == NULLHANDLE || hini.ulReturnCode != 0 )
   {
      DisplayMessageBox( IDS_ERRORHELPCREATE, IDS_HELP, MB_OK | MB_ERROR );
      return FALSE;
   }

   /*
    * Associate the help instance with the frame window
    */

   if ( !WinAssociateHelpInstance( hwndHelpInstance, hwndFrame ) )
   {
      DisplayMessageBox( IDS_ERRORHELPASSOCIATE, IDS_HELP, MB_OK | MB_ERROR );
      return FALSE;
   }

   /*
    * Help successfully initialized - return TRUE
    */

   return TRUE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   DestroyHelpInstance

Description:   Destroy the help instance

       Args:   void

       Uses:   hwndHelpInstance

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  DestroyHelpInstance( void )
{
   if ( hwndHelpInstance != NULLHANDLE )
      WinDestroyHelpInstance( hwndHelpInstance );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   DisplayHelpPanel

Description:   Display the help screen indicated by the specified
               help screen ID.

       Args:   HWND - client window handle
               SHORT - help screen ID

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  DisplayHelpPanel( HWND hwndWnd, SHORT sHelpID )
{
   WinSendMsg( hwndHelpInstance, HM_DISPLAY_HELP, MPFROMSHORT( sHelpID ), HM_RESOURCEID );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ProcessHelpMessage

Description:   Process all WM_HELP messages to the client window

       Args:   HWND - client window handle
               MPARAM - First parameter of the WM_HELP message
               MPARAM - Second parameter of the WM_HELP message

       Uses:   nothing

   Modifies:   nothing

    Returns:   void

-------------------------------------------------------------------*/

void ProcessHelpMessage( HWND hwndWnd, MPARAM mpParm1, MPARAM mpParm2 )
{
   switch ( SHORT1FROMMP( mpParm1 ) )
   {
      // Shouldn't get this particular one...
      case IDM_HELP:
         WinMessageBox ( HWND_DESKTOP, HWND_DESKTOP, "IDM_HELP", "WM_HELP message", 0, MB_OK );
         break;

      case IDM_HELP_CONTENTS:
         WinSendMsg( hwndHelpInstance, HM_HELP_CONTENTS, NULL, NULL );
         break;

      case IDM_HELP_INDEX:
         WinSendMsg( hwndHelpInstance, HM_HELP_INDEX, NULL, NULL );
         break;

      case IDM_HELP_GENERAL:
         WinSendMsg( hwndHelpInstance, HM_EXT_HELP, NULL, NULL );
         break;

      case IDM_HELP_USING:
         WinSendMsg( hwndHelpInstance, HM_DISPLAY_HELP, NULL, NULL );
         break;

      case IDM_HELP_KEYS:
         WinSendMsg( hwndHelpInstance, HM_KEYS_HELP, NULL, NULL );
         break;

      case IDM_HELP_ABOUT:
         DisplayAboutPanel( hwndWnd, NULL );
         break;

      default:
         WinSendMsg( hwndHelpInstance, HM_EXT_HELP, NULL, NULL );
         break;
   }
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

#endif   /* if !defined(H2HELP_C) */