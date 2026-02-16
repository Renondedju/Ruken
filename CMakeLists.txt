cmake_minimum_required(VERSION 3.28)

# --- Project setup
project(Ruken
    DESCRIPTION  "Ruken is a data-oriented Vulkan based game engine."
    HOMEPAGE_URL "https://github.com/Renondedju/Ruken"
)

macro(set_bool var)
    if(${ARGN})
        set(${var} ON)
    else()
        set(${var} OFF)
    endif()
endmacro()

# --- Values & options

set     (RUKEN_EXECUTABLE_NAME       Ruken)
set     (RUKEN_DEBUG_BUILD_TYPES     Debug RelWithDebInfo)
set     (RUKEN_OPTIMIZED_BUILD_TYPES Release RelWithDebInfo)
set_bool(RUKEN_BUILD_HAS_DEBUG_INFO  ${CMAKE_BUILD_TYPE} IN_LIST RUKEN_DEBUG_BUILD_TYPES)
set_bool(RUKEN_BUILD_IS_OPTIMIZED    ${CMAKE_BUILD_TYPE} IN_LIST RUKEN_OPTIMIZED_BUILD_TYPES)
set     (RUKEN_BINARY_DIR            ${CMAKE_BINARY_DIR})
set     (RUKEN_GENERATED_SOURCES_DIR ${CMAKE_BINARY_DIR}/Generated)
set     (RUKEN_SOURCES_DIR           ${CMAKE_CURRENT_LIST_DIR}/Sources)
set     (RUKEN_CMAKE_DIR             ${RUKEN_SOURCES_DIR}/CMake)
set     (RUKEN_MODULES_DIR           ${RUKEN_SOURCES_DIR}/Modules)
set     (RUKEN_CORE_DIR              ${RUKEN_MODULES_DIR}/Core)

# --- Language setup
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${RUKEN_BUILD_IS_OPTIMIZED})

option(RUKEN_ASAN_ENABLED             "Enables the address sanitizer for the whole project." OFF)
option(RUKEN_HEADLESS_BUILD           "Disables all form of rendering and presentation" OFF)
option(RUKEN_TRACE_BUILD              "Enables support for the tracy profiler" ${RUKEN_BUILD_HAS_DEBUG_INFO})
option(RUKEN_BREAK_ON_ERROR           "When on, a break interrupt will be generated on errors" ${RUKEN_BUILD_HAS_DEBUG_INFO})
option(RUKEN_TRACE_SHOW_WORKER_ZONES  "When on, tracy will also report the activity of workers" OFF)
option(RUKEN_TRACE_SHOW_ALLOC_ZONES   "When on, tracy will also trace allocation operators" OFF)
option(RUKEN_TRACE_SHOW_PROMISE_ZONES "When on, tracy will also trace coroutine promise methods" OFF)
option(RUKEN_TRACE_SHOW_LOGGER_ZONES  "When on, tracy will also report the activity of loggers" OFF)

if(RUKEN_ASAN_ENABLED)
    string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\n")
    add_compile_options(-fsanitize=address)
    add_link_options   (-fsanitize=address)
endif()

if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR})
    file(MAKE_DIRECTORY ${RUKEN_GENERATED_SOURCES_DIR})
endif ()

# --- Add dependencies
include(${RUKEN_SOURCES_DIR}/CMake/CPM.cmake)
define_property(GLOBAL PROPERTY RUKEN_MODULES_LIBS)

set(CPM_SOURCE_CACHE ${RUKEN_SOURCES_DIR}/ThirdParty)

file(LOCK ${CPM_SOURCE_CACHE} DIRECTORY GUARD FILE)
    add_subdirectory(${RUKEN_MODULES_DIR})
file(LOCK ${CPM_SOURCE_CACHE} DIRECTORY GUARD FILE RELEASE)

get_property(RUKEN_MODULES_LIB GLOBAL PROPERTY RUKEN_MODULES_LIB)

# --- Executable
add_compile_definitions(UNICODE _UNICODE)
add_executable(${RUKEN_EXECUTABLE_NAME}
        Sources/App/Main.cpp
        Sources/App/Test.hpp
        Sources/App/Rendering.hpp
        Sources/App/Components.hpp
        Sources/App/Systems/ApplyTransformHandler.hpp
        Sources/App/Systems/ApplyTransformHandler.cpp
)

target_compile_definitions(${RUKEN_EXECUTABLE_NAME} PRIVATE RUKEN_PROJECT_NAME="${RUKEN_EXECUTABLE_NAME}")
target_include_directories(${RUKEN_EXECUTABLE_NAME} PRIVATE ${RUKEN_MODULES_DIR} ${RUKEN_GENERATED_SOURCES_DIR})
target_link_libraries     (${RUKEN_EXECUTABLE_NAME} PRIVATE ${RUKEN_MODULES_LIB})