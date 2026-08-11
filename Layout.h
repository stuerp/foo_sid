
/** $VER: Preferences.h (2026.08.11) **/

#pragma once

#define W_A00   332 // Dialog width as set by foobar2000, in dialog units
#define H_A00   243 // Dialog height as set by foobar2000, in dialog units

#define H_LBL     8 // Label

#define W_BTN    50 // Button
#define H_BTN    14 // Button

#define H_EBX    14 // Edit box
#define H_CBX    14 // Combo box

#define W_CHB    10 // Check box
#define H_CHB    10 // Check box

#define H_SLD    10 // Slider

#define DX        7
#define DY        7

#define IX        4 // Spacing between two related controls
#define IY        3

// Groupbox: Quality

#define X_A01   0
#define Y_A01   0

// Label: Sample rate
#define W_A02    42
#define H_A02    H_LBL
#define X_A02    X_A01 + 5
#define Y_A02    Y_A01 + 11

// Combobox: Sample rate
#define W_A03    48
#define H_A03    H_CBX
#define X_A03    X_A02 + W_A02 + IX
#define Y_A03    Y_A02

// Label: Sample rate Unit
#define W_A04    10
#define H_A04    H_LBL
#define X_A04    X_A03 + W_A03 + IY
#define Y_A04    Y_A03

// Label: Core
#define W_A05    20
#define H_A05    H_LBL
#define X_A05    X_A04 + W_A04 + IY
#define Y_A05    Y_A04

// Combobox: Core
#define W_A06    48
#define H_A06    H_CBX
#define X_A06    X_A05 + W_A05 + IX
#define Y_A06    Y_A05

// Label: Stereo separation
#define W_A07    60
#define H_A07    H_LBL
#define X_A07    X_A02
#define Y_A07    Y_A03 + H_A03 + IY

// SLider: Stereo separation
#define W_A08    50
#define H_A08    H_SLD
#define X_A08    X_A07 + W_A07 + IX
#define Y_A08    Y_A07

// Label: Stereo separation Unit
#define W_A09    22
#define H_A09    H_LBL
#define X_A09    X_A08 + W_A08 + IY
#define Y_A09    Y_A08

#define W_A01   330
#define H_A01   11 + H_A03 + IY + H_A08 + 7


// Groupbox: Length
#define X_B01   0
#define Y_B01   Y_A01 + H_A01 + IY

    // Checkbox: Infinite
    #define W_B02   52
    #define H_B02   H_CHB
    #define X_B02   X_B01 + 5
    #define Y_B02   Y_B01 + 11


    // Label: Default length
    #define W_B03    48
    #define H_B03    H_LBL
    #define X_B03    X_B02
    #define Y_B03    Y_B02 + H_B02 + IY

    // Editbox: Default length
    #define W_B04    40
    #define H_B04    H_EBX
    #define X_B04    X_B03 + W_B03 + IX
    #define Y_B04    Y_B03

    // Label: Default length Unit
    #define W_B05    24
    #define H_B05    H_LBL
    #define X_B05    X_B04 + W_B04 + IX
    #define Y_B05    Y_B04


    // Label: Fade for
    #define W_B06    30
    #define H_B06    H_LBL
    #define X_B06    X_B05 + W_B05 + IX
    #define Y_B06    Y_B05

    // Editbox: Fade for
    #define W_B07    20
    #define H_B07    H_EBX
    #define X_B07    X_B06 + W_B06 + IX
    #define Y_B07    Y_B06

    // Label: Fade for Unit
    #define W_B08    10
    #define H_B08    H_LBL
    #define X_B08    X_B07 + W_B07 + IX
    #define Y_B08    Y_B07



    // Label: Path
    #define W_C02    50
    #define H_C02    H_LBL
    #define X_C02    X_B03
    #define Y_C02    Y_B04 + H_B04 + IY

    // Editbox: Path
    #define W_C03    188
    #define H_C03    H_EBX
    #define X_C03    X_C02 + W_C02 + IX
    #define Y_C03    Y_C02

    // Button
    #define W_C04    14
    #define H_C04    H_BTN
    #define X_C04    X_C03 + W_C03
    #define Y_C04    Y_C03

    // Button
    #define W_C05    24
    #define H_C05    H_BTN
    #define X_C05    X_C04 + W_C04
    #define Y_C05    Y_C04


    // Label: Status
    #define W_C06    36
    #define H_C06    H_LBL
    #define X_C06    X_C05 + W_C05 + IX
    #define Y_C06    Y_C05

#define W_B01   W_A01
#define H_B01   11 + H_B02 + IY + H_B04 + IY + H_C03 + 7

// Groupbox: Overrides
#define X_D01   0
#define Y_D01   Y_B01 + H_B01 + IY


    // Label: Clock speed
    #define W_D02    42
    #define H_D02    H_LBL
    #define X_D02    X_D01 + 5
    #define Y_D02    Y_D01 + 11

    // Combobox: Clock speed
    #define W_D03    84
    #define H_D03    H_CBX
    #define X_D03    X_D02 + W_D02 + IX
    #define Y_D03    Y_D02


    // Label: SID model
    #define W_D04    W_D02
    #define H_D04    H_LBL
    #define X_D04    X_D02
    #define Y_D04    Y_D03 + H_D03 + IY

    // Combobox: SID model
    #define W_D05    W_D03
    #define H_D05    H_CBX
    #define X_D05    X_D04 + W_D04 + IX
    #define Y_D05    Y_D04


#define W_D01   W_B01
#define H_D01   11 + H_D03 + IY + H_D05 + 7

// Groupbox: ReSIDfp
#define X_E01   0
#define Y_E01   Y_D01 + H_D01 + IY


    // Label: 6581 filter curve
    #define W_E02    104
    #define H_E02    H_LBL
    #define X_E02    X_E01 + 5
    #define Y_E02    Y_E01 + 11

    // Slider: 6581 filter curve
    #define W_E03    126
    #define H_E03    H_SLD
    #define X_E03    X_E02 + W_E02 + IX
    #define Y_E03    Y_E02

    // Label: 6581 filter curve value
    #define W_E04    18
    #define H_E04    H_LBL
    #define X_E04    X_E03 + W_E03 + IX
    #define Y_E04    Y_E03


    // Checkbox: Use old caps
    #define W_E05   56
    #define H_E05   H_CHB
    #define X_E05   X_E03
    #define Y_E05   Y_E03 + H_E03 + IY


    // Label: Wave offset
    #define W_E06    46
    #define H_E06    H_LBL
    #define X_E06    X_E05 + W_E05 + IX
    #define Y_E06    Y_E05

    // Slider: Wave offset
    #define W_E07    60
    #define H_E07    H_SLD
    #define X_E07    X_E06 + W_E06 + IX
    #define Y_E07    Y_E06

    // Label: Wave offset value
    #define W_E08    18
    #define H_E08    H_LBL
    #define X_E08    X_E07 + W_E07 + IX
    #define Y_E08    Y_E07


    // Label: 8580 filter curve
    #define W_E09    W_E02
    #define H_E09    H_LBL
    #define X_E09    X_E02
    #define Y_E09    Y_E07 + H_E07 + IY

    // Slider: 8580 filter curve
    #define W_E10    W_E03
    #define H_E10    H_SLD
    #define X_E10    X_E09 + W_E09 + IX
    #define Y_E10    Y_E09

    // Label: 8580 filter curve value
    #define W_E11    W_E04
    #define H_E11    H_LBL
    #define X_E11    X_E10 + W_E10 + IX
    #define Y_E11    Y_E10


    // Label: DAC leakage
    #define W_E12    W_E09
    #define H_E12    H_LBL
    #define X_E12    X_E09
    #define Y_E12    Y_E10 + H_E10 + IY

    // Slider: DAC leakage
    #define W_E13    W_E10
    #define H_E13    H_SLD
    #define X_E13    X_E12 + W_E12 + IX
    #define Y_E13    Y_E12

    // Label: DAC leakage value
    #define W_E14    W_E11
    #define H_E14    H_LBL
    #define X_E14    X_E13 + W_E13 + IX
    #define Y_E14    Y_E13


    // Label: DC-Blocker resistance
    #define W_E15    W_E12
    #define H_E15    H_LBL
    #define X_E15    X_E12
    #define Y_E15    Y_E13 + H_E13 + IY

    // Slider: DC-Blocker resistance
    #define W_E16    W_E13
    #define H_E16    H_SLD
    #define X_E16    X_E15 + W_E15 + IX
    #define Y_E16    Y_E15

    // Label: DC-Blocker resistance value
    #define W_E17    W_E14
    #define H_E17    H_LBL
    #define X_E17    X_E16 + W_E16 + IX
    #define Y_E17    Y_E16


    // Label: Combined waveforms strength
    #define W_E18    W_E15
    #define H_E18    H_LBL
    #define X_E18    X_E15
    #define Y_E18    Y_E16 + H_E16 + IY

    // Combobox: Combined waveforms strength
    #define W_E19    44
    #define H_E19    H_CBX
    #define X_E19    X_E18 + W_E18 + IX
    #define Y_E19    Y_E18


#define W_E01   W_D01
#define H_E01   11 + H_E03 + IY + H_E07 + IY + H_E10 + IY + H_E14 + IY + H_E16 + IY + H_E19 + 7
