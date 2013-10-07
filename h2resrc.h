/********************************************************************

             Copyright (C) 1994 - 1996 Bennett C. Baker and B::Ware

                          All rights reserved

     Module:   H2RESRC.H.H
     Author:   Bennett C. Baker
     CIS ID:   70274,3147
    Created:   14-Mar-1994

Description:   Resource header file for HEXEDIT/2 OS/2 Hex editor.
               Contains all resource definitions (menus, dialogs,
               help screens, etc.).

       $Log$

********************************************************************/
 
#if !defined(H2RESRC_H)
#define H2RESRC_H

/************  INCLUDE FILES  **************************************/

/************  DEFINES, TYPEDEFS AND DATA STRUCTURE DEFINITIONS  ***/

/*
 * Main client resource ID.  This is used as the top-level ID for all
 * elements (menu, help, dialogs, etc.)
 */

#define  RES_CLIENT                 1000


/*
 * Menu ID definitions.
 */

// Menu IDs for FILE menu

#define  IDM_FILE                   2000
#define  IDM_FILE_OPEN              2100
#define  IDM_FILE_SAVE              2200
#define  IDM_FILE_SAVE_AS           2300
#define  IDM_FILE_CLEAR             2400
#define  IDM_FILE_ALLOCATE          2500
#define  IDM_FILE_RESIZE            2600
#define  IDM_FILE_EXIT              2900

// Menu IDs for EDIT menu

#define  IDM_EDIT                   3000
#define  IDM_EDIT_CUT               3100
#define  IDM_EDIT_COPY              3200
#define  IDM_EDIT_PASTE             3300
#define  IDM_EDIT_INSERT            3400
#define  IDM_EDIT_DELETE            3500

// Menu IDs for SEARCH menu

#define  IDM_SEARCH                 4000
#define  IDM_SEARCH_FORWARD         4100
#define  IDM_SEARCH_BACKWARD        4200
#define  IDM_SEARCH_AGAIN           4300
#define  IDM_REPLACE_FORWARD        4400
#define  IDM_REPLACE_BACKWARD       4500
#define  IDM_REPLACE_AGAIN          4600
#define  IDM_GOTO_OFFSET            4700

// Menu IDs for OPTIONS menu

#define  IDM_OPTIONS                5000
#define  IDM_OPTIONS_FONT           5100
#define  IDM_OPTIONS_READONLY       5200
#define  IDM_OPTIONS_AUTORESIZE     5300
#define  IDM_OPTIONS_REGISTER       5400

// Menu IDs for DISPLAY menu

#define  IDM_DISPLAY                6000

// Menu IDs for HELP menu

#define  IDM_HELP                   9000
#define  IDM_HELP_CONTENTS          9100
#define  IDM_HELP_INDEX             9200
#define  IDM_HELP_GENERAL           9300
#define  IDM_HELP_USING             9400
#define  IDM_HELP_KEYS              9500
#define  IDM_HELP_ABOUT             9600


/*
 * Dialog Box ID definitions.
 */

// Dialog IDs for FILE ALLOCATE dialog box

#define  IDD_FILE_ALLOCATE          2501
#define  IDD_FILE_ALLOCATE_ENTRY    2502
#define  IDD_FILE_ALLOCATE_HELP     2503

// Dialog IDs for FILE RESIZE dialog box

#define  IDD_FILE_RESIZE            2601
#define  IDD_FILE_RESIZE_OLD        2602
#define  IDD_FILE_RESIZE_NEW        2603
#define  IDD_FILE_RESIZE_HELP       2604

// Dialog IDs for EDIT INSERT and EDIT DELETE dialog boxes

#define  IDD_EDIT_INSERT            3401
#define  IDD_EDIT_INSERT_ENTRY      3402
#define  IDD_EDIT_INSERT_HELP       3403
#define  IDD_EDIT_INSERT_VALUE      3404

#define  IDD_EDIT_DELETE            3501
#define  IDD_EDIT_DELETE_ENTRY      3502
#define  IDD_EDIT_DELETE_HELP       3503

// Dialog IDs for SEARCH and REPLACE and GOTO dialog boxes

#define  IDD_SEARCH                 4001
#define  IDD_REPLACE                4002
#define  IDD_SEARCH_OLD             4003
#define  IDD_REPLACE_NEW            4004
#define  IDD_SEARCH_HELP            4005
#define  IDD_REPLACE_HELP           4006

#define  IDD_SEARCH_OCTAL           4010
#define  IDD_SEARCH_DECIMAL         4011
#define  IDD_SEARCH_HEX             4012
#define  IDD_SEARCH_ASCII           4013
#define  IDD_SEARCH_CASE_SENS       4014

#define  IDD_GOTO_OFFSET            4701
#define  IDD_GOTO_OFFSET_OFFSET     4702
#define  IDD_GOTO_OFFSET_HELP       4703

// Dialog IDs for REGISTER dialog box

#define  IDD_REGISTER               5401
#define  IDD_REGISTER_USERNAME      5402
#define  IDD_REGISTER_USERID        5403
#define  IDD_REGISTER_CHECKSUM      5404

// Dialog IDs for DISPLAY dialog box

#define  IDD_DISPLAY                6001
#define  IDD_DISPLAY_HELP           6002
#define  IDD_DISPLAY_OFFSET_OCTAL   6010
#define  IDD_DISPLAY_OFFSET_DECIMAL 6011
#define  IDD_DISPLAY_OFFSET_HEX     6012
#define  IDD_DISPLAY_ENTITY_OCTAL   6020
#define  IDD_DISPLAY_ENTITY_DECIMAL 6021
#define  IDD_DISPLAY_ENTITY_HEX     6022
#define  IDD_DISPLAY_1_BYTE         6030
#define  IDD_DISPLAY_2_BYTE         6031
#define  IDD_DISPLAY_4_BYTE         6032

// Dialog IDs for HELP ABOUT dialog box

#define  IDD_HELP_ABOUT             9601
#define  IDD_HELP_VERSION           9602
#define  IDD_HELP_REGTO             9603
#define  IDD_HELP_NUMUSERS          9604
#define  IDD_HELP_SERIALNUM         9605
                                   

/*
 * String Table ID definitions and maximum st
 */

#define  IDS_ABANDONQUERY           0xF001
#define  IDS_ABANDONTITLE           0xF002
#define  IDS_HELPWINDOWTITLE        0xF003
#define  IDS_ERRMEMALLOC            0xF004
#define  IDS_ERRMEMRESIZE           0xF005
#define  IDS_ERRREADOPEN            0xF006
#define  IDS_ERRZEROLENFILE         0xF007
#define  IDS_ERRFILEREAD            0xF008
#define  IDS_ERRWRITEOPEN           0xF009
#define  IDS_ERRFILEWRITE           0xF00A
#define  IDS_ERRFILEMSG             0xF00B
#define  IDS_ERRERROR               0xF00C
#define  IDS_ERRMEMALLOCSIZE        0xF00D
#define  IDS_ERRMEMALLOCRESIZE      0xF00E
#define  IDS_UNTITLED               0xF00F
#define  IDS_HELP                   0xF010
#define  IDS_EDIT                   0xF011
#define  IDS_INSERTBADVALUE         0xF012
#define  IDS_INSERTBADREALLOC       0xF013
#define  IDS_DELETEBADVALUE         0xF014
#define  IDS_DELETEBADREALLOC       0xF015
#define  IDS_ERRORHELPCREATE        0xF016
#define  IDS_ERRORHELPASSOCIATE     0xF017
#define  IDS_SEARCH                 0xF018
#define  IDS_SEARCHFAILED           0xF019
#define  IDS_SEARCHREPLACEFAILED    0xF01A
#define  IDS_REPLACEQ               0xF01B
#define  IDS_SEARCHREPLACE          0xF01C
#define  IDS_SEARCHREPBADALLOC      0xF01D
#define  IDS_REPLACE                0xF01E
#define  IDS_REGTO                  0xF01F
#define  IDS_UNREG                  0xF020
#define  IDS_BADREGTITLE            0xF021
#define  IDS_BADREGSTRING           0xF022
#define  IDS_NUMUSERS               0xF023
#define  IDS_VERSIONSTRING          0xF024
#define  IDS_COMPANYLICENSE         0xF025
#define  IDS_SITELICENSE            0xF026
#define  IDS_PROJECTLICENSE         0xF027
#define  IDS_SERIALNUMBER           0xF028

/*
 * Help Panel ID definitions
 */

// Help subtable IDs

#define  SUBTABLE_MAIN              RES_CLIENT
#define  SUBTABLE_ALLOCATE          IDD_FILE_ALLOCATE
#define  SUBTABLE_RESIZE            IDD_FILE_RESIZE
#define  SUBTABLE_INSERT            IDD_EDIT_INSERT
#define  SUBTABLE_DELETE            IDD_EDIT_DELETE
#define  SUBTABLE_SEARCH            IDD_SEARCH
#define  SUBTABLE_REPLACE           IDD_REPLACE
#define  SUBTABLE_GOTO              IDD_GOTO_OFFSET
#define  SUBTABLE_DISPLAY           IDD_DISPLAY

// Main (client) window help panels

#define  IDP_MAIN_WIN               1000
#define  IDP_BWARE_WIN              1001
#define  IDP_SUPPORT_INFO           1002
#define  IDP_LICENSE_INFO           1003
#define  IDP_REG_INFO               1004

#define  IDP_FILE                   2000
#define  IDP_FILE_OPEN              2100
#define  IDP_FILE_SAVE              2200
#define  IDP_FILE_SAVE_AS           2300
#define  IDP_FILE_CLEAR             2400
#define  IDP_FILE_ALLOCATE          2500
#define  IDP_FILE_RESIZE            2600
#define  IDP_FILE_EXIT              2900

#define  IDP_EDIT                   3000
#define  IDP_EDIT_CUT               3100
#define  IDP_EDIT_COPY              3200
#define  IDP_EDIT_PASTE             3300
#define  IDP_EDIT_INSERT            3400
#define  IDP_EDIT_DELETE            3500

#define  IDP_SEARCH                 4000
#define  IDP_SEARCH_SELECTIONS      4100
#define  IDP_REPLACE_SELECTIONS     4400
#define  IDP_GOTO_OFFSET            4700

#define  IDP_OPTIONS                5000
#define  IDP_OPTIONS_FONT           5100
#define  IDP_OPTIONS_READONLY       5200
#define  IDP_OPTIONS_AUTORESIZE     5300
#define  IDP_OPTIONS_REGISTER       5400

#define  IDP_DISPLAY                6000

#define  IDP_HELP                   9000
#define  IDP_HELP_CONTENTS          9100
#define  IDP_HELP_INDEX             9200
#define  IDP_HELP_GENERAL           9300
#define  IDP_HELP_USING             9400
#define  IDP_HELP_KEYS              9500
#define  IDP_HELP_ABOUT             9600

// Help panels for the FILE OPEN dialog box

#define  IDP_FILE_OPEN_DLG          2101

// Help panels for the FILE SAVE AS dialog box

#define  IDP_FILE_SAVE_AS_DLG       2301

// Help Panels for FILE ALLOCATE dialog box

#define  IDP_FILE_ALLOCATE_DLG      2501
#define  IDP_FILE_ALLOCATE_ENTRY    2502
#define  IDP_FILE_ALLOCATE_MODE     2503

// Help Panels for FILE RESIZE dialog box

#define  IDP_FILE_RESIZE_DLG        2601
#define  IDP_FILE_RESIZE_OLD        2602
#define  IDP_FILE_RESIZE_NEW        2603
#define  IDP_FILE_RESIZE_MODE       2604

// Help Panels for EDIT INSERT and EDIT DELETE dialog boxes

#define  IDP_EDIT_INSERT_DLG        3401
#define  IDP_EDIT_INSERT_ENTRY      3402

#define  IDP_EDIT_DELETE_DLG        3501
#define  IDP_EDIT_DELETE_ENTRY      3502

// Help Panels for the SEARCH and REPLACE dialog boxes

#define  IDP_SEARCH_DLG             4101
#define  IDP_SEARCH_STRING          4102
#define  IDP_SEARCH_TYPE            4103

#define  IDP_REPLACE_DLG            4401
#define  IDP_REPLACE_STRING         4402

// Help Panels for the GOTO OFFSET dialog box

#define  IDP_GOTO_OFFSET_DLG        4701

// Help panels for the OPTIONS FONT dialog box

#define  IDP_OPTIONS_FONT_DLG       5101

// Help Panels for DISPLAY dialog box

#define  IDP_DISPLAY_DLG            6001
#define  IDP_DISPLAY_OFFSET         6002
#define  IDP_DISPLAY_DATA           6003
#define  IDP_DISPLAY_BYTE           6004

// Help Panels for Keys Info

#define  IDP_KEYS_INFO              9501
#define  IDP_MOVEMENT_KEYS          9502
#define  IDP_EDITING_KEYS           9503
#define  IDP_HOT_KEYS               9504

/************  END OF FILE  ****************************************/

#endif   /* if !defined(H2RESRC_H) */
