
/** $VER: Resource.h (2026.08.11) P. Stuer **/

#pragma once

#define TOSTRING_IMPL(x) #x
#define TOSTRING(x) TOSTRING_IMPL(x)

#define NUM_FILE_MAJOR          2
#define NUM_FILE_MINOR          1
#define NUM_FILE_PATCH          0
#define NUM_FILE_PRERELEASE     0

#define NUM_PRODUCT_MAJOR       2
#define NUM_PRODUCT_MINOR       1
#define NUM_PRODUCT_PATCH       0
#define NUM_PRODUCT_PRERELEASE  0

#define STR_RELEASE_TAG         ""

/** Component specific **/

#define STR_COMPONENT_NAME          "SID Player"
#define STR_COMPONENT_VERSION       TOSTRING(NUM_FILE_MAJOR) "." TOSTRING(NUM_FILE_MINOR) "." TOSTRING(NUM_FILE_PATCH) "." TOSTRING(NUM_FILE_PRERELEASE) STR_RELEASE_TAG
#define STR_COMPONENT_BASENAME      "foo_sid"
#define STR_COMPONENT_FILENAME      STR_COMPONENT_BASENAME ".dll"
#define STR_COMPONENT_COMPANY_NAME  "LoSno.co"

#define STR_COMPONENT_COPYRIGHT     "Copyright (c) 2003-2026 " STR_COMPONENT_COMPANY_NAME ". All rights reserved."
#define STR_COMPONENT_COMMENTS      "Written by Christopher Snowhill, Peter Stuer"

#define STR_COMPONENT_DESCRIPTION   "Adds playback of SID files to foobar2000."
#define STR_COMPONENT_COMMENT       ""
#define STR_COMPONENT_URL           "https://github.com/stuerp/" STR_COMPONENT_BASENAME

/** Generic **/

#define STR_COMPANY_NAME            TEXT(STR_COMPONENT_COMPANY_NAME)
#define STR_INTERNAL_NAME           TEXT(STR_COMPONENT_NAME)
#define STR_COMMENTS                TEXT(STR_COMPONENT_COMMENTS)

#define STR_COPYRIGHT               TEXT(STR_COMPONENT_COPYRIGHT)

#define STR_FILE_NAME               TEXT(STR_COMPONENT_FILENAME)
#define STR_FILE_VERSION            TEXT(STR_COMPONENT_VERSION)
#define STR_FILE_DESCRIPTION        TEXT(STR_COMPONENT_DESCRIPTION)

#define STR_PRODUCT_NAME            STR_INTERNAL_NAME
#define STR_PRODUCT_VERSION         TEXT(TOSTRING(NUM_PRODUCT_MAJOR)) TEXT(".") TEXT(TOSTRING(NUM_PRODUCT_MINOR)) TEXT(".") TEXT(TOSTRING(NUM_PRODUCT_PATCH)) TEXT(".") TEXT(TOSTRING(NUM_PRODUCT_PRERELEASE)) TEXT(STR_RELEASE_TAG)

#define STR_ABOUT_NAME              STR_INTERNAL_NAME
#define STR_ABOUT_WEB               TEXT(STR_COMPONENT_URL)
#define STR_ABOUT_EMAIL             TEXT("mailto:peter.stuer@outlook.com")

#define IDD_CONFIG              101

#define IDC_SAMPLERATE          1000
#define IDC_SID_BUILDER         1001
#define IDC_SLIDER_SSEP         1002
#define IDC_TEXT_SSEP           1003

#define IDC_LOOP_FOREVER            1010
#define IDC_DLENGTH             1011
#define IDC_FADE                1012

#define IDC_DB_PATH             1020
#define IDC_DB_PATH_SET         1021
#define IDC_DB_PATH_CLEAR       1022
#define IDC_DB_STATUS           1023

#define IDC_CLOCK_OVERRIDE      1030
#define IDC_SID_OVERRIDE        1031

#define IDC_SLIDER_6581         1040
#define IDC_TEXT_6581           1041

#define IDC_OLD_CAPS            1042

#define IDC_WAVE_OFFSET         1043
#define IDC_WAVE_OFFSET_TEXT    1044

#define IDC_SLIDER_8580         1050
#define IDC_TEXT_8580           1051

#define IDC_DAC_LEAKAGE         1060
#define IDC_DAC_LEAKAGE_TEXT    1061

#define IDC_DCB_RES             1070
#define IDC_DCB_RES_TEXT        1071

#define IDC_CWS                 1080
