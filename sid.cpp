#define MYVERSION "1.60"

/*
	changelog

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

#include <foobar2000.h>
#include "../helpers/dropdown_helper.h"

#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include "../helpers/atl-misc.h"
#include "../../libPPUI/wtl-pp.h"

#include "SidTuneMod.h"
#include <sidplayfp/SidConfig.h>
#include <sidplayfp/SidInfo.h>
#include <sidplayfp/SidTuneInfo.h>
#include <sidplayfp/sidplayfp.h>
#include <builders/resid-builder/resid.h>
#include <builders/residfp-builder/residfp.h>
#include <utils/SidDatabase.h>

// XXX
//#include <builders/residfp-builder/residfp/FilterModelConfig.h>

#include "resource.h"

#include "roms.hpp"

// {7ABA4483-9480-4f9b-ADB5-BA2A495EAB22}
static const GUID guid_cfg_infinite = 
{ 0x7aba4483, 0x9480, 0x4f9b, { 0xad, 0xb5, 0xba, 0x2a, 0x49, 0x5e, 0xab, 0x22 } };
// {6228BE43-CBB1-48E4-9488-0F4242091BB7}
static const GUID guid_cfg_deflength =
{ 0x6228be43, 0xcbb1, 0x48e4, { 0x94, 0x88, 0xf, 0x42, 0x42, 0x9, 0x1b, 0xb7 } };
// {4143484B-5A86-40f6-BD86-5D7B0A33D5BD}
static const GUID guid_cfg_fade = 
{ 0x4143484b, 0x5a86, 0x40f6, { 0xbd, 0x86, 0x5d, 0x7b, 0xa, 0x33, 0xd5, 0xbd } };
// {9033CEAB-97E6-451f-B3DD-F7A875657E67}
static const GUID guid_cfg_rate = 
{ 0x9033ceab, 0x97e6, 0x451f, { 0xb3, 0xdd, 0xf7, 0xa8, 0x75, 0x65, 0x7e, 0x67 } };
// {DF4F3EE1-E765-4bcb-8757-4B3A27BB0C8C}
static const GUID guid_cfg_db_path = 
{ 0xdf4f3ee1, 0xe765, 0x4bcb, { 0x87, 0x57, 0x4b, 0x3a, 0x27, 0xbb, 0xc, 0x8c } };
// {6ECF3074-FA4D-4717-B0B1-C311C3AEBA80}
static const GUID guid_cfg_history_rate = 
{ 0x6ecf3074, 0xfa4d, 0x4717, { 0xb0, 0xb1, 0xc3, 0x11, 0xc3, 0xae, 0xba, 0x80 } };
// {D0FC838C-9433-409A-93E4-0B89D3B4FB15}
static const GUID guid_cfg_clock_override = 
{ 0xd0fc838c, 0x9433, 0x409a, { 0x93, 0xe4, 0xb, 0x89, 0xd3, 0xb4, 0xfb, 0x15 } };
// {9DF20A98-DFA4-461C-8F67-6009FD934590}
static const GUID guid_cfg_sid_override = 
{ 0x9df20a98, 0xdfa4, 0x461c, { 0x8f, 0x67, 0x60, 0x9, 0xfd, 0x93, 0x45, 0x90 } };
// {8DCA8173-C912-4C79-BBCF-1AC005DCE6D3}
static const GUID guid_cfg_sid_filter_6581 = 
{ 0x8dca8173, 0xc912, 0x4c79, { 0xbb, 0xcf, 0x1a, 0xc0, 0x5, 0xdc, 0xe6, 0xd3 } };
// {58ED05BF-A672-43D1-A87D-BD4D8BFCE7B3}
static const GUID guid_cfg_sid_filter_8580_old = 
{ 0x58ed05bf, 0xa672, 0x43d1, { 0xa8, 0x7d, 0xbd, 0x4d, 0x8b, 0xfc, 0xe7, 0xb3 } };
// {69ABCEF1-F460-45A4-8583-DFF0501ADAC7}
static const GUID guid_cfg_sid_filter_8580 =
{ 0x69abcef1, 0xf460, 0x45a4,{ 0x85, 0x83, 0xdf, 0xf0, 0x50, 0x1a, 0xda, 0xc7 } };
// {C9E01956-6EAB-46E3-AEA1-2A8A3A34382D}
static const GUID guid_cfg_sid_builder =
{ 0xc9e01956, 0x6eab, 0x46e3, { 0xae, 0xa1, 0x2a, 0x8a, 0x3a, 0x34, 0x38, 0x2d } };
// {0278879D-7C46-41A4-9F42-2786D32B5BBE}
static const GUID guid_cfg_stereo_separation =
{ 0x278879d, 0x7c46, 0x41a4, { 0x9f, 0x42, 0x27, 0x86, 0xd3, 0x2b, 0x5b, 0xbe } };

enum
{
	sid_builder_resid = 0,
	sid_builder_residfp = 1
};

enum
{
	default_cfg_infinite = 0,
	default_cfg_deflength = 180000,
	default_cfg_fade = 200,
	default_cfg_rate = 44100,
	default_cfg_clock_override = 0,
	default_cfg_sid_override = 0,
	default_cfg_sid_filter_6581 = 128,
	default_cfg_sid_filter_8580 = 128,
	default_cfg_sid_builder = sid_builder_residfp,
	default_cfg_stereo_separation = 50
};

static cfg_int cfg_infinite(guid_cfg_infinite,default_cfg_infinite);
static cfg_int cfg_deflength(guid_cfg_deflength, default_cfg_deflength);
static cfg_int cfg_fade(guid_cfg_fade, default_cfg_fade);
static cfg_int cfg_rate(guid_cfg_rate,default_cfg_rate);
static cfg_int cfg_clock_override(guid_cfg_clock_override,default_cfg_clock_override);
static cfg_int cfg_sid_override(guid_cfg_sid_override,default_cfg_sid_override);
static cfg_int cfg_sid_filter_6581(guid_cfg_sid_filter_6581,default_cfg_sid_filter_6581);
static cfg_int cfg_sid_filter_8580(guid_cfg_sid_filter_8580,default_cfg_sid_filter_8580);
static cfg_int cfg_sid_filter_8580_old(guid_cfg_sid_filter_8580_old, -1);
static cfg_int cfg_sid_builder(guid_cfg_sid_builder,default_cfg_sid_builder);
static cfg_int cfg_stereo_separation(guid_cfg_stereo_separation,default_cfg_stereo_separation);
//static cfg_int cfg_bps("sid_bps",16);

static class initquit_upgrade_vars : public initquit
{
public:
	void on_init()
	{
		if (cfg_sid_filter_8580_old > 0)
		{
			if (cfg_sid_filter_8580_old <= 12500)
			{
				cfg_sid_filter_8580 = (cfg_sid_filter_8580_old - 150) * 128 / (12500 - 150);
			}
			else if (cfg_sid_filter_8580_old <= 22050)
			{
				cfg_sid_filter_8580 = 128 + ((cfg_sid_filter_8580_old - 12500) / (22050 - 12500));
			}
			else
			{
				cfg_sid_filter_8580 = 128;
			}
			cfg_sid_filter_8580_old = -1;
		}
	}

	void on_quit() { }
};

static cfg_string cfg_db_path(guid_cfg_db_path, "");

static critical_section db_lock;

static bool db_loaded = false;

static SidDatabase db;

static void convert_db_path( const char * in, pfc::string_base & out, bool from_config );

static void db_load()
{
	pfc::string8 path;
	convert_db_path( cfg_db_path, path, true );
	if (path.length() == 0) return;
	db_loaded = db.open(pfc::stringcvt::string_wide_from_utf8(path));
}

static void db_unload()
{
	db.close();
	db_loaded = false;
}

static void convert_db_path( const char * in, pfc::string_base & out, bool from_config )
{
	pfc::string8 player_path;
	pfc::string8 component_path;
	pfc::string8 profile_path;
	
	uGetModuleFileName( NULL, player_path );
	player_path.truncate( player_path.scan_filename() - 1 );

	component_path = core_api::get_my_full_path();
	component_path.truncate( component_path.scan_filename() - 1 );

	const char * path = core_api::get_profile_path();
	if ( !pfc::stricmp_ascii_ex( path, 7, "file://", 7 ) ) path += 7;
	profile_path = path;

	out.reset();

	if ( from_config )
	{
		while ( *in )
		{
			t_size first = pfc::string_find_first( in, '<' );
			out.add_string( in, first );
			if ( first != pfc::infinite_size )
			{
				if ( !pfc::strcmp_partial( in + first, "<player path>" ) ) out += player_path;
				else if ( !pfc::strcmp_partial( in + first, "<profile path>" ) ) out += profile_path;
				else if ( !pfc::strcmp_partial( in + first, "<component path>" ) ) out += component_path;
				first = pfc::string_find_first( in, '>', first );
				if ( first != pfc::infinite_size ) ++first;
			}
			if ( first == pfc::infinite_size ) first = strlen( in );
			in += first;
		}
	}
	else
	{
		while ( *in )
		{
			size_t pos_player_path = pfc::string_find_first( in, player_path );
			size_t pos_component_path = pfc::string_find_first( in, component_path );
			size_t pos_profile_path = pfc::string_find_first( in, profile_path );
			t_size first = pos_player_path;
			if ( pos_component_path < first ) first = pos_component_path;
			if ( pos_profile_path < first ) first = pos_profile_path;
			out.add_string( in, first );
			if ( first != pfc::infinite_size )
			{
				if ( first == pos_component_path ) { out += "<component path>"; in += first + component_path.length(); }
				else if ( first == pos_profile_path ) { out += "<profile path>"; in += first + profile_path.length(); }
				else if ( first == pos_player_path ) { out += "<player path>"; in += first + player_path.length(); }
			}
			if ( first == pfc::infinite_size ) first = strlen( in );
			in += first;
		}
	}
}

/*namespace reSIDfp
{
	static struct residfp_fixup
	{
		residfp_fixup()
		{
			(void) FilterModelConfig::getInstance();
		}
	} fix_crap;
};*/

class input_sid : public input_stubs
{
	int dSrate, dBps, stereo_separation;

	unsigned length, played, fade;

	bool eof;

	bool first_block;

	SidTuneMod             * pTune;

	sidplayfp              * m_engine;
    sidbuilder             * m_sidBuilder;

	pfc::array_t<t_int16>    m_sampleBuffer;

	t_filestats m_stats;

public:
	input_sid()
	{
		pTune = NULL;
		m_engine = NULL;
		m_sidBuilder = NULL;
	}

	~input_sid()
	{
		delete pTune;
		delete m_engine;
		delete m_sidBuilder;
	}

	void open( service_ptr_t<file> p_file, const char * p_path, t_input_open_reason p_reason, abort_callback & p_abort )
	{
		if ( p_reason == input_open_info_write ) throw exception_tagging_unsupported();

		if ( p_file.is_empty() )
		{
			filesystem::g_open( p_file, p_path, filesystem::open_mode_read, p_abort );
		}

		m_stats = p_file->get_stats( p_abort );

		pTune = new SidTuneMod( p_path );

		if ( ! pTune->getStatus() ) throw exception_io_unsupported_format();

		dSrate = cfg_rate;
		//dBps = cfg_bps;

		stereo_separation = cfg_stereo_separation;
	}

	unsigned get_subsong_count()
	{
		return pTune->getInfo()->songs();
	}

	t_uint32 get_subsong(unsigned p_index)
	{
		return p_index + 1;
	}

	void get_info( t_uint32 p_subsong, file_info & p_info, abort_callback & p_abort )
	{
		if ( ! p_subsong ) return;

		const SidTuneInfo * sidinfo;

		sidinfo = pTune->getInfo();

		//p_info.info_set_int("samplerate", dSrate);
		p_info.info_set( "encoding", "synthesized" );
		p_info.info_set_int("channels", 2);
		p_info.info_set_int("sid_chip_count", sidinfo->sidChips());

		int i = sidinfo->numberOfInfoStrings();

		if (i >= 1 && sidinfo->infoString(0) && sidinfo->infoString(0)[0]) p_info.meta_add(sidinfo->songs() > 1 ? "album" : "title", pfc::stringcvt::string_utf8_from_ansi(sidinfo->infoString(0)));
		if (i >= 2 && sidinfo->infoString(1) && sidinfo->infoString(1)[0]) p_info.meta_add("artist", pfc::stringcvt::string_utf8_from_ansi(sidinfo->infoString(1)));
		if (i >= 3 && sidinfo->infoString(2) && sidinfo->infoString(2)[0]) p_info.meta_add("copyright", pfc::stringcvt::string_utf8_from_ansi(sidinfo->infoString(2)));

		for (int j = 3; j < i; j++)
		{
			if (sidinfo->infoString(j) && sidinfo->infoString(j)[0]) p_info.meta_add("info", pfc::stringcvt::string_utf8_from_ansi(sidinfo->infoString(j)));
		}

		for (int i = 0, j = sidinfo->numberOfCommentStrings(); i < j; i++)
		{
			if (sidinfo->commentString(i) && sidinfo->commentString(i)[0] && strcmp(sidinfo->commentString(i), "--- SAVED WITH SIDPLAY ---")) p_info.meta_add("comment", pfc::stringcvt::string_utf8_from_ansi(sidinfo->commentString(i)));
		}

		if (sidinfo->clockSpeed() && ( sidinfo->clockSpeed() == SidTuneInfo::CLOCK_NTSC || sidinfo->clockSpeed() == SidTuneInfo::CLOCK_PAL ) )
			p_info.info_set("clock_speed", sidinfo->clockSpeed() == SidTuneInfo::CLOCK_NTSC ? "NTSC" : "PAL");
		p_info.info_set("sid_model", sidinfo->sidModel(0) == SidTuneInfo::SIDMODEL_8580 ? "8580" : "6581");

		unsigned length = cfg_deflength;

		{
			insync( db_lock );

			if ( ! db_loaded ) db_load();

			if ( db_loaded )
			{
				char md5[SidTune::MD5_LENGTH + 1];
				pTune->createMD5New(md5);
				int32_t len = db.lengthMs( md5, p_subsong );
				if ( len > 0 ) length = len;
			}
		}

		p_info.set_length( double( length ) / 1000.0 );
	}

	t_filestats get_file_stats( abort_callback & p_abort )
	{
		return m_stats;
	}

	void decode_initialize( t_uint32 p_subsong, unsigned p_flags, abort_callback & p_abort )
	{
		if ( ! p_subsong ) throw exception_io_data();

		first_block = true;

		pTune->selectSong(p_subsong);

		int dNch = pTune->getInfo()->sidChips();

		length = cfg_deflength;

		{
			insync( db_lock );

			if ( ! db_loaded ) db_load();

			if ( db_loaded )
			{
				int32_t len = db.lengthMs(*pTune);
				if ( len > 0 ) length = len;
			}
		}

		delete m_engine;

		m_engine = new sidplayfp;

		m_engine->setRoms( kernel, basic, chargen );

		if (!m_engine->load( pTune )) throw exception_io_data(m_engine->error());

		delete m_sidBuilder;
		m_sidBuilder = NULL;

		switch (cfg_sid_builder)
		{
		case sid_builder_residfp:
		{
			ReSIDfpBuilder * builder = new ReSIDfpBuilder("ReSIDfp");
			if (builder)
			{
				m_sidBuilder = builder;
				builder->create((m_engine->info()).maxsids());
				if (builder->getStatus())
				{
					builder->filter(true);
					builder->filter6581Curve(cfg_sid_filter_6581 / 256.);
					builder->filter8580Curve(cfg_sid_filter_8580 / 256.);
				}
				if (!builder->getStatus()) throw exception_io_data();
			}
		}
		break;
		case sid_builder_resid:
		{
			ReSIDBuilder * builder = new ReSIDBuilder("ReSID");
			if (builder)
			{
				m_sidBuilder = builder;
				builder->create((m_engine->info()).maxsids());
				if (builder->getStatus())
					builder->filter(true);
				if (!builder->getStatus()) throw exception_io_data();
			}
		}
		break;
		}

		SidConfig conf;
		conf = m_engine->config();
		conf.frequency = dSrate;
		conf.playback = SidConfig::STEREO;
		conf.sidEmulation = m_sidBuilder;
		if ( cfg_clock_override )
		{
			conf.forceC64Model = true;
			conf.defaultC64Model = ( cfg_clock_override == 1 ) ? SidConfig::PAL : SidConfig::NTSC;
		}
		if ( cfg_sid_override )
		{
			conf.forceSidModel = true;
			conf.defaultSidModel = ( cfg_sid_override == 1 ) ? SidConfig::MOS6581 : SidConfig::MOS8580;
		}
		if (!m_engine->config(conf)) throw exception_io_data(m_engine->error());

		played = 0;

		eof = false;

		if ( !cfg_infinite || ( p_flags & input_flag_no_looping ) )
		{
			length = (unsigned int)((__int64)length * dSrate / 1000) * 2;
			fade = (cfg_fade * dSrate / 1000) * 2;
		}
		else
		{
			length = 0;
		}

		m_sampleBuffer.set_count( 10240 * 2 );
	}

	bool decode_run( audio_chunk & p_chunk,abort_callback & p_abort )
	{
		p_abort.check();

		if ( eof || ( length && played >= length ) ) return false;

		int samples = length - played, written; //(stereo)

		if ( !length || ( samples > 10240 * 2 ) ) samples = 10240 * 2;

		p_chunk.grow_data_size( samples );
		p_chunk.set_srate( dSrate );
		p_chunk.set_channels( 2 );

		written = m_engine->play( m_sampleBuffer.get_ptr(), samples );

		audio_math::convert_from_int16( m_sampleBuffer.get_ptr(), written, p_chunk.get_data(), 1.0 );

		if ( written < samples )
		{
			if ( m_engine->error() ) throw exception_io_data( m_engine->error() );
			eof = true;
		}

		audio_sample factor = (audio_sample)stereo_separation * 0.005; /* percent, pre-scaled by half */

		for (int i = 0; i < written; i += 2)
		{
			/* convert to mid-side, scale side difference according to user setting */
			audio_sample* sample = p_chunk.get_data() + i;
			float mid = (sample[0] + sample[1]) * 0.5;
			float side = (sample[0] - sample[1]) * factor;
			sample[0] = mid + side;
			sample[1] = mid - side;
		}

		p_chunk.set_sample_count( written / 2 );

		unsigned d_start, d_end;
		d_start = played;
		played += written;
		d_end = written;

		if ( length && d_end + fade > length )
		{
			audio_sample * foo = p_chunk.get_data();
			unsigned n;
			factor = 1.0 / fade;
			{
				for ( n = d_start; n < d_end; n += 2 )
				{
					if ( n > length )
					{
						foo[0] = 0;
						foo[1] = 0;
					}
					else
					{
						audio_sample bleh = (audio_sample)(length - n) * factor;
						foo[0] *= bleh;
						foo[1] *= bleh;
					}
					foo += 2;
				}
			}
		}

		return true;
	}

	void decode_seek( double p_seconds, abort_callback & p_abort )
	{
		first_block = true;
		unsigned samples = unsigned( audio_math::time_to_samples( p_seconds, dSrate ) );
		samples *= 2;
		if ( samples < played )
		{
			decode_initialize( pTune->getInfo()->currentSong(), input_flag_playback | ( length ? input_flag_no_looping : 0 ), p_abort );
		}
		pfc::array_t<t_int16> sample_buffer;
		sample_buffer.grow_size( 10240 * 2 );
		eof = false;

		/*unsigned remain = ( samples - played ) % 32;
		played /= 32;
		samples /= 32;
		m_engine->fastForward( 100 * 32 );*/

		while ( played < samples )
		{
			p_abort.check();

			unsigned todo = samples - played;
			if ( todo > 10240 * 2 ) todo = 10240 * 2;
			unsigned done = m_engine->play( sample_buffer.get_ptr(), todo );
			if ( done < todo )
			{
				if ( m_engine->error() )
					throw exception_io_data( m_engine->error() );
				eof = true;
				break;
			}
			played += todo;
		}

		/*played *= 32;
		m_engine->fastForward( 100 );

		if ( remain )
			played += m_engine->play( sample_buffer.get_ptr(), remain );*/
	}

	bool decode_can_seek()
	{
		return true;
	}

	bool decode_get_dynamic_info(file_info & p_out, double & p_timestamp_delta)
	{
		if ( first_block )
		{
			p_out.info_set_int( "samplerate", dSrate );
			p_timestamp_delta = 0.0;
			first_block = false;
			return true;
		}

		return false;
	}

	bool decode_get_dynamic_info_track(file_info & p_out, double & p_timestamp_delta)
	{
		return false;
	}

	void decode_on_idle( abort_callback & p_abort )
	{
	}

	void retag_set_info( t_uint32 p_subsong, const file_info & p_info, abort_callback & p_abort )
	{
		throw exception_tagging_unsupported();
	}

	void retag_commit( abort_callback & p_abort )
	{
		throw exception_tagging_unsupported();
	}

	static bool g_is_our_content_type( const char * p_content_type )
	{
		return false;
	}

	static bool g_is_our_path( const char * p_path, const char * p_extension )
	{
		return ! stricmp( p_extension, "sid" ) || ! stricmp( p_extension, "mus" );
	}

	static GUID g_get_guid()
	{
		return { 0x7fff51a2, 0x5130, 0x4307,{ 0x95, 0x22, 0x16, 0xf9, 0x74, 0xd, 0x61, 0xde } };
	}

	static const char * g_get_name()
	{
		return "sidplay";
	}

	static GUID g_get_preferences_guid()
	{
		return { 0x206017ac, 0x421, 0x4d37,{ 0x9b, 0x1f, 0x99, 0xb9, 0xea, 0xde, 0x74, 0x4e } };
	}
};

static cfg_dropdown_history cfg_history_rate(guid_cfg_history_rate,16);

static const int srate_tab[]={8000,11025,16000,22050,24000,32000,44100,48000,64000,88200,96000};

class CMyPreferences : public CDialogImpl<CMyPreferences>, public preferences_page_instance {
public:
	//Constructor - invoked by preferences_page_impl helpers - don't do Create() in here, preferences_page_impl does this for us
	CMyPreferences(preferences_page_callback::ptr callback) : m_callback(callback) {}

	//Note that we don't bother doing anything regarding destruction of our class.
	//The host ensures that our dialog is destroyed first, then the last reference to our preferences_page_instance object is released, causing our object to be deleted.


	//dialog resource ID
	enum {IDD = IDD_CONFIG};
	// preferences_page_instance methods (not all of them - get_wnd() is supplied by preferences_page_impl helpers)
	t_uint32 get_state();
	void apply();
	void reset();

	//WTL message map
	BEGIN_MSG_MAP(CMyPreferences)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_HANDLER_EX(IDC_INFINITE, BN_CLICKED, OnButtonClick)
		COMMAND_HANDLER_EX(IDC_DB_PATH_SET, BN_CLICKED, OnDBPathSet)
		COMMAND_HANDLER_EX(IDC_DB_PATH_CLEAR, BN_CLICKED, OnDBPathClear)
		COMMAND_HANDLER_EX(IDC_DLENGTH, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_FADE, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_SAMPLERATE, CBN_EDITCHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_SAMPLERATE, CBN_SELCHANGE, OnSelectionChange)
		COMMAND_HANDLER_EX(IDC_SID_BUILDER, CBN_SELCHANGE, OnSelectionChange)
		COMMAND_HANDLER_EX(IDC_CLOCK_OVERRIDE, CBN_SELCHANGE, OnSelectionChange)
		COMMAND_HANDLER_EX(IDC_SID_OVERRIDE, CBN_SELCHANGE, OnSelectionChange)
		MSG_WM_HSCROLL(OnHScroll);
		DROPDOWN_HISTORY_HANDLER(IDC_SAMPLERATE, cfg_history_rate)
	END_MSG_MAP()
private:
	BOOL OnInitDialog(CWindow, LPARAM);
	void OnEditChange(UINT, int, CWindow);
	void OnSelectionChange(UINT, int, CWindow);
	void OnButtonClick(UINT, int, CWindow);
	void OnDBPathSet(UINT, int, CWindow);
	void OnDBPathClear(UINT, int, CWindow);
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);
	bool HasChanged();
	void OnChanged();
	
	void update_db_status();
	
	const preferences_page_callback::ptr m_callback;

	CTrackBarCtrl m_slider_6581, m_slider_8580, m_slider_ssep;
};

void CMyPreferences::update_db_status()
{
	insync( db_lock );
	pfc::string8 status;
	if ( db_loaded )
	{
		status << "Database loaded.";
	}
	else
	{
		status << "Not loaded.";
	}
	uSetDlgItemText( m_hWnd, IDC_DB_STATUS, status );
}

const char* parseTime(const char* str, int_least32_t& result);

BOOL CMyPreferences::OnInitDialog(CWindow, LPARAM) {
	SendDlgItemMessage( IDC_INFINITE, BM_SETCHECK, cfg_infinite );
	uSetDlgItemText( m_hWnd, IDC_DLENGTH, pfc::format_time_ex( (double)cfg_deflength / 1000.0 ) );
	uSetDlgItemText( m_hWnd, IDC_DB_PATH, cfg_db_path );
	update_db_status();
	{
		CWindow w;
		{
			char temp[16];
			int n;
			SetDlgItemInt( IDC_FADE, cfg_fade, FALSE );
			uSendMessage( GetDlgItem( IDC_FADE ), EM_LIMITTEXT, 3, 0);
			for(n=tabsize(srate_tab);n--;)
			{
				if (srate_tab[n] != cfg_rate)
				{
					itoa(srate_tab[n], temp, 10);
					cfg_history_rate.add_item(temp);
				}
			}
			itoa(cfg_rate, temp, 10);
			cfg_history_rate.add_item(temp);
			w = GetDlgItem( IDC_SAMPLERATE );
			cfg_history_rate.setup_dropdown( w );
			::SendMessage( w, CB_SETCURSEL, 0, 0 );
		}

		w = GetDlgItem( IDC_SID_BUILDER );
		uSendMessageText( w, CB_ADDSTRING, 0, "ReSID" );
		uSendMessageText( w, CB_ADDSTRING, 0, "ReSIDfp" );
		::SendMessage( w, CB_SETCURSEL, cfg_sid_builder, 0 );

		w = GetDlgItem( IDC_CLOCK_OVERRIDE );
		uSendMessageText( w, CB_ADDSTRING, 0, "As input file specifies" );
		uSendMessageText( w, CB_ADDSTRING, 0, "Force PAL" );
		uSendMessageText( w, CB_ADDSTRING, 0, "Force NTSC" );
		::SendMessage( w, CB_SETCURSEL, cfg_clock_override, 0 );

		w = GetDlgItem( IDC_SID_OVERRIDE );
		uSendMessageText( w, CB_ADDSTRING, 0, "As input file specifies" );
		uSendMessageText( w, CB_ADDSTRING, 0, "Force 6581" );
		uSendMessageText( w, CB_ADDSTRING, 0, "Force 8580" );
		::SendMessage( w, CB_SETCURSEL, cfg_sid_override, 0 );
	}

	pfc::string8_fast temp;

	m_slider_6581 = GetDlgItem( IDC_SLIDER_6581 );
	m_slider_6581.SetRangeMin( 0 );
	m_slider_6581.SetRangeMax( 256 );
	m_slider_6581.SetPos( cfg_sid_filter_6581 );
	temp = pfc::format_float(cfg_sid_filter_6581 / 256., 0, 2);
	uSetDlgItemText( m_hWnd, IDC_TEXT_6581, temp );
	if ( cfg_sid_builder != sid_builder_residfp )
		m_slider_6581.EnableWindow( FALSE );

	m_slider_8580 = GetDlgItem( IDC_SLIDER_8580 );
	m_slider_8580.SetRangeMin( 0 );
	m_slider_8580.SetRangeMax( 256 );
	m_slider_8580.SetPos( cfg_sid_filter_8580 );
	temp = pfc::format_float(cfg_sid_filter_8580 / 256., 0, 2);
	uSetDlgItemText( m_hWnd, IDC_TEXT_8580, temp );
	if ( cfg_sid_builder != sid_builder_residfp )
		m_slider_8580.EnableWindow( FALSE );

	m_slider_ssep = GetDlgItem( IDC_SLIDER_SSEP );
	m_slider_ssep.SetRangeMin( 0 );
	m_slider_ssep.SetRangeMax( 150 );
	m_slider_ssep.SetPos( cfg_stereo_separation );
	temp = pfc::format_int(cfg_stereo_separation);
	temp += "%";
	uSetDlgItemText( m_hWnd, IDC_TEXT_SSEP, temp );

	return FALSE;
}

void CMyPreferences::OnEditChange(UINT, int, CWindow) {
	OnChanged();
}

void CMyPreferences::OnSelectionChange(UINT, int, CWindow wnd) {
	if ( wnd == GetDlgItem( IDC_SID_BUILDER ) )
	{
		BOOL enable = wnd.SendMessage( CB_GETCURSEL ) == sid_builder_residfp;
		m_slider_6581.EnableWindow( enable );
		m_slider_8580.EnableWindow( enable );
	}
	OnChanged();
}

void CMyPreferences::OnButtonClick(UINT, int, CWindow) {
	OnChanged();
}

void CMyPreferences::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
{
	pfc::string8_fast temp;
	if ( pScrollBar.m_hWnd == m_slider_6581.m_hWnd )
	{
		temp = pfc::format_float(m_slider_6581.GetPos() / 256., 0, 2);
		uSetDlgItemText( m_hWnd, IDC_TEXT_6581, temp );
	}
	else if ( pScrollBar.m_hWnd == m_slider_8580.m_hWnd )
	{
		temp = pfc::format_float(m_slider_8580.GetPos() / 256., 0, 2);
		uSetDlgItemText( m_hWnd, IDC_TEXT_8580, temp );
	}
	else if ( pScrollBar.m_hWnd == m_slider_ssep.m_hWnd )
	{
		temp = pfc::format_int(m_slider_ssep.GetPos());
		temp += "%";
		uSetDlgItemText( m_hWnd, IDC_TEXT_SSEP, temp );
	}
	OnChanged();
}

void CMyPreferences::OnDBPathSet(UINT, int, CWindow) {
	pfc::string8 path;
	convert_db_path( string_utf8_from_window( m_hWnd, IDC_DB_PATH ), path, true );
	pfc::string8 directory( path );
	directory.truncate( directory.scan_filename() );
	if ( uGetOpenFileName( core_api::get_main_window(), "Song length database|Songlengths.md5",
		1, 0, "Choose SidPlay Song-Lengths Database...", directory, path, false ) )
	{
		pfc::string8 new_path;
		convert_db_path( path, new_path, false );
		uSetDlgItemText( m_hWnd, IDC_DB_PATH, new_path );
		OnChanged();
	}
}

void CMyPreferences::OnDBPathClear(UINT, int, CWindow) {
	uSetDlgItemText( m_hWnd, IDC_DB_PATH, "" );
	OnChanged();
}

t_uint32 CMyPreferences::get_state() {
	t_uint32 state = preferences_state::resettable;
	if (HasChanged()) state |= preferences_state::changed;
	return state;
}

void CMyPreferences::reset() {
	pfc::string8_fast temp;
	SendDlgItemMessage( IDC_INFINITE, BM_SETCHECK, default_cfg_infinite );
	SendDlgItemMessage( IDC_SID_BUILDER, CB_SETCURSEL, default_cfg_sid_builder );
	SendDlgItemMessage( IDC_CLOCK_OVERRIDE, CB_SETCURSEL, default_cfg_clock_override );
	SendDlgItemMessage( IDC_SID_OVERRIDE, CB_SETCURSEL, default_cfg_sid_override );
	uSetDlgItemText( m_hWnd, IDC_DLENGTH, pfc::format_time_ex( (double)default_cfg_deflength / 1000.0 ) );
	uSetDlgItemText( m_hWnd, IDC_DB_PATH, "" );
	SetDlgItemInt( IDC_SAMPLERATE, default_cfg_rate, FALSE );
	SetDlgItemInt( IDC_FADE, default_cfg_fade, FALSE );
	m_slider_6581.SetPos( default_cfg_sid_filter_6581 );
	temp = pfc::format_float(default_cfg_sid_filter_6581 / 256., 0, 2);
	uSetDlgItemText( m_hWnd, IDC_TEXT_6581, temp );
	m_slider_8580.SetPos( default_cfg_sid_filter_8580 );
	temp = pfc::format_float(default_cfg_sid_filter_8580 / 256., 0, 2);
	uSetDlgItemText( m_hWnd, IDC_TEXT_8580, temp );
	m_slider_ssep.SetPos( default_cfg_stereo_separation );
	temp = pfc::format_int(default_cfg_stereo_separation);
	temp += "%";
	uSetDlgItemText( m_hWnd, IDC_TEXT_SSEP, temp );
	
	OnChanged();
}

void CMyPreferences::apply() {
	char temp[16];
	int t = GetDlgItemInt( IDC_SAMPLERATE, NULL, FALSE );
	if ( t < 6000 ) t = 6000;
	else if ( t > 192000 ) t = 192000;
	SetDlgItemInt( IDC_SAMPLERATE, t, FALSE );
	itoa( t, temp, 10 );
	cfg_history_rate.add_item(temp);
	cfg_rate = t;
	pfc::string8 db_path;
	convert_db_path( string_utf8_from_window( m_hWnd, IDC_DB_PATH ), db_path, false );
	cfg_db_path = db_path;
	{
		insync( db_lock );
		db_unload();
		db_load();
	}
	update_db_status();
	{
		string_utf8_from_window foo( m_hWnd, IDC_DLENGTH );
		int_least32_t timestamp = 0;
		const char * bar = foo.get_ptr();
		const char* end;
		try
		{
			parseTime(bar, timestamp);
		}
		catch (...)
		{
		}
		if (timestamp) cfg_deflength = timestamp;
		else uSetDlgItemText( m_hWnd, IDC_DLENGTH, pfc::format_time_ex( (double)cfg_deflength / 1000.0 ) );
	}
	cfg_fade = GetDlgItemInt( IDC_FADE, NULL, FALSE );
	cfg_infinite = SendDlgItemMessage( IDC_INFINITE, BM_GETCHECK );
	cfg_clock_override = SendDlgItemMessage( IDC_CLOCK_OVERRIDE, CB_GETCURSEL );
	cfg_sid_override = SendDlgItemMessage( IDC_SID_OVERRIDE, CB_GETCURSEL );
	cfg_sid_builder = SendDlgItemMessage( IDC_SID_BUILDER, CB_GETCURSEL );
	cfg_sid_filter_6581 = m_slider_6581.GetPos();
	cfg_sid_filter_8580 = m_slider_8580.GetPos();
	cfg_stereo_separation = m_slider_ssep.GetPos();
	
	OnChanged();
}

bool CMyPreferences::HasChanged() {
	//returns whether our dialog content is different from the current configuration (whether the apply button should be enabled or not)
	bool changed = false;
	if ( !changed && GetDlgItemInt( IDC_SAMPLERATE, NULL, FALSE ) != cfg_rate ) changed = true;
	if ( !changed && GetDlgItemInt( IDC_FADE, NULL, FALSE ) != cfg_fade ) changed = true;
	if ( !changed && SendDlgItemMessage( IDC_INFINITE, BM_GETCHECK ) != cfg_infinite ) changed = true;
	if ( !changed && SendDlgItemMessage( IDC_CLOCK_OVERRIDE, CB_GETCURSEL ) != cfg_clock_override ) changed = true;
	if ( !changed && SendDlgItemMessage( IDC_SID_OVERRIDE, CB_GETCURSEL ) != cfg_sid_override ) changed = true;
	if ( !changed && SendDlgItemMessage( IDC_SID_BUILDER, CB_GETCURSEL ) != cfg_sid_builder ) changed = true;
	if ( !changed && m_slider_6581.GetPos() != cfg_sid_filter_6581 ) changed = true;
	if ( !changed && m_slider_8580.GetPos() != cfg_sid_filter_8580 ) changed = true;
	if ( !changed && m_slider_ssep.GetPos() != cfg_stereo_separation ) changed = true;
	if ( !changed )
	{
		pfc::string8 db_path;
		convert_db_path( string_utf8_from_window( m_hWnd, IDC_DB_PATH ), db_path, false );
		if ( stricmp_utf8( db_path, cfg_db_path ) ) changed = true;
	}
	if ( !changed )
	{
		string_utf8_from_window foo( m_hWnd, IDC_DLENGTH );
		int_least32_t timestamp = 0;
		const char * bar = foo.get_ptr();
		const char * end;
		try
		{
			end = parseTime(bar, timestamp);
		}
		catch (...)
		{
		}
		if ( timestamp && timestamp != cfg_deflength ) changed = true;
	}
	return changed;
}
void CMyPreferences::OnChanged() {
	//tell the host that our state has changed to enable/disable the apply button appropriately.
	m_callback->on_state_changed();
}

class preferences_page_myimpl : public preferences_page_impl<CMyPreferences> {
	// preferences_page_impl<> helper deals with instantiation of our dialog; inherits from preferences_page_v3.
public:
	const char * get_name() {return input_sid::g_get_name();}
	GUID get_guid() {return input_sid::g_get_preferences_guid();}
	GUID get_parent_guid() {return guid_input;}
};

DECLARE_FILE_TYPE("SID files", "*.SID;*.MUS");

static input_factory_t           <input_sid>               g_input_sid_factory;
static preferences_page_factory_t<preferences_page_myimpl> g_config_sid_factory;
static initquit_factory_t<initquit_upgrade_vars>           g_initquit_sid_factory;

#include "../patrons.h"

DECLARE_COMPONENT_VERSION("sidplay2",MYVERSION,"Based on residfp.\n\nLicensed under the GNU GPL, see COPYING.txt.\n\nhttps://www.patreon.com/kode54\n\n"
MY_PATRONS
);

VALIDATE_COMPONENT_FILENAME("foo_sid.dll");
