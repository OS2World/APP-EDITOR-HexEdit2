/********************************************************************

            Copyright (C) 1994 - 1996 Bennett C. Baker and B::Ware

                          All rights reserved

     Module:   GENREG.CPP
     Author:   Bennett C. Baker
     CIS ID:   70274,3147
   Internet:   70274.3147@compuserve.com
    Created:   17-Jul-1995

Description:   Generates registration keys for HexEdit/2 registrants

       $Log$

********************************************************************/
 
#if !defined(GENREG_CPP)
#define GENREG_CPP

/************  INCLUDE FILES  **************************************/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "hexedit2.h"
#include "h2reg.h"

/************  DEFINES, TYPEDEFS AND DATA STRUCTURE DEFINITIONS  ***/

#define  NO_ERROR       0
#define  ERROR          1

/************  FUNCTION PROTOTYPES  ********************************/

int   CharToIndex( char cChar );
int   NibblesToString( char *pcNibbleArray, int iNibbleCount, char *pcStringArray, int iMaxStringLength );
int   StringToNibbles( char *pszString, char *pcNibbleArray, int iMaxNibbles );
void  DisplayNibbleArray( char *cNibbleArray, int iNibbleCount );
ULONG crc32( char *pString );

/************  PUBLIC DATA *****************************************/

/************  PRIVATE DATA ****************************************/

char     szUserName     [ MAX_STRING_BYTES + 3 ] = { MAX_STRING_BYTES };
char     szSerialNumber [ MAX_STRING_BYTES + 3 ] = { MAX_STRING_BYTES };
char     szLicenseCount [ MAX_STRING_BYTES + 3 ] = { MAX_STRING_BYTES };
char     szCutoffVersion[ MAX_STRING_BYTES + 3 ] = { MAX_STRING_BYTES };

char    *pszUserName      = szUserName + 2;
char    *pszSerialNumber  = szSerialNumber + 2;
char    *pszLicenseCount  = szLicenseCount + 2;
char    *pszCutoffVersion = szCutoffVersion + 2;

const CHAR szXlateString[XLATE_LEN + 1]     = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";

ULONG    ulSerialNumber;
USHORT   usLicenseCount;
USHORT   usCutoffVersion;

ULONG    ulCRCU;
ULONG    ulCRCS;

char     cNibbleArray[ NIBBLE_COUNT ];
char     cStringArray[ MAX_STRING_BYTES ];

BOOL     bDebug = FALSE;

/************  MAIN ROUTINE ****************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   main

Description:   Main routine.  Can be called with arguments in order:
               username, serialnum, licensecount, cutoffversion.

       Args:

       Uses:

   Modifies:

    Returns:

-------------------------------------------------------------------*/

int   main( int argc, char **argv )
{
   int      j;


   // Initialize the data areas

   memset( cNibbleArray, 0, NIBBLE_COUNT );
   memset( pszUserName, 0, MAX_STRING_BYTES );
   memset( pszSerialNumber, 0, MAX_STRING_BYTES );
   memset( pszLicenseCount, 0, MAX_STRING_BYTES );
   memset( pszCutoffVersion, 0, MAX_STRING_BYTES );

   // Scan the argument list for data

   for ( j = 1; j < argc; j++ )
   {
      if ( *pszUserName == '\0' )
      {
         strcpy( pszUserName, argv[j] );
      }
      else if ( *pszSerialNumber == '\0' )
      {
         strcpy( pszSerialNumber, argv[j] );
      }
      else if ( *pszLicenseCount == '\0' )
      {
         strcpy( pszLicenseCount, argv[j] );
      }
      else if ( *pszCutoffVersion == '\0' )
      {
         strcpy( pszCutoffVersion, argv[j] );
      }
      else
      {
         printf( "Warning: argument #%d ""%s"" ignored\n", j, argv[j] );
      }
   }

   // Get the user data if none was specified on the command line

   if ( *pszUserName == '\0' )
   {
      _cputs( "User Name: " );
      _cgets( szUserName );
      printf( "\n" );
   }

   if ( *pszSerialNumber == '\0' )
   {
      _cputs( "Serial Number: " );
      _cgets( szSerialNumber );
      printf( "\n" );
   }

   if ( *pszLicenseCount == '\0' )
   {
      _cputs( "Number of Licenses (num|C|P|S): " );
      _cgets( szLicenseCount );
      printf( "\n" );
   }

   if ( *pszCutoffVersion == '\0' )
   {
      _cputs( "Cutoff Version: " );
      _cgets( szCutoffVersion );
      printf( "\n" );
   }

   printf( "\n" );

   // Convert and display the user data

   if ( toupper(*pszLicenseCount) == 'C' )
      usLicenseCount = USERS_COMPANY;
   else if ( toupper(*pszLicenseCount) == 'S' )
      usLicenseCount = USERS_SITE;
   else if ( toupper(*pszLicenseCount) == 'P' )
      usLicenseCount = USERS_PROJECT;
   else
      sscanf( pszLicenseCount, "%hu", &usLicenseCount );

   sscanf( pszSerialNumber, "%lu", &ulSerialNumber );
   sscanf( pszCutoffVersion, "%hu", &usCutoffVersion );

   printf( "         User Name: %s\n", pszUserName );
   printf( "     Serial Number: %lu\n", ulSerialNumber );
   if ( usLicenseCount == USERS_COMPANY )
      printf( "Number of Licenses: COMPANY LICENSE\n" );
   else if ( usLicenseCount == USERS_SITE )
      printf( "Number of Licenses: SITE LICENSE\n" );
   else if ( usLicenseCount == USERS_PROJECT )
      printf( "Number of Licenses: PROJECT LICENSE\n" );
   else if ( usLicenseCount <= MAX_USERS )
      printf( "Number of Licenses: %hu\n", usLicenseCount );
   else
   {
      printf( "Number of Licenses: ERROR!!!!\n" );
      return;
   }
   printf( "    Cutoff Version: %hu\n", usCutoffVersion );

   ulCRCU = crc32( pszUserName );
   ulCRCS = crc32( pszSerialNumber );

   if ( bDebug )
   {
      printf( "User Name crc32     = %lX\n", ulCRCU );
      printf( "Serial Number crc32 = %lX\n", ulCRCS );
   }

   // Compute the nibble array, display it, mung it, display it again,
   // and then stringify it and display it.

   cNibbleArray[0] = (ulCRCU >>  0) & 0x0F;
   cNibbleArray[1] = (ulCRCS >>  4) & 0x0F; 
   cNibbleArray[2] = (ulCRCU >> 24) & 0x0F; 
   cNibbleArray[3] = (ulCRCS >> 28) & 0x0F; 
   cNibbleArray[4] = (usLicenseCount >>  0 ) & 0x0F;
   cNibbleArray[5] = (usLicenseCount >>  4 ) & 0x0F; 
   cNibbleArray[6] = (usLicenseCount >>  8 ) & 0x0F; 
   cNibbleArray[7] = (usLicenseCount >> 12 ) & 0x0F; 
   cNibbleArray[8] = usCutoffVersion & 0x0F;

   for ( j = 0; j < NIBBLE_COUNT - 1; j++ )
       cNibbleArray[9] += cNibbleArray[j];
   cNibbleArray[9] &= 0x0F;

   if ( bDebug )
      DisplayNibbleArray( cNibbleArray, NIBBLE_COUNT );

   for ( j = 0; j < NIBBLE_COUNT - 1; j++ )
   {
      cNibbleArray[j] ^= cNibbleArray[9];
      cNibbleArray[j] &= 0x0F;
   }

   if ( bDebug )
      DisplayNibbleArray( cNibbleArray, NIBBLE_COUNT );

   NibblesToString( cNibbleArray, NIBBLE_COUNT, cStringArray, MAX_STRING_BYTES );

   printf( "   Validation Code: %s\n", cStringArray );

   // Now convert the string array back into a nibble array and display it again

   if ( bDebug )
   {
      memset( cNibbleArray, 0, NIBBLE_COUNT );
      StringToNibbles( cStringArray, cNibbleArray, NIBBLE_COUNT );
      DisplayNibbleArray( cNibbleArray, NIBBLE_COUNT );
      for ( j = 0; j < NIBBLE_COUNT - 1; j++ )
      {
         cNibbleArray[j] ^= cNibbleArray[9];
         cNibbleArray[j] &= 0x0F;
      }
      DisplayNibbleArray( cNibbleArray, NIBBLE_COUNT );
   }
}


/************  WINDOW PROCEDURE ************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:

Description:

       Args:

       Uses:

   Modifies:

    Returns:

-------------------------------------------------------------------*/

/************  PRIVATE CODE ****************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:

Description:

       Args:

       Uses:

   Modifies:

    Returns:

-------------------------------------------------------------------*/

/************  BIT BANGING  ****************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Routine:   crc32

Description:   Generate 32-bit CRC

       Args:   char *   - pointer to string

       Uses:

   Modifies:

    Returns:   ULONG - 32-bit CRC

-------------------------------------------------------------------*/

ULONG crc32_map( UCHAR b )
{
   int   i;
   ULONG v;

   v = b;
   for ( i = 8; --i >= 0; )
      v = ( ( v & 1 ) ? ( ( v >> 1 ) ^ POLYNOMIAL ) : ( v >> 1 ) );
   return v;
}


ULONG   crc32( char *pszString )
{
   int      iLen = strlen( pszString );
   ULONG    crc = 0;

   while (iLen--)
      crc = crc32_map((UCHAR)(crc ^ *pszString++)) ^ (crc >> 8);
   return crc;
}

int NibblesToString( char *pcNibbleArray, int iNibbleCount, char *pcStringArray, int iMaxStringLength )
{
   int      j;
   int      iBitCount;
   char    *pChar;
   ULONG    ulTemp;


   if ( iNibbleCount % 10 )
   {
      printf( "Improper number %u of nibbles (must be a multiple of 10)\n", iNibbleCount );
      return ERROR;
   }

   if ( ( 5 * iMaxStringLength ) < ( iNibbleCount * 4 ) )
   {
      printf( "Max string length of %u is too short\n", iMaxStringLength );
      return ERROR;
   }


   ulTemp = 0;
   iBitCount = 0;
   pChar = pcStringArray;

   for ( j = 0; j < iNibbleCount; j++ )
   {
      ulTemp |= pcNibbleArray[j] << iBitCount;
      iBitCount += 4;
      if ( iBitCount >= 5 )
      {
         iBitCount -= 5;
         *pChar = szXlateString[ulTemp & 0x1F];
         pChar++;
         ulTemp = ulTemp >> 5;
      }
   }

   *pChar = '\0';
   return NO_ERROR;
}

int StringToNibbles( char *pszString, char *pcNibbleArray, int iMaxNibbles )
{
   int      j;
   int      iStrlen;
   int      iCharIndex;
   int      iNibbleIndex;
   int      iBitCount;
   ULONG    ulTemp;


   iStrlen = strlen(pszString);
   if ( (iStrlen * 5) > (iMaxNibbles * 4) )
   {
      printf( "ERROR - string length %d of string %s too long\n", iStrlen, pszString );
      return ERROR;
   }

   iNibbleIndex = 0;
   iBitCount = 0;
   ulTemp = 0;

   for ( j = 0; j < iStrlen; j++ )
   {
      if ( ( iCharIndex = CharToIndex( pszString[j] ) ) == -1 )
         return ERROR;
      ulTemp |= iCharIndex << iBitCount;
      iBitCount += 5;
      while ( iBitCount >= 4 )
      {
         pcNibbleArray[iNibbleIndex] = ulTemp & 0x0F;
         ulTemp >>= 4;
         iBitCount -= 4;
         iNibbleIndex++;
      }
   }
   return NO_ERROR;
}

void DisplayNibbleArray( char *cNibbleArray, int iNibbleCount )
{
   int      j;


   printf( "Nibble Array: " );
   for ( j = 0; j < iNibbleCount; j++ )
   {
       printf( "%2.2X ", cNibbleArray[j] );
   }
   printf( "\n" );
}


int CharToIndex( char cChar )
{
   int      j;

   for ( j = 0; j < strlen(szXlateString); j++ )
   {
      if ( toupper(cChar) == toupper(szXlateString[j]) )
         return j;
   }

   printf ( "ERROR - cannot match Char!\n" );
   return -1;
}

/************  END OF FILE  ****************************************/

#endif   /* if !defined(GENREG_CPP) */
