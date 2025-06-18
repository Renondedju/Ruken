#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

enum class EOpenMode
{
	Read	  = 1 << 0,
	Write     = 1 << 1,
	ReadWrite = Read | Write,
};

END_RUKEN_NAMESPACE