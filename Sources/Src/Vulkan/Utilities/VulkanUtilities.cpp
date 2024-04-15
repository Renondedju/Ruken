
#include "Vulkan/Utilities/VulkanUtilities.hpp"

USING_RUKEN_NAMESPACE

RkBool RUKEN_NAMESPACE::operator==(VkExtent3D const& in_lhs, VkExtent3D const& in_rhs) noexcept
{
    return in_lhs.width  == in_rhs.width  &&
           in_rhs.height == in_lhs.height &&
           in_rhs.depth  == in_lhs.depth;
}

RkBool RUKEN_NAMESPACE::operator!=(VkExtent3D const& in_lhs, VkExtent3D const& in_rhs) noexcept
{
    return in_lhs.width  != in_rhs.width  ||
           in_rhs.height != in_lhs.height ||
           in_rhs.depth  != in_lhs.depth;
}