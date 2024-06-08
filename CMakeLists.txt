cmake_minimum_required(VERSION 3.16.3)

# --- Language setup
set(CMAKE_CXX_STANDARD 23)

# --- Project setup
project(Ruken)

macro(set_bool var)
    if(${ARGN})
        set(${var} ON)
    else()
        set(${var} OFF)
    endif()
endmacro()

set     (RUKEN_DEBUG_BUILD_TYPES     Debug RelWithDebInfo)
set_bool(RUKEN_BUILD_HAS_DEBUG_INFO  ${CMAKE_BUILD_TYPE} IN_LIST RUKEN_DEBUG_BUILD_TYPES)
set     (RUKEN_GENERATED_SOURCES_DIR ${CMAKE_BINARY_DIR}/Generated)
set     (RUKEN_SOURCES_DIR           ${CMAKE_CURRENT_LIST_DIR}/Sources)
set     (RUKEN_SERVICES_DIR          ${RUKEN_SOURCES_DIR}/Services)
set     (RUKEN_CORE_DIR              ${RUKEN_SOURCES_DIR}/Core)

option(RUKEN_HEADLESS_BUILD "Disables all form of rendering and presentation" OFF)
option(RUKEN_TRACE_BUILD    "Enables support for the tracy profiler" ${RUKEN_BUILD_HAS_DEBUG_INFO})

if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR})
    file(MAKE_DIRECTORY ${RUKEN_GENERATED_SOURCES_DIR})
endif ()

if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR}/Services)
    file(MAKE_DIRECTORY ${RUKEN_GENERATED_SOURCES_DIR}/Services)
endif ()

# --- Add dependencies
include(${RUKEN_SOURCES_DIR}/CMake/CPM.cmake)

set(CPM_SOURCE_CACHE ${RUKEN_SOURCES_DIR}/ThirdParty)
file(LOCK ${CPM_SOURCE_CACHE} DIRECTORY GUARD FILE)

add_subdirectory(${RUKEN_SERVICES_DIR})
add_subdirectory(${RUKEN_CORE_DIR})

# --- Executable

add_compile_definitions(UNICODE _UNICODE)
add_executable(Ruken Sources/Core/Src/Main.cpp
                     Sources/Core/Src/Core/Kernel.cpp)

target_compile_definitions(Ruken PRIVATE RUKEN_PROJECT_NAME="Ruken")
target_compile_options    (Ruken PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/Zc:wchar_t /Zc:char8_t- /Zc:preprocessor /wd5103>
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:>
)
target_link_options(Ruken PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:>
)

target_link_libraries(Ruken RkCore RkRenderingService RkECSService)