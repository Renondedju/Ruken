cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkModule.cmake)
register_ruken_module(RkRendering)

# Dependancies
find_package(Vulkan REQUIRED)

# Dependancies
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)

CPMAddPackage(URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip" NAME glfw)
CPMAddPackage(URL "https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/archive/refs/tags/v3.3.0.zip" NAME vma)
CPMAddPackage(URL "https://github.com/zeux/volk/archive/refs/tags/1.4.304.zip" NAME volk)

# Files
target_sources(RkRendering PUBLIC
        FILE_SET HEADERS FILES
        Renderer.hpp
        Windowing/Window.hpp
        Vulkan/VulkanInstance.hpp
        Vulkan/VulkanLoaderInstance.hpp
)

target_sources(RkRendering PRIVATE
        Renderer.cpp
        Windowing/Window.cpp
        Vulkan/VulkanInstance.cpp
        Vulkan/VulkanLoaderInstance.cpp
)

target_compile_definitions(RkRendering PUBLIC VK_NO_PROTOTYPES VULKAN_HPP_NO_STRUCT_CONSTRUCTORS GLFW_INCLUDE_VULKAN)
target_link_libraries     (RkRendering PUBLIC RkCore Vulkan::Vulkan volk VulkanMemoryAllocator glfw)