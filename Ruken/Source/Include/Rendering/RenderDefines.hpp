#pragma once

#include "Types/FundamentalTypes.hpp"

#define VK_NO_PROTOTYPES

#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_NO_SETTERS
#define VULKAN_HPP_NO_SMART_HANDLE
#define VULKAN_HPP_NO_SPACESHIP_OPERATOR

#include <vulkan/vulkan.hpp>

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0

#include <vma/vk_mem_alloc.h>

namespace VULKAN_HPP_NAMESPACE
{
	struct Scale2D
    {
        RUKEN_NAMESPACE::RkFloat x;
        RUKEN_NAMESPACE::RkFloat y;
    };
}