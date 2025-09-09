#pragma once

#include "Resources/Resource.hpp"
#include "Resources/ResourceLoadEvent.hpp"

#include <atomic>
#include <memory>

BEGIN_RUKEN_NAMESPACE

struct ResourceManifest
{
	std::atomic<ResourcePtr<>> resource_ptr {nullptr};
	std::atomic_size_t     	   references   {0}; // TODO: Call some kind of destroy when references goes to 0
	ResourceLoadEvent      	   load_event	{};
	ResourcePath 			   path		    {}; // TODO: What about provided resources ?
};

END_RUKEN_NAMESPACE