#pragma once

#define VK_NO_PROTOTYPES

#define VULKAN_HPP_NO_EXCEPTIONS 
#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_NO_SETTERS
#define VULKAN_HPP_NO_SMART_HANDLE
#define VULKAN_HPP_NO_SPACESHIP_OPERATOR

#include <vulkan/vulkan.hpp>

#define VMA_HPP_NAMESPACE VULKAN_HPP_NAMESPACE

#include <vma/vk_mem_alloc.hpp>

#include "Meta/Safety.hpp"

#include "Types/FundamentalTypes.hpp"

#define RUKEN_SAFE_LOGGER_RETURN_CALL(in_instance, ...) { RUKEN_SAFE_LOGGER_CALL(in_instance, __VA_ARGS__) return; }