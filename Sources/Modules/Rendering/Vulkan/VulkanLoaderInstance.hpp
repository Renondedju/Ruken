#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief Volk instance
struct VulkanLoaderInstance
{
	explicit VulkanLoaderInstance();
	VulkanLoaderInstance           (const VulkanLoaderInstance&) = delete;
	VulkanLoaderInstance           (VulkanLoaderInstance&&     ) = delete;
	VulkanLoaderInstance& operator=(const VulkanLoaderInstance&) = delete;
	VulkanLoaderInstance& operator=(VulkanLoaderInstance&&     ) = delete;
	~VulkanLoaderInstance() noexcept;
};

END_RUKEN_NAMESPACE