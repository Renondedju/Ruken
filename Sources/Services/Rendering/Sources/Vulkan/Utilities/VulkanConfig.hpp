#pragma once

#include "Build/BuildInfo.hpp"
#include "Types/FundamentalTypes.hpp"

#ifndef RUKEN_HEADLESS_BUILD

    #if defined(RUKEN_OS_WINDOWS)
        #define VK_USE_PLATFORM_WIN32_KHR
    #endif

    #include <volk.h>
    #include <vk_mem_alloc.h>

#endif