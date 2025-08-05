cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkModule.cmake)
include(FetchContent)
register_ruken_module(RkRendering)

# Dependencies
find_package(Vulkan REQUIRED)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)

CPMAddPackage(URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip" NAME glfw)
CPMAddPackage(URL "https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/archive/refs/tags/v3.3.0.zip" NAME vma)
CPMAddPackage(URL "https://github.com/zeux/volk/archive/refs/tags/1.4.304.zip" NAME volk)

# Files
target_sources(RkRendering PUBLIC
        FILE_SET HEADERS FILES
        RenderDevice.hpp
        SlangImporter.hpp
        Windowing/Window.hpp
        Vulkan/VulkanInstance.hpp
        Vulkan/VulkanLoaderInstance.hpp
)

target_sources(RkRendering PRIVATE
        RenderDevice.cpp
        SlangImporter.cpp
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

# Workaround to avoid having to pull the slang project recursively from git.
# find_package also does not allow us to link from the SDK even though slang is shipped with it.
# CPMAddPackage("gh:shader-slang/slang#6e0c63b723cc81efcc82c2af778b26e507c71825")
get_filename_component(RUKEN_VULKAN_LIB_DIR ${Vulkan_LIBRARY} DIRECTORY)
set(RUKEN_VULKAN_SLANG_LIB ${RUKEN_VULKAN_LIB_DIR}/slang.lib)

target_compile_definitions(RkRendering PUBLIC VK_NO_PROTOTYPES VULKAN_HPP_NO_STRUCT_CONSTRUCTORS GLFW_INCLUDE_VULKAN)
target_link_libraries     (RkRendering PUBLIC RkCore Vulkan::Headers volk VulkanMemoryAllocator glfw ${RUKEN_VULKAN_SLANG_LIB})