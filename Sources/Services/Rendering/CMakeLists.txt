cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkService.cmake)
register_ruken_service(RkRenderingService Rendering)

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
target_sources(RkRenderingService INTERFACE
        FILE_SET HEADERS BASE_DIRS Sources FILES
        Sources/Renderer.hpp
        Sources/Windowing/Window.hpp
        Sources/Vulkan/VulkanInstance.hpp
        Sources/Vulkan/VulkanLoaderInstance.hpp
)

target_sources(RkRenderingService PRIVATE
        Sources/Renderer.cpp
        Sources/Windowing/Window.cpp
        Sources/Vulkan/VulkanInstance.cpp
        Sources/Vulkan/VulkanLoaderInstance.cpp
)

target_compile_definitions(RkRenderingService PUBLIC VK_NO_PROTOTYPES VULKAN_HPP_NO_STRUCT_CONSTRUCTORS GLFW_INCLUDE_VULKAN)
target_link_libraries     (RkRenderingService PUBLIC RkCore Vulkan::Vulkan volk VulkanMemoryAllocator glfw)