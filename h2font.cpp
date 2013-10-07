/********************************************************************

              Copyright (C) 1994 Bennett C. Baker and B::Ware

                          All rights reserved

     Module:   H2FONT.C
     Author:   Bennett C. Baker
     CIS ID:   70274,3147
    Created:   14 March, 1994

Description:   Font management for HexEdit/2

       $Log$

********************************************************************/
 
#if !defined(H2FONT_C)
#define H2FONT_C

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

void  GetFontMetrics( HWND hwndWnd, BOOL fCopy );
MRESULT EXPENTRY FontDlgProc( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );

/************  PUBLIC DATA *****************************************/

SHORT       cxChar, cyChar, cxCaps, cyDesc;

/************  PRIVATE DATA ****************************************/

FONTDLG     fdFontDlg;

CHAR        achFamily[256];
CHAR        achFont[256];

const CHAR  szPrfKeyFont[]    = "Font";
const CHAR  szDefaultFont[]   = "10.System Monospaced";

/************  INITIALIZATION CODE  ********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2font_init

Description:   Initialize Font variables

       Args:   void

       Uses:

   Modifies:   Font Dialog structure

    Returns:   void

-------------------------------------------------------------------*/

void h2font_init( void )
{
   /*
    * Initialize the font dialog structure for possible later use.
    */

   memset( &fdFontDlg, 0, sizeof( FONTDLG ) );
   fdFontDlg.fAttrs.usRecordLength = sizeof( FATTRS );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2font_restore

Description:   Restore font settings from the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2font_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
   PrfQueryProfileString( hiniPrf, pszApp, szPrfKeyFont, szDefaultFont, achFont, sizeof( achFont ) );
   WinSetPresParam( hwndWnd, PP_FONTNAMESIZE, strlen( achFont ) + 1, achFont );
   GetFontMetrics( hwndWnd, TRUE );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2font_save

Description:   Save font settings to the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2font_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
   PrfWriteProfileString( hiniPrf, pszApp, szPrfKeyFont, achFont );
}

/************  FONT MANAGEMENT CODE ********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   FontSelectDialog

Description:   Bring up the system font dialog to select a font.

       Args:   HWND - window handle (of client window)

       Uses:

   Modifies:   all font parameters

    Returns:   void

-------------------------------------------------------------------*/

void  FontSelectDialog( HWND hwndWnd )
{
   /*
    * Bring up the font selection dialog; display only FIXED-pitch fonts!
    * Save the client window handle in the ulUser field.
    */

   fdFontDlg.hpsScreen = WinGetPS( hwndWnd );
   fdFontDlg.cbSize = sizeof( FONTDLG );
   fdFontDlg.pszFamilyname = achFamily;
   fdFontDlg.usFamilyBufLen = sizeof( achFamily );
   fdFontDlg.clrFore = CLR_BLACK;
   fdFontDlg.pszPreview = "AaBb";
   fdFontDlg.fl = FNTS_CENTER | FNTS_FIXEDWIDTHONLY | FNTS_INITFROMFATTRS |
                  FNTS_HELPBUTTON | FNTS_RESETBUTTON | FNTS_APPLYBUTTON;
   fdFontDlg.pfnDlgProc = FontDlgProc;
   fdFontDlg.ulUser = (ULONG)hwndWnd;
   WinFontDlg( HWND_DESKTOP, hwndWnd, (PFONTDLG)&fdFontDlg );
   WinReleasePS( fdFontDlg.hpsScreen );

   /*
    * If a font was successfully selected, copy its attributes into achFont and
    * use that to set the default PM font parameter for the client window.
    */

   if ( fdFontDlg.lReturn == DID_OK )
   {
      sprintf( achFont, "%d.%s", FIXEDINT( fdFontDlg.fxPointSize ), fdFontDlg.fAttrs.szFacename );
      WinSetPresParam( hwndWnd, PP_FONTNAMESIZE, strlen( achFont ) + 1, achFont );
      GetFontMetrics( hwndWnd, FALSE );
      UpdateScreenLimits( hwndWnd, TRUE );
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   GetFontMetrics

Description:   Get the font metric information from the WPS

       Args:   HWND - window handle (of client window)
               BOOL - flag to copy info into Font Dialog

       Uses:

   Modifies:   cxChar, cxCaps, cyChar, cyDesc

    Returns:   void

-------------------------------------------------------------------*/

void GetFontMetrics( HWND hwndWnd, BOOL fCopy )
{
   HPS         hpsClient;
   FONTMETRICS fmFontMetrics;


   hpsClient = WinGetPS( hwndWnd );
   GpiQueryFontMetrics( hpsClient, sizeof( fmFontMetrics ), &fmFontMetrics );
   cxChar = fmFontMetrics.lAveCharWidth;
   cxCaps = fmFontMetrics.lEmInc;
   cyChar = fmFontMetrics.lMaxBaselineExt;
   cyDesc = fmFontMetrics.lMaxDescender;
   WinReleasePS( hpsClient );

   if ( fCopy )
   {
      // Transfer the default font attributes into the FATTRS block of the FontDlg
      fdFontDlg.fAttrs.fsSelection = 0;
      fdFontDlg.fAttrs.lMatch = fmFontMetrics.lMatch;
      strcpy( fdFontDlg.fAttrs.szFacename, fmFontMetrics.szFacename );
   }
}

/************  STANDARD DIALOG OVERRIDE PROCEDURES  ****************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   FontDlgProc

Description:   Filtering dialog procedure for the Font dialog.  All
               it does is look for WM_HELP messages and pass the appropriate
               help screen id to DisplayHelpScreen - everything else goes
               to the WinDefFontDlgProc.

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   nothing

    Returns:   MRESULT - usually result of WinDefFileDlgProc

-------------------------------------------------------------------*/

MRESULT EXPENTRY FontDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   HWND  hwndWnd;


   switch ( ulMsg )
   {
      case WM_HELP:
         DisplayHelpPanel( hwndDlg, IDP_OPTIONS_FONT_DLG );
         return (MRESULT)FALSE;

      case WM_COMMAND:
         switch ( SHORT1FROMMP( mpParm1 ) )
         {
            case DID_APPLY_BUTTON:
               hwndWnd = (HWND)fdFontDlg.ulUser;
               sprintf( achFont, "%d.%s", FIXEDINT( fdFontDlg.fxPointSize ), fdFontDlg.fAttrs.szFacename );
               WinSetPresParam( hwndWnd, PP_FONTNAMESIZE, strlen( achFont ) + 1, achFont );
               GetFontMetrics( hwndWnd, FALSE );
               UpdateScreenLimits( hwndWnd, TRUE );
               return (MRESULT)FALSE;

            default:
               return WinDefFontDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
         }

      default:
         return WinDefFontDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
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

#endif   /* if !defined(H2FONT_C) */