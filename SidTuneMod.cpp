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
#include <string>
#include <map>
#include <mutex>

#include <sidtune/SidTuneBase.h>

#include <foobar2000.h>

const char ERR_CANT_OPEN_FILE[]      = "SIDTUNE ERROR: Could not open file for binary input";
const char ERR_EMPTY[]               = "SIDTUNE ERROR: No data to load";
const char ERR_NOT_ENOUGH_MEMORY[]   = "SIDTUNE ERROR: Not enough free memory";
const char ERR_CANT_LOAD_FILE[]      = "SIDTUNE ERROR: Could not load input file";

struct cache_object {
	size_t refCount;
	std::vector<uint8_t> data;

	cache_object()
	: refCount(0) {
	}
};

static class file_cache {
	std::mutex lock;
	std::map<std::string, cache_object> cache;

public:
	std::string add_path(std::string& path, file::ptr file) {
		std::lock_guard<std::mutex> guard(lock);
		cache_object& obj = cache[path];

		if(obj.refCount == 0) {
			try {
				abort_callback_dummy m_abort;
				t_filesize fileSize = file->get_size_ex(m_abort);
				file->seek(0, m_abort);
				obj.data.resize(fileSize);
				file->read_object(&obj.data[0], fileSize, m_abort);
				obj.refCount++;
			}
			catch (...) {
				return path;
			}
		} else {
			obj.refCount++;
		}

		return path;
	}

	bool try_path(std::string& path, std::vector<uint8_t>& out) {
		std::lock_guard<std::mutex> guard(lock);
		cache_object& obj = cache[path];

		if(obj.refCount > 0) {
			out = obj.data;
			return true;
		}

		return false;
	}

	void remove_path(std::string& path) {
		std::lock_guard<std::mutex> guard(lock);
		cache_object& obj = cache[path];

		if (obj.refCount <= 1) {
			for (auto it = cache.begin(); it != cache.end();) {
				if (it->first == path) {
					it = cache.erase(it);
				} else {
					++it;
				}
			}
		} else {
			--obj.refCount;
		}
	}
} g_file_cache;

void SidTuneMod::MyLoaderFunc(const char* fileName, std::vector<uint8_t>& bufferRef)
{
	if(!g_file_cache.try_path(std::string(fileName), bufferRef)) {
		try {
			service_ptr_t<file> myIn;
			abort_callback_dummy m_abort;
			filesystem::g_open(myIn, fileName, filesystem::open_mode_read, m_abort);
			t_filesize fileSize = myIn->get_size_ex(m_abort);
			bufferRef.resize(fileSize);
			myIn->read_object(&bufferRef[0], fileSize, m_abort);
		} catch(...) {
			throw libsidplayfp::loadError(ERR_CANT_OPEN_FILE);
		}
	} else {
		if(!bufferRef.size())
			throw libsidplayfp::loadError(ERR_CANT_OPEN_FILE);
	}
}

SidTuneMod::SidTuneMod(file::ptr file, const char* fileName, const char **fileNameExt, const bool separatorIsSlash)
: _fileName(fileName), SidTune(MyLoaderFunc, g_file_cache.add_path(_fileName, file).c_str(), fileNameExt, separatorIsSlash) {
}

SidTuneMod::~SidTuneMod() {
	g_file_cache.remove_path(_fileName);
}

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
