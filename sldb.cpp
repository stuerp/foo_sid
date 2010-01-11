#include <foobar2000.h>

#include "SidTuneMod.h"
#include "sldb.h"

#include "MD5/MD5.h"

static int hex_digit(int c)
{
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return -1;
}

bool sldb_sum::set_digest(const char * digest_string)
{
	int i, d1, d2;

	for (i = 0; i < 16; i++)
	{
		if ((d1 = hex_digit(*digest_string++)) == -1) return false;
		if ((d2 = hex_digit(*digest_string++)) == -1) return false;
		digest[i] = d1 * 16 + d2;
	}

	return true;
}

bool sldb_sum::check_digest(const void * song_digest)
{
	return !memcmp(song_digest, digest, 16);
}

void sldb_sum::add_length(unsigned length)
{
	lengths.append_single(length);
}

unsigned sldb_sum::get_length(unsigned index)
{
	if (index >= lengths.get_size()) return 0;
	return *(lengths.get_ptr() + index);
}

static unsigned atou(const char * arg)
{
	unsigned val = 0;
	while (*arg >= '0' && *arg <= '9')
	{
		val *= 10;
		val += *arg++ - '0';
	}
	return val;
}

unsigned parseTimeStamp(char * & arg)
{
    unsigned seconds = 0;
    int  passes    = 2;
    bool gotDigits = false;
    while ( passes-- )
    {
        if ( isdigit(*arg) )
		{
			int t = atou(arg);
			seconds += t;
			gotDigits = true;
		}
        while ( *arg && isdigit(*arg) )
		{
            ++arg;
        }
        if ( *arg && *arg==':' )
        {
            seconds *= 60;
            ++arg;
        }
    }
    
    // Handle -:-- time stamps and old 0:00 entries which
    // need to be rounded up by one second.
    if ( !gotDigits )
        seconds = 0;
    else if ( seconds==0 )
        ++seconds;
    
    return seconds;
}

static void find_next_line(char * & arg)
{
	while (*arg && *arg != 10 && *arg != 13) arg++;
	while (*arg && (*arg == 10 || *arg == 13)) arg++;
}

static unsigned find_end(const char * arg)
{
	unsigned s = 0;
	while (*arg && *arg != 10 && *arg != 13 && *arg != ';')
	{
		arg++;
		s++;
	}
	while (s > 0 && (!*arg || *arg == 9 || *arg == 10 || *arg == 13 || *arg == 32))
	{
		arg--;
		s--;
	}
	if (*arg && *arg != 9 && *arg != 10 && *arg != 13 && *arg != 32) s++;
	return s;
}

static void skip_whitespace(char * & arg)
{
	while (*arg && (*arg == 9 || *arg == 32)) arg++;
}

static void find_whitespace(char * & arg)
{
	while (*arg && *arg != 9 && *arg != 10 && *arg != 13 && *arg != 32) arg++;
}

static void find_value(char * & arg)
{
	skip_whitespace(arg);
	while (*arg && (*arg != '=')) arg++;
	if (*arg)
	{
		arg++;
		skip_whitespace(arg);
	}
}

bool sldb::load(const char * path, abort_callback & p_abort)
{
	insync(sync);

	char * ptr;
	pfc::array_t<t_uint8> buf;

	try
	{
		service_ptr_t<file> r;
		filesystem::g_open(r, path, filesystem::open_mode_read, p_abort);

		t_filesize len64 = r->get_size_ex( p_abort );
		if (len64 > (1 << 30)) throw exception_io_data();

		unsigned len = (unsigned) len64;

		buf.set_size( len + 1 );

		ptr = ( char * ) buf.get_ptr();

		r->read_object( ptr, len, p_abort );

		ptr[ len ] = 0;
	}
	catch ( ... )
	{
		return false;
	}

	bool found = false;

	while (*ptr && !found && !p_abort.is_aborting())
	{
		skip_whitespace(ptr);
		int len = find_end(ptr);
		if (len == 10 && !strnicmp(ptr, "[Database]", len)) found = true;
		ptr += len;
		find_next_line(ptr);
	}

	if (!found) return false;

	sldb_sum * sum;

	while (*ptr && !p_abort.is_aborting())
	{
		skip_whitespace(ptr);

		if (*ptr == '[') break;

		if (*ptr == ';')
		{
			find_next_line(ptr);
			continue;
		}

		sum = new sldb_sum;

		if (!sum->set_digest(ptr))
		{
			delete sum;
			break;
		}

		ptr += 32;

		find_value(ptr);

		if (!*ptr)
		{
			delete sum;
			return false;
		}

		while(*ptr && *ptr != 10 && *ptr != 13 && *ptr != ';')
		{
			sum->add_length(parseTimeStamp(ptr));
			find_whitespace(ptr);
			skip_whitespace(ptr);
		}

		sums.add_item(sum);

		find_next_line(ptr);
	}

	return sums.get_count() > 0 && !p_abort.is_aborting();
}

bool sldb::loaded()
{
	insync(sync);
	return sums.get_count() > 0;
}

void sldb::unload()
{
	insync(sync);
	sums.delete_all();
}

unsigned sldb::get_count()
{
	insync(sync);
	return sums.get_count();
}

unsigned sldb::find(SidTuneMod * tune, unsigned index)
{
	if (!tune->getStatus()) return 0;

	insync(sync);
	if (!sums.get_count()) return 0;

	hasher_md5_result digest;

	tune->createMD5(digest);

	unsigned i, n;

	for (i = 0, n = sums.get_count(); i < n; i++)
	{
		if (sums[i]->check_digest(digest.m_data)) break;
	}

	if (i == n)
	{
		pfc::string8 sum;
		for (i = 0; i < 16; i++)
		{
			sum += pfc::format_int( digest.m_data [i] & 0xFF, 2, 16 );
		}
		console::formatter() << "Unknown hash: " << sum;
		return NULL;
	}

	return sums[i]->get_length(index);
}
