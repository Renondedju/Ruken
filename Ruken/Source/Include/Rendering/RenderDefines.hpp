#pragma once

#include "Types/FundamentalTypes.hpp"

#define VK_NO_PROTOTYPES

#define VULKAN_HPP_NO_EXCEPTIONS 
#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_NO_SETTERS
#define VULKAN_HPP_NO_SMART_HANDLE

#include <vulkan/vulkan.hpp>

#define VMA_HPP_NAMESPACE VULKAN_HPP_NAMESPACE

#include <vma/vk_mem_alloc.hpp>