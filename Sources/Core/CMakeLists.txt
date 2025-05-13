cmake_minimum_required(VERSION 3.16.3)

set(TRACY_ENABLE    ${RUKEN_TRACE_BUILD})
set(TRACY_ON_DEMAND OFF)
set(TRACY_NO_FRAME_IMAGE ON)
set(TRACY_NO_SAMPLING ON)
set(TRACY_NO_VERIFY ON)

CPMAddPackage(URL "https://github.com/wolfpld/tracy/archive/refs/tags/v0.11.1.zip" NAME tracy)
CPMAddPackage(URL "https://github.com/max0x7ba/atomic_queue/archive/refs/tags/v1.5.zip" NAME atomic_queue)
CPMAddPackage(URL "https://github.com/tinyobjloader/tinyobjloader/archive/refs/tags/v2.0.0rc13.zip" NAME tinyobjloader)
CPMAddPackage(URL "https://github.com/Neargye/magic_enum/archive/refs/tags/v0.9.6.zip" NAME magic_enum)
CPMAddPackage(URL "https://github.com/nothings/stb/archive/refs/heads/master.zip" NAME stb)

# stb has no CMake support and is a header only library
if (stb_ADDED)
    FILE(GLOB stb_sources ${stb_SOURCE_DIR}/*.h)
    add_library(stb INTERFACE ${stb_sources})
    target_include_directories(stb INTERFACE $<BUILD_INTERFACE:${stb_SOURCE_DIR}>)
endif()

# atomic_queue has no CMake support and is a header only library
if (atomic_queue_ADDED)
    FILE(GLOB atomic_queue_sources ${atomic_queue_SOURCE_DIR}/include/atomic_queue/*.h)
    add_library(atomic_queue INTERFACE ${atomic_queue_sources})
    target_include_directories(atomic_queue INTERFACE $<BUILD_INTERFACE:${atomic_queue_SOURCE_DIR}/include>)
endif()

add_library(RkCore STATIC
        Include/Bitwise/Bitmask.hpp
        Include/Bitwise/Endian.hpp
        Include/Bitwise/SizedBitmask.hpp
        Include/Build/Attributes.hpp
        Include/Build/ProjectInfo.hpp
        Include/Build/Namespace.hpp
        Include/Containers/LinkedChunkList.hpp
        Include/Containers/LinkedChunkListNode.hpp
        Include/ExecutiveSystem/Concepts/CAwaitable.hpp
        Include/ExecutiveSystem/Concepts/CQueueHandle.hpp
        Include/ExecutiveSystem/Awaitables/Primitives/AutomaticResetEvent.hpp
        Include/ExecutiveSystem/Awaitables/Primitives/CountDownLatch.hpp
        Include/ExecutiveSystem/Awaitables/Primitives/ManualResetEvent.hpp
        Include/ExecutiveSystem/Awaitables/Tasks/DynamicTask.hpp
        Include/ExecutiveSystem/Awaitables/Tasks/Task.hpp
        Include/ExecutiveSystem/Awaitables/Tasks/TaskPromise.hpp
        Include/ExecutiveSystem/Awaitables/Awaitable.hpp
        Include/ExecutiveSystem/Awaitables/Tasks/TaskAwaiter.hpp
        Include/ExecutiveSystem/Queues/JobQueue.hpp
        Include/ExecutiveSystem/Queues/DynamicQueue.hpp
        Include/ExecutiveSystem/Queues/QueueHandle.hpp
        Include/ExecutiveSystem/JobSystem.hpp
        Include/ExecutiveSystem/ConcurrencyCounter.hpp
        Include/ExecutiveSystem/Worker.hpp
        Include/ExecutiveSystem/WorkerInfo.hpp
        Include/Core/EInitializationStatus.hpp
        Include/Core/Exception.hpp
        Include/Core/Service.hpp
        Include/Core/ServiceBase.hpp
        Include/Core/ServiceLoader.hpp
        Include/Core/ServiceProvider.hpp
        Include/Core/ServiceType.hpp
        Include/Debug/Logging/Filters/LogFilter.hpp
        Include/Debug/Logging/Formatters/ConsoleFormatter.hpp
        Include/Debug/Logging/Formatters/FileFormatter.hpp
        Include/Debug/Logging/Formatters/LogFormatter.hpp
        Include/Debug/Logging/Handlers/ConsoleHandler.hpp
        Include/Debug/Logging/Handlers/FileHandler.hpp
        Include/Debug/Logging/Handlers/LogHandler.hpp
        Include/Debug/Logging/Handlers/StreamHandler.hpp
        Include/Debug/Logging/Logger.hpp
        Include/Debug/Logging/LogLevel.hpp
        Include/Debug/Logging/LogRecord.hpp
        Include/Functional/Event.hpp
        Include/Functional/ReservedEvent.hpp
        Include/Maths/Matrix/Operations/MatrixAccess.hpp
        Include/Maths/Matrix/Operations/MatrixConversions.hpp
        Include/Maths/Matrix/Operations/MatrixInversion.hpp
        Include/Maths/Matrix/Operations/MatrixMultiplication.hpp
        Include/Maths/Matrix/Operations/MatrixOstream.hpp
        Include/Maths/Matrix/Operations/MatrixTransposition.hpp
        Include/Maths/Matrix/Operations/MatrixUtility.hpp
        Include/Maths/Matrix/Transformations/MatrixClipSpace.hpp
        Include/Maths/Matrix/Transformations/MatrixLookAt.hpp
        Include/Maths/Matrix/Transformations/MatrixModel.hpp
        Include/Maths/Matrix/Transformations/MatrixProjections.hpp
        Include/Maths/Matrix/Transformations/MatrixRotation.hpp
        Include/Maths/Matrix/Transformations/MatrixRotationX.hpp
        Include/Maths/Matrix/Transformations/MatrixRotationY.hpp
        Include/Maths/Matrix/Transformations/MatrixRotationZ.hpp
        Include/Maths/Matrix/Transformations/MatrixScale.hpp
        Include/Maths/Matrix/Transformations/MatrixTranslation.hpp
        Include/Maths/Matrix/Matrix.hpp
        Include/Maths/Matrix/MatrixForward.hpp
        Include/Maths/Quaternion/Quaternion.hpp
        Include/Maths/Vector/Helper/VectorForward.hpp
        Include/Maths/Vector/Helper/VectorHelper.hpp
        Include/Maths/Vector/Operations/VectorCross.hpp
        Include/Maths/Vector/Operations/VectorDot.hpp
        Include/Maths/Vector/Operations/VectorLength.hpp
        Include/Maths/Vector/Operations/VectorLerp.hpp
        Include/Maths/Vector/Operations/VectorMinMax.hpp
        Include/Maths/Vector/Operations/VectorNormalization.hpp
        Include/Maths/Vector/Operations/VectorOperators.hpp
        Include/Maths/Vector/Operations/VectorSlerp.hpp
        Include/Maths/Vector/AngleVector2.hpp
        Include/Maths/Vector/AngleVector3.hpp
        Include/Maths/Vector/DistanceVector2.hpp
        Include/Maths/Vector/DistanceVector3.hpp
        Include/Maths/Vector/PixelVector2.hpp
        Include/Maths/Vector/PixelVector3.hpp
        Include/Maths/Constants.hpp
        Include/Maths/Easing.hpp
        Include/Maths/Math.hpp
        Include/Maths/MinMax.hpp
        Include/Maths/Trigonometry.hpp
        Include/Maths/Utility.hpp
        Include/Maths/Vector4f.hpp
        Include/Meta/Assert.hpp
        Include/Meta/CommonTypeFallback.hpp
        Include/Meta/CopyConst.hpp
        Include/Meta/HasCommonType.hpp
        Include/Meta/IndexPack.hpp
        Include/Meta/IsBaseOfTemplate.hpp
        Include/Meta/IsInstance.hpp
        Include/Meta/Meta.hpp
        Include/Meta/MetaForeach.hpp
        Include/Meta/MinimumType.hpp
        Include/Meta/PassConst.hpp
        Include/Meta/Safety.hpp
        Include/Meta/Tag.hpp
        Include/Meta/TupleApply.hpp
        Include/Meta/TupleHasType.hpp
        Include/Meta/TupleIndex.hpp
        Include/Meta/TupleRemoveLast.hpp
        Include/Meta/TupleSubset.hpp
        Include/Meta/UniqueTuple.hpp
        Include/Meta/UniqueTypes.hpp
        Include/Threading/EAccessMode.hpp
        Include/Threading/ESynchronizationMode.hpp
        Include/Threading/ExecutionPlan.hpp
        Include/Threading/Job.hpp
        Include/Threading/OldWorker.hpp
        Include/Threading/Scheduler.hpp
        Include/Threading/Synchronized.hpp
        Include/Threading/SynchronizedAccess.hpp
        Include/Threading/ThreadSafeLockQueue.hpp
        Include/Threading/ThreadSafeQueue.hpp
        Include/Types/Concepts/ArithmeticType.hpp
        Include/Types/Concepts/FloatingPointType.hpp
        Include/Types/Concepts/FundamentalType.hpp
        Include/Types/StrongType/Operators/Arithmetic/Addition.hpp
        Include/Types/StrongType/Operators/Arithmetic/Decrement.hpp
        Include/Types/StrongType/Operators/Arithmetic/Division.hpp
        Include/Types/StrongType/Operators/Arithmetic/Increment.hpp
        Include/Types/StrongType/Operators/Arithmetic/Modulo.hpp
        Include/Types/StrongType/Operators/Arithmetic/Multiplication.hpp
        Include/Types/StrongType/Operators/Arithmetic/Subtraction.hpp
        Include/Types/StrongType/Operators/Arithmetic/UnaryMinus.hpp
        Include/Types/StrongType/Operators/Arithmetic/UnaryPlus.hpp
        Include/Types/StrongType/Operators/Bitwise/And.hpp
        Include/Types/StrongType/Operators/Bitwise/LeftShift.hpp
        Include/Types/StrongType/Operators/Bitwise/Not.hpp
        Include/Types/StrongType/Operators/Bitwise/Or.hpp
        Include/Types/StrongType/Operators/Bitwise/RightShift.hpp
        Include/Types/StrongType/Operators/Bitwise/Xor.hpp
        Include/Types/StrongType/Operators/Comparison/Equal.hpp
        Include/Types/StrongType/Operators/Comparison/Greater.hpp
        Include/Types/StrongType/Operators/Comparison/GreaterEqual.hpp
        Include/Types/StrongType/Operators/Comparison/Less.hpp
        Include/Types/StrongType/Operators/Comparison/LessEqual.hpp
        Include/Types/StrongType/Operators/Comparison/NotEqual.hpp
        Include/Types/StrongType/Operators/Comparison/Spaceship.hpp
        Include/Types/StrongType/Operators/Logical/LogicalAnd.hpp
        Include/Types/StrongType/Operators/Logical/LogicalNot.hpp
        Include/Types/StrongType/Operators/Logical/LogicalOr.hpp
        Include/Types/StrongType/Operators/Stream/InputStream.hpp
        Include/Types/StrongType/Operators/Stream/OutputStream.hpp
        Include/Types/StrongType/Operators/Arithmetic.hpp
        Include/Types/StrongType/Operators/Bitwise.hpp
        Include/Types/StrongType/Operators/Comparison.hpp
        Include/Types/StrongType/Operators/Logical.hpp
        Include/Types/StrongType/Operators/Stream.hpp
        Include/Types/StrongType/StrongType.hpp
        Include/Types/StrongType/StrongTypeSuffix.hpp
        Include/Types/Units/Angle/Angle.hpp
        Include/Types/Units/Angle/EAngleUnit.hpp
        Include/Types/Units/Distance/Distance.hpp
        Include/Types/Units/Distance/EDistanceUnit.hpp
        Include/Types/Units/Duration/Duration.hpp
        Include/Types/Units/Duration/EDurationUnit.hpp
        Include/Types/Units/Pixels.hpp
        Include/Types/FundamentalTypes.hpp
        Include/Types/NonVoid.hpp
        Include/Utility/Benchmark.hpp
        Include/Utility/Todo.hpp
        Include/Utility/WindowsOS.hpp
        Src/Bitwise/Bitmask.inl
        Src/Bitwise/SizedBitmask.inl
        Src/Containers/LinkedChunkList.inl
        Src/ExecutiveSystem/Awaitables/Primitives/CountDownLatch.cpp
        Src/ExecutiveSystem/Awaitables/Task.inl
        Src/ExecutiveSystem/JobQueue.cpp
        Src/ExecutiveSystem/JobSystem.cpp
        Src/ExecutiveSystem/Worker.cpp
        Src/Core/Service.inl
        Src/Core/ServiceBase.cpp
        Src/Core/ServiceProvider.cpp
        Src/Core/ServiceProvider.inl
        Src/Debug/Logging/Formatters/ConsoleFormatter.cpp
        Src/Debug/Logging/Formatters/LogFormatter.cpp
        Src/Debug/Logging/Handlers/ConsoleHandler.cpp
        Src/Debug/Logging/Handlers/FileHandler.cpp
        Src/Debug/Logging/Handlers/LogHandler.cpp
        Src/Debug/Logging/Handlers/StreamHandler.cpp
        Src/Debug/Logging/Logger.cpp
        Src/Functional/Event.inl
        Src/Maths/Quaternion/Quaternion.cpp
        Src/Maths/Quaternion/Quaternion.inl
        Src/Maths/Easing.cpp
        Src/Maths/Math.cpp
        Src/Maths/MinMax.cpp
        Src/Maths/Trigonometry.cpp
        Src/Maths/Utility.cpp
        Src/Threading/ExecutionPlan.cpp
        Src/Threading/OldWorker.cpp
        Src/Threading/OldWorker.inl
        Src/Threading/Scheduler.cpp
        Src/Threading/Synchronized.inl
        Src/Threading/SynchronizedAccess.inl
        Src/Threading/ThreadSafeLockQueue.inl
        Src/Threading/ThreadSafeQueue.inl
        Src/Types/Units/Angle/Angle.inl
        Src/Types/Units/Distance/Distance.inl
        Src/Types/Units/Duration/Duration.inl
        Src/Utility/Benchmark.cpp
        Src/Debug/Trace.cpp
        Src/Resource/Handle.inl
        Src/Resource/ResourceManager.inl
        Include/Resource/Enums/EGCCollectionMode.hpp
        Include/Resource/Enums/EResourceGCStrategy.hpp
        Include/Resource/Enums/EResourceLoadingFailureCode.hpp
        Include/Resource/Enums/EResourceStatus.hpp
        Include/Resource/Handle.hpp
        Include/Resource/IResource.hpp
        Include/Resource/ResourceIdentifier.hpp
        Include/Resource/ResourceLoadingDescriptor.hpp
        Include/Resource/ResourceManager.hpp
        Include/Resource/ResourceManifest.hpp
        Include/Resource/ResourceProcessingFailure.hpp
        Include/Debug/Trace.hpp
        Include/Debug/SourceLocation.hpp
        Src/ExecutiveSystem/Awaitables/TaskPromise.inl
        Src/ExecutiveSystem/Awaitables/Awaitable.cpp
        Include/Meta/FirstOf.hpp
        Include/ExecutiveSystem/Awaitables/Primitives/WhenAll.hpp
        Include/ExecutiveSystem/Awaitables/Signal.hpp
        Include/ExecutiveSystem/Awaitables/Awaiter.hpp
        Include/ExecutiveSystem/Awaitables/Primitives/SharedMutex.hpp
        Src/ExecutiveSystem/Awaitables/Awaiter.cpp
        Src/ExecutiveSystem/Awaitables/Primitives/SharedMutex.inl
        Src/ExecutiveSystem/Awaitables/Primitives/ManualResetEvent.cpp
        Src/ExecutiveSystem/Awaitables/Primitives/AutomaticResetEvent.cpp
        Src/ExecutiveSystem/Awaitables/Primitives/AutomaticResetEvent.inl
        Include/ExecutiveSystem/Concepts/CTaskResult.hpp
)

target_include_directories(RkCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
target_include_directories(RkCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Src)
target_include_directories(RkCore PUBLIC ${RUKEN_GENERATED_SOURCES_DIR}/Include)
target_include_directories(RkCore PUBLIC ${RUKEN_GENERATED_SOURCES_DIR}/Services)

if(MSVC)
    target_compile_options(RkCore PRIVATE /W4)
else()
    target_compile_options(RkCore PRIVATE -Wall -Wextra -Wpedantic -fsized-deallocation -no-pie)
    target_link_libraries (RkCore PUBLIC stdc++exp)
endif()

# --- Config file
include(${RUKEN_SOURCES_DIR}/CMake/CheckGit.cmake)

string(TOUPPER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_TOUPPER)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Include/Build/BuildInfo.hpp.in ${RUKEN_GENERATED_SOURCES_DIR}/Include/Build/BuildInfo.hpp)
CheckGitSetup()

# --- Link dependencies
target_link_libraries(RkCore PUBLIC atomic_queue git_version stb tinyobjloader TracyClient magic_enum)