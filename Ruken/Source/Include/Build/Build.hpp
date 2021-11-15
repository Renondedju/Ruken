
#pragma once

#include "Build/Config.hpp"
#include "Build/Revision.hpp"
#include "Build/Platform.hpp"
#include "Build/Compiler.hpp"
#include "Build/OperatingSystem.hpp"

#define RUKEN_BUILD_VERSION "0.0.0"
#define RUKEN_BUILD_STAMP   __DATE__ " " __TIME__

#define RUKEN_BUILD_INFO RUKEN_PROJECT_NAME " " RUKEN_BUILD_VERSION " rev " RUKEN_BUILD_REVISION " (" RUKEN_OS_STR "-" RUKEN_PLATFORM_STR " " RUKEN_CONFIG_STR " > " RUKEN_COMPILER_STR ")"