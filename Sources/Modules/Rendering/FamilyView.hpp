#pragma once

#include "Build/Namespace.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE

struct FamilyView
{
	vk::raii::Queue*	   queue;
	vk::raii::CommandPool* command_pool;
};

END_RUKEN_NAMESPACE