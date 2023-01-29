#pragma once
/*
    changelog

2022-12-04 00:00 UTC - stuerp
- Fix MUS and MUS+STR playback
- Version is now 1.91.0.2

2022-12-03 00:00 UTC - stuerp
- Created x64 version compatible with foobar2000 v2.0.
- Added Dark Mode support for foobar2000 v2.0.
- Version is now 1.91.0.1

2022-02-14 00:03 UTC - kode54
- Sync lock RESIDfp creation
- Version is now 1.91

2022-02-12 01:48 UTC - kode54
- Removed serialization, now should be fairly safe to use properly
- Updated libsidplayfp again, with synchronization for its caching
- Properly support dual file MUS/STR files again
- Version is now 1.90

2022-01-13 01:36 UTC - kode54
- Updated libsidplay to version 2.3.1
- Version is now 1.80

2021-09-15 07:38 UTC - kode54
- Let's serialize this entire library because it effing sucks
- Version is now 1.70

2021-04-05 18:56 UTC - kode54
- Implemented support for foobar2000 1.6.1+ preferred sample rate.
  There's not much overhead to this anyway, since the library is
  just downsampling to the specified rate anyway.
- Version is now 1.61

2021-04-04 23:56 UTC - kode54
- Updated libsidplayfp to version 2.2.0
- Version is now 1.60

2020-02-28 23:11 UTC - kode54
- Eliminated fast forwarding from seeking, as it now makes the sinc
  resampler crash. It apparently had no useful effect anyway.
- Version is now 1.50

2020-02-15 02:39 UTC - kode54
- Restructured build configuration a bit, moving MSVC specific files
  into the VC project directory, and enabling missing optimizations
- Version is now 1.49

2020-02-13 03:04 UTC - kode54
- Implemented configurable stereo mode with configurable separation level
- Fixed description for 8580 filter setting in two places
- Version is now 1.48

2020-02-12 02:18 UTC - kode54
- Fix 3SID playback
- Version is now 1.47

2020-02-12 01:17 UTC - kode54
- Fixed song length database handling for unlisted files
- Improved handling of song lengths in the get_info() function
  to not modify the state of the loaded song
- Version is now 1.46

2020-02-11 09:10 UTC - kode54
- Reverted fade behavior
- Version is now 1.45

2020-02-11 08:47 UTC - kode54
- Updated libsidplayfp to version 2.0.1 or so
- Update to more recent foobar2000 SDK
- Updated to new song length system
- Fade is now in addition to length
- Default length is now configurable to include milliseconds
- Fixed various errors in the database and length handling
- Version is now 1.44

2018-06-14 02:04 UTC - kode54
- Added a safety check to part of sidplay-residfp, to help prevent crashes on
  files that otherwise have no signature to verify
- Version is now 1.43

2018-05-02 02:51 UTC - kode54
- Updated libsidplayfp
- Fixed a serious bug with how the 8580 filter curve was configured,
  as the updates changed the value type and range to match the 6581
- Version is now 1.42

2018-01-30 09:48 UTC - kode54
- Updated to version 1.4 SDK
- Version is now 1.41

2017-12-27 09:15 UTC - kode54
- Updated sidplay-residfp with two years worth of new changes
- Updated component to remove references to deprecated interfaces
- Version is now 1.40

2017-02-04 05:10 UTC - kode54
- Add link to about string
- Version is now 1.36

2016-05-19 05:36 UTC - kode54
- Updated sidplay-residfp
  - Now uses std::mutex locking for several globals, to fix race conditions
- Now compiled with MSVC 2015
- Version is now 1.35

2015-11-07 04:28 UTC - kode54
- Updated residfp to revision 2100
- Version is now 1.34

2015-01-05 02:40 UTC - kode54
- Updated residfp to revision 1863
- Remodeled around new sidplay-residfp
- Implemented support for now working resid builder
- Version is now 1.33

2014-04-01 00:40 UTC - kode54
- Updated residfp to revision 1516
- Version is now 1.32

2013-04-17 03:45 UTC - kode54
- Fixed stereo playback
- Version is now 1.31

2013-03-25 18:03 UTC - kode54
- Fixed stupid race condition with initializing reSIDfp
- Version is now 1.30

2012-11-26 13:08 UTC - kode54
- Updated sidplay-residfp
- Version is now 1.29

2012-11-25 02:16 UTC - kode54
- Updated sidplay-residfp
- Version is now 1.28

2012-02-19 20:01 UTC - kode54
- Added abort check to decoder
- Version is now 1.27

2012-02-19 01:36 UTC - kode54
- Updated residfp to revision 664
- Implemented resid filter curve configuration
- Version is now 1.26

2012-01-26 22:10 UTC - kode54
- Implemented clock speed and SID chip override controls
- Version is now 1.25

2011-12-22 21:18 UTC - kode54
- Updated to latest residfp library
- Version is now 1.24

2011-02-18 14:48 UTC - kode54
- Fixed relative path token matching for the profile path, for real this time
- Version is now 1.23

2011-02-03 14:47 UTC - kode54
- Fixed relative path token matching on configuration file read
- Version is now 1.22

2011-02-03 07:50 UTC - kode54
- Fixed relative paths for the profile path, which has file:// prepended to it
- Version is now 1.21

2011-02-02 02:49 UTC - kode54
- Implemented smart relative path support for song length database
- Version is now 1.20

2011-02-02 06:15 UTC - kode54
- Implemented song length database locking

2010-08-10 20:27 UTC - kode54
- Fixed archive path handling of path separators in sidplay
- File open function now reports unsupported file when SidTune loader fails
- Version is now 1.19

2010-07-20 05:23 UTC - kode54
- Updated to sidplay-residfp
- Version is now 1.18

2010-05-01 06:54 UTC - kode54
- Updated lengths database picker to use the last file's path
- Version is now 1.17

2010-04-14 20:47 UTC - kode54
- Amended preferences WM_INITDIALOG handler
- Version is now 1.16

2010-01-11 15:36 UTC - kode54
- Updated preferences page to 1.0 API
- Version is now 1.15

2009-04-16 22:30 UTC - kode54
- Updated libsidplay to latest CVS code
- Version is now 1.14

2005-01-12 20:28 UTC - kode54
- Changed subsong behavior, for great irrelevance
- Version is now 1.13

2004-02-12 21:18 UTC - kode54
- Undid changes from v1.11 as they led to heap corruption
- Version is now 1.12

2003-10-28 01:23 UTC - kode54
- SLDB loading is now faster
- Version is now 1.11

2003-10-08 12:06 UTC - kode54
- Removed unused bits per sample configuration, now hard coded to use 16
- Updated resid to 0.1.4-p1 ... could have saved a lot of hassle if I'd noticed this patched version
- Version is now 1.10

2003-10-08 10:36 UTC - kode54
- Added Song-Lengths Database support
- Added default length and indefinite play options

2003-08-30 17:46 UTC - kode54
- Changed info handler to add extra fields, which header doesn't properly document
- Version is now 1.04

2003-08-27 06:33 UTC - kode54
- Removed MUS from indexer
- Ignore default comment fields

2003-08-27 02:42 UTC - kode54
- Added support for MUS/STR files, added MUS/STR memory loader to SidTune.cpp
- Version is now 1.03

2003-08-26 23:17 UTC - kode54
- Indexer wasn't releasing pTune
- Updated libsidplay to latest CVS code, fixes one major memory leak
  There may be more memory leaks in libsidplay...buoy
- Version is now 1.02

2003-08-26 21:35 UTC - kode54
- Fixed info/comment string handler
- Version is now 1.01

2003-08-26 20:00 UTC - kode54
- Initial release
- Version is 1.0

*/
