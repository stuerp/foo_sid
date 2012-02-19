#define MYVERSION "1.26"

// plain resid filtering is broken
// #define USE_RESID

/*
	changelog

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

#define _WIN32_WINNT 0x0501

#include <foobar2000.h>
#include "../helpers/dropdown_helper.h"
#include "../ATLHelpers/ATLHelpers.h"

#include "SidTuneMod.h"
#include <sidplayfp/sidplay2.h>
#ifdef USE_RESID
#include <builders/resid-builder/resid.h>
#endif
#include <builders/residfp-builder/residfp.h>

#include "sldb.h"

#include "resource.h"

// {7ABA4483-9480-4f9b-ADB5-BA2A495EAB22}
static const GUID guid_cfg_infinite = 
{ 0x7aba4483, 0x9480, 0x4f9b, { 0xad, 0xb5, 0xba, 0x2a, 0x49, 0x5e, 0xab, 0x22 } };
// {59ABCA51-75FB-4e8d-8B6F-5024DFA8D916}
static const GUID guid_cfg_deflength = 
{ 0x59abca51, 0x75fb, 0x4e8d, { 0x8b, 0x6f, 0x50, 0x24, 0xdf, 0xa8, 0xd9, 0x16 } };
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
static const GUID guid_cfg_sid_filter_8580 = 
{ 0x58ed05bf, 0xa672, 0x43d1, { 0xa8, 0x7d, 0xbd, 0x4d, 0x8b, 0xfc, 0xe7, 0xb3 } };
#ifdef USE_RESID
// {55884484-BE63-4425-997E-94A5D23DF605}
static const GUID guid_cfg_use_residfp = 
{ 0x55884484, 0xbe63, 0x4425, { 0x99, 0x7e, 0x94, 0xa5, 0xd2, 0x3d, 0xf6, 0x5 } };
#endif

enum
{
	default_cfg_infinite = 0,
	default_cfg_deflength = 180,
	default_cfg_fade = 200,
	default_cfg_rate = 44100,
	default_cfg_clock_override = 0,
	default_cfg_sid_override = 0,
	default_cfg_sid_filter_6581 = 128,
	default_cfg_sid_filter_8580 = 12500,
#ifdef USE_RESID
	default_cfg_use_residfp = false
#endif
};

static cfg_int cfg_infinite(guid_cfg_infinite,default_cfg_infinite);
static cfg_int cfg_deflength(guid_cfg_deflength, default_cfg_deflength);
static cfg_int cfg_fade(guid_cfg_fade, default_cfg_fade);
static cfg_int cfg_rate(guid_cfg_rate,default_cfg_rate);
static cfg_int cfg_clock_override(guid_cfg_clock_override,default_cfg_clock_override);
static cfg_int cfg_sid_override(guid_cfg_sid_override,default_cfg_sid_override);
static cfg_int cfg_sid_filter_6581(guid_cfg_sid_filter_6581,default_cfg_sid_filter_6581);
static cfg_int cfg_sid_filter_8580(guid_cfg_sid_filter_8580,default_cfg_sid_filter_8580);
#ifdef USE_RESID
static cfg_bool cfg_use_residfp(guid_cfg_use_residfp,default_cfg_use_residfp);
#endif
//static cfg_int cfg_bps("sid_bps",16);

static cfg_string cfg_db_path(guid_cfg_db_path, "");

static critical_section db_lock;

static sldb db;

static void convert_db_path( const char * in, pfc::string_base & out, bool from_config );

static void db_load(abort_callback & p_abort)
{
	pfc::string8 real_path;
	pfc::string8 path("file://");
	convert_db_path( cfg_db_path, real_path, true );
	path += real_path;
	if (path.length() == 7) return;
	db.load(path, p_abort);
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

class input_sid
{
	int dSrate, dBps, dNch;

	unsigned length, played, fade;

	bool eof;

	bool first_block;

	SidTuneMod             * pTune;

	sidplay2               * m_engine;
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
		if ( p_reason == input_open_info_write ) throw exception_io_unsupported_format();


		if ( p_file.is_empty() )
		{
			filesystem::g_open( p_file, p_path, filesystem::open_mode_read, p_abort );
		}

		m_stats = p_file->get_stats( p_abort );

		pTune = new SidTuneMod( p_path );

		if ( ! pTune->getStatus() ) throw exception_io_unsupported_format();

		dSrate = cfg_rate;
		//dBps = cfg_bps;
	}

	unsigned get_subsong_count()
	{
		return pTune->getInfo().songs;
	}

	t_uint32 get_subsong(unsigned p_index)
	{
		return p_index + 1;
	}

	void get_info( t_uint32 p_subsong, file_info & p_info, abort_callback & p_abort )
	{
		if ( ! p_subsong ) throw exception_io_data();

		//p_info.info_set_int("samplerate", dSrate);
		p_info.info_set( "encoding", "synthesized" );
		p_info.info_set_int("channels", pTune->isStereo() ? 2 : 1);

		SidTuneInfo sidinfo;

		pTune->getInfo(sidinfo);

		int i = sidinfo.numberOfInfoStrings;

		if (i >= 1 && sidinfo.infoString[0] && sidinfo.infoString[0][0]) p_info.meta_add(sidinfo.songs > 1 ? "album" : "title", pfc::stringcvt::string_utf8_from_ansi(sidinfo.infoString[0]));
		if (i >= 2 && sidinfo.infoString[1] && sidinfo.infoString[1][0]) p_info.meta_add("artist", pfc::stringcvt::string_utf8_from_ansi(sidinfo.infoString[1]));
		if (i >= 3 && sidinfo.infoString[2] && sidinfo.infoString[2][0]) p_info.meta_add("copyright", pfc::stringcvt::string_utf8_from_ansi(sidinfo.infoString[2]));

		for (int j = 3; j < i; j++)
		{
			if (sidinfo.infoString[j] && sidinfo.infoString[j][0]) p_info.meta_add("info", pfc::stringcvt::string_utf8_from_ansi(sidinfo.infoString[j]));
		}

		for (int i = 0, j = sidinfo.numberOfCommentStrings; i < j; i++)
		{
			if (sidinfo.commentString[i] && sidinfo.commentString[i][0] && strcmp(sidinfo.commentString[i], "--- SAVED WITH SIDPLAY ---")) p_info.meta_add("comment", pfc::stringcvt::string_utf8_from_ansi(sidinfo.commentString[i]));
		}

		if (sidinfo.clockSpeed && ( sidinfo.clockSpeed == SIDTUNE_CLOCK_NTSC || sidinfo.clockSpeed == SIDTUNE_CLOCK_PAL ) )
			p_info.info_set("clock_speed", sidinfo.clockSpeed == SIDTUNE_CLOCK_NTSC ? "NTSC" : "PAL");
		p_info.info_set("sid_model", sidinfo.sidModel1 == SIDTUNE_SIDMODEL_8580 ? "8580" : "6581");

		unsigned length = cfg_deflength;

		{
			insync( db_lock );

			if ( ! db.loaded() ) db_load( p_abort );

			if ( db.loaded() )
			{
				unsigned len = db.find( pTune, p_subsong - 1 );
				if (len) length = len;
			}
		}

		p_info.set_length( double( length ) );
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

		dNch = pTune->isStereo() ? 2 : 1;

		length = cfg_deflength;

		{
			insync( db_lock );

			if ( ! db.loaded() ) db_load( p_abort );

			if ( db.loaded() )
			{
				unsigned len = db.find( pTune, p_subsong - 1 );
				if (len) length = len;
			}
		}

		delete m_engine;

		m_engine = new sidplay2;

		if (m_engine->load( pTune ) < 0) throw exception_io_data(m_engine->error());

		delete m_sidBuilder;
		m_sidBuilder = NULL;

#ifdef USE_RESID
		if ( cfg_use_residfp )
#endif
		{
			ReSIDfpBuilder * builder = new ReSIDfpBuilder( "ReSIDfp" );
			if (builder)
			{
				builder->create ((m_engine->info ()).maxsids);
				if (builder->getStatus())
				{
					builder->filter (true);
					builder->filter6581Curve( cfg_sid_filter_6581 / 256. );
					builder->filter8580Curve( (double) cfg_sid_filter_8580 );
				}
				if (!builder->getStatus()) throw exception_io_data();
				m_sidBuilder = builder;
			}
		}
#ifdef USE_RESID
		else
		{
			ReSIDBuilder * builder = new ReSIDBuilder( "ReSID" );
			if (builder)
			{
				builder->create((m_engine->info ()).maxsids);
				if (builder->getStatus())
				{
					builder->filter (true);
					builder->filter6581Curve( cfg_sid_filter_6581 / 256. );
					builder->filter8580Curve( (double) cfg_sid_filter_8580 );
				}
				if (!builder->getStatus()) throw exception_io_data();
				m_sidBuilder = builder;
			}
		}
#endif

		sid2_config_t conf;
		conf = m_engine->config();
		conf.frequency = dSrate;
		conf.playback = dNch == 2 ? sid2_stereo : sid2_mono;
		conf.sidEmulation = m_sidBuilder;
		if ( cfg_clock_override )
		{
			conf.clockForced = true;
			conf.clockSpeed = ( cfg_clock_override == 1 ) ? SID2_CLOCK_PAL : SID2_CLOCK_NTSC;
		}
		if ( cfg_sid_override )
		{
			conf.sidModel = ( cfg_sid_override == 1 ) ? SID2_MOS6581 : SID2_MOS8580;
		}
		if (m_engine->config(conf) < 0) throw exception_io_data(m_engine->error());

		played = 0;

		eof = false;

		if ( !cfg_infinite || ( p_flags & input_flag_no_looping ) )
		{
			length *= dSrate * dNch;
			fade = (cfg_fade * dSrate / 1000) * dNch;
		}
		else
		{
			length = 0;
		}

		m_sampleBuffer.set_count( 10240 * dNch );
	}

	bool decode_run( audio_chunk & p_chunk,abort_callback & p_abort )
	{
		if ( eof || ( length && played >= length ) ) return false;

		int samples = length - played, written; //(stereo)

		if ( !length || ( samples > 10240 * dNch ) ) samples = 10240 * dNch;

		p_chunk.grow_data_size( samples );
		p_chunk.set_srate( dSrate );
		p_chunk.set_channels( dNch );

		written = m_engine->play( m_sampleBuffer.get_ptr(), samples );

		audio_math::convert_from_int16( m_sampleBuffer.get_ptr(), written * dNch, p_chunk.get_data(), 1.0 );

		if ( written < samples )
		{
			if ( m_engine->state() != sid2_stopped ) throw exception_io_data();
			eof = true;
		}

		p_chunk.set_sample_count( written / dNch );

		unsigned d_start, d_end;
		d_start = played;
		played += written;
		d_end = written;

		if ( length && d_end + fade > length )
		{
			audio_sample * foo = p_chunk.get_data();
			unsigned n;
			audio_sample factor = 1.0 / fade;
			if ( dNch == 1 )
			{
				for ( n = d_start; n < d_end; n++ )
				{
					if ( n > length )
					{
						*foo = 0;
					}
					else
					{
						audio_sample bleh = (audio_sample)(length - n) * factor;
						*foo *= bleh;
					}
				}
				++foo;
			}
			else if ( dNch == 2 )
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
		samples *= dNch;
		if ( samples < played )
		{
			decode_initialize( pTune->getInfo().currentSong, input_flag_playback | ( length ? input_flag_no_looping : 0 ), p_abort );
		}
		pfc::array_t<t_int16> sample_buffer;
		sample_buffer.grow_size( 10240 * dNch );
		eof = false;

		unsigned remain = ( samples - played ) % 32;
		played /= 32;
		samples /= 32;
		m_engine->fastForward( 100 * 32 );

		while ( played < samples )
		{
			p_abort.check();

			unsigned todo = samples - played;
			if ( todo > 10240 * dNch ) todo = 10240 * dNch;
			unsigned done = m_engine->play( sample_buffer.get_ptr(), todo );
			if ( done < todo )
			{
				if ( m_engine->state() != sid2_stopped )
					throw exception_io_data();
				eof = true;
				break;
			}
			played += todo;
		}

		played *= 32;
		m_engine->fastForward( 100 );

		if ( remain )
			played += m_engine->play( sample_buffer.get_ptr(), remain );
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
		throw exception_io_unsupported_format();
	}

	void retag_commit( abort_callback & p_abort )
	{
		throw exception_io_unsupported_format();
	}

	static bool g_is_our_content_type( const char * p_content_type )
	{
		return false;
	}

	static bool g_is_our_path( const char * p_path, const char * p_extension )
	{
		return ! stricmp( p_extension, "sid" ) || ! stricmp( p_extension, "mus" );
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
#ifdef USE_RESID
		COMMAND_HANDLER_EX(IDC_USE_RESIDFP, BN_CLICKED, OnButtonClick)
#endif
		COMMAND_HANDLER_EX(IDC_DB_PATH_SET, BN_CLICKED, OnDBPathSet)
		COMMAND_HANDLER_EX(IDC_DB_PATH_CLEAR, BN_CLICKED, OnDBPathClear)
		COMMAND_HANDLER_EX(IDC_DLENGTH, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_FADE, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_SAMPLERATE, CBN_EDITCHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_SAMPLERATE, CBN_SELCHANGE, OnSelectionChange)
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

	CTrackBarCtrl m_slider_6581, m_slider_8580;
};

void CMyPreferences::update_db_status()
{
	insync( db_lock );
	pfc::string8 status;
	if ( db.loaded() )
	{
		status << db.get_count() << " entries loaded.";
	}
	else
	{
		status << "Not loaded.";
	}
	uSetDlgItemText( m_hWnd, IDC_DB_STATUS, status );
}

unsigned parseTimeStamp(char * & arg);

BOOL CMyPreferences::OnInitDialog(CWindow, LPARAM) {
	SendDlgItemMessage( IDC_INFINITE, BM_SETCHECK, cfg_infinite );
#ifdef USE_RESID
	SendDlgItemMessage( IDC_USE_RESIDFP, BM_SETCHECK, cfg_use_residfp );
#endif
	uSetDlgItemText( m_hWnd, IDC_DLENGTH, pfc::format_time( cfg_deflength ) );
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

	m_slider_6581 = GetDlgItem( IDC_SLIDER_6581 );
	m_slider_6581.SetRangeMin( 0 );
	m_slider_6581.SetRangeMax( 256 );
	m_slider_6581.SetPos( cfg_sid_filter_6581 );
	uSetDlgItemText( m_hWnd, IDC_TEXT_6581, pfc::string_formatter() << pfc::format_float( cfg_sid_filter_6581 / 256., 0, 2 ) );

	m_slider_8580 = GetDlgItem( IDC_SLIDER_8580 );
	m_slider_8580.SetRangeMin( 150 );
	m_slider_8580.SetRangeMax( 22050 );
	m_slider_8580.SetPos( cfg_sid_filter_8580 );
	uSetDlgItemText( m_hWnd, IDC_TEXT_8580, pfc::string_formatter() << cfg_sid_filter_8580 << " Hz" );

	return FALSE;
}

void CMyPreferences::OnEditChange(UINT, int, CWindow) {
	OnChanged();
}

void CMyPreferences::OnSelectionChange(UINT, int, CWindow) {
	OnChanged();
}

void CMyPreferences::OnButtonClick(UINT, int, CWindow) {
	OnChanged();
}

void CMyPreferences::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
{
	if ( pScrollBar.m_hWnd == m_slider_6581.m_hWnd )
	{
		uSetDlgItemText( m_hWnd, IDC_TEXT_6581, pfc::string_formatter() << pfc::format_float( m_slider_6581.GetPos() / 256., 0, 2 ) );
	}
	else if ( pScrollBar.m_hWnd == m_slider_8580.m_hWnd )
	{
		uSetDlgItemText( m_hWnd, IDC_TEXT_8580, pfc::string_formatter() << m_slider_8580.GetPos() << " Hz" );
	}
	OnChanged();
}

void CMyPreferences::OnDBPathSet(UINT, int, CWindow) {
	pfc::string8 path;
	convert_db_path( string_utf8_from_window( m_hWnd, IDC_DB_PATH ), path, true );
	pfc::string8 directory( path );
	directory.truncate( directory.scan_filename() );
	if ( uGetOpenFileName( core_api::get_main_window(), "Text and INI files|*.TXT;*.INI",
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
	SendDlgItemMessage( IDC_INFINITE, BM_SETCHECK, default_cfg_infinite );
	SendDlgItemMessage( IDC_CLOCK_OVERRIDE, CB_SETCURSEL, default_cfg_clock_override );
	SendDlgItemMessage( IDC_SID_OVERRIDE, CB_SETCURSEL, default_cfg_sid_override );
#ifdef USE_RESID
	SendDlgItemMessage( IDC_USE_RESIDFP, BM_SETCHECK, default_cfg_use_residfp );
#endif
	uSetDlgItemText( m_hWnd, IDC_DLENGTH, pfc::format_time( default_cfg_deflength ) );
	uSetDlgItemText( m_hWnd, IDC_DB_PATH, "" );
	SetDlgItemInt( IDC_SAMPLERATE, default_cfg_rate, FALSE );
	SetDlgItemInt( IDC_FADE, default_cfg_fade, FALSE );
	m_slider_6581.SetPos( default_cfg_sid_filter_6581 );
	uSetDlgItemText( m_hWnd, IDC_TEXT_6581, pfc::string_formatter() << pfc::format_float( default_cfg_sid_filter_6581 / 256., 0, 2 ) );
	m_slider_8580.SetPos( default_cfg_sid_filter_8580 );
	uSetDlgItemText( m_hWnd, IDC_TEXT_8580, pfc::string_formatter() << default_cfg_sid_filter_8580 << " Hz" );
	
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
		db.unload();
		db_load( abort_callback_impl() );
	}
	update_db_status();
	{
		string_utf8_from_window foo( m_hWnd, IDC_DLENGTH );
		const char * bar = foo.get_ptr();
		int meh = parseTimeStamp((char *&) bar);
		if (meh) cfg_deflength = meh;
		else uSetDlgItemText( m_hWnd, IDC_DLENGTH, pfc::format_time( cfg_deflength ) );
	}
	cfg_fade = GetDlgItemInt( IDC_FADE, NULL, FALSE );
	cfg_infinite = SendDlgItemMessage( IDC_INFINITE, BM_GETCHECK );
	cfg_clock_override = SendDlgItemMessage( IDC_CLOCK_OVERRIDE, CB_GETCURSEL );
	cfg_sid_override = SendDlgItemMessage( IDC_SID_OVERRIDE, CB_GETCURSEL );
	cfg_sid_filter_6581 = m_slider_6581.GetPos();
	cfg_sid_filter_8580 = m_slider_8580.GetPos();
#ifdef USE_RESID
	cfg_use_residfp = !!SendDlgItemMessage( IDC_USE_RESIDFP, BM_GETCHECK );
#endif
	
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
	if ( !changed && m_slider_6581.GetPos() != cfg_sid_filter_6581 ) changed = true;
	if ( !changed && m_slider_8580.GetPos() != cfg_sid_filter_8580 ) changed = true;
#ifdef USE_RESID
	if ( !changed && !!SendDlgItemMessage( IDC_USE_RESIDFP, BM_GETCHECK ) != cfg_use_residfp ) changed = true;
#endif
	if ( !changed )
	{
		pfc::string8 db_path;
		convert_db_path( string_utf8_from_window( m_hWnd, IDC_DB_PATH ), db_path, false );
		if ( stricmp_utf8( db_path, cfg_db_path ) ) changed = true;
	}
	if ( !changed )
	{
		string_utf8_from_window foo( m_hWnd, IDC_DLENGTH );
		const char * bar = foo.get_ptr();
		int meh = parseTimeStamp((char *&) bar);
		if ( meh && meh != cfg_deflength ) changed = true;
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
	const char * get_name() {return "sidplay";}
	GUID get_guid() {
		// {206017AC-0421-4d37-9B1F-99B9EADE744E}
		static const GUID guid = { 0x206017ac, 0x421, 0x4d37, { 0x9b, 0x1f, 0x99, 0xb9, 0xea, 0xde, 0x74, 0x4e } };
		return guid;
	}
	GUID get_parent_guid() {return guid_input;}
};

DECLARE_FILE_TYPE("SID files", "*.SID;*.MUS");

static input_factory_t           <input_sid>               g_input_sid_factory;
static preferences_page_factory_t<preferences_page_myimpl> g_config_sid_factory;

DECLARE_COMPONENT_VERSION("sidplay2",MYVERSION,"Based on residfp.\n\nLicensed under the GNU GPL, see COPYING.txt.");

VALIDATE_COMPONENT_FILENAME("foo_sid.dll");
