#define MYVERSION "1.13"

/*
	changelog

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
#include <../helpers/dropdown_helper.h>

#include "SidTuneMod.h"
#include <sidplay/sidplay2.h>
#include <sidplay/builders/resid.h>

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

static cfg_int cfg_infinite(guid_cfg_infinite,0);
static cfg_int cfg_deflength(guid_cfg_deflength, 180);
static cfg_int cfg_fade(guid_cfg_fade, 200);
static cfg_int cfg_rate(guid_cfg_rate,44100);
//static cfg_int cfg_bps("sid_bps",16);

static cfg_string cfg_db_path(guid_cfg_db_path, "");

static sldb db;

static void db_load(abort_callback & p_abort)
{
	string8 path("file://");
	path += cfg_db_path;
	if (path.length() == 7) return;
	db.load(path, p_abort);
}

class input_sid
{
	int dSrate, dBps, dNch;

	unsigned length, played, fade;

	bool eof;

	mem_block_t<short> sample_buffer;

	SidTuneMod * pTune;
	sidplay2 * pSidplay2;
	ReSIDBuilder * pResid;

	t_filestats m_stats;

public:
	input_sid()
	{
		pTune = NULL;
		pSidplay2 = NULL;
		pResid = NULL;
	}

	~input_sid()
	{
		if (pSidplay2) delete pSidplay2;
		if (pResid) delete pResid;
		if (pTune) delete pTune;
	}

	t_io_result open( service_ptr_t<file> p_file, const char * p_path, t_input_open_reason p_reason, abort_callback & p_abort )
	{
		if ( p_reason == input_open_info_write ) return io_result_error_data;

		t_io_result status;

		if ( p_file.is_empty() )
		{
			status = filesystem::g_open( p_file, p_path, filesystem::open_mode_read, p_abort );
			if ( io_result_failed( status ) ) return status;
		}

		status = p_file->get_stats( m_stats, p_abort );
		if ( io_result_failed( status ) ) return status;
		if ( m_stats.m_size < 0 || m_stats.m_size > 1024 * 1024 ) return io_result_error_data; // ???
		unsigned size = unsigned( m_stats.m_size );

		mem_block_t<BYTE> sid_file;
		mem_block_t<BYTE> str_file;

		if ( ! sid_file.set_size( size ) )
			return io_result_error_out_of_memory;

		status = p_file->read_object( sid_file.get_ptr(), size, p_abort );
		if (io_result_failed(status)) return status;

		{
			string_extension_8 ext( p_path );

			if ( ! stricmp( ext, "MUS" ) )
			{
				string8 filename = p_path;
				const char * start = filename + filename.scan_filename();
				const char * end = start + strlen(start);
				char * ptr = (char*) end-1;
				while(ptr>start && *ptr!='.')
				{
					if (*ptr=='?') end=ptr;
					ptr--;
				}

				if (ptr>=start && *ptr=='.')
				{
					*++ptr = 's';
					*++ptr = 't';
					*++ptr = 'r';

					service_ptr_t<file> m_file2;
					status = filesystem::g_open( m_file2, filename, filesystem::open_mode_read, p_abort );
					if ( status != io_result_error_not_found )
					{
						if ( io_result_failed( status ) ) return status;

						t_filesize size264;
						status = m_file2->get_size( size264, p_abort );
						if ( io_result_failed( status ) ) return status;
						if ( size264 < 0 || size264 > 1024 * 1024 ) return io_result_error_data;
						unsigned size2 = unsigned( size264 );

						if ( ! str_file.set_size( size2 ) )
							return io_result_error_out_of_memory;

						status = m_file2->read_object( str_file.get_ptr(), size2, p_abort );
						if ( io_result_failed( status ) ) return status;
					}
				}
			}
		}

		if ( str_file.get_size() )
			pTune = new SidTuneMod( sid_file.get_ptr(), size, str_file.get_ptr(), str_file.get_size() );
		else
			pTune = new SidTuneMod( sid_file.get_ptr(), size );

		if ( ! ( *pTune ) ) return io_result_error_data;

		dSrate = cfg_rate;
		//dBps = cfg_bps;

		return io_result_success;
	}

	unsigned get_subsong_count()
	{
		return pTune->getInfo().songs;
	}

	t_uint32 get_subsong(unsigned p_index)
	{
		return p_index;
	}

	t_io_result get_info( t_uint32 p_subsong, file_info & p_info, abort_callback & p_abort )
	{
		p_info.info_set_int("samplerate", dSrate);
		p_info.info_set_int("channels", pTune->isStereo() ? 2 : 1);
		p_info.info_set_int("bitspersample", 16 /*dBps*/);

		SidTuneInfo sidinfo;

		pTune->getInfo(sidinfo);

		int i = sidinfo.numberOfInfoStrings;

		if (i >= 1 && sidinfo.infoString[0] && sidinfo.infoString[0][0]) p_info.meta_add(sidinfo.songs > 1 ? "album" : "title", string_utf8_from_ansi(sidinfo.infoString[0]));
		if (i >= 2 && sidinfo.infoString[1] && sidinfo.infoString[1][0]) p_info.meta_add("artist", string_utf8_from_ansi(sidinfo.infoString[1]));
		if (i >= 3 && sidinfo.infoString[2] && sidinfo.infoString[2][0]) p_info.meta_add("copyright", string_utf8_from_ansi(sidinfo.infoString[2]));

		for (int j = 3; j < i; j++)
		{
			if (sidinfo.infoString[j] && sidinfo.infoString[j][0]) p_info.meta_add("info", string_utf8_from_ansi(sidinfo.infoString[j]));
		}

		for (int i = 0, j = sidinfo.numberOfCommentStrings; i < j; i++)
		{
			if (sidinfo.commentString[i] && sidinfo.commentString[i][0] && strcmp(sidinfo.commentString[i], "--- SAVED WITH SIDPLAY ---")) p_info.meta_add("comment", string_utf8_from_ansi(sidinfo.commentString[i]));
		}

		if (sidinfo.clockSpeed) p_info.info_set("clock_speed", sidinfo.clockSpeed == 2 ? "NTSC" : "PAL");
		p_info.info_set("sid_model", sidinfo.sidModel == SIDTUNE_SIDMODEL_8580 ? "8580" : "6581");

		unsigned length = cfg_deflength;

		if ( ! db.loaded() ) db_load( p_abort );

		if ( db.loaded() )
		{
			unsigned len = db.find( pTune, p_subsong );
			if (len) length = len;
		}

		p_info.set_length( double( length ) );

		return io_result_success;
	}

	t_io_result get_file_stats( t_filestats & p_stats,abort_callback & p_abort )
	{
		p_stats = m_stats;
		return io_result_success;
	}

	t_io_result decode_initialize( t_uint32 p_subsong, unsigned p_flags, abort_callback & p_abort )
	{
		pTune->selectSong(p_subsong + 1);

		dNch = pTune->isStereo() ? 2 : 1;

		length = cfg_deflength;

		if ( ! db.loaded() ) db_load( p_abort );

		if ( db.loaded() )
		{
			unsigned len = db.find( pTune, p_subsong );
			if (len) length = len;
		}

		if ( pResid )
		{
			delete pResid;
			pResid = 0;
		}

		if ( pSidplay2 )
		{
			delete pSidplay2;
			pSidplay2 = 0;
		}

		pSidplay2 = new sidplay2;

		pResid = new ReSIDBuilder("buoy");
		pResid->create(pSidplay2->info().maxsids);
		pResid->filter(true);
		pResid->sampling(dSrate);

		sid2_config_t conf;
		conf = pSidplay2->config();
		conf.frequency = dSrate;
		conf.precision = 16 /*dBps*/;
		conf.playback = dNch == 2 ? sid2_stereo : sid2_mono;
		conf.sidEmulation = pResid;
		conf.optimisation = 0;
		pSidplay2->config(conf);
		pSidplay2->load(pTune);

		played = 0;

		eof = false;

		if ( !cfg_infinite || ( p_flags & input_flag_no_looping ) )
		{
			length *= dSrate * dNch * 2 /*(dBps>>3)*/;
			fade = (cfg_fade * dSrate / 1000) * dNch * 2 /*(dBps>>3)*/;
		}
		else
		{
			length = 0;
		}

		return io_result_success;
	}

	t_io_result decode_run(audio_chunk & p_chunk,abort_callback & p_abort)
	{
		if ( eof || ( length && played >= length ) ) return io_result_eof;

		int samples = length - played, written; //(stereo)

		if ( !length || ( samples > 512 * dNch * 2 ) ) samples = 512 * dNch * 2;

		if ( ! sample_buffer.check_size( samples / 2 ) )
			return io_result_error_out_of_memory;

		written = pSidplay2->play( sample_buffer.get_ptr(), samples );

		if ( written < samples )
		{
			if ( pSidplay2->state() != sid2_stopped )
			{
				return io_result_error_generic;
			}
			if ( length ) eof = true;
		}

		unsigned d_start, d_end;
		d_start = played;
		played += written;
		d_end = written;

		if ( length && d_end + fade > length )
		{
			short * foo = ( short * ) sample_buffer;
			unsigned n;
			if ( dNch == 1 )
			{
				for ( n = d_start; n < d_end; n += 2 )
				{
					if ( n > length )
					{
						*foo = 0;
					}
					else
					{
						unsigned bleh = length - n;
						*foo = MulDiv( *foo, int( bleh ), int( fade ) );
					}
				}
				++foo;
			}
			else if ( dNch == 2 )
			{
				for ( n = d_start; n < d_end; n += 4 )
				{
					if ( n > length )
					{
						foo[0] = 0;
						foo[1] = 0;
					}
					else
					{
						unsigned bleh = length - n;
						foo[0] = MulDiv( foo[0], int( bleh ), int( fade ) );
						foo[1] = MulDiv( foo[1], int( bleh ), int( fade ) );
					}
					foo += 2;
				}
			}
		}

		if ( ! p_chunk.set_data_fixedpoint( sample_buffer.get_ptr(), written, dSrate, dNch, 16, audio_chunk::g_guess_channel_config( dNch ) ) )
			return io_result_error_out_of_memory;

		return io_result_success;
	}

	t_io_result decode_seek( double p_seconds, abort_callback & p_abort )
	{
		unsigned samples = unsigned( p_seconds * double( dSrate ) + .5 );
		samples *= 2 * dNch;
		if ( samples < played )
		{
			t_io_result status = decode_initialize( pTune->getInfo().currentSong - 1, input_flag_playback | ( length ? input_flag_no_looping : 0 ), p_abort );
			if ( io_result_failed( status ) ) return status;
		}
		if ( ! sample_buffer.check_size( 1024 * dNch ) )
			return io_result_error_out_of_memory;
		eof = false;
		while ( played < samples && !p_abort.is_aborting() )
		{
			unsigned todo = samples - played;
			if ( todo > 1024 * dNch ) todo = 1024 * dNch;
			unsigned done = pSidplay2->play( sample_buffer.get_ptr(), todo );
			if ( done < todo )
			{
				if ( pSidplay2->state() != sid2_stopped )
					return io_result_error_generic;
				if ( length )
				{
					eof = true;
					break;
				}
			}
			played += todo;
		}
		return p_abort.is_aborting() ? io_result_aborted : io_result_success;
	}

	bool decode_can_seek()
	{
		return true;
	}

	bool decode_get_dynamic_info(file_info & p_out, double & p_timestamp_delta,bool & p_track_change)
	{
		return false;
	}

	void decode_on_idle( abort_callback & p_abort )
	{
	}

	t_io_result retag_set_info( t_uint32 p_subsong, const file_info & p_info, abort_callback & p_abort )
	{
		return io_result_error_data;
	}

	t_io_result retag_commit( abort_callback & p_abort )
	{
		return io_result_error_data;
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

static void update_db_status(HWND wnd)
{
	string8 status;
	if (db.loaded())
	{
		status.add_int(db.get_count());
		status.add_string(" entries loaded.");
	}
	else
	{
		status.add_string("Not loaded.");
	}
	uSetDlgItemText(wnd, IDC_DB_STATUS, status);
}

unsigned parseTimeStamp(char * & arg);

class string_print_time : public string8
{
public:
	string_print_time( unsigned seconds )
	{
		if ( seconds >= 60 * 60 )
		{
			add_int( seconds / ( 60 * 60 ) );
			add_byte( ':' );
			seconds %= 60 * 60;
			if ( ( seconds / 60 ) < 10 ) add_byte( '0' );
		}
		if ( seconds >= 60 )
		{
			add_int( seconds / 60 );
			add_byte( ':' );
			seconds %= 60;
			if ( seconds < 10 ) add_byte( '0' );
		}
		add_int( seconds );
	}
};

static BOOL CALLBACK ConfigProc(HWND wnd,UINT msg,WPARAM wp,LPARAM lp)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		uSendDlgItemMessage(wnd, IDC_INFINITE, BM_SETCHECK, cfg_infinite, 0);
		uSetDlgItemText(wnd, IDC_DLENGTH, string_print_time( cfg_deflength ) );
		uSetDlgItemText(wnd, IDC_DB_PATH, cfg_db_path);
		update_db_status(wnd);
		{
			HWND w;
			{
				char temp[16];
				int n;
				w = GetDlgItem(wnd, IDC_FADE);
				itoa(cfg_fade, temp, 10);
				uSetWindowText(w, temp);
				uSendMessage(w, EM_LIMITTEXT, 3, 0);
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
				cfg_history_rate.setup_dropdown(w = GetDlgItem(wnd,IDC_SAMPLERATE));
				uSendMessage(w, CB_SETCURSEL, 0, 0);
			}
		}
		return 1;
	case WM_COMMAND:
		switch(wp)
		{
		case IDC_INFINITE:
			cfg_infinite = uSendMessage((HWND)lp, BM_GETCHECK, 0, 0);
			break;
		case IDC_DB_PATH_SET:
			{
				string8 path(cfg_db_path);
				if (uGetOpenFileName(core_api::get_main_window(), "Text and INI files|*.TXT;*.INI",
					1, 0, "Choose SidPlay Song-Lengths Database...", 0, path, false))
				{
					uSetDlgItemText(wnd, IDC_DB_PATH, path);
					cfg_db_path = path;
				}
			}
			break;
		case IDC_DB_PATH_CLEAR:
			uSetDlgItemText(wnd, IDC_DB_PATH, "");
			cfg_db_path = "";
			break;
		case IDC_DB_LOAD:
			db.unload();
			db_load(abort_callback_impl());
			update_db_status(wnd);
			break;
		case IDC_DB_UNLOAD:
			db.unload();
			update_db_status(wnd);
			break;
		case (EN_CHANGE<<16)|IDC_DLENGTH:
			{
				string_utf8_from_window foo((HWND)lp);
				const char * bar = foo.get_ptr();
				int meh = parseTimeStamp((char *&) bar);
				if (meh) cfg_deflength = meh;
			}
			break;
		case (EN_KILLFOCUS<<16)|IDC_DLENGTH:
			{
				uSetWindowText((HWND)lp, string_print_time(cfg_deflength));
			}
			break;
		case (EN_CHANGE<<16)|IDC_FADE:
			{
				cfg_fade = atoi(string_utf8_from_window((HWND)lp));
			}
			break;
		case (CBN_KILLFOCUS<<16)|IDC_SAMPLERATE:
			{
				int t = GetDlgItemInt(wnd,IDC_SAMPLERATE,0,0);
				if (t<6000) t=6000;
				else if (t>192000) t=192000;
				cfg_rate = t;
			}
			break;
		}
		break;
	case WM_DESTROY:
		char temp[16];
		itoa(cfg_rate, temp, 10);
		cfg_history_rate.add_item(temp);
		break;
	}
	return 0;
}

class preferences_page_sid : public preferences_page
{
public:
	virtual HWND create(HWND parent)
	{
		return uCreateDialog(IDD_CONFIG,parent,ConfigProc);
	}
	GUID get_guid()
	{
		// {206017AC-0421-4d37-9B1F-99B9EADE744E}
		static const GUID guid = 
		{ 0x206017ac, 0x421, 0x4d37, { 0x9b, 0x1f, 0x99, 0xb9, 0xea, 0xde, 0x74, 0x4e } };
		return guid;
	}
	virtual const char * get_name() {return "sidplay";}
	GUID get_parent_guid() {return guid_input;}

	bool reset_query() {return true;}
	void reset()
	{
		cfg_infinite = 0;
		cfg_deflength = 180;
		cfg_fade = 200;;
		cfg_rate = 44100;
		// cfg_bps = 16;

		cfg_db_path = "";
		db.unload();
	}
};

DECLARE_FILE_TYPE("SID files", "*.SID;*.MUS");

static input_factory_t           <input_sid>            g_input_sid_factory;
static preferences_page_factory_t<preferences_page_sid> g_config_sid_factory;

DECLARE_COMPONENT_VERSION("sidplay2",MYVERSION,"Based on libsidplay-2.1.1 and ReSID 0.16-p2.\n\nLicensed under the GNU GPL, see COPYING.txt.");
