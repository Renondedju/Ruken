cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkService.cmake)
register_ruken_service(RkRenderingService Rendering)

# Dependancies
find_package(Vulkan REQUIRED)

target_compile_definitions(RkRenderingService PUBLIC VK_NO_PROTOTYPES VULKAN_HPP_NO_STRUCT_CONSTRUCTORS)

CPMAddPackage(URL "https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/archive/refs/tags/v3.3.0.zip" NAME vma)
CPMAddPackage(URL "https://github.com/zeux/volk/archive/refs/tags/1.4.304.zip" NAME volk)

# Files
target_sources(RkRenderingService INTERFACE
        FILE_SET HEADERS BASE_DIRS Sources FILES
        Sources/Renderer.hpp
        Sources/VulkanInstance.hpp
        Sources/VulkanLoaderInstance.hpp
)

target_sources(RkRenderingService PRIVATE
        Sources/Renderer.cpp
        Sources/VulkanInstance.cpp
        Sources/VulkanLoaderInstance.cpp
)

target_link_libraries(RkRenderingService PUBLIC RkCore Vulkan::Vulkan volk VulkanMemoryAllocator)