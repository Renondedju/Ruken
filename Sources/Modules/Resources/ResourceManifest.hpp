#pragma once

#include "Resources/ResourceData.hpp"
#include "Resources/ResourceLoadEvent.hpp"

#include <atomic>
#include <memory>

BEGIN_RUKEN_NAMESPACE

struct ResourceManifest
{
	std::atomic<ResourcePtr<>> resource_ptr {nullptr};
	ResourceLoadEvent      	   load_event	{};
};

END_RUKEN_NAMESPACE