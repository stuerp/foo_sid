
/** $VER: foo_sid.cpp (2025.06.16) **/

#include <pch.h>

#include <memory>

#include <foobar2000.h>
#include <coreDarkMode.h>

#include <atlbase.h>

#include <atlapp.h>

#include <atlcrack.h>
#include <atlctrls.h>
#include <atlmisc.h>

#include <libPPUI/wtl-pp.h>
#include <foobar2000/helpers/atl-misc.h>
#include <foobar2000/helpers/dropdown_helper.h>

#include "SidTuneMod.h"

#include <builders/resid-builder/resid.h>
#include <builders/residfp-builder/residfp.h>

#include <sidplayfp/SidConfig.h>
#include <sidplayfp/SidInfo.h>
#include <sidplayfp/SidTuneInfo.h>
#include <sidplayfp/sidplayfp.h>

#include <utils/SidDatabase.h>
#include <utils/STILview/stil.h>
#include <regex>

#include "resource.h"

#include "roms.hpp"

#pragma region GUIDs
// {7ABA4483-9480-4f9b-ADB5-BA2A495EAB22}
static const GUID guid_cfg_infinite = { 0x7aba4483, 0x9480, 0x4f9b, { 0xad, 0xb5, 0xba, 0x2a, 0x49, 0x5e, 0xab, 0x22 } };
// {6228BE43-CBB1-48E4-9488-0F4242091BB7}
static const GUID guid_cfg_deflength = { 0x6228be43, 0xcbb1, 0x48e4, { 0x94, 0x88, 0xf, 0x42, 0x42, 0x9, 0x1b, 0xb7 } };
// {4143484B-5A86-40f6-BD86-5D7B0A33D5BD}
static const GUID guid_cfg_fade = { 0x4143484b, 0x5a86, 0x40f6, { 0xbd, 0x86, 0x5d, 0x7b, 0xa, 0x33, 0xd5, 0xbd } };
// {9033CEAB-97E6-451f-B3DD-F7A875657E67}
static const GUID guid_cfg_rate = { 0x9033ceab, 0x97e6, 0x451f, { 0xb3, 0xdd, 0xf7, 0xa8, 0x75, 0x65, 0x7e, 0x67 } };
// {DF4F3EE1-E765-4bcb-8757-4B3A27BB0C8C}
static const GUID guid_cfg_db_path = { 0xdf4f3ee1, 0xe765, 0x4bcb, { 0x87, 0x57, 0x4b, 0x3a, 0x27, 0xbb, 0xc, 0x8c } };
// {6ECF3074-FA4D-4717-B0B1-C311C3AEBA80}
static const GUID guid_cfg_history_rate = { 0x6ecf3074, 0xfa4d, 0x4717, { 0xb0, 0xb1, 0xc3, 0x11, 0xc3, 0xae, 0xba, 0x80 } };
// {D0FC838C-9433-409A-93E4-0B89D3B4FB15}
static const GUID guid_cfg_clock_override = { 0xd0fc838c, 0x9433, 0x409a, { 0x93, 0xe4, 0xb, 0x89, 0xd3, 0xb4, 0xfb, 0x15 } };
// {9DF20A98-DFA4-461C-8F67-6009FD934590}
static const GUID guid_cfg_sid_override = { 0x9df20a98, 0xdfa4, 0x461c, { 0x8f, 0x67, 0x60, 0x9, 0xfd, 0x93, 0x45, 0x90 } };
// {8DCA8173-C912-4C79-BBCF-1AC005DCE6D3}
static const GUID guid_cfg_sid_filter_6581 = { 0x8dca8173, 0xc912, 0x4c79, { 0xbb, 0xcf, 0x1a, 0xc0, 0x5, 0xdc, 0xe6, 0xd3 } };
// {58ED05BF-A672-43D1-A87D-BD4D8BFCE7B3}
static const GUID guid_cfg_sid_filter_8580_old = { 0x58ed05bf, 0xa672, 0x43d1, { 0xa8, 0x7d, 0xbd, 0x4d, 0x8b, 0xfc, 0xe7, 0xb3 } };
// {69ABCEF1-F460-45A4-8583-DFF0501ADAC7}
static const GUID guid_cfg_sid_filter_8580 = { 0x69abcef1, 0xf460, 0x45a4, { 0x85, 0x83, 0xdf, 0xf0, 0x50, 0x1a, 0xda, 0xc7 } };
// {C9E01956-6EAB-46E3-AEA1-2A8A3A34382D}
static const GUID guid_cfg_sid_builder = { 0xc9e01956, 0x6eab, 0x46e3, { 0xae, 0xa1, 0x2a, 0x8a, 0x3a, 0x34, 0x38, 0x2d } };
// {0278879D-7C46-41A4-9F42-2786D32B5BBE}
static const GUID guid_cfg_stereo_separation = { 0x278879d, 0x7c46, 0x41a4, { 0x9f, 0x42, 0x27, 0x86, 0xd3, 0x2b, 0x5b, 0xbe } };
#pragma endregion

constexpr int MaxSIDSamples = 10240;

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

static cfg_int CfgLoopForever(guid_cfg_infinite, default_cfg_infinite);
static cfg_int CfgDefaultLengthInMS(guid_cfg_deflength, default_cfg_deflength);
static cfg_int CfgFade(guid_cfg_fade, default_cfg_fade);
static cfg_int CfgSampleRate(guid_cfg_rate, default_cfg_rate);
static cfg_int cfg_clock_override(guid_cfg_clock_override, default_cfg_clock_override);
static cfg_int cfg_sid_override(guid_cfg_sid_override, default_cfg_sid_override);
static cfg_int cfg_sid_filter_6581(guid_cfg_sid_filter_6581, default_cfg_sid_filter_6581);
static cfg_int cfg_sid_filter_8580(guid_cfg_sid_filter_8580, default_cfg_sid_filter_8580);
static cfg_int cfg_sid_filter_8580_old(guid_cfg_sid_filter_8580_old, -1);
static cfg_int cfg_sid_builder(guid_cfg_sid_builder, default_cfg_sid_builder);
static cfg_int CfgStereoSeparation(guid_cfg_stereo_separation, default_cfg_stereo_separation);

#pragma region InitQuit

class InitQuitHandler : public initquit
{
public:
    InitQuitHandler() noexcept { };
    InitQuitHandler(const InitQuitHandler&) = delete;
    InitQuitHandler(const InitQuitHandler&&) = delete;
    InitQuitHandler& operator=(const InitQuitHandler&) = delete;
    InitQuitHandler& operator=(InitQuitHandler&&) = delete;
    virtual ~InitQuitHandler() { };

    void on_init() override
    {
        if (cfg_sid_filter_8580_old > 0)
        {
            if (cfg_sid_filter_8580_old <= 12500)
            {
                cfg_sid_filter_8580 = (cfg_sid_filter_8580_old - 150) * 128 / (12500 - 150);
            }
            else
            if (cfg_sid_filter_8580_old <= 22050)
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

    void on_quit() noexcept override
    {
    }
};

#pragma endregion

/// <summary>
/// Replaces parts of the database path with pseudo variables.
/// </summary>
static void SanitizeDatabasePathName(const char * src, bool fromConfig, pfc::string_base & dst)
{
    dst.reset();

    if (src == nullptr || (src && *src == '\0'))
        return;

    pfc::string ModulePathName;

    ::uGetModuleFileName(NULL, ModulePathName);

    ModulePathName.truncate(ModulePathName.scan_filename() - 1);

    pfc::string ComponentPathName = core_api::get_my_full_path();

    ComponentPathName.truncate(ComponentPathName.scan_filename() - 1);

    pfc::string ProfileURI;

    foobar2000_io::extract_native_path(core_api::get_profile_path(), ProfileURI);

    pfc::string ProfilePathName = ProfileURI;

    if (fromConfig)
    {
        while (*src)
        {
            t_size Index = pfc::string_find_first(src, '<');

            dst.add_string(src, Index);

            if (Index != pfc::infinite_size)
            {
                if (!pfc::strcmp_partial(src + Index, "<player path>"))
                    dst += ModulePathName;
                else
                if (!pfc::strcmp_partial(src + Index, "<profile path>"))
                    dst += ProfilePathName;
                else
                if (!pfc::strcmp_partial(src + Index, "<component path>"))
                    dst += ComponentPathName;

                Index = pfc::string_find_first(src, '>', Index);

                if (Index != pfc::infinite_size)
                    ++Index;
            }

            if (Index == pfc::infinite_size)
                Index = ::strlen(src);

            src += Index;
        }
    }
    else
    {
        while (*src)
        {
            const size_t ModulePathNameIndex = pfc::string_find_first(src, ModulePathName);

            t_size Index = ModulePathNameIndex;

            const size_t ComponentPathNameIndex = pfc::string_find_first(src, ComponentPathName);

            if (ComponentPathNameIndex < Index)
                Index = ComponentPathNameIndex;

            const size_t ProfilePathNameIndex = pfc::string_find_first(src, ProfilePathName);

            if (ProfilePathNameIndex < Index)
                Index = ProfilePathNameIndex;

            dst.add_string(src, Index);

            if (Index != pfc::infinite_size)
            {
                if (Index == ComponentPathNameIndex)
                {
                    dst += "<component path>";
                    src += Index + ComponentPathName.length();
                }
                else
                if (Index == ProfilePathNameIndex)
                {
                    dst += "<profile path>";
                    src += Index + ProfilePathName.length();
                }
                else
                if (Index == ModulePathNameIndex)
                {
                    dst += "<player path>";
                    src += Index + ModulePathName.length();
                }
            }

            if (Index == pfc::infinite_size)
                Index = ::strlen(src);

            src += Index;
        }
    }
}

#pragma region HVSC

static cfg_string _CfgDatabaseFilePath(guid_cfg_db_path, "");
static SidDatabase _HVSC;
static critical_section _HVSCLock;
static bool _IsHVSCLoaded = false;

/// <summary>
/// Loads the HSVC database.
/// </summary>
static void LoadHVSC()
{
    pfc::string FilePath;

    ::SanitizeDatabasePathName(_CfgDatabaseFilePath, true, FilePath);

    if (FilePath.length() == 0)
        return;

    auto FilePathW = pfc::stringcvt::string_wide_from_utf8(FilePath);

    const DWORD Attributes = ::GetFileAttributesW(FilePathW);

    if (Attributes == INVALID_FILE_ATTRIBUTES)
    {
        console::print("Database file \"", FilePath, "\" not found.");

        return;
    }

    _IsHVSCLoaded = _HVSC.open(FilePathW);

    if (!_IsHVSCLoaded)
        console::print("Failed to load HSVC database: ", _HVSC.error());
}

/// <summary>
/// Unloads the database.
/// </summary>
static void UnloadHVSC()
{
    _HVSC.close();

    _IsHVSCLoaded = false;
}

#pragma endregion

#pragma region STIL

static STIL _STIL;
static pfc::string _STILRoot;
static critical_section _STILLock;
static bool _IsSTILLoaded = false;

/// <summary>
/// Loads the STIL database.
/// </summary>
static void LoadSTIL()
{
    pfc::string FilePath;

    ::SanitizeDatabasePathName(_CfgDatabaseFilePath, true, FilePath);

    if (FilePath.length() == 0)
        return;

    // get from path\hvsc\DOCUMENTS\Songlengths.md5 to path\hvsc
    size_t pos = FilePath.lastIndexOf('\\');

    if (pos != std::string::npos && pos > 0)
    {
        pos = FilePath.lastIndexOf('\\', pos - 1);

        if (pos != std::string::npos)
            FilePath = FilePath.subString(0, pos);
    }

    if (FilePath.length() == 0)
        return;

    _STILRoot = FilePath;
    _IsSTILLoaded = _STIL.setBaseDir(_STILRoot.c_str());
}

static std::vector<std::string> StilSplitString(const char * s, std::string regexp)
{
    std::vector<std::string> list;
    std::string str(s);

    std::regex r(regexp);
    std::sregex_iterator rt(str.begin(), str.end(), r), rend;
    std::string final(s);

    for (; rt != rend; ++rt)
    {
        std::string token(rt->prefix());
        list.push_back(token);
        final = rt->suffix();
    }

    list.push_back(final);

    return list;
}

static std::vector<std::string> StilGetMatchGroups(const char * s, std::string regexp)
{
    std::vector<std::string> list;
    std::string str(s);

    std::regex re(regexp, std::regex_constants::icase);
    std::sregex_iterator next(str.begin(), str.end(), re);
    std::sregex_iterator end;

    while (next != end)
    {
        std::smatch match = *next;

        for (size_t group = 1; group < match.size(); group++)
        {
            // Process the group stored in match[group]
            list.push_back(match[group]);
        }
        next++;
    }
    return list;
}

static std::string StilTrimEntry(const char * stilValuePtr)
{
    pfc::string stilValueStr;

    if (stilValuePtr && stilValuePtr[0])
    {

        stilValueStr = stilValuePtr;

        // skip '  TITLE: ' etc.
        if (stilValueStr.length() > 9)
            stilValueStr = stilValueStr.subString(9);

        // erase final '\n'
        if (stilValueStr.endsWith('\n'))
            stilValueStr.truncate_last_char();

        // erase any new lines and leading spaces
        stilValueStr.replace_string("\n         ", " ");
    }

    return stilValueStr.c_str();
}

static std::string StilGetAbsEntry(const char* absPath, unsigned int subSongIndex, STIL::STILField field)
{
    return StilTrimEntry(_STIL.getAbsEntry(absPath, subSongIndex, field));
}

static std::string StilGetAbsGlobalComment(const char* absPath)
{
    return StilTrimEntry(_STIL.getAbsGlobalComment(absPath));
}

static void UnloadStil() noexcept
{
    _IsHVSCLoaded = false;
}

#pragma endregion

#pragma region Input

static const char * _NoExtensions[] = { nullptr };
static const char * _Extensions[] = { ".str", nullptr };

static critical_section g_residfp_lock;

class InputHandler : public input_stubs
{
public:
    InputHandler() noexcept : _SampleRate(0), _BPS(0), _StereoSeparation(0), _TotalSIDSamples(0), _TotalSIDSamplesRendered(0), _NumSIDSamplesToFade(0), _IsFirstBlock(false), _IsEOF(false) { }

    InputHandler(const InputHandler&) = delete;
    InputHandler(const InputHandler&&) = delete;
    InputHandler& operator=(const InputHandler&) = delete;
    InputHandler& operator=(InputHandler&&) = delete;

    virtual ~InputHandler() { };

    #pragma region input_impl

    void open(service_ptr_t<file> file, const char * filePath, t_input_open_reason reason, abort_callback & abortHandler)
    {
        if (reason == input_open_info_write)
            throw exception_tagging_unsupported();

        if (file.is_empty())
            filesystem::g_open(file, filePath, filesystem::open_mode_read, abortHandler);

        _FileStats = file->get_stats(abortHandler);

        _FileStats2 = file->get_stats2_((uint32_t) stats2_all, abortHandler);

        if ((_FileStats2.m_size == 0) || (_FileStats2.m_size > (t_filesize)1 << 30))
            throw exception_io_unsupported_format();

        const char ** Extensions = (pfc::stricmp_ascii(pfc::string_extension(filePath), "mus") == 0) ? _Extensions : _NoExtensions;

        _Tune = std::make_unique<SidTuneMod>(file, std::string(filePath), Extensions);

        if (_Tune->getStatus() == 0)
            throw exception_io_unsupported_format();

        _SampleRate = CfgSampleRate;
    //  _BPS = cfg_BPS;
        _StereoSeparation = CfgStereoSeparation;
    }

    static bool g_is_our_content_type(const char *) noexcept
    {
        return false;
    }

    static bool g_is_our_path(const char *, const char * p_extension) noexcept
    {
        return !_stricmp(p_extension, "sid") || !_stricmp(p_extension, "mus");
    }

    static GUID g_get_guid() noexcept
    {
        return { 0x7fff51a2, 0x5130, 0x4307, { 0x95, 0x22, 0x16, 0xf9, 0x74, 0xd, 0x61, 0xde } };
    }

    static const char * g_get_name() noexcept
    {
        return STR_COMPONENT_NAME;
    }

    static GUID g_get_preferences_guid() noexcept
    {
        return { 0x206017ac, 0x421, 0x4d37, { 0x9b, 0x1f, 0x99, 0xb9, 0xea, 0xde, 0x74, 0x4e } };
    }

    #pragma endregion

    #pragma region input_info_reader

    unsigned get_subsong_count()
    {
        return _Tune->getInfo()->songs();
    }

    t_uint32 get_subsong(unsigned int subSongIndex) noexcept
    {
        return subSongIndex;
    }

    /// <summary>
    /// Retrieves information about specified subsong.
    /// </summary>
    void get_info(t_uint32 subSongIndex, file_info & fileInfo, abort_callback &)
    {
        const SidTuneInfo * TuneInfo = _Tune->getInfo();

        if (TuneInfo == nullptr)
            return;

        {
            unsigned int Length = (unsigned int) CfgDefaultLengthInMS;

            {
                insync(_HVSCLock);

                if (!_IsHVSCLoaded)
                    ::LoadHVSC();
            }

            if (_IsHVSCLoaded)
            {
                char md5[SidTune::MD5_LENGTH + 1];

                _Tune->createMD5New(md5);

                const int LengthFromDatabase = _HVSC.lengthMs(md5, subSongIndex + 1);

                if (LengthFromDatabase > 0)
                    Length = (unsigned int) LengthFromDatabase;
            }

            fileInfo.set_length(double(Length) / 1000.0);
        }

        // General info
        fileInfo.info_set_int("channels", 2);
        fileInfo.info_set("encoding", "synthesized");

        // Specific info
        {
            fileInfo.info_set_int("sid_chip_count", TuneInfo->sidChips());

            const clock_t ClockSpeed = TuneInfo->clockSpeed();

            if (ClockSpeed == SidTuneInfo::CLOCK_NTSC || ClockSpeed == SidTuneInfo::CLOCK_PAL)
                fileInfo.info_set("clock_speed", ClockSpeed == SidTuneInfo::CLOCK_NTSC ? "NTSC" : "PAL");

            fileInfo.info_set("sid_model", TuneInfo->sidModel(0) == SidTuneInfo::SIDMODEL_8580 ? "8580" : "6581");
        }

        // Metadata info
        {
            const unsigned int InfoCount = TuneInfo->numberOfInfoStrings();

            //if ((InfoCount >= 1) && TuneInfo->infoString(0) && TuneInfo->infoString(0)[0])
            //    fileInfo.meta_add(TuneInfo->songs() > 1 ? "album" : "title", pfc::stringcvt::string_utf8_from_ansi(TuneInfo->infoString(0)));

            //if ((InfoCount >= 2) && TuneInfo->infoString(1) && TuneInfo->infoString(1)[0])
            //    fileInfo.meta_add("artist", pfc::stringcvt::string_utf8_from_ansi(TuneInfo->infoString(1)));

            //if ((InfoCount >= 3) && TuneInfo->infoString(2) && TuneInfo->infoString(2)[0])
            //    fileInfo.meta_add("copyright", pfc::stringcvt::string_utf8_from_ansi(TuneInfo->infoString(2)));

            // STIL data
            {
                const int subSongNo = subSongIndex + 1;

                // get values from TuneInfo
                std::string sidTrackNo;
                std::string sidTitle;
                std::string sidArtist;
                std::string sidCopyright;
                std::string sidDate;

                if (TuneInfo->songs() > 1)
                    sidTrackNo += std::to_string(subSongNo);

                if (InfoCount >= 1 && TuneInfo->infoString(0) && TuneInfo->infoString(0)[0])
                    sidTitle = TuneInfo->infoString(0);

                if (InfoCount >= 2 && TuneInfo->infoString(1) && TuneInfo->infoString(1)[0])
                    sidArtist = TuneInfo->infoString(1);

                if (InfoCount >= 3 && TuneInfo->infoString(2) && TuneInfo->infoString(2)[0])
                    sidCopyright = TuneInfo->infoString(2);

                std::string tempSidDate = std::string(TuneInfo->infoString(2), 0, 4);

                if (tempSidDate.length() == 4 && strtol(tempSidDate.c_str(), NULL, 10) > 0)
                    sidDate = tempSidDate;

                // get values from STIL
                std::string stilName;
                std::string stilTitle;
                std::string stilArtist;
                std::string stilGenre;
                std::string stilSongComment;
                std::string stilFileComment;
                std::string stilGlobalComment;

                {
                    insync(_STILLock);

                    if (!_IsSTILLoaded)
                        ::LoadSTIL();
                }

                if (_IsSTILLoaded)
                {
                    pfc::string NativePath;

                    foobar2000_io::extract_native_path(TuneInfo->path(), NativePath);

                    std::string FilePath = NativePath.c_str();

                    // Determine the genre of the file.
                    {
                        std::string Genre = FilePath.c_str();

                        FilePath += std::string(TuneInfo->dataFileName());

                        size_t Index = Genre.find(_STILRoot.c_str());

                        if (std::string::npos != Index)
                        {
                            Genre.replace(Index, _STILRoot.length() + 1, "");

                            Index = Genre.find('\\');

                            if (std::string::npos != Index)
                            {
                                    stilGenre = Genre.substr(0, Index);

                                if (stilGenre.compare("DEMOS") == 0)
                                    stilGenre = "Demos";

                                if (stilGenre.compare("GAMES") == 0)
                                    stilGenre = "Games";

                                if (stilGenre.compare("MUSICIANS") == 0)
                                    stilGenre = "Musicians";

                                stilGenre = "C64 HVSC " + stilGenre;
                            }
                        }
                    }

                    stilName = StilGetAbsEntry(FilePath.c_str(), subSongNo, STIL::name);
                    stilTitle = StilGetAbsEntry(FilePath.c_str(), subSongNo, STIL::title);
                    stilArtist = StilGetAbsEntry(FilePath.c_str(), subSongNo, STIL::artist);
                    stilSongComment = StilGetAbsEntry(FilePath.c_str(), subSongNo, STIL::comment);
                    stilFileComment = StilGetAbsEntry(FilePath.c_str(), 0, STIL::comment);
                    stilGlobalComment = StilGetAbsGlobalComment(FilePath.c_str());
                }

                std::string fooTrackNo(sidTrackNo);
                std::string fooTitle(sidTitle);
                std::string fooAlbum(sidTitle);
                std::string fooArtist(sidArtist);
                std::string fooCopyright(sidCopyright);
                std::string fooDate(sidDate);
                std::string fooGenre(stilGenre);
                std::string fooOrgTitle(stilTitle);
                std::string fooOrgArtist(stilArtist);
                std::string fooSongComment(stilSongComment);
                std::string fooFileComment(stilFileComment);
                std::string fooGlobalComment(stilGlobalComment);

                if (stilName.length() > 0)
                    fooTitle = stilName;
                else
                if (TuneInfo->songs() > 1)
                    fooTitle += " (song " + sidTrackNo + ")";

                if (fooTrackNo.length() > 0)
                    fileInfo.meta_add("tracknumber", pfc::stringcvt::string_utf8_from_ansi(fooTrackNo.c_str()));

                if (fooTitle.length() > 0)
                    fileInfo.meta_add("title", pfc::stringcvt::string_utf8_from_ansi(fooTitle.c_str()));

                if (fooAlbum.length() > 0)
                    fileInfo.meta_add("album", pfc::stringcvt::string_utf8_from_ansi(fooAlbum.c_str()));

                if (fooCopyright.length() > 0)
                    fileInfo.meta_add("copyright", pfc::stringcvt::string_utf8_from_ansi(fooCopyright.c_str()));

                if (fooDate.length() > 0)
                    fileInfo.meta_add("date", pfc::stringcvt::string_utf8_from_ansi(fooDate.c_str()));

                if (fooGenre.length() > 0)
                    fileInfo.meta_add("genre", pfc::stringcvt::string_utf8_from_ansi(fooGenre.c_str()));

                // Split artists in multiple artists tags
                if (fooArtist.length() > 0)
                {
                    std::vector<std::string> artistList = StilSplitString(fooArtist.c_str(), "( *& *)|( *, *)");
                    std::vector<std::string> aliasList = StilGetMatchGroups(artistList[0].c_str(), "(.*?) *(?:<\\?>|\\(.*?\\))");

                    // add all listed artists
                    for (std::vector<std::string>::iterator artist = artistList.begin(); artist != artistList.end(); ++artist)
                    {
                        std::string tempSingleArtist((*artist).c_str());
                        fileInfo.meta_add("artist", pfc::stringcvt::string_utf8_from_ansi(tempSingleArtist.c_str()));
                    }

                    // add album artist
                    std::string tempAlbumArtist(artistList[0]);

                    if (!aliasList.empty())
                        tempAlbumArtist = aliasList[0];

                    fileInfo.meta_add("album artist", pfc::stringcvt::string_utf8_from_ansi(tempAlbumArtist.c_str()));
                }

                // NB: foobar Selection properties only show 1000 chars in main window
                if (fooSongComment.length() > 0)
                    fileInfo.meta_add("stil_song_comment", pfc::stringcvt::string_utf8_from_ansi(fooSongComment.c_str()));

                if (fooFileComment.length() > 0)
                    fileInfo.meta_add("stil_file_comment", pfc::stringcvt::string_utf8_from_ansi(fooFileComment.c_str()));

                if (stilGlobalComment.length() > 0)
                    fileInfo.meta_add("stil_global_comment", pfc::stringcvt::string_utf8_from_ansi(stilGlobalComment.c_str()));

                if (fooOrgArtist.length() > 0)
                    fileInfo.meta_add("stil_original_artist", pfc::stringcvt::string_utf8_from_ansi(fooOrgArtist.c_str()));

                if (fooOrgTitle.length() > 0)
                    fileInfo.meta_add("stil_original_title", pfc::stringcvt::string_utf8_from_ansi(fooOrgTitle.c_str()));
            }

            for (unsigned int i = 3; i < InfoCount; ++i)
            {
                const char * Info = TuneInfo->infoString(i);

                if ((Info != nullptr) && (Info[0] != '\0'))
                    fileInfo.meta_add("info", pfc::stringcvt::string_utf8_from_ansi(Info));
            }

            const unsigned int CommentCount = TuneInfo->numberOfCommentStrings();

            for (unsigned int i = 0; i < CommentCount; ++i)
            {
                const char * Comment = TuneInfo->commentString(i);

                if ((Comment != nullptr) && (Comment[0] != '\0') && (::strcmp(Comment, "--- SAVED WITH SIDPLAY ---") != 0))
                    fileInfo.meta_add("comment", pfc::stringcvt::string_utf8_from_ansi(Comment));
            }
        }
    }

    /// <summary>
    /// Provides means for communication of context specific data with the decoder. The decoder should do nothing and return 0 if it does not recognize the passed arguments.
    /// </summary>
    size_t extended_param(const GUID & type, size_t arg1, void *, size_t)
    {
        if (type == input_params::set_preferred_sample_rate)
        {
            _SampleRate = (int) arg1;

            return 1;
        }

        if (type == input_params::seeking_expensive)
            return 1;


        return 0;
    }

    #pragma endregion

    #pragma region input_info_reader_v2

    t_filestats2 get_stats2(uint32_t, abort_callback &) const noexcept
    {
        return _FileStats2;
    }

    t_filestats get_file_stats(abort_callback &) const noexcept
    {
        return _FileStats;
    }

    #pragma endregion

    #pragma region input_info_writer

    void retag_set_info(t_uint32, const file_info &, abort_callback &)
    {
        throw exception_tagging_unsupported();
    }

    void retag_commit(abort_callback&)
    {
        throw exception_tagging_unsupported();
    }

    void remove_tags(abort_callback&)
    {
        throw exception_tagging_unsupported();
    }

    #pragma endregion

    #pragma region input_decoder

    void decode_initialize(t_uint32 subSongIndex, unsigned flags, abort_callback &)
    {
        _IsFirstBlock = true;

        _Tune->selectSong(subSongIndex + 1);

//      const int RequiredChipCount = _Tune->getInfo()->sidChips();

        uint32_t LengthInMS = (unsigned int) CfgDefaultLengthInMS;

        {
            {
                insync(_HVSCLock);

                if (!_IsHVSCLoaded)
                    ::LoadHVSC();
            }

            if (_IsHVSCLoaded)
            {
                const int LengthFromDatabase = _HVSC.lengthMs(*_Tune);

                if (LengthFromDatabase > 0)
                    LengthInMS = (uint32_t) LengthFromDatabase;
            }
        }

        _Engine = std::make_unique<sidplayfp>();

        _Engine->setRoms(kernel, basic, chargen);

        if (!_Engine->load(_Tune.get()))
            throw exception_io_data(_Engine->error());

        const auto & Info = _Engine->info();

        console::print(STR_COMPONENT_BASENAME " is using ", Info.name(), " ", Info.version(), "(Kernal: ", Info.kernalDesc(), ", BASIC: ", Info.basicDesc(), ", CharGen: ", Info.chargenDesc(), ").");

        _Builder = nullptr;

        switch (cfg_sid_builder)
        {
            case sid_builder_residfp:
            {
                insync(g_residfp_lock);

                std::unique_ptr<ReSIDfpBuilder> NewBuilder = std::make_unique<ReSIDfpBuilder>("ReSIDfp");

                if (NewBuilder)
                {
                    NewBuilder->create((_Engine->info()).maxsids());

                    if (NewBuilder->getStatus())
                    {
                        NewBuilder->filter(true);
                        NewBuilder->filter6581Curve(cfg_sid_filter_6581 / 256.);
                        NewBuilder->filter8580Curve(cfg_sid_filter_8580 / 256.);
                    }

                    if (!NewBuilder->getStatus())
                        throw exception_io_data(NewBuilder->error());

                    _Builder = std::move(NewBuilder);
                }
                break;
            }

            case sid_builder_resid:
            {
                std::unique_ptr<ReSIDBuilder> NewBuilder = std::make_unique<ReSIDBuilder>("ReSID");

                if (NewBuilder)
                {
                    NewBuilder->create((_Engine->info()).maxsids());

                    if (NewBuilder->getStatus())
                        NewBuilder->filter(true);

                    if (!NewBuilder->getStatus())
                        throw exception_io_data(NewBuilder->error());

                    _Builder = std::move(NewBuilder);
                }
                break;
            }

            default:
                throw exception_io_unsupported_feature();
        }

        {
            SidConfig Config = _Engine->config();

            Config.frequency = (uint_least32_t)_SampleRate;
            Config.playback = SidConfig::STEREO;
            Config.sidEmulation = _Builder.get();

            if (cfg_clock_override)
            {
                Config.forceC64Model = true;
                Config.defaultC64Model = (cfg_clock_override == 1) ? SidConfig::PAL : SidConfig::NTSC;
            }

            if (cfg_sid_override)
            {
                Config.forceSidModel = true;
                Config.defaultSidModel = (cfg_sid_override == 1) ? SidConfig::MOS6581 : SidConfig::MOS8580;
            }

            if (!_Engine->config(Config))
                throw exception_io_data(_Engine->error());
        }

        _IsEOF = false;

        if (!CfgLoopForever || (flags & input_flag_no_looping))
        {
            _TotalSIDSamples     = (uint32_t)((__int64) LengthInMS * _SampleRate / 1000) * 2;
            _NumSIDSamplesToFade = (uint32_t)(          CfgFade    * _SampleRate / 1000) * 2;
        }
        else
        {
            _TotalSIDSamples = 0;
            _NumSIDSamplesToFade = 0;
        }

        _TotalSIDSamplesRendered = 0;

        _SampleBuffer.set_count((t_size)MaxSIDSamples * 2);
    }

    bool decode_run(audio_chunk & audioChunk, abort_callback & abortHandler)
    {
        abortHandler.check();

        if (_IsEOF || ((_TotalSIDSamples != 0) && (_TotalSIDSamplesRendered >= _TotalSIDSamples)))
            return false;

        int NumSIDSamplesToRender = _TotalSIDSamples - _TotalSIDSamplesRendered;

        if ((_TotalSIDSamples == 0) || (NumSIDSamplesToRender > MaxSIDSamples * 2))
            NumSIDSamplesToRender = MaxSIDSamples * 2;

        // Render an audio chunk.
        audioChunk.grow_data_size(NumSIDSamplesToRender);
        audioChunk.set_srate(_SampleRate);
        audioChunk.set_channels(2);

        audio_sample * Samples = audioChunk.get_data();

        if (Samples == nullptr)
            return false;

        const int NumSamplesRendered = _Engine->play(_SampleBuffer.get_ptr(), NumSIDSamplesToRender);

        if (NumSamplesRendered < NumSIDSamplesToRender)
        {
            if (_Engine->error())
                throw exception_io_data(_Engine->error());

            _IsEOF = true;
        }

        // Convert the samples from 16-bit signed integer to audio_sample format.
        audio_math::convert_from_int16(_SampleBuffer.get_ptr(), NumSamplesRendered, Samples, (audio_sample) 1.0);

        // Convert to mid-side. Scale side difference according to user setting.
        {
            const audio_sample ScaleFactor = (audio_sample) _StereoSeparation * (audio_sample) 0.005; // percent, pre-scaled by half

            audio_sample * p = Samples;

            for (int i = 0; i < NumSamplesRendered; i += 2)
            {
                const audio_sample mid  = (p[0] + p[1]) * (audio_sample)0.5;
                const audio_sample side = (p[0] - p[1]) * ScaleFactor;

                *p++ = mid + side;
                *p++ = mid - side;
            }
        }

        {
            const unsigned int Head = _TotalSIDSamplesRendered;
            const unsigned int Tail = NumSamplesRendered;

            if ((_TotalSIDSamples != 0) && (Tail + _NumSIDSamplesToFade > _TotalSIDSamples))
            {
                const audio_sample ScaleFactor = (audio_sample) 1.0 / _NumSIDSamplesToFade;

                audio_sample * p = Samples;

                for (uint32_t i = Head; i < Tail; i += 2)
                {
                    if (i > _TotalSIDSamples)
                    {
                        *p++ = (audio_sample)0.0;
                        *p++ = (audio_sample)0.0;
                    }
                    else
                    {
                        const audio_sample bleh = (audio_sample)(_TotalSIDSamples - i) * ScaleFactor;

                        *p++ *= bleh;
                        *p++ *= bleh;
                    }
                }
            }
        }

        audioChunk.set_sample_count(NumSamplesRendered / 2);

        _TotalSIDSamplesRendered += NumSamplesRendered;

        return true;
    }

    void decode_seek(double p_seconds, abort_callback & p_abort)
    {
        _IsFirstBlock = true;

        unsigned samples = unsigned(audio_math::time_to_samples(p_seconds, _SampleRate));

        samples *= 2;

        if (samples < _TotalSIDSamplesRendered)
        {
            decode_initialize(_Tune->getInfo()->currentSong(), input_flag_playback | (_TotalSIDSamples ? input_flag_no_looping : 0), p_abort);
        }

        pfc::array_t<t_int16> sample_buffer;

        sample_buffer.grow_size((t_size)MaxSIDSamples * 2);
        _IsEOF = false;
/*
        unsigned remain = ( samples - played ) % 32;
        played /= 32;
        samples /= 32;
        m_engine->fastForward( 100 * 32 );
*/
        while (_TotalSIDSamplesRendered < samples)
        {
            p_abort.check();

            unsigned todo = samples - _TotalSIDSamplesRendered;

            if (todo > MaxSIDSamples * 2)
                todo = MaxSIDSamples * 2;

            unsigned done;
            {
                done = _Engine->play(sample_buffer.get_ptr(), todo);
            }

            if (done < todo)
            {
                if (_Engine->error())
                    throw exception_io_data(_Engine->error());

                _IsEOF = true;
                break;
            }

            _TotalSIDSamplesRendered += todo;
        }
/*
        played *= 32;
        m_engine->fastForward( 100 );

        if ( remain )
            played += m_engine->play( sample_buffer.get_ptr(), remain );
*/
    }

    bool decode_can_seek() noexcept
    {
        return true;
    }

    bool decode_get_dynamic_info(file_info & fileInfo, double & timestampDelta)
    {
        if (!_IsFirstBlock)
            return false;

        fileInfo.info_set_int("samplerate", _SampleRate);

        timestampDelta = 0.0;
        _IsFirstBlock = false;

        return true;
    }

    #pragma endregion

private:
    int _SampleRate, _BPS, _StereoSeparation;

    uint32_t _TotalSIDSamples;
    uint32_t _TotalSIDSamplesRendered;
    uint32_t _NumSIDSamplesToFade;

    bool _IsFirstBlock;
    bool _IsEOF;

    std::unique_ptr<SidTuneMod> _Tune;
    std::unique_ptr<sidplayfp> _Engine;
    std::unique_ptr<sidbuilder> _Builder;

    pfc::array_t<t_int16> _SampleBuffer;

    t_filestats _FileStats;
    t_filestats2 _FileStats2;
};

#pragma endregion

#pragma region Preferences

const char * parseTime(const char * str, int_least32_t & result); // From "SidDatabase.cpp"

static cfg_dropdown_history cfg_history_rate(guid_cfg_history_rate, 16);

static const unsigned int _SampleRates[] = { 8000, 11025, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000 };

class CMyPreferences : public CDialogImpl<CMyPreferences>, public preferences_page_instance
{
public:
    CMyPreferences(preferences_page_callback::ptr callback) : _PageCallback(callback)
    {
    }
    CMyPreferences(const CMyPreferences&) = delete;
    CMyPreferences(const CMyPreferences&&) = delete;
    CMyPreferences& operator=(const CMyPreferences&) = delete;
    CMyPreferences& operator=(CMyPreferences&&) = delete;
    virtual ~CMyPreferences() { };

    //Note that we don't bother doing anything regarding destruction of our class.
    //The host ensures that our dialog is destroyed first, then the last reference to our preferences_page_instance object is released, causing our object to be deleted.

    //dialog resource ID
    enum
    {
        IDD = IDD_CONFIG
    };

    // preferences_page_instance methods (not all of them - get_wnd() is supplied by preferences_page_impl helpers)
    t_uint32 get_state() override;
    void apply() override;
    void reset() override;

    BEGIN_MSG_MAP(CMyPreferences)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_HANDLER_EX(IDC_INFINITE, BN_CLICKED, OnButtonClick)
        COMMAND_HANDLER_EX(IDC_DB_PATH_SET, BN_CLICKED, OnSetDatabasePath)
        COMMAND_HANDLER_EX(IDC_DB_PATH_CLEAR, BN_CLICKED, OnClearDatabasePath)
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
    void OnSetDatabasePath(UINT, int, CWindow);
    void OnClearDatabasePath(UINT, int, CWindow);
    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);
    bool HasChanged();
    void OnChanged();

    void UpdateDatabaseStatusText() const noexcept;

private:
    const preferences_page_callback::ptr _PageCallback;

    fb2k::CCoreDarkModeHooks _DarkModeHooks;

    CTrackBarCtrl _Slider6581;
    CTrackBarCtrl _Slider8580;
    CTrackBarCtrl _SliderSsep;
};

t_uint32 CMyPreferences::get_state()
{
    t_uint32 state = preferences_state::resettable | preferences_state::dark_mode_supported;

    if (HasChanged())
        state |= preferences_state::changed;

    return state;
}

void CMyPreferences::reset()
{
    SendDlgItemMessage(IDC_INFINITE, BM_SETCHECK, default_cfg_infinite);
    SendDlgItemMessage(IDC_SID_BUILDER, CB_SETCURSEL, default_cfg_sid_builder);
    SendDlgItemMessage(IDC_CLOCK_OVERRIDE, CB_SETCURSEL, default_cfg_clock_override);
    SendDlgItemMessage(IDC_SID_OVERRIDE, CB_SETCURSEL, default_cfg_sid_override);

    ::uSetDlgItemText(m_hWnd, IDC_DLENGTH, pfc::format_time_ex((double) default_cfg_deflength / 1000.0));
    ::uSetDlgItemText(m_hWnd, IDC_DB_PATH, "");

    SetDlgItemInt(IDC_SAMPLERATE, default_cfg_rate, FALSE);
    SetDlgItemInt(IDC_FADE, default_cfg_fade, FALSE);

    _Slider6581.SetPos(default_cfg_sid_filter_6581);

    pfc::string8_fast Text = pfc::format_float((double) default_cfg_sid_filter_6581 / 256., 0, 2);
    ::uSetDlgItemText(m_hWnd, IDC_TEXT_6581, Text);

    _Slider8580.SetPos(default_cfg_sid_filter_8580);

    Text = pfc::format_float((double) default_cfg_sid_filter_8580 / 256., 0, 2);
    ::uSetDlgItemText(m_hWnd, IDC_TEXT_8580, Text);

    _SliderSsep.SetPos(default_cfg_stereo_separation);

    Text = pfc::format_int(default_cfg_stereo_separation);
    Text += "%";
    ::uSetDlgItemText(m_hWnd, IDC_TEXT_SSEP, Text);

    OnChanged();
}

void CMyPreferences::apply()
{
    {
        int SampleRate = GetDlgItemInt(IDC_SAMPLERATE, NULL, FALSE);

        if (SampleRate < 6000)
            SampleRate = 6000;
        else
        if (SampleRate > 192000)
            SampleRate = 192000;

        SetDlgItemInt(IDC_SAMPLERATE, SampleRate, FALSE);

        char temp[16];

        ::_itoa_s(SampleRate, temp, _countof(temp), 10);

        cfg_history_rate.add_item(temp);

        CfgSampleRate = SampleRate;
    }

    {
        pfc::string Text;

        ::uGetDlgItemText(m_hWnd, IDC_DB_PATH, Text);

        pfc::string DatabaseFilePath;

        SanitizeDatabasePathName(Text, false, DatabaseFilePath);

        _CfgDatabaseFilePath = DatabaseFilePath;

        {
            insync(_HVSCLock);

            ::UnloadHVSC();
            ::LoadHVSC();
        }

        {
            insync(_STILLock);

            ::UnloadStil();
            ::LoadSTIL();
        }

        UpdateDatabaseStatusText();
    }

    {
        pfc::string LengthString;

        ::uGetDlgItemText(m_hWnd, IDC_DLENGTH, LengthString);

        int_least32_t Timestamp = 0;

        try
        {
            const char * bar = LengthString.get_ptr();

            ::parseTime(bar, Timestamp);
        }
        catch (...)
        {
        }

        if (Timestamp)
            CfgDefaultLengthInMS = Timestamp;
        else
            ::uSetDlgItemText(m_hWnd, IDC_DLENGTH, pfc::format_time_ex((double) CfgDefaultLengthInMS / 1000.0));
    }

    CfgFade = GetDlgItemInt(IDC_FADE, NULL, FALSE);
    CfgLoopForever = (t_int32)SendDlgItemMessage(IDC_INFINITE, BM_GETCHECK);
    cfg_clock_override = (t_int32)SendDlgItemMessage(IDC_CLOCK_OVERRIDE, CB_GETCURSEL);
    cfg_sid_override = (t_int32)SendDlgItemMessage(IDC_SID_OVERRIDE, CB_GETCURSEL);
    cfg_sid_builder = (t_int32)SendDlgItemMessage(IDC_SID_BUILDER, CB_GETCURSEL);
    cfg_sid_filter_6581 = _Slider6581.GetPos();
    cfg_sid_filter_8580 = _Slider8580.GetPos();
    CfgStereoSeparation = _SliderSsep.GetPos();

    OnChanged();
}

BOOL CMyPreferences::OnInitDialog(CWindow, LPARAM)
{
    SendDlgItemMessage(IDC_INFINITE, BM_SETCHECK, CfgLoopForever);

    ::uSetDlgItemText(m_hWnd, IDC_DLENGTH, pfc::format_time_ex((double) CfgDefaultLengthInMS / 1000.0));
    ::uSetDlgItemText(m_hWnd, IDC_DB_PATH, _CfgDatabaseFilePath);

    UpdateDatabaseStatusText();

    {
        CWindow w;

        {
            char temp[16];

            SetDlgItemInt(IDC_FADE, CfgFade, FALSE);

            ::uSendMessage(GetDlgItem(IDC_FADE), EM_LIMITTEXT, 3, 0);

            for (int n = _countof(_SampleRates); n--;)
            {
                if (_SampleRates[n] != (unsigned int)CfgSampleRate)
                {
                    _itoa_s(_SampleRates[n], temp, _countof(temp), 10);
                    cfg_history_rate.add_item(temp);
                }
            }

            _itoa_s(CfgSampleRate, temp, _countof(temp), 10);

            cfg_history_rate.add_item(temp);

            w = GetDlgItem(IDC_SAMPLERATE);

            cfg_history_rate.setup_dropdown(w);
            ::SendMessage(w, CB_SETCURSEL, 0, 0);
        }

        w = GetDlgItem(IDC_SID_BUILDER);

        ::uSendMessageText(w, CB_ADDSTRING, 0, "ReSID");
        ::uSendMessageText(w, CB_ADDSTRING, 0, "ReSIDfp");
        ::SendMessage(w, CB_SETCURSEL, cfg_sid_builder, 0);

        w = GetDlgItem(IDC_CLOCK_OVERRIDE);

        ::uSendMessageText(w, CB_ADDSTRING, 0, "As input file specifies");
        ::uSendMessageText(w, CB_ADDSTRING, 0, "Force PAL");
        ::uSendMessageText(w, CB_ADDSTRING, 0, "Force NTSC");
        ::SendMessage(w, CB_SETCURSEL, cfg_clock_override, 0);

        w = GetDlgItem(IDC_SID_OVERRIDE);

        ::uSendMessageText(w, CB_ADDSTRING, 0, "As input file specifies");
        ::uSendMessageText(w, CB_ADDSTRING, 0, "Force 6581");
        ::uSendMessageText(w, CB_ADDSTRING, 0, "Force 8580");
        ::SendMessage(w, CB_SETCURSEL, cfg_sid_override, 0);
    }

    pfc::string8_fast temp;

    _Slider6581 = GetDlgItem(IDC_SLIDER_6581);
    _Slider6581.SetRangeMin(0);
    _Slider6581.SetRangeMax(256);
    _Slider6581.SetPos(cfg_sid_filter_6581);

    temp = pfc::format_float(cfg_sid_filter_6581 / 256., 0, 2);

    ::uSetDlgItemText(m_hWnd, IDC_TEXT_6581, temp);

    if (cfg_sid_builder != sid_builder_residfp)
        _Slider6581.EnableWindow(FALSE);

    _Slider8580 = GetDlgItem(IDC_SLIDER_8580);
    _Slider8580.SetRangeMin(0);
    _Slider8580.SetRangeMax(256);
    _Slider8580.SetPos(cfg_sid_filter_8580);

    temp = pfc::format_float(cfg_sid_filter_8580 / 256., 0, 2);

    ::uSetDlgItemText(m_hWnd, IDC_TEXT_8580, temp);

    if (cfg_sid_builder != sid_builder_residfp)
        _Slider8580.EnableWindow(FALSE);

    _SliderSsep = GetDlgItem(IDC_SLIDER_SSEP);
    _SliderSsep.SetRangeMin(0);
    _SliderSsep.SetRangeMax(150);
    _SliderSsep.SetPos(CfgStereoSeparation);

    temp = pfc::format_int(CfgStereoSeparation);
    temp += "%";

    ::uSetDlgItemText(m_hWnd, IDC_TEXT_SSEP, temp);

    _DarkModeHooks.AddDialogWithControls(*this);

    return FALSE;
}

void CMyPreferences::OnEditChange(UINT, int, CWindow)
{
    OnChanged();
}

void CMyPreferences::OnSelectionChange(UINT, int, CWindow wnd)
{
    if (wnd == GetDlgItem(IDC_SID_BUILDER))
    {
        const BOOL Enable = wnd.SendMessage(CB_GETCURSEL) == sid_builder_residfp;

        _Slider6581.EnableWindow(Enable);
        _Slider8580.EnableWindow(Enable);
    }

    OnChanged();
}

void CMyPreferences::OnButtonClick(UINT, int, CWindow)
{
    OnChanged();
}

void CMyPreferences::OnHScroll(UINT, UINT, CScrollBar pScrollBar)
{
    pfc::string8_fast temp;

    if (pScrollBar.m_hWnd == _Slider6581.m_hWnd)
    {
        temp = pfc::format_float(_Slider6581.GetPos() / 256., 0, 2);
        ::uSetDlgItemText(m_hWnd, IDC_TEXT_6581, temp);
    }
    else
    if (pScrollBar.m_hWnd == _Slider8580.m_hWnd)
    {
        temp = pfc::format_float(_Slider8580.GetPos() / 256., 0, 2);
        ::uSetDlgItemText(m_hWnd, IDC_TEXT_8580, temp);
    }
    else
    if (pScrollBar.m_hWnd == _SliderSsep.m_hWnd)
    {
        temp = pfc::format_int(_SliderSsep.GetPos());
        temp += "%";
        ::uSetDlgItemText(m_hWnd, IDC_TEXT_SSEP, temp);
    }

    OnChanged();
}

void CMyPreferences::OnSetDatabasePath(UINT, int, CWindow)
{
    pfc::string Text;

    ::uGetDlgItemText(m_hWnd, IDC_DB_PATH, Text);

    pfc::string DatabaseFilePath;

    ::SanitizeDatabasePathName(Text, true, DatabaseFilePath);

    pfc::string DatabaseDirectoryPathName(DatabaseFilePath);

    DatabaseDirectoryPathName.truncate(DatabaseDirectoryPathName.scan_filename());

    if (::uGetOpenFileName(core_api::get_main_window(), "Song length database|Songlengths.md5", 1, 0, "Choose SidPlay Song-Lengths Database...", DatabaseDirectoryPathName, DatabaseFilePath, false))
    {
        ::SanitizeDatabasePathName(DatabaseFilePath, false, Text);

        ::uSetDlgItemText(m_hWnd, IDC_DB_PATH, Text);

        OnChanged();
    }
}

void CMyPreferences::OnClearDatabasePath(UINT, int, CWindow)
{
    ::uSetDlgItemText(m_hWnd, IDC_DB_PATH, "");
    OnChanged();
}

bool CMyPreferences::HasChanged()
{
    bool IsChanged = false;

    if (!IsChanged && GetDlgItemInt(IDC_SAMPLERATE, NULL, FALSE) != (UINT) CfgSampleRate)
        IsChanged = true;

    if (!IsChanged && GetDlgItemInt(IDC_FADE, NULL, FALSE) != (UINT) CfgFade)
        IsChanged = true;

    if (!IsChanged && SendDlgItemMessage(IDC_INFINITE, BM_GETCHECK) != CfgLoopForever)
        IsChanged = true;

    if (!IsChanged && SendDlgItemMessage(IDC_CLOCK_OVERRIDE, CB_GETCURSEL) != cfg_clock_override)
        IsChanged = true;

    if (!IsChanged && SendDlgItemMessage(IDC_SID_OVERRIDE, CB_GETCURSEL) != cfg_sid_override)
        IsChanged = true;

    if (!IsChanged && SendDlgItemMessage(IDC_SID_BUILDER, CB_GETCURSEL) != cfg_sid_builder)
        IsChanged = true;

    if (!IsChanged && _Slider6581.GetPos() != cfg_sid_filter_6581)
        IsChanged = true;

    if (!IsChanged && _Slider8580.GetPos() != cfg_sid_filter_8580)
        IsChanged = true;

    if (!IsChanged && _SliderSsep.GetPos() != CfgStereoSeparation)
        IsChanged = true;

    if (!IsChanged)
    {
        pfc::string Text;

        ::uGetDlgItemText(m_hWnd, IDC_DB_PATH, Text);

        pfc::string DatabaseFilePath;

        ::SanitizeDatabasePathName(Text, false, DatabaseFilePath);

        IsChanged = (::stricmp_utf8(DatabaseFilePath, _CfgDatabaseFilePath) != 0);
    }

    if (!IsChanged)
    {
        pfc::string Text;

        uGetDlgItemText(m_hWnd, IDC_DLENGTH, Text);

        int_least32_t Timestamp = 0;

        try
        {
            const char * bar = Text.get_ptr();

            std::ignore = ::parseTime(bar, Timestamp);
        }
        catch (...)
        {
        }

        IsChanged = (Timestamp && Timestamp != CfgDefaultLengthInMS);
    }

    return IsChanged;
}

void CMyPreferences::OnChanged()
{
    //tell the host that our state has changed to enable/disable the apply button appropriately.
    _PageCallback->on_state_changed();
}

void CMyPreferences::UpdateDatabaseStatusText() const noexcept
{
    insync(_HVSCLock);

    ::uSetDlgItemText(m_hWnd, IDC_DB_STATUS, _IsHVSCLoaded ? "Database loaded.": "Database not loaded.");
}

#pragma endregion

#pragma region Preferences Page

class PreferencePage : public preferences_page_impl<CMyPreferences>
{
public:
    PreferencePage() noexcept { };
    PreferencePage(const PreferencePage&) = delete;
    PreferencePage(const PreferencePage&&) = delete;
    PreferencePage& operator=(const PreferencePage&) = delete;
    PreferencePage& operator=(PreferencePage&&) = delete;
    virtual ~PreferencePage() { }

    const char * get_name() noexcept override
    {
        return InputHandler::g_get_name();
    }

    GUID get_guid() noexcept override
    {
        return InputHandler::g_get_preferences_guid();
    }

    GUID get_parent_guid() noexcept override
    {
        return guid_input;
    }
};

#pragma endregion

DECLARE_FILE_TYPE("SID files", "*.SID;*.MUS");

static initquit_factory_t<InitQuitHandler> _InitQuitFactory;
static input_factory_t<InputHandler> _InputFactory;
static preferences_page_factory_t<PreferencePage> _PreferencePageFactory;

#include "Patrons.h"

DECLARE_COMPONENT_VERSION
(
    STR_COMPONENT_NAME,
    STR_COMPONENT_VERSION,
    STR_COMPONENT_BASENAME " " STR_COMPONENT_VERSION "\n"
        STR_COMPONENT_COPYRIGHT "\n"
        STR_COMPONENT_COMMENTS "\n"
        "\n"
        STR_COMPONENT_DESCRIPTION "\n"
        "\n"
        "Built with foobar2000 SDK " TOSTRING(FOOBAR2000_SDK_VERSION) "\n"
        "on " __DATE__ " " __TIME__ ".\n"
        "\n"
        "Based on residfp.\n"
        "\n"
        "Licensed under the GNU GPL, see COPYING.txt.\n"
        "\n"
        "kode54's patrons: https://www.patreon.com/kode54\n"
        "\n"
        KODE54_PATRONS
);

VALIDATE_COMPONENT_FILENAME(STR_COMPONENT_FILENAME);
