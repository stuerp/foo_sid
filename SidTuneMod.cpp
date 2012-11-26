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

#include <sidplayfp/sidendian.h>
#include "config.h"
#include "SidTuneMod.h"

#include <sidplayfp/sidtune/SidTuneBase.h>
#include <sidplayfp/sidtune/SidTuneInfoImpl.h>

#include <foobar2000.h>

const char ERR_CANT_OPEN_FILE[]      = "SIDTUNE ERROR: Could not open file for binary input";
const char ERR_EMPTY[]               = "SIDTUNE ERROR: No data to load";
const char ERR_NOT_ENOUGH_MEMORY[]   = "SIDTUNE ERROR: Not enough free memory";
const char ERR_CANT_LOAD_FILE[]      = "SIDTUNE ERROR: Could not load input file";

void SidTuneBase::loadFile(const char* fileName, Buffer_sidtt<const uint_least8_t>& bufferRef)
{
    Buffer_sidtt<const uint_least8_t> fileBuf;
    uint_least32_t fileLen = 0;

	service_ptr_t<file> myIn;
	abort_callback_impl m_abort;

	try
	{
		filesystem::g_open( myIn, fileName, filesystem::open_mode_read, m_abort );
	}
	catch (...)
	{
        throw loadError(ERR_CANT_OPEN_FILE);
	}

    {
		fileLen = myIn->get_size_ex( m_abort );
		if ( fileLen == 0 )
		{
			throw loadError(ERR_EMPTY);
		}
#ifdef HAVE_EXCEPTIONS
        if ( !fileBuf.assign(new(std::nothrow) uint_least8_t[fileLen],fileLen) )
#else
        if ( !fileBuf.assign(new uint_least8_t[fileLen],fileLen) )
#endif
        {
            throw loadError(ERR_NOT_ENOUGH_MEMORY);
        }
		try
		{
			myIn->read_object( (char*) fileBuf.get(), fileLen, m_abort );
		}
		catch (...)
		{
			throw loadError(ERR_CANT_LOAD_FILE);
		}
    }

    bufferRef.assign(fileBuf.xferPtr(),fileBuf.xferLen());
}

void SidTuneMod::createMD5(hasher_md5_result & digest)
{
    if (getStatus())
    {   // Include C64 data.
		static_api_ptr_t<hasher_md5> p_md5;
		hasher_md5_state m_state;
        unsigned char tmp[2];
		p_md5->initialize(m_state);
		p_md5->process(m_state, tune->cache.get()+tune->fileOffset,tune->info->m_c64dataLen);
        // Include INIT and PLAY address.
        endian_little16 (tmp, tune->info->m_initAddr);
        p_md5->process(m_state, tmp, sizeof(tmp));
        endian_little16 (tmp, tune->info->m_playAddr);
        p_md5->process(m_state, tmp, sizeof(tmp));
        // Include number of songs.
        endian_little16 (tmp, tune->info->songs());
        p_md5->process(m_state, tmp, sizeof(tmp));
        {   // Include song speed for each song.
            uint_least16_t currentSong = tune->info->currentSong();
            for (uint_least16_t s = 1; s <= tune->info->songs(); s++)
            {
                selectSong (s);
				uint_least8_t speed = (uint_least8_t) tune->info->songSpeed();
                p_md5->process(m_state, &speed, sizeof(speed));
            }
            // Restore old song
            selectSong (currentSong);
        }
        // Deal with PSID v2NG clock speed flags: Let only NTSC
        // clock speed change the MD5 fingerprint. That way the
        // fingerprint of a PAL-speed sidtune in PSID v1, v2, and
        // PSID v2NG format is the same.
		if (tune->info->m_clockSpeed == SidTuneInfo::CLOCK_NTSC)
            p_md5->process(m_state, &tune->info->m_clockSpeed, sizeof(tune->info->m_clockSpeed));
        // NB! If the fingerprint is used as an index into a
        // song-lengths database or cache, modify above code to
        // allow for PSID v2NG files which have clock speed set to
        // SIDTUNE_CLOCK_ANY. If the SID player program fully
        // supports the SIDTUNE_CLOCK_ANY setting, a sidtune could
        // either create two different fingerprints depending on
        // the clock speed chosen by the player, or there could be
        // two different values stored in the database/cache.

		digest = p_md5->get_result(m_state);
    }
}

