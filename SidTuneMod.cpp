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

#include <sidplay/sidendian.h>
#include "config.h"
#include "SidTuneMod.h"

#include <foobar2000.h>

bool SidTune::loadFile(const char* fileName, Buffer_sidtt<const uint_least8_t>& bufferRef)
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
        info.statusString = SidTune::txt_cantOpenFile;
        return false;
	}

    {
		fileLen = myIn->get_size_ex( m_abort );
#ifdef HAVE_EXCEPTIONS
        if ( !fileBuf.assign(new(std::nothrow) uint_least8_t[fileLen],fileLen) )
#else
        if ( !fileBuf.assign(new uint_least8_t[fileLen],fileLen) )
#endif
        {
            info.statusString = SidTune::txt_notEnoughMemory;
            return false;
        }
		myIn->read_object( (char*) fileBuf.get(), fileLen, m_abort );
        info.statusString = SidTune::txt_noErrors;
    }
    if ( fileLen==0 )
    {
        info.statusString = SidTune::txt_empty;
        return false;
    }

    if ( decompressPP20(fileBuf) < 0 )
        return false;

    bufferRef.assign(fileBuf.xferPtr(),fileBuf.xferLen());
    return true;
}

void SidTuneMod::createMD5(hasher_md5_result & digest)
{
    if (status)
    {   // Include C64 data.
		static_api_ptr_t<hasher_md5> p_md5;
		hasher_md5_state m_state;
        unsigned char tmp[2];
		p_md5->initialize(m_state);
		p_md5->process(m_state, cache.get()+fileOffset,info.c64dataLen);
        // Include INIT and PLAY address.
        endian_little16 (tmp, info.initAddr);
        p_md5->process(m_state, tmp, sizeof(tmp));
        endian_little16 (tmp,info.playAddr);
        p_md5->process(m_state, tmp, sizeof(tmp));
        // Include number of songs.
        endian_little16 (tmp,info.songs);
        p_md5->process(m_state, tmp, sizeof(tmp));
        {   // Include song speed for each song.
            uint_least16_t currentSong = info.currentSong;
            for (uint_least16_t s = 1; s <= info.songs; s++)
            {
                selectSong (s);
                p_md5->process(m_state, &info.songSpeed, sizeof(info.songSpeed));
            }
            // Restore old song
            selectSong (currentSong);
        }
        // Deal with PSID v2NG clock speed flags: Let only NTSC
        // clock speed change the MD5 fingerprint. That way the
        // fingerprint of a PAL-speed sidtune in PSID v1, v2, and
        // PSID v2NG format is the same.
        if (info.clockSpeed == SIDTUNE_CLOCK_NTSC)
            p_md5->process(m_state, &info.clockSpeed, sizeof(info.clockSpeed));
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

