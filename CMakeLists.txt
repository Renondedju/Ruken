cmake_minimum_required(VERSION 3.16.3)

# --- Language setup
set(CMAKE_CXX_STANDARD 23)

# --- Project setup
project(Ruken)

macro(set_boolean var)
    if(${ARGN})
        set(${var} ON)
    else()
        set(${var} OFF)
    endif()
endmacro()

option(RUKEN_HEADLESS_BUILD "Disables all form of rendering and presentation " OFF)

set(RUKEN_SOURCES_DIR           ${CMAKE_CURRENT_LIST_DIR}/Sources)
set(RUKEN_GENERATED_SOURCES_DIR ${CMAKE_BINARY_DIR}/Generated)

if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR})
    file(MAKE_DIRECTORY ${RUKEN_GENERATED_SOURCES_DIR})
endif ()

add_compile_definitions(UNICODE _UNICODE)
add_executable(Ruken Sources/Src/Main.cpp
        Sources/Include/Bitwise/Bitmask.hpp
        Sources/Include/Bitwise/Endian.hpp
        Sources/Include/Bitwise/SizedBitmask.hpp
        Sources/Include/Build/Attributes.hpp
        Sources/Include/Build/ProjectInfo.hpp
        Sources/Include/Build/Namespace.hpp
        Sources/Include/Containers/LinkedChunkList.hpp
        Sources/Include/Containers/LinkedChunkListNode.hh
        Sources/Include/Core/ExecutiveSystem/Concepts/AwaitableType.hpp
        Sources/Include/Core/ExecutiveSystem/Concepts/ProcessingQueueType.hpp
        Sources/Include/Core/ExecutiveSystem/Concepts/ProcessingUnitType.hpp
        Sources/Include/Core/ExecutiveSystem/Concepts/QueueHandleType.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Awaitables/Primitives/AutomaticResetEvent.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Awaitables/Primitives/CountDownLatch.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Awaitables/Primitives/ManualResetEvent.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUDynamicTask.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUTask.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskPromise.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitableExceptionStorage.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitableHandle.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitableResultStorage.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Awaitables/CPUAwaiter.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Continuations/CPUContinuation.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Continuations/CPUCoroutineContinuation.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Continuations/CPUPropagatingContinuation.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Queues/CentralProcessingQueue.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Queues/CPUDynamicQueue.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Queues/CPUQueueHandle.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/ConcurrencyCounter.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/Worker.hpp
        Sources/Include/Core/ExecutiveSystem/CPU/WorkerInfo.hpp
        Sources/Include/Core/ExecutiveSystem/GPU/GraphicsProcessingQueue.hpp
        Sources/Include/Core/ExecutiveSystem/GPU/GraphicsProcessingUnit.hpp
        Sources/Include/Core/ExecutiveSystem/Awaitable.hpp
        Sources/Include/Core/ExecutiveSystem/Awaiter.hpp
        Sources/Include/Core/ExecutiveSystem/EExecutionPolicy.hpp
        Sources/Include/Core/ExecutiveSystem/EInstructionType.hpp
        Sources/Include/Core/ExecutiveSystem/EventBridge.hpp
        Sources/Include/Core/ExecutiveSystem/ProcessingQueue.hpp
        Sources/Include/Core/ExecutiveSystem/ProcessingUnit.hpp
        Sources/Include/Core/ExecutiveSystem/Promise.hpp
        Sources/Include/Core/ExecutiveSystem/QueueHandle.hpp
        Sources/Include/Core/ExecutiveSystem/Task.hpp
        Sources/Include/Core/EInitializationStatus.hpp
        Sources/Include/Core/Exception.hpp
        Sources/Include/Core/Kernel.hpp
        Sources/Include/Core/KernelProxy.hpp
        Sources/Include/Core/Service.hpp
        Sources/Include/Core/ServiceBase.hpp
        Sources/Include/Core/ServiceLoader.hpp
        Sources/Include/Core/ServiceProvider.hpp
        Sources/Include/Core/ServiceType.hpp
        Sources/Include/Debug/Logging/Filters/LogFilter.hpp
        Sources/Include/Debug/Logging/Formatters/ConsoleFormatter.hpp
        Sources/Include/Debug/Logging/Formatters/FileFormatter.hpp
        Sources/Include/Debug/Logging/Formatters/LogFormatter.hpp
        Sources/Include/Debug/Logging/Handlers/ConsoleHandler.hpp
        Sources/Include/Debug/Logging/Handlers/FileHandler.hpp
        Sources/Include/Debug/Logging/Handlers/LogHandler.hpp
        Sources/Include/Debug/Logging/Handlers/StreamHandler.hpp
        Sources/Include/Debug/Logging/Logger.hpp
        Sources/Include/Debug/Logging/LogLevel.hpp
        Sources/Include/Debug/Logging/LogRecord.hpp
        Sources/Include/ECS/Meta/ComponentHelper.hpp
        Sources/Include/ECS/Meta/FieldHelper.hpp
        Sources/Include/ECS/Meta/ItemHelper.hpp
        Sources/Include/ECS/Safety/AnyComponentType.hpp
        Sources/Include/ECS/Safety/ComponentFieldType.hpp
        Sources/Include/ECS/Safety/ComponentType.hpp
        Sources/Include/ECS/Safety/EventHandlerType.hpp
        Sources/Include/ECS/Safety/ExclusiveComponentType.hpp
        Sources/Include/ECS/Safety/SystemType.hpp
        Sources/Include/ECS/Safety/TagComponentType.hpp
        Sources/Include/ECS/Safety/ViewType.hpp
        Sources/Include/ECS/Test/CounterComponent.hpp
        Sources/Include/ECS/Test/CounterSystem.hpp
        Sources/Include/ECS/Archetype.hpp
        Sources/Include/ECS/ArchetypeFingerprint.hpp
        Sources/Include/ECS/Component.hpp
        Sources/Include/ECS/ComponentBase.hpp
        Sources/Include/ECS/ComponentCounter.hpp
        Sources/Include/ECS/ComponentField.hpp
        Sources/Include/ECS/ComponentQuery.hpp
        Sources/Include/ECS/ComponentView.hpp
        Sources/Include/ECS/EEventName.hpp
        Sources/Include/ECS/Entity.hpp
        Sources/Include/ECS/EntityAdmin.hpp
        Sources/Include/ECS/EventHandler.hpp
        Sources/Include/ECS/EventHandlerBase.hpp
        Sources/Include/ECS/ExclusiveComponent.hpp
        Sources/Include/ECS/Range.hpp
        Sources/Include/ECS/System.hpp
        Sources/Include/ECS/TagComponent.hpp
        Sources/Include/Functional/Event.hpp
        Sources/Include/Functional/ReservedEvent.hpp
        Sources/Include/Maths/Matrix/Operations/MatrixAccess.hpp
        Sources/Include/Maths/Matrix/Operations/MatrixConversions.hpp
        Sources/Include/Maths/Matrix/Operations/MatrixInversion.hpp
        Sources/Include/Maths/Matrix/Operations/MatrixMultiplication.hpp
        Sources/Include/Maths/Matrix/Operations/MatrixOstream.hpp
        Sources/Include/Maths/Matrix/Operations/MatrixTransposition.hpp
        Sources/Include/Maths/Matrix/Operations/MatrixUtility.hpp
        Sources/Include/Maths/Matrix/Transformations/MatrixClipSpace.hpp
        Sources/Include/Maths/Matrix/Transformations/MatrixLookAt.hpp
        Sources/Include/Maths/Matrix/Transformations/MatrixModel.hpp
        Sources/Include/Maths/Matrix/Transformations/MatrixProjections.hpp
        Sources/Include/Maths/Matrix/Transformations/MatrixRotation.hpp
        Sources/Include/Maths/Matrix/Transformations/MatrixRotationX.hpp
        Sources/Include/Maths/Matrix/Transformations/MatrixRotationY.hpp
        Sources/Include/Maths/Matrix/Transformations/MatrixRotationZ.hpp
        Sources/Include/Maths/Matrix/Transformations/MatrixScale.hpp
        Sources/Include/Maths/Matrix/Transformations/MatrixTranslation.hpp
        Sources/Include/Maths/Matrix/Matrix.hpp
        Sources/Include/Maths/Matrix/MatrixForward.hpp
        Sources/Include/Maths/Quaternion/Quaternion.hpp
        Sources/Include/Maths/Vector/Helper/VectorForward.hpp
        Sources/Include/Maths/Vector/Helper/VectorHelper.hpp
        Sources/Include/Maths/Vector/Operations/VectorCross.hpp
        Sources/Include/Maths/Vector/Operations/VectorDot.hpp
        Sources/Include/Maths/Vector/Operations/VectorLength.hpp
        Sources/Include/Maths/Vector/Operations/VectorLerp.hpp
        Sources/Include/Maths/Vector/Operations/VectorMinMax.hpp
        Sources/Include/Maths/Vector/Operations/VectorNormalization.hpp
        Sources/Include/Maths/Vector/Operations/VectorOperators.hpp
        Sources/Include/Maths/Vector/Operations/VectorSlerp.hpp
        Sources/Include/Maths/Vector/AngleVector2.hpp
        Sources/Include/Maths/Vector/AngleVector3.hpp
        Sources/Include/Maths/Vector/DistanceVector2.hpp
        Sources/Include/Maths/Vector/DistanceVector3.hpp
        Sources/Include/Maths/Vector/PixelVector2.hpp
        Sources/Include/Maths/Vector/PixelVector3.hpp
        Sources/Include/Maths/Constants.hpp
        Sources/Include/Maths/Easing.hpp
        Sources/Include/Maths/Math.hpp
        Sources/Include/Maths/MinMax.hpp
        Sources/Include/Maths/Trigonometry.hpp
        Sources/Include/Maths/Utility.hpp
        Sources/Include/Maths/Vector4f.hpp
        Sources/Include/Meta/Assert.hpp
        Sources/Include/Meta/CommonTypeFallback.hpp
        Sources/Include/Meta/CopyConst.hpp
        Sources/Include/Meta/HasCommonType.hpp
        Sources/Include/Meta/IndexPack.hpp
        Sources/Include/Meta/IsBaseOfTemplate.hpp
        Sources/Include/Meta/IsInstance.hpp
        Sources/Include/Meta/Meta.hpp
        Sources/Include/Meta/MetaForeach.hpp
        Sources/Include/Meta/MinimumType.hpp
        Sources/Include/Meta/PassConst.hpp
        Sources/Include/Meta/Safety.hpp
        Sources/Include/Meta/Tag.hpp
        Sources/Include/Meta/TupleApply.hpp
        Sources/Include/Meta/TupleHasType.hpp
        Sources/Include/Meta/TupleIndex.hpp
        Sources/Include/Meta/TupleRemoveLast.hpp
        Sources/Include/Meta/TupleSubset.hpp
        Sources/Include/Meta/UniqueTuple.hpp
        Sources/Include/Meta/UniqueTypes.hpp
        Sources/Include/Rendering/RenderContext.hpp
        Sources/Include/Rendering/Renderer.hpp
        Sources/Include/Rendering/RenderFrame.hpp
        Sources/Include/Rendering/RenderTarget.hpp
        Sources/Include/Rendering/RenderTechnique.hpp
        Sources/Include/Rendering/RenderView.hpp
        Sources/Include/Rendering/UV.hpp
        Sources/Include/Resource/Enums/EGCCollectionMode.hpp
        Sources/Include/Resource/Enums/EResourceGCStrategy.hpp
        Sources/Include/Resource/Enums/EResourceLoadingFailureCode.hpp
        Sources/Include/Resource/Enums/EResourceStatus.hpp
        Sources/Include/Resource/Handle.hpp
        Sources/Include/Resource/IResource.hpp
        Sources/Include/Resource/ResourceIdentifier.hpp
        Sources/Include/Resource/ResourceLoadingDescriptor.hpp
        Sources/Include/Resource/ResourceManager.hpp
        Sources/Include/Resource/ResourceManifest.hpp
        Sources/Include/Resource/ResourceProcessingFailure.hpp
        Sources/Include/Threading/EAccessMode.hpp
        Sources/Include/Threading/ESynchronizationMode.hpp
        Sources/Include/Threading/ExecutionPlan.hpp
        Sources/Include/Threading/Job.hpp
        Sources/Include/Threading/OldWorker.hpp
        Sources/Include/Threading/Scheduler.hpp
        Sources/Include/Threading/Synchronized.hpp
        Sources/Include/Threading/SynchronizedAccess.hpp
        Sources/Include/Threading/ThreadSafeLockQueue.hpp
        Sources/Include/Threading/ThreadSafeQueue.hpp
        Sources/Include/Types/Concepts/ArithmeticType.hpp
        Sources/Include/Types/Concepts/FloatingPointType.hpp
        Sources/Include/Types/Concepts/FundamentalType.hpp
        Sources/Include/Types/StrongType/Operators/Arithmetic/Addition.hpp
        Sources/Include/Types/StrongType/Operators/Arithmetic/Decrement.hpp
        Sources/Include/Types/StrongType/Operators/Arithmetic/Division.hpp
        Sources/Include/Types/StrongType/Operators/Arithmetic/Increment.hpp
        Sources/Include/Types/StrongType/Operators/Arithmetic/Modulo.hpp
        Sources/Include/Types/StrongType/Operators/Arithmetic/Multiplication.hpp
        Sources/Include/Types/StrongType/Operators/Arithmetic/Subtraction.hpp
        Sources/Include/Types/StrongType/Operators/Arithmetic/UnaryMinus.hpp
        Sources/Include/Types/StrongType/Operators/Arithmetic/UnaryPlus.hpp
        Sources/Include/Types/StrongType/Operators/Bitwise/And.hpp
        Sources/Include/Types/StrongType/Operators/Bitwise/LeftShift.hpp
        Sources/Include/Types/StrongType/Operators/Bitwise/Not.hpp
        Sources/Include/Types/StrongType/Operators/Bitwise/Or.hpp
        Sources/Include/Types/StrongType/Operators/Bitwise/RightShift.hpp
        Sources/Include/Types/StrongType/Operators/Bitwise/Xor.hpp
        Sources/Include/Types/StrongType/Operators/Comparison/Equal.hpp
        Sources/Include/Types/StrongType/Operators/Comparison/Greater.hpp
        Sources/Include/Types/StrongType/Operators/Comparison/GreaterEqual.hpp
        Sources/Include/Types/StrongType/Operators/Comparison/Less.hpp
        Sources/Include/Types/StrongType/Operators/Comparison/LessEqual.hpp
        Sources/Include/Types/StrongType/Operators/Comparison/NotEqual.hpp
        Sources/Include/Types/StrongType/Operators/Comparison/Spaceship.hpp
        Sources/Include/Types/StrongType/Operators/Logical/LogicalAnd.hpp
        Sources/Include/Types/StrongType/Operators/Logical/LogicalNot.hpp
        Sources/Include/Types/StrongType/Operators/Logical/LogicalOr.hpp
        Sources/Include/Types/StrongType/Operators/Stream/InputStream.hpp
        Sources/Include/Types/StrongType/Operators/Stream/OutputStream.hpp
        Sources/Include/Types/StrongType/Operators/Arithmetic.hpp
        Sources/Include/Types/StrongType/Operators/Bitwise.hpp
        Sources/Include/Types/StrongType/Operators/Comparison.hpp
        Sources/Include/Types/StrongType/Operators/Logical.hpp
        Sources/Include/Types/StrongType/Operators/Stream.hpp
        Sources/Include/Types/StrongType/StrongType.hpp
        Sources/Include/Types/StrongType/StrongTypeSuffix.hpp
        Sources/Include/Types/Units/Angle/Angle.hpp
        Sources/Include/Types/Units/Angle/EAngleUnit.hpp
        Sources/Include/Types/Units/Distance/Distance.hpp
        Sources/Include/Types/Units/Distance/EDistanceUnit.hpp
        Sources/Include/Types/Units/Duration/Duration.hpp
        Sources/Include/Types/Units/Duration/EDurationUnit.hpp
        Sources/Include/Types/Units/Pixels.hpp
        Sources/Include/Types/FundamentalTypes.hpp
        Sources/Include/Types/NonVoid.hpp
        Sources/Include/Utility/Benchmark.hpp
        Sources/Include/Utility/Todo.hpp
        Sources/Include/Utility/WindowsOS.hpp
        Sources/Include/Vulkan/Core/VulkanBuffer.hpp
        Sources/Include/Vulkan/Core/VulkanCommandBuffer.hpp
        Sources/Include/Vulkan/Core/VulkanCommandPool.hpp
        Sources/Include/Vulkan/Core/VulkanDevice.hpp
        Sources/Include/Vulkan/Core/VulkanFence.hpp
        Sources/Include/Vulkan/Core/VulkanImage.hpp
        Sources/Include/Vulkan/Core/VulkanInstance.hpp
        Sources/Include/Vulkan/Core/VulkanPhysicalDevice.hpp
        Sources/Include/Vulkan/Core/VulkanPipelineCache.hpp
        Sources/Include/Vulkan/Core/VulkanQueue.hpp
        Sources/Include/Vulkan/Core/VulkanSemaphore.hpp
        Sources/Include/Vulkan/Core/VulkanShaderModule.hpp
        Sources/Include/Vulkan/Core/VulkanSwapchain.hpp
        Sources/Include/Vulkan/Core/VulkanTimelineSemaphore.hpp
        Sources/Include/Vulkan/Resources/Material.hpp
        Sources/Include/Vulkan/Resources/MaterialLoadingDescriptor.hpp
        Sources/Include/Vulkan/Resources/Mesh.hpp
        Sources/Include/Vulkan/Resources/MeshLoadingDescriptor.hpp
        Sources/Include/Vulkan/Resources/Shader.hpp
        Sources/Include/Vulkan/Resources/ShaderLoadingDescriptor.hpp
        Sources/Include/Vulkan/Resources/Texture.hpp
        Sources/Include/Vulkan/Resources/TextureLoadingDescriptor.hpp
        Sources/Include/Vulkan/Resources/Vertex.hpp
        Sources/Include/Vulkan/Utilities/VulkanConfig.hpp
        Sources/Include/Vulkan/Utilities/VulkanDebug.hpp
        Sources/Include/Vulkan/Utilities/VulkanDeviceAllocator.hpp
        Sources/Include/Vulkan/Utilities/VulkanLoader.hpp
        Sources/Include/Vulkan/Utilities/VulkanUtilities.hpp
        Sources/Include/Vulkan/CommandPool.hpp
        Sources/Include/Vulkan/FencePool.hpp
        Sources/Include/Vulkan/SemaphorePool.hpp
        Sources/Include/Windowing/GammaRamp.hpp
        Sources/Include/Windowing/Metrics.hpp
        Sources/Include/Windowing/Screen.hpp
        Sources/Include/Windowing/Utilities.hpp
        Sources/Include/Windowing/VideoMode.hpp
        Sources/Include/Windowing/Window.hpp
        Sources/Include/Windowing/WindowManager.hpp
        Sources/Include/Windowing/WindowParams.hpp
        Sources/Src/Bitwise/Bitmask.inl
        Sources/Src/Bitwise/SizedBitmask.inl
        Sources/Src/Containers/LinkedChunkList.inl
        Sources/Src/Core/ExecutiveSystem/CPU/Awaitables/Primitives/CountDownLatch.cpp
        Sources/Src/Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.inl
        Sources/Src/Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitableHandle.inl
        Sources/Src/Core/ExecutiveSystem/CPU/Continuations/CPUContinuation.cpp
        Sources/Src/Core/ExecutiveSystem/CPU/CentralProcessingQueue.cpp
        Sources/Src/Core/ExecutiveSystem/CPU/CentralProcessingUnit.cpp
        Sources/Src/Core/ExecutiveSystem/CPU/Worker.cpp
        Sources/Src/Core/ExecutiveSystem/Promise.inl
        Sources/Src/Core/Kernel.cpp
        Sources/Src/Core/Kernel.inl
        Sources/Src/Core/KernelProxy.cpp
        Sources/Src/Core/Service.inl
        Sources/Src/Core/ServiceBase.cpp
        Sources/Src/Core/ServiceProvider.cpp
        Sources/Src/Core/ServiceProvider.inl
        Sources/Src/Debug/Logging/Formatters/ConsoleFormatter.cpp
        Sources/Src/Debug/Logging/Formatters/LogFormatter.cpp
        Sources/Src/Debug/Logging/Handlers/ConsoleHandler.cpp
        Sources/Src/Debug/Logging/Handlers/FileHandler.cpp
        Sources/Src/Debug/Logging/Handlers/LogHandler.cpp
        Sources/Src/Debug/Logging/Handlers/StreamHandler.cpp
        Sources/Src/Debug/Logging/Logger.cpp
        Sources/Src/ECS/Archetype.cpp
        Sources/Src/ECS/Archetype.inl
        Sources/Src/ECS/ArchetypeFingerprint.inl
        Sources/Src/ECS/Component.inl
        Sources/Src/ECS/ComponentBase.cpp
        Sources/Src/ECS/ComponentQuery.cpp
        Sources/Src/ECS/ComponentQuery.inl
        Sources/Src/ECS/ComponentView.inl
        Sources/Src/ECS/Entity.cpp
        Sources/Src/ECS/EntityAdmin.cpp
        Sources/Src/ECS/EntityAdmin.inl
        Sources/Src/ECS/EventHandler.inl
        Sources/Src/ECS/EventHandlerBase.cpp
        Sources/Src/ECS/Range.cpp
        Sources/Src/ECS/System.cpp
        Sources/Src/ECS/System.inl
        Sources/Src/ECS/TagComponent.cpp
        Sources/Src/Functional/Event.inl
        Sources/Src/Maths/Quaternion/Quaternion.cpp
        Sources/Src/Maths/Quaternion/Quaternion.inl
        Sources/Src/Maths/Easing.cpp
        Sources/Src/Maths/Math.cpp
        Sources/Src/Maths/MinMax.cpp
        Sources/Src/Maths/Trigonometry.cpp
        Sources/Src/Maths/Utility.cpp
        Sources/Src/Rendering/RenderContext.cpp
        Sources/Src/Rendering/Renderer.cpp
        Sources/Src/Rendering/RenderFrame.cpp
        Sources/Src/Rendering/RenderTarget.cpp
        Sources/Src/Rendering/RenderTechnique.cpp
        Sources/Src/Rendering/RenderView.cpp
        Sources/Src/Resource/Exceptions/ResourceProcessingFailure.cpp
        Sources/Src/Resource/Handle.inl
        Sources/Src/Resource/ResourceIdentifier.cpp
        Sources/Src/Resource/ResourceManager.cpp
        Sources/Src/Resource/ResourceManager.inl
        Sources/Src/Resource/ResourceManifest.cpp
        Sources/Src/Threading/ExecutionPlan.cpp
        Sources/Src/Threading/OldWorker.cpp
        Sources/Src/Threading/OldWorker.inl
        Sources/Src/Threading/Scheduler.cpp
        Sources/Src/Threading/Synchronized.inl
        Sources/Src/Threading/SynchronizedAccess.inl
        Sources/Src/Threading/ThreadSafeLockQueue.inl
        Sources/Src/Threading/ThreadSafeQueue.inl
        Sources/Src/Types/Units/Angle/Angle.inl
        Sources/Src/Types/Units/Distance/Distance.inl
        Sources/Src/Types/Units/Duration/Duration.inl
        Sources/Src/Utility/Benchmark.cpp
        Sources/Src/Vulkan/Core/VulkanBuffer.cpp
        Sources/Src/Vulkan/Core/VulkanCommandBuffer.cpp
        Sources/Src/Vulkan/Core/VulkanCommandPool.cpp
        Sources/Src/Vulkan/Core/VulkanDevice.cpp
        Sources/Src/Vulkan/Core/VulkanFence.cpp
        Sources/Src/Vulkan/Core/VulkanImage.cpp
        Sources/Src/Vulkan/Core/VulkanInstance.cpp
        Sources/Src/Vulkan/Core/VulkanPhysicalDevice.cpp
        Sources/Src/Vulkan/Core/VulkanPipelineCache.cpp
        Sources/Src/Vulkan/Core/VulkanQueue.cpp
        Sources/Src/Vulkan/Core/VulkanSemaphore.cpp
        Sources/Src/Vulkan/Core/VulkanShaderModule.cpp
        Sources/Src/Vulkan/Core/VulkanSwapchain.cpp
        Sources/Src/Vulkan/Core/VulkanTimelineSemaphore.cpp
        Sources/Src/Vulkan/Resources/Material.cpp
        Sources/Src/Vulkan/Resources/MaterialLoadingDescriptor.cpp
        Sources/Src/Vulkan/Resources/Mesh.cpp
        Sources/Src/Vulkan/Resources/MeshLoadingDescriptor.cpp
        Sources/Src/Vulkan/Resources/Shader.cpp
        Sources/Src/Vulkan/Resources/ShaderLoadingDescriptor.cpp
        Sources/Src/Vulkan/Resources/Texture.cpp
        Sources/Src/Vulkan/Resources/TextureLoadingDescriptor.cpp
        Sources/Src/Vulkan/Resources/Vertex.cpp
        Sources/Src/Vulkan/Utilities/VulkanDebug.cpp
        Sources/Src/Vulkan/Utilities/VulkanDeviceAllocator.cpp
        Sources/Src/Vulkan/Utilities/VulkanLoader.cpp
        Sources/Src/Vulkan/Utilities/VulkanUtilities.cpp
        Sources/Src/Vulkan/CommandPool.cpp
        Sources/Src/Vulkan/FencePool.cpp
        Sources/Src/Vulkan/SemaphorePool.cpp
        Sources/Src/Windowing/Screen.cpp
        Sources/Src/Windowing/Utilities.cpp
        Sources/Src/Windowing/Window.cpp
        Sources/Src/Windowing/WindowManager.cpp)

target_include_directories(Ruken PUBLIC  ${RUKEN_GENERATED_SOURCES_DIR}/Include)
target_include_directories(Ruken PUBLIC  ${RUKEN_SOURCES_DIR}/Include)
target_include_directories(Ruken PRIVATE ${RUKEN_SOURCES_DIR}/Src)
target_compile_definitions(Ruken PRIVATE RUKEN_PROJECT_NAME="Ruken")
target_compile_options    (Ruken PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/Zc:wchar_t /Zc:char8_t- /Zc:preprocessor /wd5103>
    $<$<CXX_COMPILER_ID:GNU>:>
)


# --- Add dependencies
include(${RUKEN_SOURCES_DIR}/CMake/CPM.cmake)
set(CPM_SOURCE_CACHE ${RUKEN_SOURCES_DIR}/ThirdParty)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)

if (NOT RUKEN_HEADLESS_BUILD)
    find_package(Vulkan REQUIRED)

    CPMAddPackage(URL "https://github.com/glfw/glfw/releases/download/3.3.9/glfw-3.3.9.zip" NAME glfw)
    CPMAddPackage(URL "https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/archive/refs/tags/v3.0.1.zip" NAME vma)
    CPMAddPackage(URL "https://github.com/zeux/volk/archive/refs/tags/1.3.270.zip" NAME volk)
    set(RUKEN_VULKAN_LIBS glfw Vulkan::Vulkan volk vma)
else ()
    set(RUKEN_VULKAN_LIBS)
endif()

CPMAddPackage(URL "https://github.com/max0x7ba/atomic_queue/archive/refs/tags/v1.5.zip" NAME atomic_queue)
CPMAddPackage(URL "https://github.com/g-truc/glm/archive/refs/tags/1.0.0.zip" NAME glm)
CPMAddPackage(URL "https://github.com/tinyobjloader/tinyobjloader/archive/refs/tags/v2.0.0rc13.zip" NAME tinyobjloader)
CPMAddPackage("gh:nothings/stb#f4a71b13373436a2866c5d68f8f80ac6f0bc1ffe")

# vma has no CMake support and is a header only library
if (vma_ADDED)
    FILE(GLOB vma_sources ${vma_SOURCE_DIR}/include/*.h)
    add_library(vma INTERFACE ${vma_sources})
    target_include_directories(vma INTERFACE $<BUILD_INTERFACE:${vma_SOURCE_DIR}/include>)
endif()

# atomic_queue has no CMake support and is a header only library
if (atomic_queue_ADDED)
    FILE(GLOB atomic_queue_sources ${atomic_queue_SOURCE_DIR}/include/atomic_queue/*.h)
    add_library(atomic_queue INTERFACE ${atomic_queue_sources})
    target_include_directories(atomic_queue INTERFACE $<BUILD_INTERFACE:${atomic_queue_SOURCE_DIR}/include>)
endif()

# stb has no CMake support and is a header only library
if (stb_ADDED)
    FILE(GLOB stb_sources ${stb_SOURCE_DIR}/*.h)
    add_library(stb INTERFACE ${stb_sources})
    target_include_directories(stb INTERFACE $<BUILD_INTERFACE:${stb_SOURCE_DIR}>)
endif()

# --- Config file
include(Sources/CMake/CheckGit.cmake)

string(TOUPPER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_TOUPPER)

set(RUKEN_DEBUG_BUILD_TYPES Debug RelWithDebInfo)
set_boolean(RUKEN_BUILD_HAS_DEBUG_INFO ${CMAKE_BUILD_TYPE} IN_LIST RUKEN_DEBUG_BUILD_TYPES)

configure_file(${RUKEN_SOURCES_DIR}/Include/Build/BuildInfo.hpp.in ${RUKEN_GENERATED_SOURCES_DIR}/Include/Build/BuildInfo.hpp)

CheckGitSetup()

# --- Link dependencies
target_link_libraries(Ruken PRIVATE glm::glm atomic_queue stb tinyobjloader git_version ${RUKEN_VULKAN_LIBS})