#pragma once

#define ON true
#define OFF false

struct BuildInfo
{
    enum class ECompiler
    {
        AppleClang,
        Clang,
        GNU,
        MSVC,
        SunPro,
        Intel
    };

    enum class EOperatingSystem
    {
        Windows,
        Linux,
        Android,
        FreeBSD,
        Darwin // (Apple)
    };

    static constexpr const char*      CompilerName {"MSVC"};
    static constexpr const char*      SystemName   {"Windows"};
    static constexpr ECompiler        CompilerID   {ECompiler::MSVC};
    static constexpr EOperatingSystem SystemID     {EOperatingSystem::Windows};

    static constexpr const char*  BuildType      {"Release"};
    static constexpr bool         HasDebugInfo   {OFF};
    static constexpr bool         HeadlessBuild  {OFF};

    static const char* Version;
    static const char* Revision;
};

/* #undef RUKEN_HEADLESS_BUILD */

#define RUKEN_COMPILER_MSVC
#define RUKEN_OS_WINDOWS

#undef ON
#undef OFF
