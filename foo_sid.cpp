
/** $VER: foo_sid.cpp (2026.03.04) **/

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

#include <builders/residfp-builder/residfp.h>
#include <builders/sidlite-builder/sidlite.h>

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

/* Quality */

static const GUID CfgCoreGUIDOld            = { 0xc9e01956, 0x6eab, 0x46e3, { 0xae, 0xa1, 0x2a, 0x8a, 0x3a, 0x34, 0x38, 0x2d } };
static const GUID CfgCoreGUID               = { 0x2d465c6e, 0xcf78, 0x4f80, { 0x8d, 0x30, 0x46, 0x22, 0x12, 0xe4, 0x27, 0x2f } };
static const GUID CfgSampleRateGUID         = { 0x9033ceab, 0x97e6, 0x451f, { 0xb3, 0xdd, 0xf7, 0xa8, 0x75, 0x65, 0x7e, 0x67 } };
static const GUID CfgStereoSeparationGUID   = { 0x0278879d, 0x7c46, 0x41a4, { 0x9f, 0x42, 0x27, 0x86, 0xd3, 0x2b, 0x5b, 0xbe } };

/* Length */

static const GUID CfgLoopForeverGUID        = { 0x7aba4483, 0x9480, 0x4f9b, { 0xad, 0xb5, 0xba, 0x2a, 0x49, 0x5e, 0xab, 0x22 } };
static const GUID CfgDefaultLengthInMSGUID  = { 0x6228be43, 0xcbb1, 0x48e4, { 0x94, 0x88, 0x0f, 0x42, 0x42, 0x09, 0x1b, 0xb7 } };
static const GUID CfgFadeLengthGUID         = { 0x4143484b, 0x5a86, 0x40f6, { 0xbd, 0x86, 0x5d, 0x7b, 0x0a, 0x33, 0xd5, 0xbd } };
static const GUID CfgDatabasePathGUID       = { 0xdf4f3ee1, 0xe765, 0x4bcb, { 0x87, 0x57, 0x4b, 0x3a, 0x27, 0xbb, 0x0c, 0x8c } };
static const GUID CfgHistoryRateGUID        = { 0x6ecf3074, 0xfa4d, 0x4717, { 0xb0, 0xb1, 0xc3, 0x11, 0xc3, 0xae, 0xba, 0x80 } };

/* Overrides */

static const GUID CfgClockOverrideGUID      = { 0xd0fc838c, 0x9433, 0x409a, { 0x93, 0xe4, 0x0b, 0x89, 0xd3, 0xb4, 0xfb, 0x15 } };
static const GUID CfgModelOverrideGUID      = { 0x9df20a98, 0xdfa4, 0x461c, { 0x8f, 0x67, 0x60, 0x09, 0xfd, 0x93, 0x45, 0x90 } };

static const GUID CfgFilter6581CurveGUID    = { 0x8dca8173, 0xc912, 0x4c79, { 0xbb, 0xcf, 0x1a, 0xc0, 0x05, 0xdc, 0xe6, 0xd3 } };
static const GUID CfgFilter6581RangeGUID    = { 0xb77131d0, 0x4cab, 0x434a, { 0xb5, 0x02, 0xb4, 0x15, 0xc2, 0x64, 0xe2, 0xd1 } };

static const GUID CfgFilter8580CurveGUIDOld = { 0x58ed05bf, 0xa672, 0x43d1, { 0xa8, 0x7d, 0xbd, 0x4d, 0x8b, 0xfc, 0xe7, 0xb3 } };
static const GUID CfgFilter8580CurveGUID    = { 0x69abcef1, 0xf460, 0x45a4, { 0x85, 0x83, 0xdf, 0xf0, 0x50, 0x1a, 0xda, 0xc7 } };

#pragma endregion

constexpr int MaxSIDSamples = 10240;

enum
{
    CoreReSIDfp = 0,
    CoreSIDLite = 1,
};

enum
{
    CfgSampleRateDefault = 44100,
    CfgCoreDefault = CoreReSIDfp,
    CfgStereoSeparationDefault = 50,

    CfgLoopForeverDefault = 0,
    CfgDefaultLengthInMSDefault = 180000,
    CfgFadeLengthDefault = 200,

    CfgClockOverrideDefault = 0,
    CfgModelOverrideDefault = 0,

    CfgFilter6581CurveDefault = 128,
    CfgFilter6581RangeDefault = 128,

    CfgFilter8580CurveDefault = 128
};

static cfg_int CfgSampleRate        (CfgSampleRateGUID,         CfgSampleRateDefault);
static cfg_int CfgCoreOld           (CfgCoreGUIDOld,            -1);
static cfg_int CfgCore              (CfgCoreGUID,               CfgCoreDefault);
static cfg_int CfgStereoSeparation  (CfgStereoSeparationGUID,   CfgStereoSeparationDefault);

static cfg_int CfgLoopForever       (CfgLoopForeverGUID,        CfgLoopForeverDefault);
static cfg_int CfgDefaultLengthInMS (CfgDefaultLengthInMSGUID,  CfgDefaultLengthInMSDefault);
static cfg_int CfgFadeLength        (CfgFadeLengthGUID,         CfgFadeLengthDefault);

static cfg_int CfgClockOverride     (CfgClockOverrideGUID,      CfgClockOverrideDefault);
static cfg_int CfgModelOverride     (CfgModelOverrideGUID,      CfgModelOverrideDefault);

static cfg_int CfgFilter8580CurveOld(CfgFilter8580CurveGUIDOld, -1);
static cfg_int CfgFilter6581Curve   (CfgFilter6581CurveGUID,    CfgFilter6581CurveDefault);
static cfg_int CfgFilter6581Range   (CfgFilter6581RangeGUID,    CfgFilter6581RangeDefault);

static cfg_int CfgFilter8580Curve   (CfgFilter8580CurveGUID,    CfgFilter8580CurveDefault);

#pragma region InitQuit

class InitQuitHandler : public initquit
{
public:
    InitQuitHandler() noexcept { };

    InitQuitHandler(const InitQuitHandler&) = delete;
    InitQuitHandler(const InitQuitHandler&&) = delete;
    InitQuitHandler& operator=(const InitQuitHandler&) = delete;
    InitQuitHandler& operator=(InitQuitHandler&&) = delete;

    virtual ~InitQuitHandler() noexcept { };

    void on_init() override
    {
        if (CfgFilter8580CurveOld > 0)
        {
            if (CfgFilter8580CurveOld <= 12500)
            {
                CfgFilter8580Curve = (CfgFilter8580CurveOld - 150) * 128 / (12500 - 150);
            }
            else
            if (CfgFilter8580CurveOld <= 22050)
            {
                CfgFilter8580Curve = 128 + ((CfgFilter8580CurveOld - 12500) / (22050 - 12500));
            }
            else
            {
                CfgFilter8580Curve = 128;
            }

            CfgFilter8580CurveOld = -1;
        }

        if (CfgCoreOld > 0)
        {
            CfgCore = CfgCoreDefault;

            CfgCoreOld = -1;
        }
    }

    void on_quit() noexcept override
    {
    }
};

#pragma endregion

#pragma region HVSC

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

    pfc::string ProfilePathName;

    foobar2000_io::extract_native_path(core_api::get_profile_path(), ProfilePathName);

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

static cfg_string _CfgDatabaseFilePath(CfgDatabasePathGUID, "");
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

static std::string StilGetAbsEntry(const char* absPath, uint32_t subSongIndex, STIL::STILField field)
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

static critical_section _ReSIDfpLock;

class InputHandler : public input_stubs
{
public:
    InputHandler() noexcept : _SampleRate(), _BPS(), _StereoSeparation(), _TotalSIDSamples(), _TotalSIDSamplesRendered(), _NumSIDSamplesToFade(), _IsFirstChunk(false), _IsEOF(false) { }

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
            throw exception_io_unsupported_format(_Tune->statusString());

        _SampleRate       = CfgSampleRate;
        _StereoSeparation = CfgStereoSeparation;
    }

    static bool g_is_our_content_type(const char *) noexcept
    {
        return false;
    }

    static bool g_is_our_path(const char *, const char * p_extension) noexcept
    {
        return !::_stricmp(p_extension, "sid") || !::_stricmp(p_extension, "psid") || !::_stricmp(p_extension, "rsid") || !::_stricmp(p_extension, "mus");
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
            uint32_t Length = (uint32_t) CfgDefaultLengthInMS;

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
                    Length = (uint32_t) LengthFromDatabase;
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
            const uint32_t InfoCount = TuneInfo->numberOfInfoStrings();

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

                if ((tempSidDate.length() == 4) && (::strtol(tempSidDate.c_str(), nullptr, 10) > 0))
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

                    if (foobar2000_io::extract_native_path(TuneInfo->path(), NativePath))
                    {
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

                        stilName          = StilGetAbsEntry(FilePath.c_str(), subSongNo, STIL::name);
                        stilTitle         = StilGetAbsEntry(FilePath.c_str(), subSongNo, STIL::title);
                        stilArtist        = StilGetAbsEntry(FilePath.c_str(), subSongNo, STIL::artist);
                        stilSongComment   = StilGetAbsEntry(FilePath.c_str(), subSongNo, STIL::comment);
                        stilFileComment   = StilGetAbsEntry(FilePath.c_str(), 0, STIL::comment);
                        stilGlobalComment = StilGetAbsGlobalComment(FilePath.c_str());
                    }
                }

                std::string fooTrackNo(sidTrackNo);
                std::string fooTitle(sidTitle);
                std::string fooAlbum(sidTitle);
                std::string fooArtist(sidArtist);
                std::string fooCopyright(sidCopyright);
                std::string fooDate(sidDate);

                std::string fooOrgTitle(stilTitle);
                std::string fooOrgArtist(stilArtist);
                std::string fooSongComment(stilSongComment);
                std::string fooFileComment(stilFileComment);
                std::string fooGlobalComment(stilGlobalComment);

                if (!stilName.empty())
                    fooTitle = stilName;
                else
                if (TuneInfo->songs() > 1)
                    fooTitle += " (song " + sidTrackNo + ")";

                if (!fooTrackNo.empty())
                    fileInfo.meta_add("tracknumber", pfc::stringcvt::string_utf8_from_ansi(fooTrackNo.c_str()));

                if (!fooTitle.empty())
                    fileInfo.meta_add("title", pfc::stringcvt::string_utf8_from_ansi(fooTitle.c_str()));

                if (!fooAlbum.empty())
                    fileInfo.meta_add("album", pfc::stringcvt::string_utf8_from_ansi(fooAlbum.c_str()));

                if (!fooCopyright.empty())
                    fileInfo.meta_add("copyright", pfc::stringcvt::string_utf8_from_ansi(fooCopyright.c_str()));

                if (!fooDate.empty())
                    fileInfo.meta_add("date", pfc::stringcvt::string_utf8_from_ansi(fooDate.c_str()));

                // Split artists in multiple artists tags
                if (!fooArtist.empty())
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
                if (!fooSongComment.empty())
                    fileInfo.meta_add("stil_song_comment", pfc::stringcvt::string_utf8_from_ansi(fooSongComment.c_str()));

                if (!fooFileComment.empty())
                    fileInfo.meta_add("stil_file_comment", pfc::stringcvt::string_utf8_from_ansi(fooFileComment.c_str()));

                if (!stilGlobalComment.empty())
                    fileInfo.meta_add("stil_global_comment", pfc::stringcvt::string_utf8_from_ansi(stilGlobalComment.c_str()));

                if (!fooOrgArtist.empty())
                    fileInfo.meta_add("stil_original_artist", pfc::stringcvt::string_utf8_from_ansi(fooOrgArtist.c_str()));

                if (!fooOrgTitle.empty())
                    fileInfo.meta_add("stil_original_title", pfc::stringcvt::string_utf8_from_ansi(fooOrgTitle.c_str()));
            }

            for (uint32_t i = 3; i < InfoCount; ++i)
            {
                const char * Info = TuneInfo->infoString(i);

                if ((Info != nullptr) && (Info[0] != '\0'))
                    fileInfo.meta_add("info", pfc::stringcvt::string_utf8_from_ansi(Info));
            }

            const uint32_t CommentCount = TuneInfo->numberOfCommentStrings();

            for (uint32_t i = 0; i < CommentCount; ++i)
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
        _IsFirstChunk = true;

        _Tune->selectSong(subSongIndex + 1);

//      const int RequiredChipCount = _Tune->getInfo()->sidChips();

        uint32_t LengthInMS = (uint32_t) CfgDefaultLengthInMS;

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

        {
            _Engine = std::make_unique<sidplayfp>();

            _Engine->setRoms(kernel, basic, chargen);

            if (!_Engine->load(_Tune.get()))
                throw exception_io_data(_Engine->error());
        }

        {
            _Builder = nullptr;

            switch (CfgCore)
            {
                case CoreReSIDfp:
                {
                    insync(_ReSIDfpLock);

                    std::unique_ptr<ReSIDfpBuilder> NewBuilder(new ReSIDfpBuilder("ReSIDfp"));

                    if (NewBuilder)
                    {
                        NewBuilder->filter6581Curve(CfgFilter6581Curve / 256.);
                        NewBuilder->filter6581Range(CfgFilter6581Range / 256.);

                        NewBuilder->filter8580Curve(CfgFilter8580Curve / 256.);

                        _Builder = std::move(NewBuilder);
                    }
                    break;
                }

                case CoreSIDLite:
                {
                    std::unique_ptr<SIDLiteBuilder> NewBuilder(new SIDLiteBuilder("SIDLite"));

                    if (NewBuilder)
                        _Builder = std::move(NewBuilder);
                    break;
                }

                default:
                    throw exception_io_unsupported_feature("Unknown core");
            }
        }

        {
            auto Config = _Engine->config();

            Config.frequency      = (uint_least32_t) _SampleRate;
            Config.samplingMethod = SidConfig::INTERPOLATE;
            Config.playback       = SidConfig::STEREO;
            Config.sidEmulation   = _Builder.get();

            if (CfgClockOverride)
            {
                Config.forceC64Model = true;
                Config.defaultC64Model = (CfgClockOverride == 1) ? SidConfig::PAL : SidConfig::NTSC;
            }

            if (CfgModelOverride)
            {
                Config.forceSidModel = true;
                Config.defaultSidModel = (CfgModelOverride == 1) ? SidConfig::MOS6581 : SidConfig::MOS8580;
            }

            if (!_Engine->config(Config))
                throw exception_io_data(_Engine->error());
        }

        const auto & Info = _Engine->info();

        console::print(STR_COMPONENT_BASENAME " is using ", Info.name(), " ", Info.version(), " (Kernal: ", Info.kernalDesc(), ", BASIC: ", Info.basicDesc(), ", CharGen: ", Info.chargenDesc(), ").");

        _Engine->initMixer(true);

        _IsEOF = false;

        if (!CfgLoopForever || (flags & input_flag_no_looping))
        {
            _TotalSIDSamples     = (uint32_t)((__int64) LengthInMS * _SampleRate / 1000) * 2;
            _NumSIDSamplesToFade = (uint32_t)(          CfgFadeLength    * _SampleRate / 1000) * 2;
        }
        else
        {
            _TotalSIDSamples = 0;
            _NumSIDSamplesToFade = 0;
        }

        _TotalSIDSamplesRendered = 0;

        _SampleBuffer.set_count((t_size) MaxSIDSamples * 2);
    }

    bool decode_run(audio_chunk & audioChunk, abort_callback & abortHandler)
    {
        abortHandler.check();

        if (_IsEOF || ((_TotalSIDSamples != 0) && (_TotalSIDSamplesRendered >= _TotalSIDSamples)))
            return false;

        int NumSamplesToRender = _TotalSIDSamples - _TotalSIDSamplesRendered;

        if ((_TotalSIDSamples == 0) || (NumSamplesToRender > MaxSIDSamples * 2))
            NumSamplesToRender = MaxSIDSamples * 2;

        // Render an audio chunk.
        audioChunk.grow_data_size(NumSamplesToRender);
        audioChunk.set_srate(_SampleRate);
        audioChunk.set_channels(2);

        audio_sample * Samples = audioChunk.get_data();

        if (Samples == nullptr)
            return false;

        const int NumSIDSamplesRendered = _Engine->play(5000);

        if (NumSIDSamplesRendered < 0)
        {
            if (_Engine->error())
                throw exception_io_data(_Engine->error());

            _IsEOF = true;
        }

        unsigned int NumSamplesMixed = _Engine->mix(_SampleBuffer.get_ptr(), NumSIDSamplesRendered);

        // Convert the samples from 16-bit signed integer to audio_sample format.
        audio_math::convert_from_int16(_SampleBuffer.get_ptr(), NumSamplesMixed, Samples, (audio_sample) 1.0);

        // Convert to mid/side. Scale side difference according to user setting.
        {
            const audio_sample ScaleFactor = (audio_sample) _StereoSeparation * (audio_sample) 0.005; // percent, pre-scaled by half

            audio_sample * p = Samples;

            for (unsigned int i = 0; i < NumSamplesMixed; i += 2)
            {
                const audio_sample Mid  = (p[0] + p[1]) * (audio_sample) 0.5;
                const audio_sample Side = (p[0] - p[1]) * ScaleFactor;

                *p++ = Mid + Side;
                *p++ = Mid - Side;
            }
        }

        // Fade the samples when getting near the end of the song.
        {
            const uint32_t Head = _TotalSIDSamplesRendered;
            const uint32_t Tail = NumSamplesMixed;

            if ((_TotalSIDSamples != 0) && (Tail + _NumSIDSamplesToFade > _TotalSIDSamples))
            {
                const audio_sample ScaleFactor = (audio_sample) 1.0 / _NumSIDSamplesToFade;

                audio_sample * p = Samples;

                for (uint32_t i = Head; i < Tail; i += 2)
                {
                    if (i <= _TotalSIDSamples)
                    {
                        const audio_sample FadeFactor = (audio_sample)(_TotalSIDSamples - i) * ScaleFactor;

                        *p++ *= FadeFactor;
                        *p++ *= FadeFactor;
                    }
                    else
                    {
                        *p++ = (audio_sample) 0.0;
                        *p++ = (audio_sample) 0.0;
                    }
                }
            }
        }

        audioChunk.set_sample_count(NumSamplesMixed / 2);

        _TotalSIDSamplesRendered += NumSamplesMixed;

        return true;
    }

    void decode_seek(double positionInSeconds, abort_callback & abortHandler)
    {
        _IsFirstChunk = true;

        uint32_t PositionInSamples = (uint32_t) audio_math::time_to_samples(positionInSeconds, _SampleRate);

        PositionInSamples *= 2;

        if (PositionInSamples < _TotalSIDSamplesRendered)
        {
            decode_initialize(_Tune->getInfo()->currentSong(), input_flag_playback | (_TotalSIDSamples ? input_flag_no_looping : 0), abortHandler);
        }

        pfc::array_t<t_int16> sample_buffer;

        sample_buffer.grow_size((t_size) MaxSIDSamples * 2);
        _IsEOF = false;
/*
        unsigned remain = ( samples - played ) % 32;
        played /= 32;
        samples /= 32;
        m_engine->fastForward( 100 * 32 );
*/
        while (_TotalSIDSamplesRendered < PositionInSamples)
        {
            abortHandler.check();

            uint32_t ToDO = PositionInSamples - _TotalSIDSamplesRendered;

            if (ToDO > MaxSIDSamples * 2)
                ToDO = MaxSIDSamples * 2;

            uint32_t Done;
            {
//              Done = _Engine->play(sample_buffer.get_ptr(), ToDO);
                Done = _Engine->play(ToDO);
            }

            if (Done < ToDO)
            {
                if (_Engine->error())
                    throw exception_io_data(_Engine->error());

                _IsEOF = true;
                break;
            }

            _TotalSIDSamplesRendered += ToDO;
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
        if (!_IsFirstChunk)
            return false;

        fileInfo.info_set_int("samplerate", _SampleRate);

        timestampDelta = 0.0;
        _IsFirstChunk = false;

        return true;
    }

    #pragma endregion

private:
    int _SampleRate;
    int _BPS;
    int _StereoSeparation;

    uint32_t _TotalSIDSamples;
    uint32_t _TotalSIDSamplesRendered;
    uint32_t _NumSIDSamplesToFade;

    bool _IsFirstChunk;
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

static cfg_dropdown_history CfgSampleRateHistory(CfgHistoryRateGUID, 16);

static const uint32_t _SampleRates[] = { 8000, 11025, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000 };

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
        DROPDOWN_HISTORY_HANDLER(IDC_SAMPLERATE, CfgSampleRateHistory)
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
    SendDlgItemMessage(IDC_INFINITE, BM_SETCHECK, CfgLoopForeverDefault);
    SendDlgItemMessage(IDC_SID_BUILDER, CB_SETCURSEL, CfgCoreDefault);
    SendDlgItemMessage(IDC_CLOCK_OVERRIDE, CB_SETCURSEL, CfgClockOverrideDefault);
    SendDlgItemMessage(IDC_SID_OVERRIDE, CB_SETCURSEL, CfgModelOverrideDefault);

    ::uSetDlgItemText(m_hWnd, IDC_DLENGTH, pfc::format_time_ex((double) CfgDefaultLengthInMSDefault / 1000.0));
    ::uSetDlgItemText(m_hWnd, IDC_DB_PATH, "");

    SetDlgItemInt(IDC_SAMPLERATE, CfgSampleRateDefault, FALSE);
    SetDlgItemInt(IDC_FADE, CfgFadeLengthDefault, FALSE);

    _Slider6581.SetPos(CfgFilter6581CurveDefault);

    pfc::string Text = pfc::format_float((double) CfgFilter6581CurveDefault / 256., 0, 2);
    ::uSetDlgItemText(m_hWnd, IDC_TEXT_6581, Text);

    _Slider8580.SetPos(CfgFilter8580CurveDefault);

    Text = pfc::format_float((double) CfgFilter8580CurveDefault / 256., 0, 2);
    ::uSetDlgItemText(m_hWnd, IDC_TEXT_8580, Text);

    _SliderSsep.SetPos(CfgStereoSeparationDefault);

    Text = pfc::format_int(CfgStereoSeparationDefault);
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

        CfgSampleRateHistory.add_item(temp);

        CfgSampleRate = SampleRate;
    }

    {
        pfc::string Text;

        ::uGetDlgItemText(m_hWnd, IDC_DB_PATH, Text);

        pfc::string DatabaseFilePath;

        ::SanitizeDatabasePathName(Text, false, DatabaseFilePath);

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

    CfgFadeLength             = GetDlgItemInt(IDC_FADE, NULL, FALSE);
    CfgLoopForever      = (t_int32) SendDlgItemMessage(IDC_INFINITE, BM_GETCHECK);
    CfgClockOverride    = (t_int32) SendDlgItemMessage(IDC_CLOCK_OVERRIDE, CB_GETCURSEL);
    CfgModelOverride      = (t_int32) SendDlgItemMessage(IDC_SID_OVERRIDE, CB_GETCURSEL);
    CfgCore       = (t_int32) SendDlgItemMessage(IDC_SID_BUILDER, CB_GETCURSEL);
    CfgFilter6581Curve    = _Slider6581.GetPos();
    CfgFilter8580Curve    = _Slider8580.GetPos();
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

            SetDlgItemInt(IDC_FADE, CfgFadeLength, FALSE);

            ::uSendMessage(GetDlgItem(IDC_FADE), EM_LIMITTEXT, 3, 0);

            for (int n = _countof(_SampleRates); n--;)
            {
                if (_SampleRates[n] != (uint32_t) CfgSampleRate)
                {
                    _itoa_s(_SampleRates[n], temp, _countof(temp), 10);
                    CfgSampleRateHistory.add_item(temp);
                }
            }

            _itoa_s(CfgSampleRate, temp, _countof(temp), 10);

            CfgSampleRateHistory.add_item(temp);

            w = GetDlgItem(IDC_SAMPLERATE);

            CfgSampleRateHistory.setup_dropdown(w);
            ::SendMessage(w, CB_SETCURSEL, 0, 0);
        }

        w = GetDlgItem(IDC_SID_BUILDER);

        ::uSendMessageText(w, CB_ADDSTRING, 0, "ReSIDfp");
        ::uSendMessageText(w, CB_ADDSTRING, 0, "SIDLite");

        ::SendMessage(w, CB_SETCURSEL, CfgCore, 0);

        w = GetDlgItem(IDC_CLOCK_OVERRIDE);

        ::uSendMessageText(w, CB_ADDSTRING, 0, "As input file specifies");
        ::uSendMessageText(w, CB_ADDSTRING, 0, "Force PAL");
        ::uSendMessageText(w, CB_ADDSTRING, 0, "Force NTSC");

        ::SendMessage(w, CB_SETCURSEL, CfgClockOverride, 0);

        w = GetDlgItem(IDC_SID_OVERRIDE);

        ::uSendMessageText(w, CB_ADDSTRING, 0, "As input file specifies");
        ::uSendMessageText(w, CB_ADDSTRING, 0, "Force 6581");
        ::uSendMessageText(w, CB_ADDSTRING, 0, "Force 8580");

        ::SendMessage(w, CB_SETCURSEL, CfgModelOverride, 0);
    }

    pfc::string Value;

    {
        _Slider6581 = GetDlgItem(IDC_SLIDER_6581);

        _Slider6581.SetRangeMin(0);
        _Slider6581.SetRangeMax(256);
        _Slider6581.SetPos(CfgFilter6581Curve);

        Value = pfc::format_float(CfgFilter6581Curve / 256., 0, 2);

        ::uSetDlgItemText(m_hWnd, IDC_TEXT_6581, Value);

        _Slider6581.EnableWindow((CfgCore == CoreReSIDfp) ? TRUE : FALSE);
    }

    {
        _Slider8580 = GetDlgItem(IDC_SLIDER_8580);

        _Slider8580.SetRangeMin(0);
        _Slider8580.SetRangeMax(256);
        _Slider8580.SetPos(CfgFilter8580Curve);

        Value = pfc::format_float(CfgFilter8580Curve / 256., 0, 2);

        ::uSetDlgItemText(m_hWnd, IDC_TEXT_8580, Value);

        _Slider8580.EnableWindow((CfgCore == CoreReSIDfp) ? TRUE : FALSE);
    }

    {
        _SliderSsep = GetDlgItem(IDC_SLIDER_SSEP);

        _SliderSsep.SetRangeMin(0);
        _SliderSsep.SetRangeMax(150);
        _SliderSsep.SetPos(CfgStereoSeparation);

        Value = pfc::format_int(CfgStereoSeparation);
        Value += "%";

        ::uSetDlgItemText(m_hWnd, IDC_TEXT_SSEP, Value);
    }

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
        const BOOL Enable = wnd.SendMessage(CB_GETCURSEL) == CoreReSIDfp;

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
    pfc::string Value;

    if (pScrollBar.m_hWnd == _Slider6581.m_hWnd)
    {
        Value = pfc::format_float(_Slider6581.GetPos() / 256., 0, 2);

        ::uSetDlgItemText(m_hWnd, IDC_TEXT_6581, Value);
    }
    else
    if (pScrollBar.m_hWnd == _Slider8580.m_hWnd)
    {
        Value = pfc::format_float(_Slider8580.GetPos() / 256., 0, 2);

        ::uSetDlgItemText(m_hWnd, IDC_TEXT_8580, Value);
    }
    else
    if (pScrollBar.m_hWnd == _SliderSsep.m_hWnd)
    {
        Value = pfc::format_int(_SliderSsep.GetPos());
        Value += "%";

        ::uSetDlgItemText(m_hWnd, IDC_TEXT_SSEP, Value);
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

    if (!IsChanged && GetDlgItemInt(IDC_FADE, NULL, FALSE) != (UINT) CfgFadeLength)
        IsChanged = true;

    if (!IsChanged && SendDlgItemMessage(IDC_INFINITE, BM_GETCHECK) != CfgLoopForever)
        IsChanged = true;

    if (!IsChanged && SendDlgItemMessage(IDC_CLOCK_OVERRIDE, CB_GETCURSEL) != CfgClockOverride)
        IsChanged = true;

    if (!IsChanged && SendDlgItemMessage(IDC_SID_OVERRIDE, CB_GETCURSEL) != CfgModelOverride)
        IsChanged = true;

    if (!IsChanged && SendDlgItemMessage(IDC_SID_BUILDER, CB_GETCURSEL) != CfgCore)
        IsChanged = true;

    if (!IsChanged && _Slider6581.GetPos() != CfgFilter6581Curve)
        IsChanged = true;

    if (!IsChanged && _Slider8580.GetPos() != CfgFilter8580Curve)
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

DECLARE_FILE_TYPE("SID files", "*.SID;*.PSID;*.RSID;*.MUS");

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
