
#pragma once

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_RUKEN_NAMESPACE

RkBool operator==(VkExtent3D const& in_lhs, VkExtent3D const& in_rhs) noexcept;
RkBool operator!=(VkExtent3D const& in_lhs, VkExtent3D const& in_rhs) noexcept;

END_RUKEN_NAMESPACE