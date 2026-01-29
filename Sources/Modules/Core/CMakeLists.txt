cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkModule.cmake)
register_ruken_module(RkCore)

set(TRACY_ENABLE    ${RUKEN_TRACE_BUILD})
set(TRACY_NO_VERIFY ON)

CPMAddPackage(URL "https://github.com/wolfpld/tracy/archive/refs/tags/v0.13.0.zip"      NAME tracy)
CPMAddPackage(URL "https://github.com/max0x7ba/atomic_queue/archive/refs/tags/v1.5.zip" NAME atomic_queue)

# atomic_queue has no CMake support and is a header only library
if (atomic_queue_ADDED)
    FILE(GLOB atomic_queue_sources ${atomic_queue_SOURCE_DIR}/include/atomic_queue/*.h)
    add_library(atomic_queue INTERFACE ${atomic_queue_sources})
    target_include_directories(atomic_queue INTERFACE $<BUILD_INTERFACE:${atomic_queue_SOURCE_DIR}/include>)
endif()

target_sources(RkCore PUBLIC
        FILE_SET HEADERS FILES
    JobSystem/Awaitables/AwaitableTraits.hpp
    Time/Clock.hpp
    Bitwise/Bitmask.hpp
    Bitwise/Endian.hpp
    Bitwise/SizedBitmask.hpp
    Build/Attributes.hpp
    Build/ProjectInfo.hpp
    Build/Namespace.hpp
    JobSystem/Queues/IsQueueHandle.hpp
    JobSystem/Awaitables/AsyncTask/AsyncTaskAwaiter.inl
    JobSystem/Awaitables/Primitives/AutomaticResetEvent.hpp
    JobSystem/Awaitables/Primitives/CountDownLatch.hpp
    JobSystem/Awaitables/Primitives/ManualResetEvent.hpp
    JobSystem/Awaitables/Primitives/Mutex.hpp
    JobSystem/Awaitables/AsyncTask/DynamicTask.hpp
    JobSystem/Awaitables/AsyncTask/AsyncTask.hpp
    JobSystem/Awaitables/AsyncTask/AsyncTaskPromise.hpp
    JobSystem/Awaitables/AsyncAwaitable.hpp
    JobSystem/Awaitables/AsyncTask/AsyncTaskAwaiter.hpp
    JobSystem/Awaitables/Primitives/ParallelFor.hpp
    JobSystem/Awaitables/Primitives/ParallelForEach.hpp
    JobSystem/Queues/JobQueue.hpp
    JobSystem/Queues/DynamicQueue.hpp
    JobSystem/Queues/QueueHandle.hpp
    JobSystem/JobSystem.hpp
    JobSystem/Concurrency.hpp
    JobSystem/WorkerInfo.hpp
    JobSystem/Executors/SingleThreadSingleQueueExecutor.hpp
    JobSystem/Awaitables/SyncTask/SyncTaskAwaiter.hpp
    JobSystem/Awaitables/SyncTask/SyncTaskAwaiter.inl
    JobSystem/Awaitables/SyncTask/SyncTaskPromise.hpp
    JobSystem/Awaitables/SyncTask/SyncTaskPromise.inl
    JobSystem/Awaitables/SyncTask/SyncTask.hpp
    JobSystem/Awaitables/SyncTask/SyncTask.inl
    Debug/Exception.hpp
    Service.hpp
    ServiceLoader.hpp
    ServiceProvider.hpp
    Debug/Logging/Filters/LogFilter.hpp
    Debug/Logging/Formatters/ConsoleFormatter.hpp
    Debug/Logging/Formatters/FileFormatter.hpp
    Debug/Logging/Formatters/LogFormatter.hpp
    Debug/Logging/Handlers/ConsoleHandler.hpp
    Debug/Logging/Handlers/FileHandler.hpp
    Debug/Logging/Handlers/LogHandler.hpp
    Debug/Logging/Handlers/StreamHandler.hpp
    Debug/Logging/Logger.hpp
    Debug/Logging/LogLevel.hpp
    Debug/Logging/LogRecord.hpp
    Maths/Matrix/Operations/MatrixInversion.hpp
    Maths/Matrix/Operations/MatrixOstream.hpp
    Maths/Matrix/Operations/MatrixTransposition.hpp
    Maths/Matrix/Matrix.hpp
    Maths/Matrix/MatrixForward.hpp
    Maths/Quaternion/Quaternion.hpp
    Maths/Vector/Helper/VectorForward.hpp
    Maths/Vector/Helper/VectorHelper.hpp
    Maths/Vector/Operations/VectorCross.hpp
    Maths/Vector/Operations/VectorDot.hpp
    Maths/Vector/Operations/VectorLength.hpp
    Maths/Vector/Operations/VectorLerp.hpp
    Maths/Vector/Operations/VectorMinMax.hpp
    Maths/Vector/Operations/VectorNormalization.hpp
    Maths/Vector/Operations/VectorOperators.hpp
    Maths/Vector/Operations/VectorSlerp.hpp
    Maths/Vector/AngleVector.hpp
    Maths/Vector/DistanceVector.hpp
    Maths/Vector/PixelVector.hpp
    Maths/Constants.hpp
    Maths/Easing.hpp
    Maths/Math.hpp
    Maths/MinMax.hpp
    Maths/Trigonometry.hpp
    Maths/Utility.hpp
    Maths/Vector4f.hpp
    Debug/Assert.hpp
    Meta/CommonTypeFallback.hpp
    Meta/CopyConst.hpp
    Meta/HasCommonType.hpp
    Meta/IndexPack.hpp
    Meta/IsBaseOfTemplate.hpp
    Meta/IsInstance.hpp
    Meta/Meta.hpp
    Meta/MetaForeach.hpp
    Meta/MinimumType.hpp
    Meta/PassConst.hpp
    Meta/Safety.hpp
        Meta/Empty.hpp
    Meta/TupleApply.hpp
    Meta/TupleHasType.hpp
    Meta/TupleIndex.hpp
    Meta/TupleRemoveLast.hpp
    Meta/TupleSubset.hpp
    Meta/UniqueTuple.hpp
    Meta/UniqueTypes.hpp
    Types/Concepts/ArithmeticType.hpp
    Types/Concepts/FloatingPointType.hpp
    Types/Concepts/FundamentalType.hpp
    Types/StrongType/Operators/Arithmetic/Addition.hpp
    Types/StrongType/Operators/Arithmetic/Decrement.hpp
    Types/StrongType/Operators/Arithmetic/Division.hpp
    Types/StrongType/Operators/Arithmetic/Increment.hpp
    Types/StrongType/Operators/Arithmetic/Modulo.hpp
    Types/StrongType/Operators/Arithmetic/Multiplication.hpp
    Types/StrongType/Operators/Arithmetic/Subtraction.hpp
    Types/StrongType/Operators/Arithmetic/UnaryMinus.hpp
    Types/StrongType/Operators/Arithmetic/UnaryPlus.hpp
    Types/StrongType/Operators/Bitwise/And.hpp
    Types/StrongType/Operators/Bitwise/LeftShift.hpp
    Types/StrongType/Operators/Bitwise/Not.hpp
    Types/StrongType/Operators/Bitwise/Or.hpp
    Types/StrongType/Operators/Bitwise/RightShift.hpp
    Types/StrongType/Operators/Bitwise/Xor.hpp
    Types/StrongType/Operators/Comparison/Equal.hpp
    Types/StrongType/Operators/Comparison/Greater.hpp
    Types/StrongType/Operators/Comparison/GreaterEqual.hpp
    Types/StrongType/Operators/Comparison/Less.hpp
    Types/StrongType/Operators/Comparison/LessEqual.hpp
    Types/StrongType/Operators/Comparison/NotEqual.hpp
    Types/StrongType/Operators/Comparison/Spaceship.hpp
    Types/StrongType/Operators/Logical/LogicalAnd.hpp
    Types/StrongType/Operators/Logical/LogicalNot.hpp
    Types/StrongType/Operators/Logical/LogicalOr.hpp
    Types/StrongType/Operators/Stream/InputStream.hpp
    Types/StrongType/Operators/Stream/OutputStream.hpp
    Types/StrongType/Operators/Arithmetic.hpp
    Types/StrongType/Operators/Bitwise.hpp
    Types/StrongType/Operators/Comparison.hpp
    Types/StrongType/Operators/Logical.hpp
    Types/StrongType/Operators/Stream.hpp
    Types/StrongType/StrongType.hpp
    Types/StrongType/StrongTypeSuffix.hpp
    Types/Units/Angle/Angle.hpp
    Types/Units/Angle/EAngleUnit.hpp
    Types/Units/Distance/Distance.hpp
    Types/Units/Distance/EDistanceUnit.hpp
    Types/Units/Duration/Duration.hpp
    Types/Units/Duration/EDurationUnit.hpp
    Types/Units/Pixels.hpp
    Types/FundamentalTypes.hpp
    Types/Concepts/IsIntegral.hpp
    Types/Concepts/NonVoid.hpp
    Utility/Benchmark.hpp
    Utility/Todo.hpp
    Utility/WindowsOS.hpp
    Bitwise/Bitmask.inl
    Bitwise/SizedBitmask.inl
        JobSystem/Awaitables/AsyncTask/AsyncTask.inl
        JobSystem/Awaitables/SyncTask/SyncTaskResult.hpp
    Service.inl
    ServiceProvider.inl
    Maths/Quaternion/Quaternion.inl
    Types/Units/Angle/Angle.inl
    Types/Units/Distance/Distance.inl
    Types/Units/Duration/Duration.inl
    Debug/Trace.hpp
    Debug/SourceLocation.hpp
        JobSystem/Awaitables/AsyncTask/AsyncTaskPromise.inl
    Meta/FirstOf.hpp
    JobSystem/Awaitables/Primitives/WhenAll.hpp
    JobSystem/Awaitables/SignalReceiver.hpp
        JobSystem/Awaitables/AsyncAwaiter.hpp
    JobSystem/Awaitables/Primitives/SharedMutex.hpp
    JobSystem/Awaitables/Primitives/SharedMutex.inl
    JobSystem/Awaitables/Primitives/AutomaticResetEvent.inl
    JobSystem/Awaitables/Primitives/Mutex.inl
    Debug/Logging/Handlers/DebugHandler.hpp
    Debug/Logging/Logger.inl
    Containers/BinaryTree.hpp
    JobSystem/WorkerRequestTree.hpp
    Containers/BinaryTreePath.hpp
    Containers/BinaryTree.inl
)

target_sources(RkCore PRIVATE
    Time/Clock.cpp
    JobSystem/Awaitables/Primitives/CountDownLatch.cpp
    JobSystem/JobQueue.cpp
    JobSystem/JobSystem.cpp
    ServiceProvider.cpp
    Debug/Logging/Formatters/ConsoleFormatter.cpp
    Debug/Logging/Formatters/LogFormatter.cpp
    Debug/Logging/Handlers/ConsoleHandler.cpp
    Debug/Logging/Handlers/FileHandler.cpp
    Debug/Logging/Handlers/LogHandler.cpp
    Debug/Logging/Handlers/StreamHandler.cpp
    Debug/Logging/Logger.cpp
    Maths/Quaternion/Quaternion.cpp
    Maths/Easing.cpp
    Maths/Math.cpp
    Maths/MinMax.cpp
    Maths/Trigonometry.cpp
    Maths/Utility.cpp
    Utility/Benchmark.cpp
    Debug/Trace.cpp
    JobSystem/Awaitables/AsyncAwaitable.cpp
    JobSystem/Awaitables/AsyncAwaiter.cpp
    JobSystem/Awaitables/Primitives/ManualResetEvent.cpp
    JobSystem/Awaitables/Primitives/AutomaticResetEvent.cpp
    JobSystem/Executors/SingleThreadSingleQueueExecutor.cpp
    Debug/Logging/Handlers/DebugHandler.cpp
    Service.cpp
    Containers/BinaryTreePath.cpp
    Debug/Exception.cpp
)

# --- Config file
include(${RUKEN_SOURCES_DIR}/CMake/CheckGit.cmake)

string(TOUPPER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_TOUPPER)

configure_file(Build/BuildInfo.hpp.in ${RUKEN_GENERATED_SOURCES_DIR}/Build/BuildInfo.hpp)
target_include_directories(RkCore PUBLIC ${RUKEN_GENERATED_SOURCES_DIR}) # TODO: THIS IS A WORKAROUND. Config files should be handled by the module
CheckGitSetup()

# --- Link dependencies
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(RkCore PUBLIC
            -fsized-deallocation                     # required by libbacktrace
            --param=destructive-interference-size=64 # makes sure consumers have the same value
    )
    target_link_libraries (RkCore PUBLIC -lstdc++exp) # Prior to GCC14 you should be using -lstdc++_libbacktrace instead
endif ()

target_link_libraries(RkCore PUBLIC atomic_queue git_version TracyClient)