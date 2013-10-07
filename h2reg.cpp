/********************************************************************

              Copyright (C) 1994 Bennett C. Baker and B::Ware

                          All rights reserved

     Module:   H2REG.C
     Author:   Bennett C. Baker
     CIS ID:   70274,3147
    Created:   14 March, 1994

Description:   Registration routines for Hexedit/2, an OS/2 Hex editor

       $Log$

********************************************************************/
 
#if !defined(H2REG_C)
#define H2REG_C

/************  INCLUDE FILES  **************************************/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hexedit2.h"
#include "h2resrc.h"
#include "h2reg.h"

/************  DEFINES, TYPEDEFS AND DATA STRUCTURE DEFINITIONS  ***/

/************  STATIC FUNCTION PROTOTYPES  *************************/

MRESULT EXPENTRY AboutDlgProc ( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );
MRESULT EXPENTRY RegisterDlgProc ( HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 );

int   CharToIndex( char cChar );
ULONG CRC32( BYTE *pb, int len, ULONG crc );
BOOL  StringToNibbleArray( char *pszString, BYTE *pcNibbleArray, int iMaxNibbles );
BOOL  ValidateRegistrationData( CHAR *pszRegUserName, CHAR *pszRegSerNum, CHAR *pszRegValCode );
void  MakeRegistrationCurrent( void );

/************  PUBLIC DATA *****************************************/

/************  PRIVATE DATA ****************************************/

// Profile keys names and associated key values (as strings)

const CHAR  szPrfKeyRegUserName[]   = "UserName";
const CHAR  szPrfKeyRegSerialNum[]  = "SerialNumber";
const CHAR  szPrfKeyRegValCode[] = "ValidationCode";

CHAR  szRegUserName[MAX_STRING_BYTES]  = "";
CHAR  szRegSerialNum[MAX_STRING_BYTES] = "";
CHAR  szRegValCode[MAX_STRING_BYTES]   = "";

// For display in the Help About box

CHAR  szVerString[MAX_STRING_BYTES];
CHAR  szRegString[MAX_STRING_BYTES];
CHAR  szNumString[MAX_STRING_BYTES];
CHAR  szSerString[MAX_STRING_BYTES];

// Number of users and last valid version (derived from registration data)

INT   iNumUsers         = 0;
INT   iLastVersion      = 0;

// Internal flag - whether valid registration data was entered during this session

BOOL  bRegistrationData = FALSE;

/************  INITIALIZATION CODE  ********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2reg_init

Description:   Initialize dialog variables

       Args:   void

       Uses:

   Modifies:   ulDefaultXxxId and ulNewXxxId

    Returns:   void

-------------------------------------------------------------------*/

void h2reg_init( void )
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2reg_restore

Description:   Restore help settings from the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2reg_restore( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
   ULONG ulNameLen, ulIDLen, ulSumLen;
   CHAR  szTemp[MAX_STRING_BYTES];


   ulNameLen   = PrfQueryProfileString( hiniPrf, pszApp, szPrfKeyRegUserName, NULL, szRegUserName, sizeof(szRegUserName) );
   ulIDLen     = PrfQueryProfileString( hiniPrf, pszApp, szPrfKeyRegSerialNum, NULL, szRegSerialNum, sizeof(szRegSerialNum) );
   ulSumLen    = PrfQueryProfileString( hiniPrf, pszApp, szPrfKeyRegValCode, NULL, szRegValCode, sizeof(szRegValCode) );

   WinLoadString( habAnchor, NULLHANDLE, IDS_VERSIONSTRING, sizeof( szTemp ), szTemp );
   sprintf( szVerString, szTemp, VERSION_MAJOR, VERSION_MINOR_TENTHS, VERSION_MINOR_HUNDREDTHS );

   if ( ValidateRegistrationData( szRegUserName, szRegSerialNum, szRegValCode ) )
   {
      MakeRegistrationCurrent();
      WinSendMsg( hwndMenu, MM_DELETEITEM, MPFROM2SHORT( IDM_OPTIONS_REGISTER, TRUE ), (MPARAM)NULL );
   }
   else
   {
      WinLoadString( habAnchor, NULLHANDLE, IDS_UNREG, sizeof( szRegString ), szRegString );
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   h2reg_save

Description:   Save help settings to the application profile file

       Args:   HWND - client window handle
               HINI - INI file handle
               PSZ  - pointer to profile application name

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  h2reg_save( HWND hwndWnd, HINI hiniPrf, PSZ pszApp )
{
   if ( bRegistrationData )
   {
      PrfWriteProfileString( hiniPrf, pszApp, szPrfKeyRegUserName, szRegUserName );
      PrfWriteProfileString( hiniPrf, pszApp, szPrfKeyRegSerialNum, szRegSerialNum );
      PrfWriteProfileString( hiniPrf, pszApp, szPrfKeyRegValCode, szRegValCode );
   }
}

/************  PUBLIC CODE *****************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   DisplayAboutPanel

Description:   Display the "About" box for the specified number of seconds

       Args:   HWND - client window handle
               ULONG - number of seconds to display box for

       Uses:

   Modifies:

    Returns:   void

-------------------------------------------------------------------*/

void  DisplayAboutPanel( HWND hwndWnd, ULONG ulDisplaySecs )
{
   WinDlgBox( HWND_DESKTOP, hwndWnd, AboutDlgProc, NULLHANDLE, IDD_HELP_ABOUT, (PVOID)ulDisplaySecs );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   DisplayRegistrationPanel

Description:   Display the Registration dialog and get the registration info

       Args:   HWND - window handle of client window

       Uses:

   Modifies:

    Returns:

-------------------------------------------------------------------*/

void  DisplayRegistrationPanel( HWND hwndWnd )
{
   WinDlgBox( HWND_DESKTOP, hwndWnd, RegisterDlgProc, NULLHANDLE, IDD_REGISTER, NULL );
}

/************  DIALOG PROCEDURES ***********************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   AboutDlgProc

Description:   Display the "About" product info screen

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   nothing

    Returns:   MRESULT - usually FALSE

-------------------------------------------------------------------*/

MRESULT EXPENTRY AboutDlgProc ( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   static ULONG ulTimerID;


   switch ( ulMsg )
   {
      case WM_INITDLG:
         WinSetDlgItemText( hwndDlg, IDD_HELP_VERSION, szVerString );
         WinSetDlgItemText( hwndDlg, IDD_HELP_REGTO, szRegString );
         WinSetDlgItemText( hwndDlg, IDD_HELP_NUMUSERS, szNumString );
         WinSetDlgItemText( hwndDlg, IDD_HELP_SERIALNUM, szSerString );
         if ( mpParm2 )
            ulTimerID = WinStartTimer( habAnchor, hwndDlg, TID_USERMAX, 1000 * (ULONG)mpParm2);
         else
            ulTimerID = 0;
         break;

      case WM_COMMAND:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case DID_OK:
               if ( ulTimerID )
                  WinStopTimer( habAnchor, hwndDlg, TID_USERMAX );
               WinDismissDlg( hwndDlg, FALSE );
               break;

            default:
               return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
         }
         break;

      case WM_TIMER:
         WinDismissDlg( hwndDlg, FALSE );
         break;

      default:
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
   }

   return MRFROMSHORT ( FALSE ) ;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   RegisterDlgProc

Description:   Display the "Register HexEdit/2" product info screen

       Args:   HWND - window handle
               ULONG - message type
               MPARAM - parameter 1
               MPARAM - parameter 2

       Uses:   nothing

   Modifies:   nothing

    Returns:   MRESULT - usually FALSE

-------------------------------------------------------------------*/

MRESULT EXPENTRY RegisterDlgProc ( HWND hwndDlg, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2 )
{
   switch ( ulMsg )
   {
      case WM_COMMAND:
         switch ( SHORT1FROMMP ( mpParm1 ) )
         {
            case DID_OK:
               WinQueryDlgItemText( hwndDlg, IDD_REGISTER_USERNAME, MAX_STRING_BYTES, szRegUserName );
               WinQueryDlgItemText( hwndDlg, IDD_REGISTER_USERID, MAX_STRING_BYTES, szRegSerialNum );
               WinQueryDlgItemText( hwndDlg, IDD_REGISTER_CHECKSUM, MAX_STRING_BYTES, szRegValCode );
               if ( ValidateRegistrationData( szRegUserName, szRegSerialNum, szRegValCode ) )
               {
                  bRegistrationData = TRUE;
                  MakeRegistrationCurrent();
                  WinSendMsg( hwndMenu, MM_DELETEITEM, MPFROM2SHORT( IDM_OPTIONS_REGISTER, TRUE ), (MPARAM)NULL );
                  WinDismissDlg( hwndDlg, FALSE );
               }
               else
               {
                  DisplayMessageBox ( IDS_BADREGSTRING, IDS_BADREGTITLE,
                                               MB_OK | MB_WARNING | MB_MOVEABLE );
               }
               break;

            default:
               return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
         }
         break;

      default:
         return WinDefDlgProc( hwndDlg, ulMsg, mpParm1, mpParm2 );
   }

   return MRFROMSHORT ( FALSE ) ;
}


/************  PRIVATE CODE  ***************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   ValidateRegistrationData

Description:   Verify the supplied registration data

       Args:   CHAR * - Registration Name
               CHAR * - Registration ID
               CHAR * - Registration Checksum

       Uses:

   Modifies:   iNumUsers

    Returns:   BOOL - TRUE if data is valid, FALSE if not

-------------------------------------------------------------------*/

BOOL  ValidateRegistrationData( CHAR *pszRegUserName, CHAR *pszRegSerialNum, CHAR *pszRegValCode )
{
   ULONG ulCRCN, ulCRCS;
   BYTE  bNibbleArray[ NIBBLE_COUNT ];
   int   j;


   // If any of the strings are null or zero-length, the registration is invalid

   if (
         ( pszRegUserName  == NULL ) || ( *pszRegUserName  == '\0' ) ||
         ( pszRegSerialNum == NULL ) || ( *pszRegSerialNum == '\0' ) ||
         ( pszRegValCode   == NULL ) || ( *pszRegValCode   == '\0' )
      )
      return FALSE;


   // We have strings, now compute the rest of the information

   ulCRCN = CRC32( (BYTE *)pszRegUserName, strlen( pszRegUserName ), 0 );
   ulCRCS = CRC32( (BYTE *)pszRegSerialNum, strlen( pszRegSerialNum ), 0 );

   if ( !StringToNibbleArray( pszRegValCode, bNibbleArray, NIBBLE_COUNT ) )
      return FALSE;

   for ( j = 0; j < NIBBLE_COUNT - 1; j++ )
      bNibbleArray[j] ^= bNibbleArray[9];

   if (
         ( bNibbleArray[0] != ((ulCRCN >>  0) & 0x0F) ) ||
         ( bNibbleArray[1] != ((ulCRCS >>  4) & 0x0F) ) ||
         ( bNibbleArray[2] != ((ulCRCN >> 24) & 0x0F) ) ||
         ( bNibbleArray[3] != ((ulCRCS >> 28) & 0x0F) )
      )
      return FALSE;

   iNumUsers = bNibbleArray[4] + (bNibbleArray[5] << 4) + (bNibbleArray[6] << 8) + (bNibbleArray[7] << 12) ;

   iLastVersion = bNibbleArray[8];

   return TRUE;
}

/************  BIT BANGING  ****************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   crc32

Description:   Generate 32-bit CRC

       Args:   BYTE *  - pointer to string
               INT      - length of string
               ULONG    - 32-bit "seed" crc

       Uses:

   Modifies:

    Returns:   ULONG - 32-bit CRC

-------------------------------------------------------------------*/

ULONG crc32_map( BYTE b )
{
   int   i;
   ULONG v;

   v = b;
   for ( i = 8; --i >= 0; )
      v = ( ( v & 1 ) ? ( ( v >> 1 ) ^ POLYNOMIAL ) : ( v >> 1 ) );
   return v;
}

ULONG CRC32(BYTE *p, int len, ULONG crc)
{
   while (len--)
      crc = crc32_map((BYTE)(crc ^ *p++)) ^ (crc >> 8);
   return crc;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   StringToNibbleArray

Description:   Converts the Validation Code string into an array of nibbles

       Args:

       Uses:

   Modifies:

    Returns:   BOOL - TRUE if converted successfully, FALSE if not

-------------------------------------------------------------------*/

BOOL StringToNibbleArray( char *pszString, BYTE *pbNibbleArray, int iMaxNibbles )
{
   int      j;
   int      iStrlen;
   int      iCharIndex;
   int      iNibbleIndex;
   int      iBitCount;
   ULONG    ulTemp;


   iStrlen = strlen(pszString);
   if ( (iStrlen * 5) > (iMaxNibbles * 4) )
      return FALSE;

   iNibbleIndex = 0;
   iBitCount = 0;
   ulTemp = 0;

   for ( j = 0; j < iStrlen; j++ )
   {
      if ( ( iCharIndex = CharToIndex( pszString[j] ) ) == -1 )
         return FALSE;

      ulTemp |= iCharIndex << iBitCount;
      iBitCount += 5;
      while ( iBitCount >= 4 )
      {
         pbNibbleArray[iNibbleIndex] = ulTemp & 0x0F;
         ulTemp >>= 4;
         iBitCount -= 4;
         iNibbleIndex++;
      }
   }

   return TRUE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   CharToIndex

Description:   Converts ASCII character into index into szXlateArray

       Args:   char - character to convert

       Uses:   szXlateArray

   Modifies:

    Returns:   int - index of character in array (-1 if not found)

-------------------------------------------------------------------*/

int CharToIndex( char cChar )
{
   int      j;

   for ( j = 0; j < strlen(szXlateArray); j++ )
   {
      if ( toupper(cChar) == toupper(szXlateArray[j]) )
         return j;
   }

   return -1;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   MakeRegistrationCurrent

Description:   Bring the registration info up to date, so a Help About shows the
               proper stuff.

       Args:   void

       Uses:   szRegUserName, szRegSerialNum, iNumUsers

   Modifies:   szRegString, szNumString, szSerString

    Returns:   void

-------------------------------------------------------------------*/

void  MakeRegistrationCurrent( void )
{
   int   j;
   char  szTemp[ MAX_STRING_BYTES ];


   // First, the User Name

   WinLoadString( habAnchor, NULLHANDLE, IDS_REGTO, sizeof( szRegString ), szRegString );
   strcat( szRegString, szRegUserName );

   // Next the license count

   switch ( iNumUsers )
   {
      case  USERS_COMPANY:
         j = IDS_COMPANYLICENSE;
         break;

      case  USERS_SITE:
         j = IDS_SITELICENSE;
         break;

      case  USERS_PROJECT:
         j = IDS_PROJECTLICENSE;
         break;

      default:
         j = IDS_NUMUSERS;
         break;
   }
   WinLoadString( habAnchor, NULLHANDLE, j, sizeof( szNumString), szTemp );
   sprintf( szNumString, szTemp, iNumUsers );

   // Next, the serial number

   WinLoadString( habAnchor, NULLHANDLE, IDS_SERIALNUMBER, sizeof( szNumString ), szSerString );
   strcat( szSerString, szRegSerialNum );
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

#endif   /* if !defined(H2REG_C) */