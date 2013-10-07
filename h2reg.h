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
 
#if !defined(H2REG_H)
#define H2REG_H

/************  INCLUDE FILES  **************************************/

/************  DEFINES, TYPEDEFS AND DATA STRUCTURE DEFINITIONS  ***/

#define  MAX_USERS         0xFFEF

#define  USERS_COMPANY     0xFFFF
#define  USERS_SITE        0xFFFE
#define  USERS_PROJECT     0xFFFD

#define XLATE_LEN          32

const CHAR  szXlateArray[XLATE_LEN + 1]     = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";

#define NIBBLE_COUNT       10

#define POLYNOMIAL         0xEDB88320

/************  END OF FILE  ****************************************/

#endif   /* if !defined(H2REG_H) */
