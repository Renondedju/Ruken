#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief An enumeration of all the major locations or "volumes" of the filesystem.
enum class EFilesystemLocation
{
	ProjectDirectory,
	Temporary,
};

END_RUKEN_NAMESPACE