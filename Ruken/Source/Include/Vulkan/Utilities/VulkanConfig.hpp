
#pragma once

#include "Build/OperatingSystem.hpp"
#include "Types/FundamentalTypes.hpp"

#if defined(RUKEN_OS_WINDOWS)
    #define VK_USE_PLATFORM_WIN32_KHR
#else
    #error Platform not supported
#endif

#include <volk/volk.h>
#include <vma/vk_mem_alloc.h>