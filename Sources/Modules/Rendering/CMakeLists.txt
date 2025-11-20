cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_CMAKE_DIR}/RkModule.cmake)
register_ruken_module(RkRendering)

# Dependencies
find_package(Vulkan REQUIRED)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "")

CPMAddPackage(URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip" NAME glfw)
CPMAddPackage(URL "https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/archive/refs/tags/v3.3.0.zip" NAME vma)
CPMAddPackage(URL "https://github.com/zeux/volk/archive/refs/tags/1.4.304.zip" NAME volk)

# Workaround to avoid having to pull the slang project recursively from git using CPM.
# Instead we use precompiled binaries from github releases.
# find_package(Vulkan COMPONENTS slang) does not work even though slang is shipped with it.
# And we still have to copy manually the shared libraries like slang-glslang. Adding them to path does not seem to be working.
include(${RUKEN_CMAKE_DIR}/FetchSlang.cmake)
target_copy_slang_binaries(RkRendering ${RUKEN_BINARY_DIR})

# Files
target_sources(RkRendering PUBLIC
        FILE_SET HEADERS FILES
        RenderDevice.hpp
        GPUFence.hpp
        FamilyView.hpp
        Resources/ShaderModule.hpp
        Resources/SlangImporter.hpp
        Resources/SpirvLoader.hpp
        Resources/GPUSwapchain.hpp
        Resources/GPUBuffer.hpp
        Resources/GPUImage.hpp
        WorkGraph/GPUWorkGraph.hpp
        WorkGraph/GPUBufferAccess.hpp
        WorkGraph/GPUImageAccess.hpp
        WorkGraph/GPUWorkNode.hpp
        WorkGraph/Recordable.hpp
        Coroutines/TracyVkUtilities.hpp
        Windowing/Window.hpp
        Vulkan/VulkanInstance.hpp
        Vulkan/VulkanLoaderInstance.hpp
)

target_sources(RkRendering PRIVATE
        RenderDevice.cpp
        GPUFence.cpp
        Resources/SlangImporter.cpp
        Resources/SpirvLoader.cpp
        Resources/ShaderModule.cpp
        Resources/GPUSwapchain.cpp
        Resources/GPUBuffer.cpp
        Resources/GPUImage.cpp
        WorkGraph/GPUWorkGraph.cpp
        Coroutines/TracyVkUtilities.cpp
        Windowing/Window.cpp
        Vulkan/VulkanInstance.cpp
        Vulkan/VulkanLoaderInstance.cpp
)

set(RUKEN_SLANG_INCLUDE_DIR ${RUKEN_GENERATED_SOURCES_DIR}/Shaders)
if (NOT EXISTS ${RUKEN_SLANG_INCLUDE_DIR})
    file(MAKE_DIRECTORY ${RUKEN_SLANG_INCLUDE_DIR})
endif ()

# Adding shaders to the slang include directory as the 'Ruken' module.
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/Shaders ${RUKEN_SLANG_INCLUDE_DIR}/Ruken COPY_ON_ERROR SYMBOLIC)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SlangSearchPath.hpp.in ${RUKEN_GENERATED_SOURCES_DIR}/Rendering/SlangSearchPath.hpp)

target_compile_definitions(RkRendering PUBLIC TRACY_VK_USE_SYMBOL_TABLE VK_NO_PROTOTYPES VULKAN_HPP_NO_STRUCT_CONSTRUCTORS GLFW_INCLUDE_VULKAN)
target_link_libraries     (RkRendering PUBLIC RkCore Vulkan::Headers slang volk VulkanMemoryAllocator glfw)