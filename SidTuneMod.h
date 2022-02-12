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

#ifndef SIDTUNEMOD_H
#define SIDTUNEMOD_H

#include <sidplayfp/SidTune.h>

#include <foobar2000.h>

#include <vector>

class SID_EXTERN SidTuneMod : public SidTune
{
	std::string _fileName;

	static void MyLoaderFunc(const char* fileName, std::vector<uint8_t>& bufferRef);

public:  // --------------------------------------------------------- public

	SidTuneMod(file::ptr inFile, const char* fileName, const char **fileNameExt = 0, const bool separatorIsSlash = false);
	~SidTuneMod();

	void createMD5(hasher_md5_result&);
};

#endif  /* SIDTUNEMOD_H */
