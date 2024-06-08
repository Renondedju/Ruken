cmake_minimum_required(VERSION 3.16.3)

find_package(Vulkan REQUIRED)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)

if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR}/Services/Rendering)
    file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/Sources ${RUKEN_GENERATED_SOURCES_DIR}/Services/Rendering SYMBOLIC)
endif ()

CPMAddPackage(URL "https://github.com/glfw/glfw/releases/download/3.3.9/glfw-3.3.9.zip" NAME glfw)
CPMAddPackage(URL "https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/archive/refs/tags/v3.0.1.zip" NAME vma)
CPMAddPackage(URL "https://github.com/zeux/volk/archive/refs/tags/1.3.270.zip" NAME volk)

add_library(RkRenderingService STATIC
    Sources/Rendering/RenderContext.hpp
    Sources/Rendering/Renderer.hpp
    Sources/Rendering/RenderFrame.hpp
    Sources/Rendering/RenderTarget.hpp
    Sources/Rendering/RenderTechnique.hpp
    Sources/Rendering/RenderView.hpp
    Sources/Rendering/UV.hpp

    Sources/Vulkan/Core/VulkanBuffer.hpp
    Sources/Vulkan/Core/VulkanCommandBuffer.hpp
    Sources/Vulkan/Core/VulkanCommandPool.hpp
    Sources/Vulkan/Core/VulkanDevice.hpp
    Sources/Vulkan/Core/VulkanFence.hpp
    Sources/Vulkan/Core/VulkanImage.hpp
    Sources/Vulkan/Core/VulkanInstance.hpp
    Sources/Vulkan/Core/VulkanPhysicalDevice.hpp
    Sources/Vulkan/Core/VulkanPipelineCache.hpp
    Sources/Vulkan/Core/VulkanQueue.hpp
    Sources/Vulkan/Core/VulkanSemaphore.hpp
    Sources/Vulkan/Core/VulkanShaderModule.hpp
    Sources/Vulkan/Core/VulkanSwapchain.hpp
    Sources/Vulkan/Core/VulkanTimelineSemaphore.hpp
    Sources/Vulkan/Resources/Material.hpp
    Sources/Vulkan/Resources/MaterialLoadingDescriptor.hpp
    Sources/Vulkan/Resources/Mesh.hpp
    Sources/Vulkan/Resources/MeshLoadingDescriptor.hpp
    Sources/Vulkan/Resources/Shader.hpp
    Sources/Vulkan/Resources/ShaderLoadingDescriptor.hpp
    Sources/Vulkan/Resources/Texture.hpp
    Sources/Vulkan/Resources/TextureLoadingDescriptor.hpp
    Sources/Vulkan/Resources/Vertex.hpp
    Sources/Vulkan/Utilities/VulkanConfig.hpp
    Sources/Vulkan/Utilities/VulkanDebug.hpp
    Sources/Vulkan/Utilities/VulkanDeviceAllocator.hpp
    Sources/Vulkan/Utilities/VulkanLoader.hpp
    Sources/Vulkan/Utilities/VulkanUtilities.hpp
    Sources/Vulkan/CommandPool.hpp
    Sources/Vulkan/FencePool.hpp
    Sources/Vulkan/SemaphorePool.hpp

    Sources/Vulkan/Core/VulkanBuffer.cpp
    Sources/Vulkan/Core/VulkanCommandBuffer.cpp
    Sources/Vulkan/Core/VulkanCommandPool.cpp
    Sources/Vulkan/Core/VulkanDevice.cpp
    Sources/Vulkan/Core/VulkanFence.cpp
    Sources/Vulkan/Core/VulkanImage.cpp
    Sources/Vulkan/Core/VulkanInstance.cpp
    Sources/Vulkan/Core/VulkanPhysicalDevice.cpp
    Sources/Vulkan/Core/VulkanPipelineCache.cpp
    Sources/Vulkan/Core/VulkanQueue.cpp
    Sources/Vulkan/Core/VulkanSemaphore.cpp
    Sources/Vulkan/Core/VulkanShaderModule.cpp
    Sources/Vulkan/Core/VulkanSwapchain.cpp
    Sources/Vulkan/Core/VulkanTimelineSemaphore.cpp
    Sources/Vulkan/Resources/Material.cpp
    Sources/Vulkan/Resources/MaterialLoadingDescriptor.cpp
    Sources/Vulkan/Resources/Mesh.cpp
    Sources/Vulkan/Resources/MeshLoadingDescriptor.cpp
    Sources/Vulkan/Resources/Shader.cpp
    Sources/Vulkan/Resources/ShaderLoadingDescriptor.cpp
    Sources/Vulkan/Resources/Texture.cpp
    Sources/Vulkan/Resources/TextureLoadingDescriptor.cpp
    Sources/Vulkan/Resources/Vertex.cpp
    Sources/Vulkan/Utilities/VulkanDebug.cpp
    Sources/Vulkan/Utilities/VulkanDeviceAllocator.cpp
    Sources/Vulkan/Utilities/VulkanLoader.cpp
    Sources/Vulkan/Utilities/VulkanUtilities.cpp
    Sources/Vulkan/CommandPool.cpp
    Sources/Vulkan/FencePool.cpp
    Sources/Vulkan/SemaphorePool.cpp
    Sources/Windowing/Screen.cpp
    Sources/Windowing/Utilities.cpp
    Sources/Windowing/Window.cpp
    Sources/Windowing/WindowManager.cpp

    Sources/Windowing/GammaRamp.hpp
    Sources/Windowing/Metrics.hpp
    Sources/Windowing/Screen.hpp
    Sources/Windowing/Utilities.hpp
    Sources/Windowing/VideoMode.hpp
    Sources/Windowing/Window.hpp
    Sources/Windowing/WindowManager.hpp
    Sources/Windowing/WindowParams.hpp

    Sources/Rendering/RenderContext.cpp
    Sources/Rendering/Renderer.cpp
    Sources/Rendering/RenderFrame.cpp
    Sources/Rendering/RenderTarget.cpp
    Sources/Rendering/RenderTechnique.cpp
    Sources/Rendering/RenderView.cpp
)

target_include_directories(RkRenderingService PRIVATE ${RUKEN_GENERATED_SOURCES_DIR}/Include)
target_include_directories(RkRenderingService PRIVATE ${RUKEN_SOURCES_DIR}/Core/Include)
target_include_directories(RkRenderingService PRIVATE ${RUKEN_SOURCES_DIR}/Core/Src)
target_include_directories(RkRenderingService PRIVATE ${RUKEN_GENERATED_SOURCES_DIR}/Services)
target_link_libraries     (RkRenderingService PUBLIC  glfw Vulkan::Vulkan volk VulkanMemoryAllocator RkCore)