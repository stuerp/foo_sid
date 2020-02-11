/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "sidendian.h"
#include "SidTuneMod.h"

#include <vector>

#include <sidtune/SidTuneBase.h>

#include <foobar2000.h>

const char ERR_CANT_OPEN_FILE[]      = "SIDTUNE ERROR: Could not open file for binary input";
const char ERR_EMPTY[]               = "SIDTUNE ERROR: No data to load";
const char ERR_NOT_ENOUGH_MEMORY[]   = "SIDTUNE ERROR: Not enough free memory";
const char ERR_CANT_LOAD_FILE[]      = "SIDTUNE ERROR: Could not load input file";

void SidTuneMod::MyLoaderFunc(const char* fileName, std::vector<uint_least8_t>& bufferRef)
{
	service_ptr_t<file> myIn;
	abort_callback_dummy m_abort;

	try
	{
		filesystem::g_open( myIn, fileName, filesystem::open_mode_read, m_abort );
	}
	catch (...)
	{
        throw libsidplayfp::loadError(ERR_CANT_OPEN_FILE);
	}

    std::vector<uint_least8_t> fileBuf;

    {
		const size_t fileLen = myIn->get_size_ex( m_abort );
		if ( fileLen == 0 )
		{
			throw libsidplayfp::loadError(ERR_EMPTY);
		}
        
		fileBuf.resize(fileLen);

		try
		{
			myIn->read_object( &fileBuf[0], fileLen, m_abort );
		}
		catch (...)
		{
			throw libsidplayfp::loadError(ERR_CANT_LOAD_FILE);
		}
    }

    bufferRef.swap(fileBuf);
}

SidTuneMod::SidTuneMod(const char* fileName, const char **fileNameExt, const bool separatorIsSlash)
	: SidTune(fileName, fileNameExt, separatorIsSlash, MyLoaderFunc) { }

static void decode_hex(const char *& in, char & out)
{
	unsigned char byte;
	if (in[0] >= '0' && in[0] <= '9') byte = (in[0] - '0') * 16;
	else if (in[0] >= 'A' && in[0] <= 'F') byte = (in[0] - 'A' + 10) * 16;
	else if (in[0] >= 'a' && in[0] <= 'f') byte = (in[0] - 'a' + 10) * 16;
	else throw exception_io_data();
	if (in[1] >= '0' && in[1] <= '9') byte += in[1] - '0';
	else if (in[1] >= 'A' && in[1] <= 'F') byte += in[1] - 'A' + 10;
	else if (in[1] >= 'a' && in[1] <= 'f') byte += in[1] - 'a' + 10;
	else throw exception_io_data();
	out = byte;
	in += 2;
}

void SidTuneMod::createMD5(hasher_md5_result & digest)
{
	char hash_string[33];
	const char * hash_string_ptr = SidTune::createMD5(hash_string);
	if (hash_string_ptr)
	{
		for (unsigned int i = 0; i < 16; ++i)
		{
			decode_hex(hash_string_ptr, digest.m_data[i]);
		}
	}
}
