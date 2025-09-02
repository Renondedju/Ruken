#pragma once

#include "Filesystem/IOJobQueue.hpp"
#include "Resources/Resource.hpp"

#include <atomic>
#include <memory>

BEGIN_RUKEN_NAMESPACE

struct ResourceManifest
{
	std::atomic<ResourcePtr<>> resource_ptr {nullptr};
	std::atomic_size_t     	   references   {0}; // TODO: Call some kind of destroy when references goes to 0
	IOTask<RkVoid>      	   load_task	{};
	ResourcePath 			   path		    {};
};

END_RUKEN_NAMESPACE