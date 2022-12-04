
# foo_sid

[foo_sid](https://github.com/stuerp/foo_sid/releases) is a [foobar2000](https://www.foobar2000.org/) component that adds playback of SID and MUS files to foobar2000.

It is based on [foo_sid](https://gitlab.com/kode54/foo_sid/-/commits/main) by [kode54](https://gitlab.com/kode54).

## Features

* Decodes music files for the Commodore 64 or 128 computers, ripped in the .SID, .MUS or .MUS+.STR formats.
* Supports up to 3SID rips.
* Supports the latest [High Voltage SID Collection](https://www.hvsc.c64.org/) song length database.
* Supports dark mode.

* Compatible with foobar2000 1.6.13 and foobar2000 2.0 or later (32 and 64-bit version).

## Requirements

* Tested on Microsoft Windows 10 or later.
* [foobar2000](https://www.foobar2000.org/download) v1.6.13 or later. ![foobar2000](https://www.foobar2000.org/button-small.png)

## Getting started

* Double-click `foo_sid.fbk2-component`.

or

* Import `foo_sid.fbk2-component` into foobar2000 using "File / Preferences / Components / Install...".

## Developing

The code builds out-of-the box with Visual Studio.

### Requirements

To build the code:

* [Microsoft Visual Studio 2022 Community Edition](https://visualstudio.microsoft.com/downloads/)
* [foobar2000 SDK](https://www.foobar2000.org/SDK)
* [Windows Template Library (WTL)](https://github.com/Win32-WTL/WTL) 10.0.10320
* [xa](http://www.floodgap.com/retrotech/xa/)

To create the deployment package:

* [PowerShell 7.2](https://github.com/PowerShell/PowerShell)

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

## Contributing

If you'd like to contribute, please fork the repository and use a feature
branch. Pull requests are warmly welcome.

## Change Log

v1.91.0.1, 2022-12-03, *"Scratchin' the itch"*

* Initial release of a x64 version for [foobar2000](https://www.foobar2000.org/) v2.0.

## Acknowledgements / Credits

* Peter Pawlowski for the [foobar2000](https://www.foobar2000.org/) audio player. ![foobar2000](https://www.foobar2000.org/button-small.png)
* [kode54](https://gitlab.com/kode54) for the original [foo_sid](https://gitlab.com/kode54/foo_sid) component.
* [libsidplayfp](https://github.com/libsidplayfp/libsidplayfp) for a library to play Commodore 64 music.

## Links

* Home page: https://github.com/stuerp/foo_sid
* Repository: https://github.com/stuerp/foo_sid.git
* Issue tracker: https://github.com/stuerp/foo_sid/issues

## License

GNU General Public License v2
