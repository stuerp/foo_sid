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

#include <pch.h>

#include <sidendian.h>

#include "SidTuneMod.h"

#include <map>
#include <mutex>
#include <string>
#include <vector>

#include <sidtune/SidTuneBase.h>

#include <foobar2000.h>

#include <CppCoreCheck/Warnings.h>

#pragma warning(disable: 4100 4625 4626 4710 4711 4738 5045 ALL_CPPCORECHECK_WARNINGS)

const char ERR_CANT_OPEN_FILE[] = "SIDTUNE ERROR: Could not open file for binary input";
const char ERR_EMPTY[] = "SIDTUNE ERROR: No data to load";
const char ERR_CANT_LOAD_FILE[] = "SIDTUNE ERROR: Could not load input file";

struct CacheItem
{
    size_t _ReferenceCount;
    std::vector<uint8_t> _Data;

    CacheItem() noexcept : _ReferenceCount(0)
    {
    }
};

static class FileCache
{
public:
    std::string Add(const std::string& filePath, const file::ptr file)
    {
        std::lock_guard<std::mutex> Guard(_Lock);

        CacheItem & Item = _Items[filePath];

        if (Item._ReferenceCount == 0)
        {
            try
            {
                abort_callback_dummy AbortHandler;

                const t_filesize FileSize = file->get_size_ex(AbortHandler);

                file->seek(0, AbortHandler);

                Item._Data.resize((size_t)FileSize);

                file->read_object(&Item._Data[0], (t_size)FileSize, AbortHandler);

                Item._ReferenceCount++;
            }
            catch (...)
            {
                return filePath;
            }
        }
        else
        {
            Item._ReferenceCount++;
        }

        return filePath;
    }

    void Remove(const std::string& filePath)
    {
        std::lock_guard<std::mutex> Guard(_Lock);

        CacheItem& Item = _Items[filePath];

        if (Item._ReferenceCount <= 1)
        {
            for (auto it = _Items.begin(); it != _Items.end();)
            {
                if (it->first == filePath)
                {
                    it = _Items.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
        else
        {
            --Item._ReferenceCount;
        }
    }

    bool TryGet(const std::string& filePath, std::vector<uint8_t>& data)
    {
        std::lock_guard<std::mutex> Guard(_Lock);

        const CacheItem& Item = _Items[filePath];

        if (Item._ReferenceCount == 0)
            return false;

        data = Item._Data;

        return true;
    }

private:
    std::mutex _Lock;
    std::map<std::string, CacheItem> _Items;
} _FileCache;

void SidTuneMod::MyLoaderFunc(const char * filePath, std::vector<uint8_t>& bufferRef)
{
    std::string FilePath = filePath;

    if (!_FileCache.TryGet(FilePath, bufferRef))
    {
        try
        {
            service_ptr_t<file> File;
            abort_callback_dummy AbortHandler;

            filesystem::g_open(File, filePath, filesystem::open_mode_read, AbortHandler);

            const t_filesize FileSize = File->get_size_ex(AbortHandler);

            bufferRef.resize((size_t)FileSize);

            File->read_object(&bufferRef[0], (t_size)FileSize, AbortHandler);
        }
        catch (...)
        {
            throw libsidplayfp::loadError(ERR_CANT_OPEN_FILE);
        }
    }
    else
    {
        if (!bufferRef.size())
            throw libsidplayfp::loadError(ERR_CANT_OPEN_FILE);
    }
}

SidTuneMod::SidTuneMod(file::ptr file, std::string fileName, const char ** fileNameExt, const bool separatorIsSlash)
    : _fileName(fileName), SidTune(MyLoaderFunc, _FileCache.Add(fileName, file).c_str(), fileNameExt, separatorIsSlash)
{
}

SidTuneMod::~SidTuneMod()
{
    try
    {
        _FileCache.Remove(_fileName);
    }
    catch (...)
    {
    }
}

static unsigned char htoi(const char * src) noexcept
{
    if (src == nullptr)
        return 0;

    unsigned char byte;

    if (src[0] >= '0' && src[0] <= '9')
        byte = (src[0] - '0') << 4;
    else
    if (src[0] >= 'A' && src[0] <= 'F')
        byte = (src[0] - 'A' + 10) << 4;
    else
    if (src[0] >= 'a' && src[0] <= 'f')
        byte = (src[0] - 'a' + 10) << 4;
    else
        return 0;

    if (src[1] >= '0' && src[1] <= '9')
        byte += src[1] - '0';
    else
    if (src[1] >= 'A' && src[1] <= 'F')
        byte += src[1] - 'A' + (unsigned char)10;
    else
    if (src[1] >= 'a' && src[1] <= 'f')
        byte += src[1] - 'a' + (unsigned char)10;
    else
        return 0;

    return byte;
}

void SidTuneMod::createMD5(hasher_md5_result& digest)
{
    char Hash[33];

    const char * HashPtr = SidTune::createMD5(Hash);

    if (HashPtr)
    {
        for (unsigned int i = 0; i < 16; ++i, HashPtr += 2)
            digest.m_data[i] = htoi(HashPtr);
    }
}
