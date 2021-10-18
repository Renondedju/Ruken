#pragma once

#include "Meta/Safety.hpp"

#include "Types/FundamentalTypes.hpp"

#define VK_NO_PROTOTYPES

#define VULKAN_HPP_NO_EXCEPTIONS 
#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_NO_SETTERS
#define VULKAN_HPP_NO_SMART_HANDLE
#define VULKAN_HPP_NO_SPACESHIP_OPERATOR

#include <vulkan/vulkan.hpp>

#define VMA_HPP_NO_STRUCT_CONSTRUCTORS

#define VMA_HPP_NAMESPACE VULKAN_HPP_NAMESPACE

#include <vma/vk_mem_alloc.hpp>

BEGIN_RUKEN_NAMESPACE

struct VkScale2D
{
    RkFloat x;
    RkFloat y;
};

END_RUKEN_NAMESPACE