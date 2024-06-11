
# foo_sid

[foo_sid](https://github.com/stuerp/foo_sid/releases) is a [foobar2000](https://www.foobar2000.org/) component that adds playback of SID and MUS files to foobar2000.

It is based on [foo_sid](https://gitlab.com/kode54/foo_sid/-/commits/main) by [kode54](https://gitlab.com/kode54).

## Features

* Decodes music files for the Commodore 64 or 128 computers, ripped in the .SID, .MUS or .MUS+.STR formats.
* Supports up to 3SID rips.
* Supports the latest [High Voltage SID Collection](https://www.hvsc.c64.org/) song length database.
* Supports foobar2000 2.0 and later (32-bit and 64-bit version)
* Supports dark mode.

## Requirements

* [foobar2000](https://www.foobar2000.org/download) v1.6.16 or later (32 or 64-bit). ![foobar2000](https://www.foobar2000.org/button-small.png)
* Tested on Microsoft Windows 10 and later.

## Getting started

* Double-click `foo_sid.fbk2-component`.

or

* Import `foo_sid.fbk2-component` into foobar2000 using the "*File / Preferences / Components / Install...*" menu item.

    **Important**

    Configure the path to the HVSC ([High Voltage SID Collection](https://www.hvsc.c64.org/)) database before adding any .SID files to a playlist to make sure that the song length of any recognised file gets set correctly. If not all unrecognised songs get the same song length (default 3 min.) but will play correctly.

    You can also use the "*Tagging/Reload info from file(s)*" context menu after configuring the database to rescan the files.

## Tags

The following info tags are available:

| Name           | Value |
| -------------- | ----- |
| channels       | 2 |
| encoding       | synthesized|
| sid_chip_count | Number of SID chips used by the song |
| clock_speed    | NTSC or PAL|
| sid_model      | 8580 or 6581|

The following meta data tags are available:

| Name      | Value |
| --------- | ----- |
| title     | In case the track only contains 1 song |
| album     | In case the track contains multiple songs |
| artist    ||
| copyright ||
| info      ||
| comment   ||

## Developing

The code builds out-of-the box with Visual Studio.

### Requirements

To build the code:

* [Microsoft Visual Studio 2022 Community Edition](https://visualstudio.microsoft.com/downloads/) or later
* [foobar2000 SDK](https://www.foobar2000.org/SDK) 2023-09-23
* [Windows Template Library (WTL)](https://github.com/Win32-WTL/WTL) 10.0.10320

The following library is included:

* [libsidplayfp](https://github.com/libsidplayfp/libsidplayfp)

To create the deployment package:

* [PowerShell 7.2](https://github.com/PowerShell/PowerShell) or later

### Setup

Create the following directory structure:

    3rdParty
        WTL10_10320
    bin
        x86
    foo_sid
    out
    sdk

* `3rdParty/WTL10_10320` contains WTL 10.0.10320.
* `bin` contains a portable version of foobar2000 64-bit for debugging purposes.
* `bin/x86` contains a portable version of foobar2000 32-bit for debugging purposes.
* `foo_sid` contains the [Git](https://github.com/stuerp/foo_sid) repository.
* `out` receives a deployable version of the component.
* `sdk` contains the foobar2000 SDK.

### Building

Open `foo_sid.sln` with Visual Studio and build the solution.

### Packaging

To create the component first build the x86 configuration and next the x64 configuration.

## Change Log

v1.97.0.0, 2024-06-11

* Upgraded libsidplayfp to 2.8.0.

v1.96.0.0, 2024-06-09

* Upgraded libsidplayfp to 2.7.1.
* Fixed: The song length database could not be loaded from paths containing non-ASCII characters.
* Fixed: A couple of labels on the Preferences page were cut-off.

v1.95.0.0, 2024-04-10

* Upgraded libsidplayfp to 2.7.0.

v1.94.0.0, 2024-01-01

* Upgraded libsidplayfp to 2.6.0.
* Builds with foobar2000 SDK 2023-09-23.

v1.93.0.0, 2023-06-02

* The Preferences page is now DPI-aware.
* Upgraded libsidplayfp to 2.5.0.
* Builds with foobar2000 SDK 2023-05-10.

v1.92.0.0, 2023-01-29, *"Stayin' Alive"*

* Upgraded libsidplayfp to 2.4.2.
* Builds with foobar2000 SDK 2023-01-18.

v1.91.0.2, 2022-12-04, *"Revenge of the SID"*

* Fixed: MUS and MUS+STR files play again.

v1.91.0.1, 2022-12-03, *"Scratchin' the itch"*

* Initial release of a x64 version for [foobar2000](https://www.foobar2000.org/) v2.0.

## Acknowledgements / Credits

* Peter Pawlowski for the [foobar2000](https://www.foobar2000.org/) audio player. ![foobar2000](https://www.foobar2000.org/button-small.png)
* [kode54](https://gitlab.com/kode54) for the original [foo_sid](https://gitlab.com/kode54/foo_sid) component.
* [libsidplayfp](https://github.com/libsidplayfp/libsidplayfp) for a library to play Commodore 64 music.

## Links

* Home page: [https://github.com/stuerp/foo_sid](https://github.com/stuerp/foo_sid)
* Repository: [https://github.com/stuerp/foo_sid.git](https://github.com/stuerp/foo_sid.git)
* Issue tracker: [https://github.com/stuerp/foo_sid/issues](https://github.com/stuerp/foo_sid.git)

## License

[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
