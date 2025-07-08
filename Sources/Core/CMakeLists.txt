cmake_minimum_required(VERSION 3.16.3)

set(TRACY_ENABLE    ${RUKEN_TRACE_BUILD})
set(TRACY_ON_DEMAND OFF)
set(TRACY_NO_FRAME_IMAGE ON)
set(TRACY_NO_SAMPLING ON)
set(TRACY_NO_VERIFY ON)

CPMAddPackage(URL "https://github.com/wolfpld/tracy/archive/refs/tags/v0.12.1.zip" NAME zq)
CPMAddPackage(URL "https://github.com/max0x7ba/atomic_queue/archive/refs/tags/v1.5.zip" NAME atomic_queue)
CPMAddPackage(URL "https://github.com/tinyobjloader/tinyobjloader/archive/refs/tags/v2.0.0rc13.zip" NAME tinyobjloader)
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
        Sources/Bitwise/Bitmask.hpp
        Sources/Bitwise/Endian.hpp
        Sources/Bitwise/SizedBitmask.hpp
        Sources/Build/Attributes.hpp
        Sources/Build/ProjectInfo.hpp
        Sources/Build/Namespace.hpp
        Sources/Containers/LinkedChunkList.hpp
        Sources/Containers/LinkedChunkListNode.hpp
        Sources/JobSystem/Concepts/CAwaitable.hpp
        Sources/JobSystem/Concepts/CQueueHandle.hpp
        Sources/JobSystem/Awaitables/Primitives/AutomaticResetEvent.hpp
        Sources/JobSystem/Awaitables/Primitives/CountDownLatch.hpp
        Sources/JobSystem/Awaitables/Primitives/ManualResetEvent.hpp
        Sources/JobSystem/Awaitables/Tasks/DynamicTask.hpp
        Sources/JobSystem/Awaitables/Tasks/Task.hpp
        Sources/JobSystem/Awaitables/Tasks/TaskPromise.hpp
        Sources/JobSystem/Awaitables/Awaitable.hpp
        Sources/JobSystem/Awaitables/Tasks/TaskAwaiter.hpp
        Sources/JobSystem/Queues/JobQueue.hpp
        Sources/JobSystem/Queues/DynamicQueue.hpp
        Sources/JobSystem/Queues/QueueHandle.hpp
        Sources/JobSystem/JobSystem.hpp
        Sources/JobSystem/Concurrency.hpp
        Sources/JobSystem/WorkerInfo.hpp
        Sources/Debug/Exception.hpp
        Sources/Core/Service.hpp
        Sources/Core/ServiceLoader.hpp
        Sources/Core/ServiceProvider.hpp
        Sources/Debug/Logging/Filters/LogFilter.hpp
        Sources/Debug/Logging/Formatters/ConsoleFormatter.hpp
        Sources/Debug/Logging/Formatters/FileFormatter.hpp
        Sources/Debug/Logging/Formatters/LogFormatter.hpp
        Sources/Debug/Logging/Handlers/ConsoleHandler.hpp
        Sources/Debug/Logging/Handlers/FileHandler.hpp
        Sources/Debug/Logging/Handlers/LogHandler.hpp
        Sources/Debug/Logging/Handlers/StreamHandler.hpp
        Sources/Debug/Logging/Logger.hpp
        Sources/Debug/Logging/LogLevel.hpp
        Sources/Debug/Logging/LogRecord.hpp
        Sources/Maths/Matrix/Operations/MatrixAccess.hpp
        Sources/Maths/Matrix/Operations/MatrixConversions.hpp
        Sources/Maths/Matrix/Operations/MatrixInversion.hpp
        Sources/Maths/Matrix/Operations/MatrixMultiplication.hpp
        Sources/Maths/Matrix/Operations/MatrixOstream.hpp
        Sources/Maths/Matrix/Operations/MatrixTransposition.hpp
        Sources/Maths/Matrix/Operations/MatrixUtility.hpp
        Sources/Maths/Matrix/Transformations/MatrixClipSpace.hpp
        Sources/Maths/Matrix/Transformations/MatrixLookAt.hpp
        Sources/Maths/Matrix/Transformations/MatrixModel.hpp
        Sources/Maths/Matrix/Transformations/MatrixProjections.hpp
        Sources/Maths/Matrix/Transformations/MatrixRotation.hpp
        Sources/Maths/Matrix/Transformations/MatrixRotationX.hpp
        Sources/Maths/Matrix/Transformations/MatrixRotationY.hpp
        Sources/Maths/Matrix/Transformations/MatrixRotationZ.hpp
        Sources/Maths/Matrix/Transformations/MatrixScale.hpp
        Sources/Maths/Matrix/Transformations/MatrixTranslation.hpp
        Sources/Maths/Matrix/Matrix.hpp
        Sources/Maths/Matrix/MatrixForward.hpp
        Sources/Maths/Quaternion/Quaternion.hpp
        Sources/Maths/Vector/Helper/VectorForward.hpp
        Sources/Maths/Vector/Helper/VectorHelper.hpp
        Sources/Maths/Vector/Operations/VectorCross.hpp
        Sources/Maths/Vector/Operations/VectorDot.hpp
        Sources/Maths/Vector/Operations/VectorLength.hpp
        Sources/Maths/Vector/Operations/VectorLerp.hpp
        Sources/Maths/Vector/Operations/VectorMinMax.hpp
        Sources/Maths/Vector/Operations/VectorNormalization.hpp
        Sources/Maths/Vector/Operations/VectorOperators.hpp
        Sources/Maths/Vector/Operations/VectorSlerp.hpp
        Sources/Maths/Vector/AngleVector2.hpp
        Sources/Maths/Vector/AngleVector3.hpp
        Sources/Maths/Vector/DistanceVector2.hpp
        Sources/Maths/Vector/DistanceVector3.hpp
        Sources/Maths/Vector/PixelVector2.hpp
        Sources/Maths/Vector/PixelVector3.hpp
        Sources/Maths/Constants.hpp
        Sources/Maths/Easing.hpp
        Sources/Maths/Math.hpp
        Sources/Maths/MinMax.hpp
        Sources/Maths/Trigonometry.hpp
        Sources/Maths/Utility.hpp
        Sources/Maths/Vector4f.hpp
        Sources/Debug/Assert.hpp
        Sources/Meta/CommonTypeFallback.hpp
        Sources/Meta/CopyConst.hpp
        Sources/Meta/HasCommonType.hpp
        Sources/Meta/IndexPack.hpp
        Sources/Meta/IsBaseOfTemplate.hpp
        Sources/Meta/IsInstance.hpp
        Sources/Meta/Meta.hpp
        Sources/Meta/MetaForeach.hpp
        Sources/Meta/MinimumType.hpp
        Sources/Meta/PassConst.hpp
        Sources/Meta/Safety.hpp
        Sources/Meta/Tag.hpp
        Sources/Meta/TupleApply.hpp
        Sources/Meta/TupleHasType.hpp
        Sources/Meta/TupleIndex.hpp
        Sources/Meta/TupleRemoveLast.hpp
        Sources/Meta/TupleSubset.hpp
        Sources/Meta/UniqueTuple.hpp
        Sources/Meta/UniqueTypes.hpp
        Sources/Types/Concepts/ArithmeticType.hpp
        Sources/Types/Concepts/FloatingPointType.hpp
        Sources/Types/Concepts/FundamentalType.hpp
        Sources/Types/StrongType/Operators/Arithmetic/Addition.hpp
        Sources/Types/StrongType/Operators/Arithmetic/Decrement.hpp
        Sources/Types/StrongType/Operators/Arithmetic/Division.hpp
        Sources/Types/StrongType/Operators/Arithmetic/Increment.hpp
        Sources/Types/StrongType/Operators/Arithmetic/Modulo.hpp
        Sources/Types/StrongType/Operators/Arithmetic/Multiplication.hpp
        Sources/Types/StrongType/Operators/Arithmetic/Subtraction.hpp
        Sources/Types/StrongType/Operators/Arithmetic/UnaryMinus.hpp
        Sources/Types/StrongType/Operators/Arithmetic/UnaryPlus.hpp
        Sources/Types/StrongType/Operators/Bitwise/And.hpp
        Sources/Types/StrongType/Operators/Bitwise/LeftShift.hpp
        Sources/Types/StrongType/Operators/Bitwise/Not.hpp
        Sources/Types/StrongType/Operators/Bitwise/Or.hpp
        Sources/Types/StrongType/Operators/Bitwise/RightShift.hpp
        Sources/Types/StrongType/Operators/Bitwise/Xor.hpp
        Sources/Types/StrongType/Operators/Comparison/Equal.hpp
        Sources/Types/StrongType/Operators/Comparison/Greater.hpp
        Sources/Types/StrongType/Operators/Comparison/GreaterEqual.hpp
        Sources/Types/StrongType/Operators/Comparison/Less.hpp
        Sources/Types/StrongType/Operators/Comparison/LessEqual.hpp
        Sources/Types/StrongType/Operators/Comparison/NotEqual.hpp
        Sources/Types/StrongType/Operators/Comparison/Spaceship.hpp
        Sources/Types/StrongType/Operators/Logical/LogicalAnd.hpp
        Sources/Types/StrongType/Operators/Logical/LogicalNot.hpp
        Sources/Types/StrongType/Operators/Logical/LogicalOr.hpp
        Sources/Types/StrongType/Operators/Stream/InputStream.hpp
        Sources/Types/StrongType/Operators/Stream/OutputStream.hpp
        Sources/Types/StrongType/Operators/Arithmetic.hpp
        Sources/Types/StrongType/Operators/Bitwise.hpp
        Sources/Types/StrongType/Operators/Comparison.hpp
        Sources/Types/StrongType/Operators/Logical.hpp
        Sources/Types/StrongType/Operators/Stream.hpp
        Sources/Types/StrongType/StrongType.hpp
        Sources/Types/StrongType/StrongTypeSuffix.hpp
        Sources/Types/Units/Angle/Angle.hpp
        Sources/Types/Units/Angle/EAngleUnit.hpp
        Sources/Types/Units/Distance/Distance.hpp
        Sources/Types/Units/Distance/EDistanceUnit.hpp
        Sources/Types/Units/Duration/Duration.hpp
        Sources/Types/Units/Duration/EDurationUnit.hpp
        Sources/Types/Units/Pixels.hpp
        Sources/Types/FundamentalTypes.hpp
        Sources/Types/NonVoid.hpp
        Sources/Utility/Benchmark.hpp
        Sources/Utility/Todo.hpp
        Sources/Utility/WindowsOS.hpp
        Sources/Bitwise/Bitmask.inl
        Sources/Bitwise/SizedBitmask.inl
        Sources/Containers/LinkedChunkList.inl
        Sources/JobSystem/Awaitables/Primitives/CountDownLatch.cpp
        Sources/JobSystem/Awaitables/Task.inl
        Sources/JobSystem/JobQueue.cpp
        Sources/JobSystem/JobSystem.cpp
        Sources/Core/Service.inl
        Sources/Core/ServiceProvider.cpp
        Sources/Core/ServiceProvider.inl
        Sources/Debug/Logging/Formatters/ConsoleFormatter.cpp
        Sources/Debug/Logging/Formatters/LogFormatter.cpp
        Sources/Debug/Logging/Handlers/ConsoleHandler.cpp
        Sources/Debug/Logging/Handlers/FileHandler.cpp
        Sources/Debug/Logging/Handlers/LogHandler.cpp
        Sources/Debug/Logging/Handlers/StreamHandler.cpp
        Sources/Debug/Logging/Logger.cpp
        Sources/Maths/Quaternion/Quaternion.cpp
        Sources/Maths/Quaternion/Quaternion.inl
        Sources/Maths/Easing.cpp
        Sources/Maths/Math.cpp
        Sources/Maths/MinMax.cpp
        Sources/Maths/Trigonometry.cpp
        Sources/Maths/Utility.cpp
        Sources/Types/Units/Angle/Angle.inl
        Sources/Types/Units/Distance/Distance.inl
        Sources/Types/Units/Duration/Duration.inl
        Sources/Utility/Benchmark.cpp
        Sources/Debug/Trace.cpp
        Sources/Debug/Trace.hpp
        Sources/Debug/SourceLocation.hpp
        Sources/JobSystem/Awaitables/TaskPromise.inl
        Sources/JobSystem/Awaitables/Awaitable.cpp
        Sources/Meta/FirstOf.hpp
        Sources/JobSystem/Awaitables/Primitives/WhenAll.hpp
        Sources/JobSystem/Awaitables/SignalReceiver.hpp
        Sources/JobSystem/Awaitables/Awaiter.hpp
        Sources/JobSystem/Awaitables/Primitives/SharedMutex.hpp
        Sources/JobSystem/Awaitables/Awaiter.cpp
        Sources/JobSystem/Awaitables/Primitives/SharedMutex.inl
        Sources/JobSystem/Awaitables/Primitives/ManualResetEvent.cpp
        Sources/JobSystem/Awaitables/Primitives/AutomaticResetEvent.cpp
        Sources/JobSystem/Awaitables/Primitives/AutomaticResetEvent.inl
        Sources/JobSystem/Concepts/CTaskResult.hpp
        Sources/Debug/Logging/Handlers/DebugHandler.cpp
        Sources/Debug/Logging/Handlers/DebugHandler.hpp
        Sources/Core/Service.cpp
        Sources/Debug/Logging/Logger.inl
        Sources/Containers/BinaryTree.hpp
        Sources/JobSystem/WorkerRequestTree.hpp
        Sources/Containers/BinaryTreePath.hpp
        Sources/Containers/BinaryTreePath.cpp
        Sources/Containers/BinaryTree.inl
        Sources/Debug/Exception.cpp
)

target_include_directories(RkCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Sources)
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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Sources/Build/BuildInfo.hpp.in ${RUKEN_GENERATED_SOURCES_DIR}/Include/Build/BuildInfo.hpp)
CheckGitSetup()

# --- Link dependencies
target_link_libraries(RkCore PUBLIC atomic_queue git_version stb tinyobjloader TracyClient)