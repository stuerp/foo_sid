
/** $VER: Resource.h (2024.10.20) **/

#pragma once

#define TOSTRING_IMPL(x) #x
#define TOSTRING(x) TOSTRING_IMPL(x)

#define NUM_FILE_MAJOR          1
#define NUM_FILE_MINOR          99
#define NUM_FILE_PATCH          0
#define NUM_FILE_PRERELEASE     0

#define NUM_PRODUCT_MAJOR       1
#define NUM_PRODUCT_MINOR       99
#define NUM_PRODUCT_PATCH       0
#define NUM_PRODUCT_PRERELEASE  0

/** Component specific **/

#define STR_COMPONENT_NAME          "SID Player"
#define STR_COMPONENT_VERSION       TOSTRING(NUM_FILE_MAJOR) "." TOSTRING(NUM_FILE_MINOR) "." TOSTRING(NUM_FILE_PATCH) "." TOSTRING(NUM_FILE_PRERELEASE)
#define STR_COMPONENT_BASENAME      "foo_sid"
#define STR_COMPONENT_FILENAME      STR_COMPONENT_BASENAME ".dll"
#define STR_COMPONENT_COMPANY_NAME  "LoSno.co"

#define STR_COMPONENT_COPYRIGHT     "Copyright (c) 2003-2024 " STR_COMPONENT_COMPANY_NAME ". All rights reserved."
#define STR_COMPONENT_COMMENTS      "Written by Christopher Snowhill, Peter Stuer"

#define STR_COMPONENT_DESCRIPTION   "Adds playback of SID files to foobar2000."
#define STR_COMPONENT_COMMENT       ""

/** Generic **/

#define STR_COMPANY_NAME            TEXT(STR_COMPONENT_COMPANY_NAME)
#define STR_INTERNAL_NAME           TEXT(STR_COMPONENT_NAME)
#define STR_COMMENTS                TEXT(STR_COMPONENT_COMMENTS)

#define STR_COPYRIGHT               TEXT(STR_COMPONENT_COPYRIGHT)

#define STR_FILE_NAME               TEXT(STR_COMPONENT_FILENAME)
#define STR_FILE_VERSION            TOSTRING(NUM_FILE_MAJOR) TEXT(".") TOSTRING(NUM_FILE_MINOR) TEXT(".") TOSTRING(NUM_FILE_PATCH) TEXT(".") TOSTRING(NUM_FILE_PRERELEASE)
#define STR_FILE_DESCRIPTION        TEXT(STR_COMPONENT_DESCRIPTION)

#define STR_PRODUCT_NAME            STR_INTERNAL_NAME
#define STR_PRODUCT_VERSION         TOSTRING(NUM_PRODUCT_MAJOR) TEXT(".") TOSTRING(NUM_PRODUCT_MINOR) TEXT(".") TOSTRING(NUM_PRODUCT_PATCH) TEXT(".") TOSTRING(NUM_PRODUCT_PRERELEASE)

#define STR_ABOUT_NAME              STR_INTERNAL_NAME
#define STR_ABOUT_WEB               TEXT("")
#define STR_ABOUT_EMAIL             TEXT("")

#define IDD_CONFIG                      101
#define IDC_SAMPLERATE                  1000
#define IDC_INFINITE                    1001
#define IDC_DLENGTH                     1002
#define IDC_FADE                        1003
#define IDC_DB_PATH                     1004
#define IDC_DB_PATH_SET                 1005
#define IDC_DB_PATH_CLEAR               1006
#define IDC_DB_LOAD                     1007
#define IDC_SAMPLERATE2                 1007
#define IDC_SID_BUILDER                 1007
#define IDC_DB_UNLOAD                   1008
#define IDC_DB_STATUS                   1009
#define IDC_DATABASE_SEP                1010
#define IDC_LENGTH_SEP                  1011
#define IDC_QUALITY_SEP                 1012
#define IDC_CLOCK_OVERRIDE              1013
#define IDC_OVERRIDE_SID                1014
#define IDC_SID_OVERRIDE                1014
#define IDC_SLIDER_6581                 1015
#define IDC_SLIDER2                     1016
#define IDC_SLIDER_8580                 1016
#define IDC_TEXT_6581                   1017
#define IDC_TEXT_8580                   1018
#define IDC_SLIDER_6582                 1019
#define IDC_SLIDER_SSEP                 1019
#define IDC_TEXT_SSEP                   1020
